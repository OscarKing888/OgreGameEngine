#pragma once
#include "ITrackEditor.h"

class TrackEditor_LightColour : public ITrackEditor
{
public:

	TrackEditor_LightColour();
	virtual ~TrackEditor_LightColour();

	virtual const String& GetTypeName() const;
	virtual bool CanCreateKeyFrame(IGameObject* object);
	virtual bool NeedShowDlgWhenCreateKeyFrame() const;
	virtual void EditKeyFrameWithDlg(ICinemaTrack* track, CinemaKeyFrame* kf);
};
//--------------------------------------------------------------------------------
// TrackEditor_MainLightColour
//--------------------------------------------------------------------------------
class TrackEditor_MainLightColour : public TrackEditor_LightColour
{
public:

	TrackEditor_MainLightColour();
	virtual ~TrackEditor_MainLightColour();

	virtual const String& GetTypeName() const;
	virtual bool CanCreateKeyFrame(IGameObject* object);
};
//--------------------------------------------------------------------------------
// TrackEditor_AmbientLightColour
//--------------------------------------------------------------------------------
class TrackEditor_AmbientLightColour : public TrackEditor_LightColour
{
public:

	TrackEditor_AmbientLightColour();
	virtual ~TrackEditor_AmbientLightColour();

	virtual const String& GetTypeName() const;
	virtual bool CanCreateKeyFrame(IGameObject* object);
};
