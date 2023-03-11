#pragma once
#include "ITrackEditor.h"

class TrackEditor_Property : public ITrackEditor
{
public:

	TrackEditor_Property();
	virtual ~TrackEditor_Property();

	virtual const String& GetTypeName() const;
	virtual bool CanCreateKeyFrame(IGameObject* object);
	virtual bool NeedShowDlgWhenCreateKeyFrame() const;
	virtual void EditKeyFrameWithDlg(ICinemaTrack* track, CinemaKeyFrame* kf);

protected:

};