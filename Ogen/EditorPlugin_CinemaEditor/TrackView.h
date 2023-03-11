#pragma once

#include "TrackCtrl.h"

// TrackView view

class TrackView : public CView
{
	DECLARE_DYNCREATE(TrackView)

protected:
	TrackView();           // protected constructor used by dynamic creation
	virtual ~TrackView();

	TrackCtrl _trackCtrl;

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	TrackCtrl* GetTrackCtrl() { return &_trackCtrl; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


