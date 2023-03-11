#pragma once
#include "EnginePrerequisites.h"
#include "XAudioPrerequisites.h"
#include "IXAudioSound.h"

#pragma pack(push)
// warning C4099: 'IXAudio2' : type name first seen using 'struct' now seen using 'class'
//#pragma warning(disable : 4099)
struct IXAudio2;
struct IXAudio2MasteringVoice;
#pragma pack(pop)

namespace XAudio2Ex
{
	class _EngineExport XAudioSoundManager : public SingletonMyers<XAudioSoundManager>
	{
	public:
		
		XAudioSoundManager(void);
		virtual ~XAudioSoundManager(void);

		IXAudioSoundPtr CreateSound(const String& name, const String& fileName, bool isStreamSound);
		IXAudioSoundPtr GetSound(const String& name);

		void ShutDown();

		IXAudio2* GetXAudio2() { return _xaudio2; }
		IXAudio2MasteringVoice* GetMasteringVoice() { return _masterVoice; }

		void SetVolumn(float vol);
		bool SetSoundVolume(const String& soundName,float gain);
		virtual float GetVolumn() const;

		float GetSoundPitch(const String& soundName);
		void SetSoundPitch(const String& soundName,float fValue);

		void Play(const String& soundName);
		void StopSound(const String& soundName);
		void PauseAllSounds(void);
		void ResumeAllSounds(void);
		void StopAllSound();
		void StopAllStreamSound();

		bool IsPlaying(const String& soundName);
		bool IsPaused(const String& soundName);
		bool IsStoped(const String& soundName);
		bool HasSound(const String& soundName);

		void BufferSounds();

	protected:

		typedef std::map<String, IXAudioSoundPtr>	SoundMap;
		SoundMap	_soundMap;

		IXAudio2*	_xaudio2;
		IXAudio2MasteringVoice*	_masterVoice;

		typedef std::vector<IXAudioSoundPtr> PauseSoundList;
		PauseSoundList		_pauseList;

		OGE_AUTO_MUTEX;

#if XAUDIO_THREADED		
		static boost::thread* _dataLoadingThread;
		static bool _isShuttingDown;

		static void ThreadUpdate()
		{
			while(!_isShuttingDown)
			{
				XAudioSoundManager::getSingleton().BufferSounds();
				_dataLoadingThread->yield();
				::Sleep(1);
			}
		}
#endif
	};
} // namespace