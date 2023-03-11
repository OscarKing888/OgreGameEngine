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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CinemaEditor.h"
#include "CinemaEditorFrame.h"
#include "CinemaTrackView.h"
#include "TrackView.h"
#include "Resource.h"
#include "CinemaEditorEvents.h"
#include "CinemaEditor.h"

IMPLEMENT_DYNCREATE(CinemaEditorFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CinemaEditorFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()

	ON_COMMAND(ID_NEW_CINEMA, OnNewCinema)

	ON_COMMAND(ID_SAVE_CINEMA, OnSaveAll)	
	ON_UPDATE_COMMAND_UI(ID_SAVE_CINEMA, OnUpdateSaveAll)

	ON_COMMAND(IDI_CINEMA_UNDO, OnUndo)
	ON_UPDATE_COMMAND_UI(IDI_CINEMA_UNDO, OnUpdateUndo)
	ON_COMMAND(IDI_CINEMA_REDO, OnRedo)
	ON_UPDATE_COMMAND_UI(IDI_CINEMA_REDO, OnUpdateRedo)

	ON_COMMAND(ID_CREATE_KEYFRAME, OnCreateKeyFrame)
	ON_UPDATE_COMMAND_UI(ID_CREATE_KEYFRAME, OnUpdateCreateKeyFrame)

	ON_COMMAND(ID_CINEMA_PLAY, OnPlay)
	ON_COMMAND(ID_CINEMA_PAUSE, OnPause)
	ON_COMMAND(ID_CINEMA_STOP, OnStop)
	ON_COMMAND(ID_CINEMA_TO_BEGIN, OnToBegin)
	ON_COMMAND(ID_CINEMA_TO_END, OnToEnd)

	ON_COMMAND(ID_REMOVE_TRACK, OnRemoveTrack)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_TRACK, OnUpdateRemoveTrack)

END_MESSAGE_MAP()

// CMainFrame construction/destruction

CinemaEditorFrame::CinemaEditorFrame()
{
	// TODO: add member initialization code here
//	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_AQUA);
	EventsManager::getSingleton().AddListener(this);

	ControllerManager::getSingleton().setFrameDelay(1.0f/10.0f);
}

CinemaEditorFrame::~CinemaEditorFrame()
{
	EventsManager::getSingleton().RemoveListener(this);
}

int CinemaEditorFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->SetWindowText(_UIDisplayString("CinemaEditor").c_str());
	//BOOL bNameValid;
	// set the visual manager and style based on persisted value

	_wndRibbonBar.Create(this);
	InitializeRibbon();

	if (!_wndStatusBar.Create(this))
	{
//		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

// 	HICON icon = AfxGetApp()->LoadIcon(IDI_ICON_CINEMAEDITOR);
// 	this->SetIcon(icon, TRUE);
// 	this->SetIcon(icon, FALSE);

// 	CString strTitlePane1;
// 	CString strTitlePane2;
// 	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
// 	ASSERT(bNameValid);
// 	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
// 	ASSERT(bNameValid);
// 	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
// 	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
//	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
//		TRACE0("CinemaEditor: failed to create docking windows\n");
		return -1;
	}

	_cinemaList.EnableDocking(CBRS_ALIGN_ANY);
	_propGrid.EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&_cinemaList);
	CDockablePane* pTabbedBar = NULL;
	_propGrid.AttachToTabWnd(&_cinemaList, DM_SHOW, TRUE, &pTabbedBar);
	//DockPane(&_propGrid);

	return 0;
}

BOOL CinemaEditorFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	BOOL r = m_wndSplitter.CreateStatic(this, 2, 1);
	//// TODO: adjust the number of rows, columns
	//	CSize(10, 10),      // TODO: adjust the minimum pane size
	//	pContext);

	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(TrackView), CSize(100, 300), pContext);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CinemaTrackView), CSize(100, 300), pContext);

	m_wndSplitter.SetRowInfo(0, 300, 100);
	m_wndSplitter.SetRowInfo(1, 600, 50);

	CWnd* w1 = m_wndSplitter.GetPane(0, 0);
	CWnd* w2 = m_wndSplitter.GetPane(1, 0);

	TrackView* v1 = dynamic_cast<TrackView*>(w1);
	CinemaTrackView* v2 = dynamic_cast<CinemaTrackView*>(w2);

	_viewSyncMeditor.SetTrackView(v1->GetTrackCtrl());
	_viewSyncMeditor.SetCinemaView(v2->GetCinemaTrackCtrl());

	v1->GetTrackCtrl()->SetListener(&_viewSyncMeditor);
	v2->GetCinemaTrackCtrl()->SetListener(&_viewSyncMeditor);

	return r;
}

BOOL CinemaEditorFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION/* | FWS_ADDTOTITLE*/
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

#ifndef _DEBUG
	cs.dwExStyle |= WS_EX_TOPMOST;
#endif

	cs.cx = 1024;
	cs.cy = 600;

	return TRUE;
}

