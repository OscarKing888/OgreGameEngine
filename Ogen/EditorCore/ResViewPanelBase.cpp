#include "stdafx.h"
#include "ResViewPanelBase.h"
#include "EditorApp.h"
#include "Resource.h"


#define M_INSERT_DEBUG_DATA 0

#define ID_RES_FILTER_BOX 1
#define ID_RES_TREE_CTRL 2
#define ID_RES_LIST_CTRL 3

const int C_SPACE = 1;
const int C_LineHeight = 21;

ResListTreeCtrl::ResListTreeCtrl()
{
}

ResListTreeCtrl::~ResListTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(ResListTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &ResListTreeCtrl::OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_CLICK, &ResListTreeCtrl::OnNMClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ResListTreeCtrl message handlers

BOOL ResListTreeCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}


	return bRes;
}

void ResListTreeCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	CWnd* p = GetParent();
	ResViewPanel* panel = dynamic_cast<ResViewPanel*>(p);
	if(panel)
	{
		panel->OnDBClickTree(pNMHDR, pResult);
	}
}

void ResListTreeCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CWnd* p = GetParent();
	ResViewPanel* panel = dynamic_cast<ResViewPanel*>(p);
	if(panel)
	{
		panel->OnNMClick(pNMHDR, pResult);
	}	*pResult = 0;
}



//------------------------------------------------------------------------
// ResViewPanel
//------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(ResViewPanel, CDockablePane)

const uint16 ResViewPanel::WORKQUEUE_THUMB_REQUEST = 1;

ResViewPanel::ResViewPanel(bool hasTreeList)
: _hasTreeList(hasTreeList)
, _leftCtlrWidthRatio(0.3f)
, _workQueueChannel(0)
, _isLoading(false)
, _canDragCreate(false)
{
	VERIFY(_defaultFont.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Tahoma"));                 // lpszFacename

	GameApp::getSingleton().AddAppListener(this);
}

ResViewPanel::~ResViewPanel()
{
	if(GameApp::getSingletonPtr())
	{
		GameApp::getSingleton().RemoveAppListener(this);
	}
}

void ResViewPanel::OnAppInitCompleted()
{
	OnInitTreeItem();
	OnInitListItem();

	WorkQueue* wq = Root::getSingleton().getWorkQueue();
	static int channedlID = 0;
	_workQueueChannel = wq->getChannel(
		formatString("Editor/Thumb[%d]", channedlID));

	++channedlID;

	wq->addRequestHandler(_workQueueChannel, this);
	wq->addResponseHandler(_workQueueChannel, this);
}

void ResViewPanel::OnAppShutdown()
{
	WorkQueue* wq = Root::getSingleton().getWorkQueue();
	wq->removeRequestHandler(_workQueueChannel, this);
	wq->removeResponseHandler(_workQueueChannel, this);
}

BEGIN_MESSAGE_MAP(ResViewPanel, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CBN_EDITCHANGE(ID_RES_FILTER_BOX, &ResViewPanel::OnFilterChange)
	ON_CBN_SELCHANGE(ID_RES_FILTER_BOX, &ResViewPanel::OnFilterChange)
	ON_CBN_KILLFOCUS(ID_RES_FILTER_BOX, &ResViewPanel::OnFilterLostFocus)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	//ON_WM_CONTEXTMENU()

	//ON_NOTIFY(TVN_SELCHANGED, ID_RES_TREE_CTRL, &ResViewPanel::OnNMClick)
	ON_NOTIFY(NM_DBLCLK, ID_RES_TREE_CTRL, &ResViewPanel::OnDBClickTree)

	ON_NOTIFY(LVN_ITEMCHANGED, ID_RES_LIST_CTRL, &ResViewPanel::OnListItemChange)
	ON_NOTIFY(HDN_ITEMCLICK, ID_RES_LIST_CTRL, &ResViewPanel::OnListItemClick)
	ON_NOTIFY(HDN_ITEMDBLCLICK, ID_RES_LIST_CTRL, &ResViewPanel::OnListItemDBClick)

	// 	ON_COMMAND(ID_RES_LIST_BIGICON, &ResViewPanel::OnViewBigIconList)
	// 	ON_COMMAND(ID_RES_LIST_SMALLICON, &ResViewPanel::OnViewSmallIconList)
	// 	ON_COMMAND(ID_RES_LIST_LIST, &ResViewPanel::OnViewList)

END_MESSAGE_MAP()

// ResViewPanel message handlers
//--------------------------------------------------------------------------------
BOOL ResViewPanel::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}

void ResViewPanel::OnViewBigIconList()
{
	_listCtrl.ModifyStyle(LVS_TYPEMASK, LVS_ICON& LVS_TYPEMASK);
}

void ResViewPanel::OnViewSmallIconList()
{
	_listCtrl.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON & LVS_TYPEMASK);
}

