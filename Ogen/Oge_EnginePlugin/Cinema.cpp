#include "StdAfx.h"
#include "Cinema.h"

Ogre::uint16 Cinema::LoadingWorkQueueChannel = 0;

Cinema::Cinema(const String& name, float length)
: _name(name)
, _totalLength(length)
, _keyFrameTimesDirty(false)
, _listener(0)
, _loadCounter(0)
, _isPlaying(false)
, _autoPlayOnLoadCompleted(true)
, _currentTimePosition(0.0f)
, _hasLoaded(false)
, _loopPlay(false)
, _addTimeSign(1.0f)
{
	WorkQueue* wq = Root::getSingleton().getWorkQueue();
	Cinema::LoadingWorkQueueChannel = wq->getChannel("Engine/Cinema");
}
//-----------------------------------------------------------------------------
Cinema::~Cinema(void)
{
}
//-----------------------------------------------------------------------------
float Cinema::GetLength() const
{
	return _totalLength;
}
//-----------------------------------------------------------------------------
void Cinema::SetLength( float l )
{
	_totalLength = l;
}
//-----------------------------------------------------------------------------
void Cinema::_CalcAutoLength()
{
	if (_keyFrameTimesDirty)
	{
		_BuildKeyFrameTimeList();
	}

	if(_keyFrameTimes.empty())
	{
		_totalLength = 0.0f;
		return;
	}

	_totalLength = _keyFrameTimes.back();
}
//-----------------------------------------------------------------------------
TimeIndex Cinema::_GetTimeIndex(float timePos) const
{
	// Uncomment following statement for work as previous
	//return timePos;

	// Build keyframe time list on demand
	if (_keyFrameTimesDirty)
	{
		_BuildKeyFrameTimeList();
	}

	// Wrap time
	Real totalAnimationLength = _totalLength;

	while ((timePos > totalAnimationLength) && (totalAnimationLength > 0.0f))
	{
		timePos -= totalAnimationLength;
	}

	// Search for global index
	KeyFrameTimeList::iterator it =
		std::lower_bound(_keyFrameTimes.begin(), _keyFrameTimes.end(), timePos);

	return TimeIndex(timePos, std::distance(_keyFrameTimes.begin(), it));
}
//-----------------------------------------------------------------------------
void Cinema::_BuildKeyFrameTimeList(void) const
{
	CinemaTrackList::const_iterator i;

	// Clear old keyframe times
	_keyFrameTimes.clear();

	// Collect all keyframe times from each track
	for (i = _trackList.begin(); i != _trackList.end(); ++i)
	{
		(*i)->_CollectKeyFrameTimes(_keyFrameTimes);
	}

	// Build global index to local index map for each track
	for (i = _trackList.begin(); i != _trackList.end(); ++i)
	{
		(*i)->_BuildKeyFrameIndexMap(_keyFrameTimes);
	}

	// Reset dirty flag
	_keyFrameTimesDirty = false;
}
//-----------------------------------------------------------------------------
void Cinema::Load( bool forceSynchronous /*= false*/ )
{
	_loadCounter = 0;
	_hasLoaded = true;

	if(_listener)
	{
		_listener->OnLoadStart(this);
	}

	CinemaTrackList::const_iterator i;
	for (i = _trackList.begin(); i != _trackList.end(); ++i)
	{
		ICinemaTrack* track = *i;
		if(track->NeedLoad())
		{
			(*i)->Load(forceSynchronous);
		}
		else
		{
			OnTrackLoadCompleted(track);
		}
	}
}
//-----------------------------------------------------------------------------
void Cinema::CancelLoad()
{
	_loadCounter = 0;
	WorkQueue* wq = Root::getSingleton().getWorkQueue();
	wq->abortRequestsByChannel(LoadingWorkQueueChannel);

	if(_listener)
	{
		_listener->OnLoadCancel(this);
	}

	_hasLoaded = false;
// 	CinemaTrackList::const_iterator i;
// 	for (i = _trackList.begin(); i != _trackList.end(); ++i)
// 	{
// 		(*i)->CancelLoad();
// 	}
}
//-----------------------------------------------------------------------------
void Cinema::OnTrackLoadCompleted( ICinemaTrack* track )
{
	++_loadCounter;
	if(_listener)
	{
		float r = (float)_loadCounter / (float)_trackList.size();
		r *= 100.0f;
		unsigned char percent = static_cast<unsigned char>(r);
		_listener->OnLoadProcess(this, percent);
	}

	if(_loadCounter == _trackList.size())
	{
		if(_listener)
		{
			_listener->OnLoadCompleted(this);
		}
		
		if(_autoPlayOnLoadCompleted)
		{
			//Play();
		}
	}
}
//-----------------------------------------------------------------------------
ICinemaTrack* Cinema::CreateTrack( const String& typeName, IGameObject* object )
{
	ICinemaTrackFactory* f = CinemaManager::getSingleton().GetTrackFactory(typeName);
	ICinemaTrack* track = f->Create(this, object);
	_trackList.push_back(track);
	if(track->NeedLoad() && !track->HasLoad())
	{
		_hasLoaded = false;
	}
	track->BuildPropertyMap();
	return track;
}
//-----------------------------------------------------------------------------
void Cinema::AddTime( float deltaTime )
{
	if(_isPlaying)
	{
		SetTimePosition(_currentTimePosition + deltaTime * _addTimeSign);
	}
}
//-----------------------------------------------------------------------------
void Cinema::Play()
{
	_isPlaying = true;
	//_currentTimePosition = 0.0f;
	_addTimeSign = 1.0f;
	if(_listener)
	{
		_listener->OnPlay(this);
	}

	if(!_hasLoaded)
	{
		Load(true);
	}
}
//-----------------------------------------------------------------------------
void Cinema::Pause( bool on )
{
	_isPlaying = !on;

	if(_listener)
	{
		_listener->OnPause(this, on);
	}
}
//-----------------------------------------------------------------------------
void Cinema::Stop()
{
	_isPlaying = false;

	if(_listener)
	{
		_listener->OnStop(this);
	}

	ToEnd();
}
//-----------------------------------------------------------------------------
void Cinema::SetTimePosition( float timePos )
{
	if(_currentTimePosition != timePos)
	{
		_currentTimePosition = timePos;

		if (_loopPlay)
		{
			// Wrap
			_currentTimePosition = fmod(_currentTimePosition, _totalLength);
			if(_currentTimePosition < 0)
			{
				_currentTimePosition += _totalLength;     
			}
		}
		else
		{
			// Clamp
			if(_currentTimePosition < 0)
			{
				_currentTimePosition = 0;
			}
			else if (_currentTimePosition > _totalLength)
			{
				_currentTimePosition = _totalLength;
			}
		}


		CinemaTrackList::const_iterator i;
		for (i = _trackList.begin(); i != _trackList.end(); ++i)
		{
			if((*i)->IsEnabled())
			{
				(*i)->SetTimePosition(_currentTimePosition);
			}
		}

		if((_addTimeSign > 0.0f) && (_currentTimePosition >= _totalLength))
		{
			_currentTimePosition = _totalLength;
			_isPlaying = false;
			if(_listener)
			{
				_listener->OnEnd(this);
			}
		}
		else if((_addTimeSign < 0.0f) && (_currentTimePosition <= 0.0f))
		{
			_currentTimePosition = 0.0f;
			_isPlaying = false;
			if(_listener)
			{
				_listener->OnEnd(this);
			}
		}

		if(GameApp::CurrentRunType == ERT_Editor)
		{
			EventsManager::getSingleton().CreateEvent(
				"CinemaTimeChange", Any(this));
		}
	}
}
//-----------------------------------------------------------------------------
void Cinema::ToBegin()
{
	SetTimePosition(0.0f);

	if(_listener)
	{
		_listener->OnStart(this);
	}
}
//-----------------------------------------------------------------------------
void Cinema::ToEnd()
{
	SetTimePosition(_totalLength);

	if(_listener)
	{
		_listener->OnEnd(this);
	}
}
//-----------------------------------------------------------------------------
ICinemaTrack* Cinema::GetTrack( size_t idx ) const
{
	if(idx < _trackList.size())
	{
		return _trackList[idx];
	}

	return 0;
}
//-----------------------------------------------------------------------------
int Cinema::GetTrackIndex( ICinemaTrack* track ) const
{
	for (size_t i = 0; i < _trackList.size(); ++i)
	{
		if(track == _trackList[i])
		{
			return i;
		}
	}

	return -1;
}
//-----------------------------------------------------------------------------
void Cinema::DestroyTrack( size_t idx )
{
	if(idx < _trackList.size())
	{
		ICinemaTrack* track = _trackList[idx];
		ICinemaTrackFactory* f = CinemaManager::getSingleton()
			.GetTrackFactory(track->GetTypeName());
		f->Destroy(track);
		_trackList.erase(_trackList.begin() + idx);
	}
}
//-----------------------------------------------------------------------------
void Cinema::DestroyTrack( ICinemaTrack* track )
{
	for (size_t i = 0; i < _trackList.size(); ++i)
	{
		if(track == _trackList[i])
		{
			ICinemaTrackFactory* f = CinemaManager::getSingleton()
				.GetTrackFactory(track->GetTypeName());
			f->Destroy(track);
			_trackList.erase(_trackList.begin() + i);

			return;
		}
	}
}
//-----------------------------------------------------------------------------
void Cinema::EnableAll( bool on )
{
	for (size_t i = 0; i < _trackList.size(); ++i)
	{
		_trackList[i]->SetEnable(on);
	}
}
//-----------------------------------------------------------------------------
void Cinema::ReversePlay()
{
	_isPlaying = true;
	_addTimeSign = -1.0f;

	if(_listener)
	{
		_listener->OnReversePlay(this);
	}

	if(!_hasLoaded)
	{
		Load(true);
	}
}
//-----------------------------------------------------------------------------
