#pragma once
#include "ITrackEditor.h"

extern int ActiveCinemaEditor;

///
/// CinemaEditor
///
class CinemaEditor
	: public Singleton<CinemaEditor>
	, public IEventLisitener
	, public OgeAlloc
	, public ActionCommandManager
{
public:

	CinemaEditor();
	virtual ~CinemaEditor();

	bool IsVisualObject();
	VisualObject* GetAsVisualObject();

	bool IsActive() const { return _active; }
	void SetActive(bool val) { _active = val; }

	void EditCinema(const String& cinemaName);
	Cinema* GetCinema() const { return _cinema; }

	void AddCommand(ICommandXPtr cmd, bool execute = true);
	void Undo();
	void Redo();
	bool IsUndoEnable() const;
	bool IsRedoEnable() const;

	ICinemaTrack* GetTrack() const { return _track; }
	void SetTrack(ICinemaTrack* val) { _track = val; }
	
	void SetCurrentTime(float time);
	float GetCurrentTime() const { return _time; }

	ITrackEditor* GetTrackEditor(ICinemaTrack* track);
	ITrackEditor* GetTrackEditor(const String& trackTypeName);
	void AddTrackEditor(ITrackEditor* trackEditor);

protected:

	bool _active;

	Cinema* _cinema;
	CommandHistory	_cmdHistory;
	ICinemaTrack* _track;

	float _time;

	typedef Ogre::map<String, ITrackEditor*>::type TrackEditorMap;
	TrackEditorMap _trackEditor;

	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast); 
};

///
/// CinemaEditorMsgHandler
///

class CinemaEditorMsgHandler
	: public CCmdTarget
	, public Singleton<CinemaEditorMsgHandler>
{

public:

	CinemaEditorMsgHandler();
	virtual ~CinemaEditorMsgHandler();


	void OnMainFrameClose(CFrameWnd* wnd);

protected:

	CSingleDocTemplate* _cinemaEditorDocTemplate;
	CFrameWnd* _mainWnd;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnShowCinemaEditor();
	afx_msg void OnUpdateCinemaEditor(CCmdUI* ui);
};