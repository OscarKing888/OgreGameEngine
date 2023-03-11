#include "stdafx.h"
#include "TimeLinePanel.h"
#include "TimeLinePanelController.h"

template<> TimeLinePanel* Singleton<TimeLinePanel>::ms_Singleton = 0;

TimeLinePanel::TimeLinePanel()
{
	_controller = OGRE_NEW TimeLinePanelController(this);
}
//--------------------------------------------------------------------------------
TimeLinePanel::~TimeLinePanel()
{
	OGRE_DELETE _controller;
}
//--------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(TimeLinePanel, CDockablePane)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
void TimeLinePanel::OnSize( UINT nType, int cx, int cy )
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	RelayoutEditor();

}
//--------------------------------------------------------------------------------
void TimeLinePanel::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

 	if(_container && _container.GetSafeHwnd())
 	{
 		_container.SetFocus();
 	}
}
//--------------------------------------------------------------------------------
void TimeLinePanel::RelayoutEditor()
{
	CRect rcClient;
	GetClientRect(rcClient);

 	if(_container && _container.GetSafeHwnd())
 	{
 		_container.MoveWindow(&rcClient);
 	}
}
//--------------------------------------------------------------------------------
void TimeLinePanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}
//--------------------------------------------------------------------------------
BOOL TimeLinePanel::OnEraseBkgnd(CDC* pDC)
{
	//return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}
//--------------------------------------------------------------------------------
int TimeLinePanel::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	CDockablePane::OnCreate(lpCreateStruct);

	BOOL r = _container.Create(NULL, "SFX Timeblock Container",
		WS_CHILD | WS_VISIBLE, CRect(0, 0, 20, 20), this, 1234);

	return 0;
}
//--------------------------------------------------------------------------------