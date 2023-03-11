// TrackView.cpp : implementation file
//

#include "stdafx.h"
#include "CinemaEditor.h"
#include "TrackView.h"


// TrackView

IMPLEMENT_DYNCREATE(TrackView, CView)

TrackView::TrackView()
{

}

TrackView::~TrackView()
{
}

BEGIN_MESSAGE_MAP(TrackView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// TrackView drawing

void TrackView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
// 	CString str;
// 	pDC->TextOut(10, 10, pDoc->GetTitle());
}


// TrackView diagnostics

#ifdef _DEBUG
void TrackView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void TrackView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// TrackView message handlers

int TrackView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	_trackCtrl.Create(NULL, "Cinema Single Track", WS_CHILD | WS_VISIBLE, CRect(0, 0, 20, 20), this, 1235);
	return 0;
}

void TrackView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(_trackCtrl.GetSafeHwnd())
	{
		_trackCtrl.MoveWindow(0, 0, cx, cy);
	}
}

BOOL TrackView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}

void TrackView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	if(_trackCtrl.GetSafeHwnd())
	{
		_trackCtrl.SetFocus();
	}
}