void CinemaEditorFrame::InitializeRibbon()
{
	///
	/// Main panel
	///

	CMFCRibbonCategory* mainCat =
		_wndRibbonBar.AddCategory(_UIDisplayString("IDS_HOME").c_str(),
		IDB_FILESMALL, IDB_FILELARGE);

//	_viewCategory = mainCat;

	CMFCRibbonPanel* mainPanel = mainCat->AddPanel(
		_UIDisplayString("IDS_HOME").c_str());

	// New
	mainPanel->Add(new CMFCRibbonButton(ID_NEW_CINEMA,
		_UIDisplayString("ID_NEW_CINEMA").c_str(), 0, 0));

	// Save
	mainPanel->Add(new CMFCRibbonButton(ID_SAVE_CINEMA,
		_UIDisplayString("ID_FILE_SAVE").c_str(), 2, 2));

	// Undo
	HICON icon = AfxGetApp()->LoadIcon(IDI_CINEMA_UNDO);
	mainPanel->Add(new CMFCRibbonButton(IDI_CINEMA_UNDO,
		_UIDisplayString("ID_EDIT_UNDO").c_str(), icon));

	// Redo
	icon = AfxGetApp()->LoadIcon(IDI_CINEMA_REDO);
	mainPanel->Add(new CMFCRibbonButton(IDI_CINEMA_REDO,
		_UIDisplayString("ID_EDIT_REDO").c_str(), icon));


	// track
	CMFCRibbonPanel* trackPanel = mainCat->AddPanel(
		_UIDisplayString("IDS_TRACK").c_str());

	icon = AfxGetApp()->LoadIcon(IDI_ICON_REMOVE_TRACK);
	trackPanel->Add(new CMFCRibbonButton(ID_REMOVE_TRACK,
		_UIDisplayString("ID_REMOVE_TRACK").c_str(), icon));

	icon = AfxGetApp()->LoadIcon(IDI_ICON_CREATEKF);
	trackPanel->Add(new CMFCRibbonButton(ID_CREATE_KEYFRAME,
		_UIDisplayString("ID_CREATE_KEYFRAME").c_str(), icon));

	// Play
	CMFCRibbonPanel*  playPanel = mainCat->AddPanel(
		_UIDisplayString("ID_CINEMA_PLAY").c_str());

	icon = AfxGetApp()->LoadIcon(IDI_ICON_PLAY);
	playPanel->Add(new CMFCRibbonButton(ID_CINEMA_PLAY,
		_UIDisplayString("ID_CINEMA_PLAY").c_str(), icon));

	icon = AfxGetApp()->LoadIcon(IDI_ICON_PAUSE);
	playPanel->Add(new CMFCRibbonButton(ID_CINEMA_PAUSE,
		_UIDisplayString("ID_CINEMA_PAUSE").c_str(), icon));

	icon = AfxGetApp()->LoadIcon(IDI_ICON_STOP);
	playPanel->Add(new CMFCRibbonButton(ID_CINEMA_STOP,
		_UIDisplayString("ID_CINEMA_STOP").c_str(), icon));

	icon = AfxGetApp()->LoadIcon(IDI_ICON_TOBEGIN);
	playPanel->Add(new CMFCRibbonButton(ID_CINEMA_TO_BEGIN,
		_UIDisplayString("ID_CINEMA_TO_BEGIN").c_str(), icon));

	icon = AfxGetApp()->LoadIcon(IDI_ICON_TOEND);
	playPanel->Add(new CMFCRibbonButton(ID_CINEMA_TO_END,
		_UIDisplayString("ID_CINEMA_TO_END").c_str(), icon));



	//BOOL bNameValid;

	//CString strTemp;
	//bNameValid = strTemp.LoadString(IDS_RIBBON_FILE);
	//ASSERT(bNameValid);

	//// Load panel images:
	//m_PanelImages.SetImageSize(CSize(16, 16));
	//m_PanelImages.Load(IDB_BUTTONS);

	//// Init main button:
	_mainButton.SetImage(IDB_MAIN);
	_mainButton.SetText("\nf");
	//_mainButton.SetToolTipText(strTemp);

	_wndRibbonBar.SetApplicationButton(&_mainButton, CSize (40, 40));
	
	//// Add quick access toolbar commands:
	CList<UINT, UINT> lstQATCmds;

	lstQATCmds.AddTail(ID_NEW_CINEMA);
	lstQATCmds.AddTail(ID_SAVE_CINEMA);
	lstQATCmds.AddTail(IDI_CINEMA_UNDO);
	lstQATCmds.AddTail(IDI_CINEMA_REDO);
	lstQATCmds.AddTail(ID_REMOVE_TRACK);
	lstQATCmds.AddTail(ID_CREATE_KEYFRAME);

	_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);
}

