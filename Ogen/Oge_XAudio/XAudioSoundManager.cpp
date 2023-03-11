#include "StdAfx.h"
#include "XAudioSoundManager.h"
#include "XAudioSound.h"
#include "XAudioStreamSound.h"
#include <xaudio2.h>
#include <Dxerr.h>
#include <ShellAPI.h>

//#pragma comment(lib, "DxErr9.lib")
#pragma comment(lib, "DxErr.lib")

#ifdef _DEBUG
#	pragma comment(lib, "vorbis_static_d.lib")
#	pragma comment(lib, "vorbisfile_static_d.lib")
#	pragma comment(lib, "ogg_static_d.lib")
#else
#	pragma comment(lib, "vorbis_static.lib")
#	pragma comment(lib, "vorbisfile_static.lib")
#	pragma comment(lib, "ogg_static.lib")
#endif

static bool g_IsSoundDeviceOK = true;

namespace XAudio2Ex
{

#if XAUDIO_THREADED
boost::thread* XAudioSoundManager::_dataLoadingThread = 0;
bool XAudioSoundManager::_isShuttingDown = false;
#endif

class NullXAudioSound : public IXAudioSound
{
public:

	virtual void* GetXAudio2Voice(){ return 0; }
	virtual const String& GetFileName() const { return StringUtil::BLANK; }
	virtual const String& GetName() const { return StringUtil::BLANK; }
	virtual void Play(size_t operationSet = 0){}
	virtual void Stop(size_t operationSet = 0){}
	virtual void Pause(){}
	virtual bool IsPaused() const{ return false; }
	virtual void SetLoop(bool loop){}
	virtual bool IsLoop() const { return true; }
	virtual void SetOffset(double seconds){}
	virtual double GetOffset() const { return 0.0; }
	virtual void SetVolumn(float volumn){}
	virtual float GetVolumn() const { return 0.0f; }
	virtual int GetChannelCount() const{ return 1; }
	virtual void SetVolumn(int channelID, float volumn){}
	virtual float GetVolumn(int channelID) const { return 0.0f; }
	virtual void SetPitch(float ratio) {}
	virtual float GetPitch() const { return 1.0f; }
	virtual bool IsPlaying() const { return false; }
	virtual double GetLengthInSeconds() const { return 0.0; }
	virtual void BufferSounds(){}
	virtual bool IsStreamSound() const { return false; }
};

static NullXAudioSound g_NullXAudioSound;

//------------------------------------------------------------------------

XAudioSoundManager::XAudioSoundManager(void)
: _xaudio2(0)
, _masterVoice(0)
{
	char sysDir[OGE_MAX_PATH];
	::GetSystemDirectory(sysDir, OGE_MAX_PATH - 1);
	String xaudioDll = sysDir;
	xaudioDll += "\\XAudio2_0.dll";

	String cmdLine = "regsvr32 /s \"" + xaudioDll + "\"";
	LogSysIDf("Engine", "%s", cmdLine.c_str());

	::ShellExecute(NULL, NULL, cmdLine.c_str(), NULL, NULL, SW_HIDE);

	HRESULT hr = XAudio2Create(&_xaudio2);
	if(FAILED(hr))
	{
		LogErrorIDf("Engine", "XAudio2Create Failed[%x] %s", hr, DXGetErrorDescription(hr));
		//String strMsg("Can't create audio device, verify sound card driver is installed correctly:");
		//strMsg += DXGetErrorDescription(hr);
		//MessageBox_Error(strMsg);
		//_exit(0);
		g_IsSoundDeviceOK = false;
	}
	else
	{
		UINT deviceCnt = 0;
		_xaudio2->GetDeviceCount(&deviceCnt);
		UINT selectID = 0;
		for (UINT i = 0; i < deviceCnt; ++i)
		{
			XAUDIO2_DEVICE_DETAILS xdd;
			hr = _xaudio2->GetDeviceDetails(i, &xdd);

			//(SUCCEEDED(hr));
			LogSysIDf("Engine", "Audio device[%d] [%S] Channels:%d\n", i, xdd.DisplayName, xdd.OutputFormat.Format.nChannels);

			if(xdd.OutputFormat.Format.nChannels >= 2)
			{
				selectID = i;
			}
		}

		//hr = g_Audio2->CreateMasteringVoice(&g_MasteringVocie, XAUDIO2_DEFAULT_CHANNELS, 0, 0, selectID);
		hr = _xaudio2->CreateMasteringVoice(&_masterVoice);

		//_xaudio2->StartEngine();

		if(FAILED(hr))
		{
			LogErrorIDf("Engine", "CreateMasteringVoice Failed[%x] %s", hr, DXGetErrorDescription(hr));
			//MessageBox_Error("Can't create audio device, verify sound card driver is installed correctly.");
			g_IsSoundDeviceOK = false;
			//_exit(0);
		}

#if XAUDIO_THREADED
		// Kick off the update thread
		LogSysID("Engine", "Creating XAudio Thread");
		_dataLoadingThread = new boost::thread(boost::function0<void>(&XAudioSoundManager::ThreadUpdate));
#endif
	}
}

XAudioSoundManager::~XAudioSoundManager(void)
{
}

IXAudioSoundPtr XAudioSoundManager::CreateSound(const String& name, const String& fileName, bool isStreamSound)
{
	OGE_AUTO_SCOPED_LOCK;

	if(!g_IsSoundDeviceOK)
	{
		return &g_NullXAudioSound;
	}

	SoundMap::iterator i = _soundMap.find(name);
	if(i != _soundMap.end())
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
			"Sound name already exist [" + name + "]", "XAudioSoundManager:CreateSound");
	}

	IXAudioSoundPtr snd = 0;

	if(isStreamSound)
	{
		snd = OGRE_NEW XAudioStreamSound(name, fileName);
	}
	else
	{
		snd = OGRE_NEW XAudioSound(name, fileName);
	}
	
	_soundMap.insert(std::make_pair(name, snd));

	return snd;
}

