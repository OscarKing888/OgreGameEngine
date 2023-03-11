#pragma once
#include "EnginePrerequisites.h"

///
/// Root sound interface
///
class _EngineExport IGameSound
{
public:
	
	/// Get the sound id
	virtual const String& GetName() const = 0;
	/// Get the file name
	virtual const String& GetFileName() const = 0;
	/// Play from current time position
	virtual void Play() const = 0;
	/// Start play and fade in with duration(seconds)
	virtual void StartFadeIn(float duration) = 0;
	/// Fade out with duration(seconds) and stop
	virtual void StartFadeOut(float duration) = 0;
	/// Set the offset time to play
	virtual void SetOffset(float duration) = 0;
	/// Return the current offset time
	virtual float GetOffset() const = 0;
	/// Return the channel count
	virtual int GetChannelCount() const = 0;
	/// Set volumn with channel index, -1 for all channels
	virtual void SetVolumn(float volumn, int channel = -1) = 0;
	/// Return the volumn with channel index, -1 for master volumn
	virtual float GetVolumn(int channel = -1) const = 0;
	/// Pause sound
	virtual void Pause() = 0;
	/// Return pause status
	virtual bool IsPaused() const = 0;
	/// Set loop play mode
	virtual void SetLoop(bool loop) = 0;
	/// Return loop play mode
	virtual bool IsLoop() const = 0;
};

//--------------------------------------------------------------------------------
///
///  3D sound object interface
///
class _EngineExport IGameSound3D : public IGameSound
{
public:

	/// Set the sound position in scene
	virtual void SetPosition(const Ogre::Vector3& pos) = 0;
	/// Get sound position
	virtual const Ogre::Vector3& GetPosition() const = 0;
};

// Stream sound for playing long/large music sound file

//--------------------------------------------------------------------------------
///
/// Sound manager for create sound source
///
class _EngineExport SoundManager
	: public Singleton<SoundManager>
	, public OgeAlloc
{
public:

	/// Create a 2D sound(music) from file
	virtual IGameSound* CreateSound(const String& name, const String& fileName) = 0;
	/// Create a 3D sound from file
	virtual IGameSound3D* CreateSound3D(const String& name, const String& fileName) = 0;
	/// Destroy sound object
	virtual void DestroySound(const String& name) = 0;
	virtual void DestroySound(IGameSound* snd) = 0;
	/// Destroy 3D sound object
	virtual void DestroySound3D(const String& name) = 0;
	virtual void DestroySound3D(IGameSound3D* snd) = 0;
	/// Set the master voice volumn
	virtual void SetMasterVolumn(float vol) = 0;
	/// Return the master voice volumn
	virtual float GetMasterVolumn() const = 0;
	/// Shutdown the sound system
	virtual void ShutDown() = 0;
	/// Pre-load a sound file into memory
	virtual void PreLoadSoundFile(const String& fileName) = 0;
	/// Pause all sounds
	virtual void PauseAll(bool pause) = 0;
};
