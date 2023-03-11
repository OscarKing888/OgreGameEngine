`
#include "stdafx.h"
#include "ViewTree.h"
#include "EditorGameApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ViewTree

ViewTree::ViewTree()
{
}

ViewTree::~ViewTree()
{
}

BEGIN_MESSAGE_MAP(ViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &ViewTree::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &ViewTree::OnDBClick)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ViewTree 消息处理程序

BOOL ViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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

void ViewTree::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	if(pNMTreeView->itemNew.hItem != GetRootItem())
	{
		CString str = GetItemText(pNMTreeView->itemNew.hItem);
		if(!GetChildItem(pNMTreeView->itemNew.hItem))
		{
			EventsManager::getSingleton().CreateEvent("CreateObject", Any(str));
		}
	}
}


void ViewTree::OnDBClick(NMHDR * pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	if(pNMTreeView->itemNew.hItem != GetRootItem())
	{
		CString str = GetItemText(pNMTreeView->itemNew.hItem);
		if(!GetChildItem(pNMTreeView->itemNew.hItem))
		{
			EditorApp::getSingleton().ActiveState("Create Object");
			//EditorGame::getSingleton().GetStateManager().ActiveState(EES_Create);
		}
	}
}
