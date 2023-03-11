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

// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ViewSyncMediator.h"
#include "CinemaListPanel.h"

class CinemaEditorFrame
	: public CFrameWndEx
	, public IEventLisitener
{
	
protected: // create from serialization only
	CinemaEditorFrame();
	DECLARE_DYNCREATE(CinemaEditorFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:

	PropertyGrid& GetPropGrid() { return _propGrid; }

// Operations
public:

// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CinemaEditorFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCRibbonBar     _wndRibbonBar;
	CMFCRibbonApplicationButton _mainButton;
	CMFCToolBarImages _panelImages;
	CMFCRibbonStatusBar  _wndStatusBar;
	ViewSyncMediator  _viewSyncMeditor;
	CinemaListPanel _cinemaList;
	PropertyGrid	_propGrid;

// Generated message map functions
protected:

	void InitializeRibbon();
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNewCinema();
	afx_msg void OnNewCinema(CCmdUI* pCmdUI);
	afx_msg void OnSaveAll();
	afx_msg void OnUpdateSaveAll(CCmdUI* pCmdUI);
	afx_msg void OnUndo();
	afx_msg void OnUpdateUndo(CCmdUI* pCmdUI);
	afx_msg void OnRedo();
	afx_msg void OnUpdateRedo(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnCreateKeyFrame();
	afx_msg void OnUpdateCreateKeyFrame(CCmdUI* pCmdUI);

	afx_msg void OnPlay();
	afx_msg void OnPause();
	afx_msg void OnStop();
	afx_msg void OnToBegin();
	afx_msg void OnToEnd();

	afx_msg void OnRemoveTrack();
	afx_msg void OnUpdateRemoveTrack(CCmdUI* pCmdUI);
};