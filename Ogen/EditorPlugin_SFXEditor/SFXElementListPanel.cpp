#include "stdafx.h"
#include "SFXElementListPanel.h"
#include "SFXEditorEvents.h"

BEGIN_MESSAGE_MAP(SFXElementList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &SFXElementList::OnNMDblclk)
END_MESSAGE_MAP()

SFXElementList::SFXElementList()
{

}

SFXElementList::~SFXElementList()
{

}

void SFXElementList::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString txt = GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem);
	//ParticleEditorFrm::getSingleton().EditParticle((LPCTSTR)txt);
	EventsManager::getSingleton().CreateEvent(
		SFXEditorEvents::AddElement, Any(String((LPCTSTR)txt)));
}


//------------------------------------------------------------------------

SFXElementListPanel::SFXElementListPanel()
{
}

SFXElementListPanel::~SFXElementListPanel()
{
}

BEGIN_MESSAGE_MAP(SFXElementListPanel, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SFXElementListPanel 消息处理程序

int SFXElementListPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	SpecialEffectsManager& mgr = SpecialEffectsManager::getSingleton();
	const EffectsElementFactoryMap& efmap = mgr.GetEffectsElementFactoryMap();
	EffectsElementFactoryMap::const_iterator it = efmap.begin();
	int i = 0;
	for (; it != efmap.end(); ++it)
	{
		_listCtrl.InsertItem(i, it->first.c_str());
		++i;
	}

	EnableDocking(CBRS_ALIGN_ANY);

	return 0;
}

void SFXElementListPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	CRect rectClient;
	GetClientRect(rectClient);
	_listCtrl.MoveWindow(rectClient);
}


void SFXElementListPanel::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	_listCtrl.SetFocus();
}