IXAudioSoundPtr XAudioSoundManager::GetSound(const String& name)
{
	OGE_AUTO_SCOPED_LOCK;

	if(!g_IsSoundDeviceOK)
	{
		return &g_NullXAudioSound;
	}

	SoundMap::iterator i = _soundMap.find(name);
	if(i == _soundMap.end())
	{
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
			"Sound not found [" + name + "]", "XAudioSoundManager:GetSound");
	}

	return i->second;
}

bool XAudioSoundManager::HasSound(const String& soundName)
{
	OGE_AUTO_SCOPED_LOCK;
	SoundMap::iterator i = _soundMap.find(soundName);
	return i != _soundMap.end();
}


void XAudioSoundManager::ShutDown()
{
	//OGRE_DELETE _masterVoice;

#if XAUDIO_THREADED
	// Clean up the threading
//	LogInfo("Shutting Down XAudio Thread");
	StopAllSound();
	_isShuttingDown = true;
	if(_dataLoadingThread)
	{
		_dataLoadingThread->join();
		delete _dataLoadingThread;
		_dataLoadingThread = 0;
	}
#endif

	_pauseList.clear();
	SoundMap::iterator i = _soundMap.begin();
	for (; i != _soundMap.end(); ++i)
	{
		IXAudio2SourceVoice* voice = reinterpret_cast<IXAudio2SourceVoice*>(i->second->GetXAudio2Voice());
		voice->Stop(0);
		voice->DestroyVoice();
		OGRE_DELETE i->second;
	}

	//_xaudio2->StopEngine();
	if(_xaudio2)
	{
		if(_masterVoice)
		{
			_masterVoice->DestroyVoice();
		}

		_soundMap.clear();
		_xaudio2->Release();
		_masterVoice = 0;
		_xaudio2 = 0;
	}
}