void ResViewPanel::OnViewList()
{
	_listCtrl.ModifyStyle(LVS_TYPEMASK, LVS_LIST & LVS_TYPEMASK);
}

int ResViewPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->SetFont(&_defaultFont, FALSE);

	CRect rc(0, 0, 5, 5);
	_filterBoxCaption.Create("Filter:", WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rc, this, IDC_STATIC);
	_filterBoxCaption.SetFont(this->GetFont());

	_filterBox.Create(WS_BORDER | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBS_HASSTRINGS | CBS_DROPDOWN | CBS_SORT | WS_VSCROLL | WS_TABSTOP,
		rc, this, ID_RES_FILTER_BOX);

	CRect cbSize;			// current size of combo box
	int Height;            // OGRE_NEW height for drop-down portion of combo box

	_filterBox.GetClientRect(cbSize);
	Height = _filterBox.GetItemHeight(-1);      // start with size of the edit-box portion
	Height += _filterBox.GetItemHeight(0) * 10;	// add height of lines of text

	// Note: The use of SM_CYEDGE assumes that we're using Windows '95
	// Now add on the height of the border of the edit box
	Height += GetSystemMetrics(SM_CYEDGE) * 2; // top & bottom edges

	// The height of the border of the drop-down box
	Height += GetSystemMetrics(SM_CYEDGE) * 2; // top & bottom edges

	// now set the size of the window
	_filterBox.SetWindowPos(NULL,            // not relative to any other windows
		0, 0,                         // TopLeft corner doesn't change
		cbSize.right, Height,         // existing width, OGRE_NEW height
		SWP_NOMOVE | SWP_NOZORDER     // don't move box or change z-ordering.
		);


	if(_hasTreeList)
	{
		_treeCtrl.Create(WS_BORDER | WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			rc, this, ID_RES_FILTER_BOX);
#if M_INSERT_DEBUG_DATA
		_treeCtrl.InsertItem("abc");
		_treeCtrl.InsertItem("123");
#endif
	}

	_listCtrl.Create(WS_CHILD | WS_VISIBLE | LVS_ICON | LVS_SINGLESEL | /*LVS_SORTASCENDING |*/ LVS_ALIGNLEFT | WS_BORDER | WS_TABSTOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		rc, this, ID_RES_LIST_CTRL);

#if M_INSERT_DEBUG_DATA
	for(int i = 0; i < 3000; ++i)
	{
		CString str;
		str.Format("%d.xml", i);
		_listCtrl.InsertItem(0, str);
	}
#endif

	EnableDocking(CBRS_ALIGN_ANY);

	SetWindowText(_UIDisplayString((LPCTSTR)GetPanelTitle()).c_str());
	UpdateWindow();

	return 0;
}

void ResViewPanel::OnInitTreeItem(void)
{

}

void ResViewPanel::OnRefreshTreeItem(void)
{
}

void ResViewPanel::OnInitListItem(void)
{

}

void ResViewPanel::OnRefreshListItem(void)
{
	// 	if(_currentFilter.IsEmpty() && _hideItemList.empty())
	// 	{
	// 		return;
	// 	}

	_listCtrl.ShowWindow(SW_HIDE);
	typedef std::list<int> DeleteItemList;
	int cnt = _listCtrl.GetItemCount();

	for (int i = 0; i < cnt;)
	{
		CString str = _listCtrl.GetItemText(i, 0);
		bool e = IsStringCanDisplay(str);

		if(!e)
		{
			SItemData dat;
			dat.Text = str;

			LVITEM vitem;
			vitem.iItem = i;
			vitem.iSubItem = 0;
			vitem.mask = LVIF_IMAGE;
			_listCtrl.GetItem(&vitem);
			dat.ImageIndex = vitem.iImage;

			_hideItemList.push_back(dat);
			BOOL r = _listCtrl.DeleteItem(i);
			assert(r == TRUE);
			UNREFERENCED_PARAMETER(r);
			i = std::max(0, i - 1);
			cnt = _listCtrl.GetItemCount();
		}
		else
		{
			++i;
		}
	}

	HideItemList::iterator it = _hideItemList.begin();
	while(it != _hideItemList.end())
	{
		const SItemData& s = *it;
		if(IsStringCanDisplay(s.Text))
		{
			_listCtrl.InsertItem(0, s.Text, s.ImageIndex);
			it = _hideItemList.erase(it);
		}
		else
		{
			++it;
		}
	}

	_listCtrl.ShowWindow(SW_SHOW);
}

