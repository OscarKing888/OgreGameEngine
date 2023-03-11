#pragma once

class ITrackEditor
{
public:

	virtual const String& GetTypeName() const = 0;
	virtual bool CanCreateKeyFrame(IGameObject* object) = 0;
	virtual bool NeedShowDlgWhenCreateKeyFrame() const = 0;
	virtual void EditKeyFrameWithDlg(ICinemaTrack* track, CinemaKeyFrame* kf) = 0;
};

struct SRegTrackEditorHelper
{
	SRegTrackEditorHelper(ITrackEditor* editor);
};

#define M_REGEDIT_TRACKEDITOR(x) static x __##x; static SRegTrackEditorHelper __##x##RegHelper(&__##x);