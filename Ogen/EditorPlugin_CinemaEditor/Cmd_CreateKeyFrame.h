#pragma once

class Cmd_CreateKeyFrame : public ICommandX
{
public:

	Cmd_CreateKeyFrame(ICinemaTrack* track, float time, const String& val);
	virtual ~Cmd_CreateKeyFrame(void);

	virtual ICommandXPtr Execute();

protected:

	ICinemaTrack* _track;
	float _time;
	String _keyframeValue;
};
//--------------------------------------------------------------------------------
class Cmd_DeleteKeyFrame : public ICommandX
{
public:

	Cmd_DeleteKeyFrame(ICinemaTrack* track, CinemaKeyFrame* kf);
	virtual ~Cmd_DeleteKeyFrame(void);

	virtual ICommandXPtr Execute();

protected:

	ICinemaTrack* _track;
	CinemaKeyFrame* _kf;
};