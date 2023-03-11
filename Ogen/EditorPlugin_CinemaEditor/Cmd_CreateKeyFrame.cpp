#include "stdafx.h"
#include "Cmd_CreateKeyFrame.h"
#include "CinemaEditorEvents.h"

Cmd_CreateKeyFrame::Cmd_CreateKeyFrame(ICinemaTrack* track, float time, const String& val)
: _track(track)
, _time(time)
, _keyframeValue(val)
{

}
//--------------------------------------------------------------------------------
Cmd_CreateKeyFrame::~Cmd_CreateKeyFrame( void )
{

}
//--------------------------------------------------------------------------------
ICommandXPtr Cmd_CreateKeyFrame::Execute()
{
	CinemaKeyFrame* kf = _track->CreateKeyFrame(_time);
	kf->FromString(_keyframeValue);

	EventsManager::getSingleton().CreateEvent(
		CinemaEditorEvents::UpdateUI_CinemaCtrl, Any(_track));

	return OGRE_NEW Cmd_DeleteKeyFrame(_track, kf);
}
//--------------------------------------------------------------------------------
// Cmd_DeleteKeyFrame
//--------------------------------------------------------------------------------
Cmd_DeleteKeyFrame::Cmd_DeleteKeyFrame( ICinemaTrack* track, CinemaKeyFrame* kf )
: _track(track)
, _kf(kf)
{

}
//--------------------------------------------------------------------------------
Cmd_DeleteKeyFrame::~Cmd_DeleteKeyFrame( void )
{

}
//--------------------------------------------------------------------------------
ICommandXPtr Cmd_DeleteKeyFrame::Execute()
{
	float t = _kf->GetTime();
	String val = _kf->ToString();

	_track->DestroyKeyFrame(_kf);

	EventsManager::getSingleton().CreateEvent(
		CinemaEditorEvents::UpdateUI_CinemaCtrl, Any(_track));

	return OGRE_NEW Cmd_CreateKeyFrame(_track, t, val);
}