void ResViewPanel::OnClickTreeItem(HTREEITEM item, const CString& resName)
{	
}

void ResViewPanel::OnDBClickTreeItem(HTREEITEM item, const CString& resName)
{	
}

void ResViewPanel::AdjustLayout(void)
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rc;
	GetClientRect(&rc);
	int cx = rc.Width();
	int cy = rc.Height();

	int x = C_SPACE;
	int y = C_SPACE;

	int treeWidth = 350;

	if(_hasTreeList)
	{
		_filterBoxCaption.SetWindowPos(NULL,x, y, 42, C_LineHeight, SWP_NOACTIVATE | SWP_NOZORDER);
		x += 42;

		_filterBox.SetWindowPos(NULL, x, y, (int)((float)treeWidth/*cx * _leftCtlrWidthRatio*/ - x - C_SPACE), C_LineHeight, SWP_NOACTIVATE | SWP_NOZORDER);
		x = C_SPACE;

		y += C_LineHeight + C_SPACE * 4;
		_treeCtrl.SetWindowPos(NULL, x, y, (int)((float)treeWidth/*cx * _leftCtlrWidthRatio*/ - x - C_SPACE), cy - y - C_SPACE, SWP_NOACTIVATE | SWP_NOZORDER);

		y = C_SPACE; 
		x = (int)((float)treeWidth/*cx * _leftCtlrWidthRatio*/) + C_SPACE;
		_listCtrl.SetWindowPos(NULL, x, y, cx - x - C_SPACE, cy - y - C_SPACE, SWP_NOACTIVATE | SWP_NOZORDER);
	}
	else
	{
		_filterBoxCaption.SetWindowPos(NULL,x, y, 42, C_LineHeight, SWP_NOACTIVATE | SWP_NOZORDER);
		x += 42;

		_filterBox.SetWindowPos(NULL, x, y, cx - x - C_SPACE, C_LineHeight, SWP_NOACTIVATE | SWP_NOZORDER);
		x = C_SPACE;

		y += C_LineHeight + C_SPACE * 4;
		_listCtrl.SetWindowPos(NULL,x, y, cx - x - C_SPACE, cy - y - C_SPACE, SWP_NOACTIVATE | SWP_NOZORDER);
	}	
}

void ResViewPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	GetClientRect(&rectTree);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));

	if(_hasTreeList)
	{
		CRect rc;
		_treeCtrl.GetWindowRect(rc);
		ScreenToClient(rc);

		rectTree = rc;
		rectTree.left = 0;
		rectTree.top -= C_SPACE * 4;
		rectTree.bottom = rectTree.top + C_SPACE * 4;
		dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));

		rectTree = rc;
		rectTree.top = 0;
		rectTree.bottom += C_SPACE;
		dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
	}
	else
	{
		CRect rc;
		_listCtrl.GetWindowRect(rc);
		ScreenToClient(rc);

		rectTree = rc;
		rectTree.left = 0;
		rectTree.top -= C_SPACE * 4;
		rectTree.bottom = rectTree.top + C_SPACE * 4;
		dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
	}
}

void ResViewPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
}

BOOL ResViewPanel::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	*pResult = 0;

	switch(wParam)
	{
	case ID_RES_TREE_CTRL:
		{
			//NMHDR* pNMTreeView = reinterpret_cast<NMHDR*>(lParam);
			LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(lParam);
			if(pNMTreeView)
			{

			}

			break;
		}

	case ID_RES_LIST_CTRL:
		{
			LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(lParam);
			if(pNMListView)
			{

				switch(pNMListView->hdr.code)
				{
				case NM_CLICK:
					{
						LRESULT r = 0;
						OnListItemClick(&pNMListView->hdr, &r);
						break;
					}

				case NM_DBLCLK:
					{
						LRESULT r = 0;
						OnListItemDBClick(&pNMListView->hdr, &r);
						break;
					}
				}

			}
			break;
		}
	}

	BOOL r = CDockablePane::OnNotify(wParam, lParam, pResult);

	return r;
}

BOOL ResViewPanel::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return CDockablePane::OnChildNotify(message, wParam, lParam, pResult);
}

