#pragma once
#include "ICinemaTrack.h"

class _OgeEnginePluginExport ICinemaEventsListener
{
public:

	virtual void OnStart(Cinema* cinema) = 0;
	virtual void OnEnd(Cinema* cinema) = 0;
	virtual void OnStop(Cinema* cinema) = 0;
	virtual void OnPause(Cinema* cinema, bool pause) = 0;
	virtual void OnPlay(Cinema* cinema) = 0;
	virtual void OnReversePlay(Cinema* cinema) = 0;
	virtual void OnLoadStart(Cinema* cinema) = 0;
	virtual void OnLoadCancel(Cinema* cinema) = 0;
	virtual void OnLoadCompleted(Cinema* cinema) = 0;
	virtual void OnLoadProcess(Cinema* cinema, unsigned char percent) = 0;
};
//-----------------------------------------------------------------------------
class _OgeEnginePluginExport Cinema : public Ogre::AnimationAlloc
{
public:
	
	Cinema(const String& name, float length);
	virtual ~Cinema(void);

	static uint16 LoadingWorkQueueChannel;

	float GetLength() const;
	void SetLength(float l);

	TimeIndex _GetTimeIndex(float timePos) const;

	void _OnKeyFrameListChanged(void) { _keyFrameTimesDirty = true; }

	ICinemaEventsListener* GetListener() const { return _listener; }
	void SetListener(ICinemaEventsListener* val) { _listener = val; }


	void Load(bool forceSynchronous = false);
	void CancelLoad();
	void OnTrackLoadCompleted(ICinemaTrack* track);

	ICinemaTrack* CreateTrack(const String& typeName, IGameObject* object);
	size_t GetTrackCount() const { return _trackList.size(); }
	int GetTrackIndex(ICinemaTrack* track) const;
	ICinemaTrack* GetTrack(size_t idx) const;

	template<class T>
	T* CreateTrack(const String& typeName, IGameObject* object)
	{
		dynamic_cast<T*>(CreateTrack(typeName, object));
	}

	void DestroyTrack(size_t idx);
	void DestroyTrack(ICinemaTrack* track);


	void _CalcAutoLength();

	void AddTime(float deltaTime);

	void Play();
	void ReversePlay();
	void Pause(bool on);
	void Stop();
	void SetTimePosition(float timePos);
	float GetTimePosition() const { return _currentTimePosition; }
	void ToBegin();
	void ToEnd();

	bool IsPlaying() const { return _isPlaying; }

	bool GetLoopPlay() const { return _loopPlay; }
	void SetLoopPlay(bool val) { _loopPlay = val; }

	bool GetAutoPlayOnLoadCompleted() const { return _autoPlayOnLoadCompleted; }
	void SetAutoPlayOnLoadCompleted(bool val) { _autoPlayOnLoadCompleted = val; }

	const String& GetName() const { return _name; }
	void SetName(const String& val) { _name = val; }

	void EnableAll(bool on);

protected:

	String _name;
	float _totalLength;
	bool _isPlaying;
	bool _autoPlayOnLoadCompleted;
	float _currentTimePosition;
	// Sign for add time
	float _addTimeSign;

	typedef Ogre::vector<ICinemaTrack*>::type CinemaTrackList;	
	CinemaTrackList _trackList;

	 typedef vector<Real>::type KeyFrameTimeList;
	 mutable KeyFrameTimeList _keyFrameTimes;
	 mutable bool _keyFrameTimesDirty;

	 ICinemaEventsListener* _listener;

	 size_t _loadCounter;
	 bool _hasLoaded;
	 bool _loopPlay;

	 void _BuildKeyFrameTimeList(void) const;
};