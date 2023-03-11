#pragma once
#include "ITrackEditor.h"

class TrackEditor_FadeScreen : public ITrackEditor
{
public:

	TrackEditor_FadeScreen();
	virtual ~TrackEditor_FadeScreen();

	virtual const String& GetTypeName() const;
	virtual bool CanCreateKeyFrame(IGameObject* object);
	virtual bool NeedShowDlgWhenCreateKeyFrame() const;
	virtual void EditKeyFrameWithDlg(ICinemaTrack* track, CinemaKeyFrame* kf);

protected:

};