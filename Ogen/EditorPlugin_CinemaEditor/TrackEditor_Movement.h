#pragma once
#include "ITrackEditor.h"

class TrackEditor_Movement : public ITrackEditor
{
public:

	TrackEditor_Movement();
	virtual ~TrackEditor_Movement();

	virtual const String& GetTypeName() const;
	virtual bool CanCreateKeyFrame(IGameObject* object);
	virtual bool NeedShowDlgWhenCreateKeyFrame() const;
	virtual void EditKeyFrameWithDlg(ICinemaTrack* track, CinemaKeyFrame* kf);

protected:

};