#include "stdafx.h"
#include "TrackEditor_Movement.h"

TrackEditor_Movement::TrackEditor_Movement()
{

}
//--------------------------------------------------------------------------------
TrackEditor_Movement::~TrackEditor_Movement()
{

}
//--------------------------------------------------------------------------------
const String& TrackEditor_Movement::GetTypeName() const
{
	return CinemaTrack_MovementFactory::FactoryName;
}
//--------------------------------------------------------------------------------
bool TrackEditor_Movement::CanCreateKeyFrame( IGameObject* object )
{
	VisualObject* obj = dynamic_cast<VisualObject*>(object);
	return obj != 0;
}
//--------------------------------------------------------------------------------
bool TrackEditor_Movement::NeedShowDlgWhenCreateKeyFrame() const
{
	return false;
}
//--------------------------------------------------------------------------------
void TrackEditor_Movement::EditKeyFrameWithDlg( ICinemaTrack* track, CinemaKeyFrame* kf )
{
	CinemaKeyFrame_Transform* k = dynamic_cast<CinemaKeyFrame_Transform*>(kf);
}
//--------------------------------------------------------------------------------
