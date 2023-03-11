// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// CinemaEditorView.cpp : implementation of the CCinemaEditorView class
//

#include "stdafx.h"
#include "CinemaEditor.h"

#include "CinemaEditorDoc.h"
#include "CinemaTrackView.h"

// CCinemaEditorView

IMPLEMENT_DYNCREATE(CinemaTrackView, CView)

BEGIN_MESSAGE_MAP(CinemaTrackView, CView)
	// Standard printing commands
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CCinemaEditorView construction/destruction

CinemaTrackView::CinemaTrackView()
{
	// TODO: add construction code here

}

CinemaTrackView::~CinemaTrackView()
{
}

BOOL CinemaTrackView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCinemaEditorView drawing

void CinemaTrackView::OnDraw(CDC* pDC)
{
	CinemaEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
// 	pDC->TextOut(20, 10, pDoc->GetTitle() + "SSSS");
}


// CCinemaEditorView printing


void CinemaTrackView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CinemaTrackView::OnContextMenu(CWnd* pWnd, CPoint point)
{
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CCinemaEditorView diagnostics

#ifdef _DEBUG
void CinemaTrackView::AssertValid() const
{
	CView::AssertValid();
}

void CinemaTrackView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CinemaEditorDoc* CinemaTrackView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CinemaEditorDoc)));
	return (CinemaEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CCinemaEditorView message handlers

BOOL CinemaTrackView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}

void CinemaTrackView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(_cinemaTrackCtrl.GetSafeHwnd())
	{
		_cinemaTrackCtrl.MoveWindow(0, 0, cx, cy);
	}
}

int CinemaTrackView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	_cinemaTrackCtrl.Create(NULL, "Cinema Tracks", WS_CHILD | WS_VISIBLE, CRect(0, 0, 20, 20), this, 1234);
	// TODO:  Add your specialized creation code here
// 	for(int i = 0; i < 10; ++i)
// 	{
// 		CinemaTrackBlock* atb = OGRE_NEW CinemaTrackBlock();
// 		atb->SetParent(&_cinemaTrackCtrl);
// 		CString cap;
// 		cap.Format("Game Actor Object%d", i);
// 		atb->SetCaption(cap);
// 		atb->SetStartTime(0);
// 		//atb->SetTimeLength((float)(i + 1));
// 		_cinemaTrackCtrl.AddTimeblock(atb);
// 	}
	return 0;
}

void CinemaTrackView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	if(_cinemaTrackCtrl.GetSafeHwnd())
	{
		_cinemaTrackCtrl.SetFocus();
	}
}