void ResViewPanel::OnContextMenu(CWnd* pWnd, CPoint point)
{	
	if (pWnd != &_listCtrl)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		_listCtrl.ScreenToClient(&ptTree);

		UINT flags = 0;
		int idx = _listCtrl.HitTest(ptTree, &flags);
		if (idx != -1)
		{
			_listCtrl.SetSelectionMark(idx);
			//_listCtrl.SelectItem(hTreeItem);
		}
	}

	_listCtrl.SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_RES_LIST_STYLE);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, TRUE);
	}

	// 	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_RES_LIST_STYLE, point.x, point.y, this, TRUE);
}

BOOL ResViewPanel::OnBeforeShowPaneMenu(CMenu& m)
{
	BOOL r = CDockablePane::OnBeforeShowPaneMenu(m);
	m.AppendMenu(MF_SEPARATOR);
	m.AppendMenu(MF_STRING, ID_RES_LIST_BIGICON, "Big Icon(&A)");
	m.AppendMenu(MF_STRING, ID_RES_LIST_SMALLICON, "Small Icon(&S)");
	m.AppendMenu(MF_STRING, ID_RES_LIST_LIST, "List(&D)");
	return r;
}

BOOL ResViewPanel::OnAfterShowPaneMenu(int nMenuResult)
{
	switch(nMenuResult)
	{
	case ID_RES_LIST_BIGICON:
		OnViewBigIconList();
		break;

	case ID_RES_LIST_SMALLICON:
		OnViewSmallIconList();
		break;

	case ID_RES_LIST_LIST:
		OnViewList();
		break;
	}

	return TRUE;
}

void ResViewPanel::OnFilterChange()
{
	_filterBox.GetWindowText(_currentFilter);
	_currentFilter.MakeLower();
	OnRefreshListItem();
}

void ResViewPanel::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	//if(pNMTreeView->itemNew.hItem != GetRootItem())
	{
		_selectedTreeItemName = _treeCtrl.GetItemText(_treeCtrl.GetSelectedItem());
		OnClickTreeItem(pNMTreeView->itemNew.hItem, _selectedTreeItemName);
		//AfxMessageBox(_selectedTreeItemName + " Click");
	}
}


void ResViewPanel::OnDBClickTree(NMHDR * pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	//if(pNMTreeView->itemNew.hItem != GetRootItem())
	{		
		_selectedTreeItemName = _treeCtrl.GetItemText(_treeCtrl.GetSelectedItem());
		OnDBClickTreeItem(pNMTreeView->itemNew.hItem, _selectedTreeItemName);
		//AfxMessageBox(_selectedTreeItemName + " DBClick");
	}
}

void ResViewPanel::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	_listCtrl.SetFocus();
}

void ResViewPanel::OnFilterLostFocus()
{
	if((CB_ERR == _filterBox.FindString(0, _currentFilter)) && !_currentFilter.IsEmpty())
	{
		_filterBox.AddString(_currentFilter);
	}
}

void ResViewPanel::OnListItemChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}

void ResViewPanel::OnListItemClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	_selectedListItemName = _listCtrl.GetItemText(phdr->iItem, 0);
	OnClickListItem(phdr->iItem, _selectedListItemName);
	*pResult = 0;
}

void ResViewPanel::OnListItemDBClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	_selectedListItemName = _listCtrl.GetItemText(phdr->iItem, 0);
	OnDBClickListItem(phdr->iItem, _selectedListItemName);
	*pResult = 0;
}

void ResViewPanel::OnDBClickListItem(int item, const CString& resName)
{

}

void ResViewPanel::OnClickListItem(int item, const CString& resName)
{

}


bool ResViewPanel::IsStringCanDisplay(const CString& str)
{
	if(str.IsEmpty())
	{
		return false;
	}

	CString s = str;
	s.MakeLower();
	return !(-1 == s.Find(_currentFilter));
}

void ResViewPanel::ClearListItem()
{
	Root::getSingleton().getWorkQueue()->abortRequestsByChannel(_workQueueChannel);
	//WaitForLoadingProcesses();

	_listCtrl.DeleteAllItems();
	_hideItemList.clear();
}

void ResViewPanel::ClearTreeItem()
{
	_treeCtrl.DeleteAllItems();
}

void ResViewPanel::Show()
{
	ShowPane(TRUE, FALSE, TRUE);
}

