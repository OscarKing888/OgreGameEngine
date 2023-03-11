
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

template<> OutputWnd* Singleton<OutputWnd>::ms_Singleton = 0;

OutputWnd::OutputWnd()
{ 
	GameApp::getSingleton().AddAppListener(this);
}

void OutputWnd::OnAppInitCompleted()
{
	LogManager::getSingleton().getDefaultLog()->addListener(OutputWnd::getSingletonPtr());
}

OutputWnd::~OutputWnd()
{
	//LogManager::getSingleton().getDefaultLog()->removeListener(this);
}

BEGIN_MESSAGE_MAP(OutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

int OutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreateStockObject(DEFAULT_GUI_FONT);

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建选项卡窗口:
// 	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
// 	{
// 		// TRACE0("未能创建输出选项卡窗口\n");
// 		return -1;      // 未能创建
// 	}

	// 创建输出窗格:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputBuild.Create(dwStyle, rectDummy, this, 2))
	{
		// TRACE0("未能创建输出窗口\n");
		return -1;      // 未能创建
	}

	m_wndOutputBuild.SetFont(&m_Font);

	//CString strTabName;
//	BOOL bNameValid;

	// 将列表窗口附加到选项卡:
	/*bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputBuild, strTabName, (UINT)0);*/

	//LogManager::getSingleton().getDefaultLog()->addListener(this);

	return 0;
}

void OutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// 选项卡控件应覆盖整个工作区:
	m_wndOutputBuild.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void OutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&m_Font);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = std::max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}


void OutputWnd::messageLogged( const String& message, LogMessageLevel lml,
							  bool maskDebug, const String &logName )
{
	if(!m_wndOutputBuild.GetSafeHwnd())
	{
		return;
	}

	//CString str;
	//str.Format("[%s]%s", logName.c_str(), message.c_str());
	//m_wndOutputBuild.AddString(str);
	//m_wndOutputBuild.SetCaretIndex(m_wndOutputBuild.GetCount());
}

void OutputWnd::OnClose()
{
	CDockablePane::OnClose();
}

BOOL OutputWnd::DestroyWindow()
{
	return CDockablePane::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// OutputList1

OutputList::OutputList()
{
}

OutputList::~OutputList()
{
}

BEGIN_MESSAGE_MAP(OutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// OutputList 消息处理程序

void OutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void OutputList::OnEditCopy()
{
	//MessageBox(("复制输出"));
}

void OutputList::OnEditClear()
{
	//MessageBox(("清除输出"));
	//this->SetWindowText((""));	
}

void OutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();
	}
}
