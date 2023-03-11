#include "stdafx.h"
#include "CinemaListPanel.h"
#include "CinemaEditorEvents.h"

BEGIN_MESSAGE_MAP(CinemaList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CinemaList::OnNMDblclk)
END_MESSAGE_MAP()

CinemaList::CinemaList()
{

}

CinemaList::~CinemaList()
{

}

void CinemaList::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString txt = GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem);
	//ParticleEditorFrm::getSingleton().EditParticle((LPCTSTR)txt);
	EventsManager::getSingleton().CreateEvent(
		CinemaEditorEvents::EditCinema, Any(String((LPCTSTR)txt)));
}


//------------------------------------------------------------------------

CinemaListPanel::CinemaListPanel()
{
	EventsManager::getSingleton().AddListener(this);
}

CinemaListPanel::~CinemaListPanel()
{
	EventsManager::getSingleton().RemoveListener(this);
}

BEGIN_MESSAGE_MAP(CinemaListPanel, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SFXElementListPanel 消息处理程序

int CinemaListPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | LVS_LIST | LVS_SHOWSELALWAYS | LVS_ALIGNLEFT | WS_TABSTOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!_listCtrl.Create(dwViewStyle, rectDummy, this, 2))
	{
		// TRACE0("未能创建列表控件\n");
		return -1;      // 未能创建
	}

	RefreshCinemaList();
	EnableDocking(CBRS_ALIGN_ANY);

	return 0;
}

void CinemaListPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	CRect rectClient;
	GetClientRect(rectClient);
	_listCtrl.MoveWindow(rectClient);
}


void CinemaListPanel::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	_listCtrl.SetFocus();
}

void CinemaListPanel::RefreshCinemaList()
{
	_listCtrl.DeleteAllItems();
	CinemaManager& mgr = CinemaManager::getSingleton();
	const CinemaManager::CinemaMap& cinms = mgr.GetAllCinema();
	CinemaManager::CinemaMap::const_iterator it = cinms.begin();
	int i = 0;
	for (; it != cinms.end(); ++it)
	{
		_listCtrl.InsertItem(i, it->first.c_str());
		++i;
	}
}

void CinemaListPanel::OnEvents( const String& eventName, const Any& eventContex,
							   bool& stopBroadCast )
{
	if(eventName == CinemaEditorEvents::UpdateUI_CinemaList)
	{
		RefreshCinemaList();
	}
}