void ResViewPanel::CreateImageList()
{
	if(_iconImages.GetSafeHandle() == 0)
	{
		_iconImages.Create(128, 128, ILC_COLOR32, 0, 4);
		_iconImagesSmall.Create(32, 32, ILC_COLOR32, 0, 4);

		//CBitmap bmp;
		//bmp.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_ERROR));
		String errBmp = FileEx::GetResourceFilePath("Error.bmp");

		HBITMAP hbmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
			errBmp.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		CBitmap* bmp = CBitmap::FromHandle(hbmp);

		_iconImages.Add(bmp, RGB(0, 0, 0));
		_iconImagesSmall.Add(bmp, RGB(0, 0, 0));

		_listCtrl.SetImageList(&_iconImages, LVSIL_NORMAL);
		_listCtrl.SetImageList(&_iconImagesSmall, LVSIL_SMALL);
	}
}

int ResViewPanel::AddToImageList(const String& fileName)
{
	HBITMAP hbmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
		fileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	CBitmap* bmp = CBitmap::FromHandle(hbmp);
	int imgIdx = _iconImages.Add(bmp, RGB(0, 0, 0));
	_iconMap.insert(std::make_pair(fileName, imgIdx));

	AddToSmallImageList(bmp);

	return imgIdx;
}

void ResViewPanel::AddToSmallImageList(CBitmap* bmp)
{
	CPaintDC dc(this);
	CDC dcSrc;
	dcSrc.CreateCompatibleDC(&dc);
	dcSrc.SelectObject(bmp);

	CDC dcDest;
	dcDest.CreateCompatibleDC(&dc);

	BITMAP bmpObj;
	::GetObject(bmp->GetSafeHandle(), sizeof(BITMAP), &bmpObj);

	CBitmap bmpSmall;
	bmpSmall.CreateCompatibleBitmap(&dc, 32, 32);
	CBitmap* old = dcDest.SelectObject(&bmpSmall);

	dcDest.SetStretchBltMode(COLORONCOLOR);
	dcDest.StretchBlt(0, 0, 32, 32, &dcSrc, 0, 0, bmpObj.bmWidth, bmpObj.bmHeight, SRCCOPY);
	dcDest.SelectObject(old);

	_iconImagesSmall.Add(&bmpSmall, RGB(0, 0, 0));
}

void ResViewPanel::AddLoadingRequest( const Any& req, uint16 requestType,
									 bool forceSynchronous /*= false*/ )
{
	_isLoading = true;
	Root::getSingleton().getWorkQueue()->addRequest(
		_workQueueChannel, requestType, 
		req, 0, forceSynchronous);
}

void ResViewPanel::WaitForLoadingProcesses()
{
	while (_isLoading)
	{
		// we need to wait for this to finish
		OGRE_THREAD_SLEEP(50);
		Root::getSingleton().getWorkQueue()->processResponses();
	}
}

WorkQueue::Response* ResViewPanel::handleRequest( const WorkQueue::Request* req, const WorkQueue* srcQ )
{
	return 0;
}

void ResViewPanel::handleResponse( const WorkQueue::Response* res, const WorkQueue* srcQ )
{

}

const String& ResViewPanel::GetDragdropCreateGameObjectTypeName() const
{
	return _dragdropGameObjectTypeName;
}

bool ResViewPanel::canShowInTree( const String& /*resGroup*/ ) const
{
	return true;
}
//------------------------------------------------------------------------
// ResPanel_ResGrouped
//------------------------------------------------------------------------

void ResPanel_ResGrouped::OnInitTreeItem(void)
{
	ClearTreeItem();
	OnRefreshTreeItem();
}

void ResPanel_ResGrouped::OnRefreshTreeItem(void)
{
	_selectedTreeItemName = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME.c_str();

	ResourceGroupManager& rgm = ResourceGroupManager::getSingleton();

	StringVector resGrps = rgm.getResourceGroups();
	for (size_t i = 0; i < resGrps.size(); ++i)
	{
		String resGrp = resGrps[i];

		HTREEITEM hgrp = _treeCtrl.InsertItem(resGrp.c_str());

#if 1
		StringVectorPtr locs = rgm.listResourceLocations(resGrp);
		for (size_t j = 0; j < locs->size(); ++j)
		{
			const String& loc = (*locs)[j];
			if(canShowInTree(loc))
			{
				_treeCtrl.InsertItem(loc.c_str(), hgrp);
			}
		}
#else
		ResourceGroupManager::ResourceGroup* rgp = rgm.getResourceGroup(resGrp);
		for(ResourceGroupManager::LocationList::iterator it = rgp->locationList.begin();
			it != rgp->locationList.end(); ++it)
		{
			ResourceGroupManager::ResourceLocation* loc = *it;
			//HTREEITEM harc = 
			_treeCtrl.InsertItem(loc->archive->getName().c_str(), hgrp);
		}
#endif

		_treeCtrl.Expand(hgrp, TVE_EXPAND);
	}
}