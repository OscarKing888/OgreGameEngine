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

// CinemaTrackView.h : interface of the CCinemaEditorView class
//


#pragma once

#include "CinemaTrackCtrl.h"

class CinemaEditorDoc;

class CinemaTrackView : public CView
{
protected: // create from serialization only
	CinemaTrackView();
	DECLARE_DYNCREATE(CinemaTrackView)

// Attributes
public:
	CinemaEditorDoc* GetDocument() const;

	CinemaTrackCtrl* GetCinemaTrackCtrl() { return &_cinemaTrackCtrl; }
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CinemaTrackView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	CinemaTrackCtrl _cinemaTrackCtrl;
// Generated message map functions
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

#ifndef _DEBUG  // debug version in CinemaEditorView.cpp
inline CinemaEditorDoc* CinemaTrackView::GetDocument() const
   { return reinterpret_cast<CinemaEditorDoc*>(m_pDocument); }
#endif

