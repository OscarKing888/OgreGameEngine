#include "stdafx.h"
#include "CinemaEditor.h"
#include "resource.h"
#include "CinemaEditorDoc.h"
#include "CinemaEditorFrame.h"
#include "CinemaTrackView.h"
#include "CinemaEditActionCmd.h"

int ActiveCinemaEditor = GetAutoWMID();

template<> CinemaEditor* Singleton<CinemaEditor>::ms_Singleton = 0;
template<> CinemaEditorMsgHandler* Singleton<CinemaEditorMsgHandler>::ms_Singleton = 0;

BEGIN_MESSAGE_MAP(CinemaEditorMsgHandler, CCmdTarget)
	ON_COMMAND(ActiveCinemaEditor, OnShowCinemaEditor)
	ON_UPDATE_COMMAND_UI(ActiveCinemaEditor, OnUpdateCinemaEditor)
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
CinemaEditor::CinemaEditor()
: _track(0)
, _time(0.0f)
, _cinema(0)
{
	EventsManager::getSingleton().AddListener(this);

#define REGCMD(e, x) RegisterCommand(CinemaEditorEvents::e, OGRE_NEW x());

	// Add action commands
	REGCMD(NewCinema, Action_NewCinema);
	REGCMD(SaveAll, Action_SaveAll);
	REGCMD(EditCinema, Action_EditCinema);
	REGCMD(AddTrack, Action_AddTrack);
	REGCMD(RemoveTrack, Action_RemoveTrack);
	REGCMD(CreateKeyFrame, Action_CreateKeyFrame);

// 	REGCMD(Play, Action_CinemaPlay);
// 	REGCMD(Pause, Action_CinemaPause);
// 	REGCMD(Stop, Action_CinemaStop);
// 	REGCMD(ToBegin, Action_CinemaToBegin);
// 	REGCMD(ToEnd, Action_CinemaToEnd);
}
//--------------------------------------------------------------------------------
CinemaEditor::~CinemaEditor()
{
	EventsManager::getSingleton().RemoveListener(this);
}
//--------------------------------------------------------------------------------
void CinemaEditor::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(Execute(eventName, eventContex))
	{
	}
}
//--------------------------------------------------------------------------------
bool CinemaEditor::IsVisualObject()
{
	return GetAsVisualObject() != 0;
}
//--------------------------------------------------------------------------------
VisualObject* CinemaEditor::GetAsVisualObject()
{
	return dynamic_cast<VisualObject*>(
		EditorApp::getSingleton().GetSelectedObject());
}
//--------------------------------------------------------------------------------
void CinemaEditor::EditCinema( const String& cinemaName )
{
	_cinema = CinemaManager::getSingleton().GetCinema(cinemaName);

	EventsManager::getSingleton().CreateEvent(
		CinemaEditorEvents::UpdateUI_CinemaCtrl, Any(_cinema));
}
//--------------------------------------------------------------------------------
void CinemaEditor::AddCommand( ICommandXPtr cmd, bool execute /*= true*/ )
{
	_cmdHistory.AddCommand(cmd, execute);
}
//--------------------------------------------------------------------------------
void CinemaEditor::Undo()
{
	_cmdHistory.Undo();
}
//--------------------------------------------------------------------------------
void CinemaEditor::Redo()
{
	_cmdHistory.Redo();
}
//--------------------------------------------------------------------------------
bool CinemaEditor::IsUndoEnable() const
{
	return _cmdHistory.IsUndoEnable();
}
//--------------------------------------------------------------------------------
bool CinemaEditor::IsRedoEnable() const
{
	return _cmdHistory.IsRedoEnable();
}
//--------------------------------------------------------------------------------
void CinemaEditor::SetCurrentTime( float time )
{
	_time = time;
	if(_cinema)
	{
		_cinema->SetTimePosition(time);
	}
}
//--------------------------------------------------------------------------------
void CinemaEditor::AddTrackEditor( ITrackEditor* trackEditor )
{
	_trackEditor[trackEditor->GetTypeName()] = trackEditor;
}
//--------------------------------------------------------------------------------
ITrackEditor* CinemaEditor::GetTrackEditor( ICinemaTrack* track )
{
	const String& name = track->GetTypeName();
	return GetTrackEditor(name);
}

ITrackEditor* CinemaEditor::GetTrackEditor( const String& trackTypeName )
{
	TrackEditorMap::iterator i = _trackEditor.find(trackTypeName);
	if(i != _trackEditor.end())
	{
		return i->second;
	}

	return 0;
}
//--------------------------------------------------------------------------------
// EditState_CinemaMsgHandler
//--------------------------------------------------------------------------------
CinemaEditorMsgHandler::CinemaEditorMsgHandler()
: _cinemaEditorDocTemplate(0)
, _mainWnd(0)
{
	EditorApp::getSingleton().RegisterWMHandler(this);
	_cinemaEditorDocTemplate = new CSingleDocTemplate(
		ID_CINEMAEDITOR,
		RUNTIME_CLASS(CinemaEditorDoc),
		RUNTIME_CLASS(CinemaEditorFrame),       // main SDI frame window
		RUNTIME_CLASS(CinemaTrackView));
	assert(_cinemaEditorDocTemplate);
	AfxGetApp()->AddDocTemplate(_cinemaEditorDocTemplate);
}
//--------------------------------------------------------------------------------
CinemaEditorMsgHandler::~CinemaEditorMsgHandler()
{
	if(_mainWnd)
	{
		try
		{
			_mainWnd->DestroyWindow();
		}		
		catch (...)
		{
		}
	}

	EditorApp::getSingleton().UnRegisterWMHandler(this);
}
//--------------------------------------------------------------------------------
void CinemaEditorMsgHandler::OnShowCinemaEditor()
{
	bool show = CinemaEditor::getSingleton().IsActive();
	show = !show;
	CinemaEditor::getSingleton().SetActive(show);

	if(show)
	{
		if(!_mainWnd)
		{
			_mainWnd = MFCCreateNewWindow(_cinemaEditorDocTemplate, 0);
			_mainWnd->ShowWindow(SW_SHOW);
		}
	}
	else if(_mainWnd)
	{
		_mainWnd->ShowWindow(SW_HIDE);
	}
}
//--------------------------------------------------------------------------------
void CinemaEditorMsgHandler::OnUpdateCinemaEditor(CCmdUI* ui)
{
	ui->SetCheck(CinemaEditor::getSingleton().IsActive());
}
//--------------------------------------------------------------------------------
void CinemaEditorMsgHandler::OnMainFrameClose( CFrameWnd* wnd )
{
	_mainWnd = 0;
	CinemaEditor::getSingleton().SetActive(false);
}
//--------------------------------------------------------------------------------
