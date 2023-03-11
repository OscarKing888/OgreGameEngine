#include "stdafx.h"
#include "Cmd_AddTrack.h"
#include "CinemaEditorEvents.h"

Cmd_AddTrack::Cmd_AddTrack(
  IGameObject* obj, Cinema* cinema, const String& trackType,
  const NameValuePairList& parms, const CinemaKeyFrameDataList& keyframes,
  const ICinemaTrack::KeyFrameTipList& tips )
: _cinema(cinema)
, _trackType(trackType)
, _params(parms)
, _object(obj)
, _keyFrames(keyframes)
, _tips(tips)
{

}
//--------------------------------------------------------------------------------
Cmd_AddTrack::~Cmd_AddTrack( void )
{

}
//--------------------------------------------------------------------------------
ICommandXPtr Cmd_AddTrack::Execute()
{
	ICinemaTrack* track = _cinema->CreateTrack(_trackType, _object);
	track->SetValues(_params);
	track->SetTips(_tips);

	CinemaKeyFrameDataList::const_iterator it = _keyFrames.begin();
	for (; it != _keyFrames.end(); ++it)
	{
		CinemaKeyFrame* kf = track->CreateKeyFrame(it->Time);
		kf->FromString(it->Value);
	}

	EventsManager::getSingleton().CreateEvent(
		CinemaEditorEvents::UpdateUI_AddTrack, Any(_cinema));

	return OGRE_NEW Cmd_RemoveTrack(_object, _cinema, _cinema->GetTrackCount() - 1);
}
//--------------------------------------------------------------------------------
Cmd_RemoveTrack::Cmd_RemoveTrack( IGameObject* obj, Cinema* cinema, size_t idx )
: _object(obj)
, _cinema(cinema)
, _idx(idx)
{

}
//--------------------------------------------------------------------------------
Cmd_RemoveTrack::~Cmd_RemoveTrack( void )
{

}
//--------------------------------------------------------------------------------
ICommandXPtr Cmd_RemoveTrack::Execute()
{
	ICinemaTrack* track = _cinema->GetTrack(_idx);
	String trackType = track->GetTypeName();
	NameValuePairList pars = track->GetValues();
	ICinemaTrack::KeyFrameTipList tips = track->GetTips();
	
	CinemaKeyFrameDataList kfs;
	for (size_t i = 0; i < track->GetKeyFrameCount(); ++i)
	{
		CinemaKeyFrame* kf = track->GetKeyFrameByIndex(i);
		CinemaKeyFrameData kfData;
		kfData.Time = kf->GetTime();
		kfData.Value = kf->ToString();
		kfs.push_back(kfData);
	}

	_cinema->DestroyTrack(_idx);

	EventsManager::getSingleton().CreateEvent(
		CinemaEditorEvents::UpdateUI_RemoveTrack, Any(_cinema));

	return OGRE_NEW Cmd_AddTrack(_object, _cinema, trackType,
		pars, kfs, tips);
}