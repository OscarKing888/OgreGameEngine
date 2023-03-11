#include "stdafx.h"
#include "TrackEditor_Camera.h"

TrackEditor_Camera::TrackEditor_Camera()
{

}
//--------------------------------------------------------------------------------
TrackEditor_Camera::~TrackEditor_Camera()
{

}
//--------------------------------------------------------------------------------
const String& TrackEditor_Camera::GetTypeName() const
{
	return CinemaTrack_CameraFactory::FactoryName;
}
//--------------------------------------------------------------------------------
bool TrackEditor_Camera::CanCreateKeyFrame( IGameObject* object )
{
	return true;
}
//--------------------------------------------------------------------------------
bool TrackEditor_Camera::NeedShowDlgWhenCreateKeyFrame() const
{
	return false;
}
//--------------------------------------------------------------------------------
void TrackEditor_Camera::EditKeyFrameWithDlg( ICinemaTrack* track, CinemaKeyFrame* kf )
{
	
}
//--------------------------------------------------------------------------------
