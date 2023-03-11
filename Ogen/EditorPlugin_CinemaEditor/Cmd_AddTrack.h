#pragma once

struct CinemaKeyFrameData
{
	float Time;
	String Value;
};

typedef Ogre::vector<CinemaKeyFrameData>::type CinemaKeyFrameDataList;

class Cmd_AddTrack : public ICommandX
{
public:

	Cmd_AddTrack(IGameObject* obj, Cinema* cinema, const String& trackType,
		const NameValuePairList& parms, const CinemaKeyFrameDataList& keyframes,
		const ICinemaTrack::KeyFrameTipList& tips);
	virtual ~Cmd_AddTrack(void);

	virtual ICommandXPtr Execute();

protected:

	Cinema* _cinema;
	IGameObject* _object;
	String _trackType;
	NameValuePairList _params;
	CinemaKeyFrameDataList _keyFrames;
	ICinemaTrack::KeyFrameTipList _tips;
};
//--------------------------------------------------------------------------------
class Cmd_RemoveTrack : public ICommandX
{
public:

	Cmd_RemoveTrack(IGameObject* obj, Cinema* cinema, size_t idx);
	virtual ~Cmd_RemoveTrack(void);

	virtual ICommandXPtr Execute();

protected:

	IGameObject* _object;
	Cinema* _cinema;
	size_t _idx;
};