void XAudioSoundManager::SetVolumn(float vol)
{
	if(_masterVoice)
	{
		_masterVoice->SetVolume(vol);
	}
}

bool XAudioSoundManager::SetSoundVolume(const String& soundName,float gain)
{
	GetSound(soundName)->SetVolumn(gain);
	return true;
}

float XAudioSoundManager::GetSoundPitch(const String& soundName)
{
	return GetSound(soundName)->GetPitch();
}

void XAudioSoundManager::SetSoundPitch(const String& soundName,float fValue)
{
	GetSound(soundName)->SetPitch(fValue);
}

void XAudioSoundManager::StopSound(const String& soundName)
{
	GetSound(soundName)->Stop();
}

void XAudioSoundManager::PauseAllSounds(void)
{
	OGE_AUTO_SCOPED_LOCK;

	_pauseList.clear();
	SoundMap::iterator i = _soundMap.begin();
	for (; i != _soundMap.end(); ++i)
	{
		IXAudioSoundPtr snd = i->second;
		if(snd->IsPlaying()) // 正在播放的暂停
		{
			snd->Pause();
			LogConsoleIDf("XAudio", "PauseAllSounds: Is playing [%s]", snd->GetName().c_str());
		}
		else if(snd->IsPaused()) // 暂停的记录下
		{
			_pauseList.push_back(snd);
			LogConsoleIDf("XAudio", "PauseAllSounds: Is paused [%s]", snd->GetName().c_str());
		}
	}
}

void XAudioSoundManager::ResumeAllSounds(void)
{
	OGE_AUTO_SCOPED_LOCK;

	SoundMap::iterator i = _soundMap.begin();
	for (; i != _soundMap.end(); ++i)
	{
		IXAudioSoundPtr snd = i->second;
		PauseSoundList::iterator it = std::find(_pauseList.begin(), _pauseList.end(), snd);
		if((it == _pauseList.end()) && snd->IsPaused()) // 没有记录的并且暂停的恢复播放
		{
			snd->Play();
			LogConsoleIDf("XAudio", "ResumeAllSounds sound [%s]", snd->GetName().c_str());
		}
	}

	_pauseList.clear();
}

void XAudioSoundManager::StopAllSound()
{
	OGE_AUTO_SCOPED_LOCK;

	SoundMap::iterator i = _soundMap.begin();
	for (; i != _soundMap.end(); ++i)
	{
		i->second->Stop();
	}

	_pauseList.clear();

	LogConsoleID("XAudio", "StopAllSound");
}

bool XAudioSoundManager::IsPlaying(const String& soundName)
{
	return GetSound(soundName)->IsPlaying();
}

bool XAudioSoundManager::IsPaused(const String& soundName)
{
	return GetSound(soundName)->IsPaused();
}

bool XAudioSoundManager::IsStoped(const String& soundName)
{
	return !GetSound(soundName)->IsPlaying();
}

float XAudioSoundManager::GetVolumn() const
{
	if(!g_IsSoundDeviceOK)
	{
		return 0.0f;
	}

	assert(_masterVoice);
	float v = 0.0f;
	_masterVoice->GetVolume(&v);
	return v;
}

void XAudioSoundManager::Play(const String& soundName)
{
	GetSound(soundName)->Play();
}

void XAudioSoundManager::BufferSounds()
{
	OGE_AUTO_SCOPED_LOCK;

	SoundMap::iterator i = _soundMap.begin();
	for (; i != _soundMap.end(); ++i)
	{
		i->second->BufferSounds();
	}
}

void XAudioSoundManager::StopAllStreamSound()
{
	SoundMap::iterator i = _soundMap.begin();
	for (; i != _soundMap.end(); ++i)
	{
		IXAudioSoundPtr snd = i->second;
		if(snd->IsStreamSound())
		{
			snd->Stop();
		}
	}
}

} //namespace