BOOL CinemaEditorFrame::CreateDockingWindows()
{
	//------------------------------------------------------------------------
	// CinemaList Panel
	//------------------------------------------------------------------------
	if (!_cinemaList.Create(_UIDisplayString("ID_CINEMA_LIST").c_str(),
		this, CRect(0, 0, 300, 200), TRUE, ID_CINEMA_LIST,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		return FALSE; // 未能创建
	}

	//------------------------------------------------------------------------
	// Property Grid Panel
	//------------------------------------------------------------------------
	if (!_propGrid.Create(_UIDisplayString("ID_CINEMA_PROPGRID").c_str(),
		this, CRect(0, 0, 300, 500), TRUE, ID_CINEMA_PROPGRID,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		return FALSE; // 未能创建
	}

 	SetDockingWindowIcons(true);
	return TRUE;
}

void CinemaEditorFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
// 	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
// 	m_wndFileView.SetIcon(hFileViewIcon, FALSE);
// 
// 	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
// 	m_wndClassView.SetIcon(hClassViewIcon, FALSE);
// 
// 	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
// 	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);
// 
// 	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
// 	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

// CMainFrame diagnostics

#ifdef _DEBUG
void CinemaEditorFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CinemaEditorFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnNewCinema()
{
	InputDlg dlg(_UIDisplayString("New Cinema").c_str());
	do
	{
		INT_PTR r = dlg.DoModal();
		if(r == IDOK)
		{
			String cinemaName = dlg.GetInputText();
			if(CinemaManager::getSingleton().GetCinema(cinemaName) != 0)
			{
				String msg = formatString(
					_UIDisplayString("IDS_CINEMA_ALREADY_EXIST").c_str(),
					cinemaName.c_str());
				AfxMessageBox(msg.c_str());
			}
			else
			{
				EventsManager::getSingleton().CreateEvent(
					CinemaEditorEvents::NewCinema, Any(cinemaName));

				EventsManager::getSingleton().CreateEvent(
					CinemaEditorEvents::UpdateUI_CinemaList, Any());

				EventsManager::getSingleton().CreateEvent(
					CinemaEditorEvents::EditCinema, Any(cinemaName));

				return;
			}
		}
		else if(r == IDCANCEL)
		{
			return;
		}
	}while(true);
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnNewCinema( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(TRUE);
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnSaveAll()
{
	EventsManager::getSingleton().CreateEvent(
		CinemaEditorEvents::SaveAll, Any());
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnUpdateSaveAll( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(CinemaManager::getSingleton().HasCinema());
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnClose()
{
	CinemaEditorMsgHandler::getSingleton().OnMainFrameClose(this);
	CFrameWndEx::OnClose();
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnCreateKeyFrame()
{
	EventsManager::getSingleton().CreateEvent(
		CinemaEditorEvents::CreateKeyFrame, Any());
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnUpdateCreateKeyFrame( CCmdUI* pCmdUI )
{
	ICinemaTrack* track = CinemaEditor::getSingleton().GetTrack();
	if(!track)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	ITrackEditor* editor = CinemaEditor::getSingleton().GetTrackEditor(track);
	if(!editor)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	IGameObject* obj = EditorApp::getSingleton().GetSelectedObject();
	pCmdUI->Enable(editor->CanCreateKeyFrame(obj));
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(eventName == CinemaEditorEvents::ShowCinemaPropObject)
	{
		IPropertyObject* obj = any_cast<IPropertyObject*>(eventContex);
		_propGrid.SetObject(obj);
	}
	else if(eventName == CinemaEditorEvents::ClearCinemaPropObject)
	{
		_propGrid.SetObject(0);
	}
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnPlay()
{
	Cinema* cinema = CinemaEditor::getSingleton().GetCinema();
	cinema->Load(true);
	cinema->EnableAll(true);
	cinema->Play();
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnPause()
{
	bool isPlaying = CinemaEditor::getSingleton().GetCinema()->IsPlaying();
	CinemaEditor::getSingleton().GetCinema()->Pause(isPlaying);
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnStop()
{
	CinemaEditor::getSingleton().GetCinema()->Stop();
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnToBegin()
{
	CinemaEditor::getSingleton().GetCinema()->ToBegin();
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnToEnd()
{
	CinemaEditor::getSingleton().GetCinema()->ToEnd();
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnUndo()
{
	CinemaEditor::getSingleton().Undo();
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnRedo()
{
	CinemaEditor::getSingleton().Redo();
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnUpdateUndo( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(CinemaEditor::getSingleton().IsUndoEnable());
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnUpdateRedo( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(CinemaEditor::getSingleton().IsRedoEnable());
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnUpdateRemoveTrack( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(CinemaEditor::getSingleton().GetTrack() != 0);
}
//--------------------------------------------------------------------------------
void CinemaEditorFrame::OnRemoveTrack()
{
	CinemaEditor& editor = CinemaEditor::getSingleton();
	ICinemaTrack* track = editor.GetTrack();

	EventsManager::getSingleton().CreateEvent(
		CinemaEditorEvents::RemoveTrack, Any(track));
}
//--------------------------------------------------------------------------------
