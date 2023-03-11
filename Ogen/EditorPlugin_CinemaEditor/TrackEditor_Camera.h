#pragma once
#include "ITrackEditor.h"

class TrackEditor_Camera : public ITrackEditor
{
public:

	TrackEditor_Camera();
	virtual ~TrackEditor_Camera();

	virtual const String& GetTypeName() const;
	virtual bool CanCreateKeyFrame(IGameObject* object);
	virtual bool NeedShowDlgWhenCreateKeyFrame() const;
	virtual void EditKeyFrameWithDlg(ICinemaTrack* track, CinemaKeyFrame* kf);

protected:

};