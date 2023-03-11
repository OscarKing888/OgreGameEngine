// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-10-21
// *************************************************************************************

// #include <al.h>
// #include <alc.h>
#include "stdafx.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "GameApp.h"
#include <MMSystem.h>

#if OGE_USE_OPENAL
#	include <OgreAL.h>
#else
#	include "XAudioSoundManager.h"
#	include "IXAudioSound.h"

template<> SoundManager* Singleton<SoundManager>::ms_Singleton = 0;
#endif

//#pragma comment(lib, "winmm.lib")

//HWAVEOUT g_hwo;





	
	SoundManager::~SoundManager()
	{
		SetSoundVolume();
		//waveOutClose(g_hwo);		
	}

	void SoundManager::SetSoundVolume(int rightChannelVol, int leftChannelVol)
	{
		//DWORD v = ((DWORD)(rightChannelVol & 0xffff) << 16) | ((DWORD)leftChannelVol);
		//waveOutSetVolume(g_hwo, v); 
	}

	void SoundManager::LoadDirect(const String& xmlFile)
	{
		XMLDoc doc;
		if(doc.loadFromFile(xmlFile))
		{
			XMLElement e = doc.getRoot();
			_LoadConfig(e);
		}
		else
		{
			LogErrorIDf("Engine", ("[SoundManager::LoadDirect]Error xml file format:\"%s\""),
				xmlFile.c_str());
			assert(false);
		}
	}

	void SoundManager::LoadFromResourceSystem(const String& xmlFile)
	{
		DataStreamPtr dataStreamPtr =
			ResourceGroupManager::getSingleton().openResource(xmlFile);

		LoadFromXML(dataStreamPtr->getAsString());
	}

	void SoundManager::LoadFromXML(const String& xmlString)
	{
		XMLDoc doc;
		if(doc.loadFromXML(xmlString.c_str()))
		{
			XMLElement e = doc.getRoot();
			_LoadConfig(e);
		}
		else
		{
			LogErrorIDf("Engine", ("[SoundManager::LoadFromXML]\
					   Error xml string format:\"%s\""),
					   xmlString.c_str());
			assert(false);
		}
	}

	void SoundManager::_LoadConfig(XMLElement& rootNode)
	{
		XMLElement e = rootNode.firstChild();
		while(!e.isNull())
		{
			SSoundConfig cfg;
			cfg.Name = e.getAttribute(("Name"));
			cfg.FileName = e.getAttribute(("FileName"));
			cfg.IsLoop = getBoolAttribuate(("IsLoop"), e);
			cfg.Volume = getFloatAttribuate(("Volume"), e);
			String file, ext;
			StringUtil::splitBaseFilename(cfg.FileName, file, ext);
			StringUtil::toLowerCase(ext);
			cfg.IsStream = (ext == "ogg");/*GetBoolAttribuate(("IsStream"), e, false);*/

			AddConfig(cfg);

			e = e.nextSibling();
		}
	}


	void SoundManager::AddConfig(const SSoundConfig& cfg)
	{
		SSoundConfig c = cfg;
		//c.Name = c.Name;		
		_soundCfgMap[c.Name] = c;
	}

	bool SoundManager::GetConfig(const String& soundName, SSoundConfig& cfg)
	{
		if (soundName.c_str()==NULL)
		{
			return false;
		}

		SoundConfigMap::iterator i = _soundCfgMap.find(soundName);
		if(i == _soundCfgMap.end() )
		{
			return false;
		}

		cfg = i->second;
		//cfg.FileName  = i->second.FileName;
		//cfg.IsLoop = i->second.IsLoop;
		//cfg.IsStream = i->second.IsStream;
		//cfg.Name = i->second.Name;
		////cfg.Name = i->second.Name;
		//cfg.Volume = i->second.Volume;
		return true;
	}


#if OGE_USE_OPENAL
	SoundManager::SoundManager()
	{
		OGRE_NEW OgreAL::SoundManager();
		_effectManager = OGRE_NEW EffectManager();
		GameApp::getSingleton().AddUpdateable(_effectManager);
		_autoID = 0;
		SetListenerGain(0.5);

		WAVEFORMATEX wf;
		wf.wFormatTag = WAVE_FORMAT_PCM;
		wf.nChannels = 2;
		wf.nSamplesPerSec = 8000 * 1000;
		wf.wBitsPerSample = 8;
		wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
		wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
		wf.cbSize = 0;

		HWAVEOUT hwo;
		UINT c = waveOutGetNumDevs();
		for (UINT id = 0; id < c; id++)
		{
			if (waveOutOpen(&g_hwo, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL) 
				== MMSYSERR_NOERROR) 
			{
				break;
			}
		}

//  		void* p = (void*)alGetProcAddress("alf_panning");	
// 		if(p)
// 		{
// 			LogConsole("panning");
// 		}
// 
// 		ALloadEFX();
	}
	
	void SoundManager::ShutDown()
	{
		SoundMap::iterator i = _soundMap.begin();
		while (i!=_soundMap.end())
		{
			String soundName = i->first;
			_StopSound(soundName);
			//i = _soundMap.begin();
			++i;
		}
		_soundMap.clear();
		_effectManager->RemoveAll();
		GameApp::getSingleton().RemoveUpdateable(_effectManager);
		OGRE_DELETE OgreAL::SoundManager::getSingletonPtr();
	}

	
	void SoundManager::ResetSound(const String& soundName)
	{
		SSoundConfig cfg;
		if (!this->GetConfig(soundName,cfg) )
		{
			return;
		}

		OgreAL::Sound* snd = GetSoundImp(cfg.Name);
		if(snd)
		{
			snd->setSecondOffset(0.0f);
		}
	}
	
	OgreAL::Sound* SoundManager::GetSoundImp(const String& soundName)
	{
		SoundMap::iterator i = _soundMap.find(soundName);
		if(i != _soundMap.end())
		{
			return i->second;
		}
		return NULL;
	}

	void SoundManager::StopSound(const String& soundName, float fadeOutTime)
	{
		_StopSound(soundName);
		//SoundMap::iterator i = _soundMap.find(soundName);
		//if(i != _soundMap.end())
		//{
		//	if (fadeOutTime > 0.0f)
		//		GetEffectManager().AddEffect(OGRE_NEW SoundEffect_FadeOut(soundName, fadeOutTime));
		//	else
		//	{				
		//		//SceneNode *node = i->second->getParentSceneNode();
		//		//if (node)
		//		//{
		//		//	node->detachObject(i->second);
		//		//}
		//		//try 
		//		//{
		//		//	OgreAL::SoundManager::getSingleton().destroySound(i->second);
		//		//}
		//		//catch (...)
		//		//{
		//		//}
		//		//_soundMap.erase(i);
		//	}
		//}
	}

	void SoundManager::_StopSound(const String& soundName)
	{
		SoundMap::iterator i = _soundMap.find(soundName);
		if(i != _soundMap.end())
		{
			i->second->stop();
		}
	}

	void SoundManager::StopAllSound()
	{
		SoundMap::iterator i = _soundMap.begin();
		for (; i != _soundMap.end(); ++i)
		{
			i->second->stop();
		}
	}

	Real SoundManager::GetSoundLength(const String& soundName)
	{
		OgreAL::Sound*   pSound =  GetSoundImp(soundName);
		if (pSound)
		{
			return pSound->getSecondDuration();
		}
		else
		{
			return -1.0f;
		}
	}
		

	Real SoundManager::GetSoundCurPos(const String& soundName)
	{
		OgreAL::Sound*   pSound =  GetSoundImp(soundName);
		if (pSound)
		{
			return pSound->getSecondOffset();
		}
		else
		{
			return -1.0f;
		}
	}

	bool   SoundManager::SetSoundVolume(const String& soundName,float gain)
	{
			SSoundConfig ss;

			if (false==this->GetConfig(soundName,ss) )
			{
				return false;
			}

			OgreAL::Sound* pSnd = NULL;
			pSnd = this->GetSoundImp(ss.Name);
			if (pSnd)
			{
				try 
				{
					pSnd->setGain(ss.Volume * gain);
					//pSnd->play();
				}
				catch (...) 
				{
					return false;
				}
			}

			return true;
	}
	 
	bool SoundManager::SetSoundVolumeRange(const String& soundName, float minGain, float maxGain)
	{
		SSoundConfig ss;

		if (false==this->GetConfig(soundName,ss) )
		{
			return false;
		}

		OgreAL::Sound* pSnd = NULL;
		pSnd = this->GetSoundImp(ss.Name);
		if (pSnd)
		{
			try 
			{
				pSnd->setMaxGain(maxGain);
				pSnd->setMinGain(minGain);
			}
			catch (...) 
			{
				return false;
			}
		}

		return true;
	}

	bool SoundManager::SetSoundCurPos(const String& soundName,Real seconds)
	{
		SSoundConfig ss;

		if (false==this->GetConfig(soundName,ss) )
		{
			return false;
		}

		OgreAL::Sound*   pSound =  GetSoundImp(ss.Name);
		if (pSound==NULL)
		{
			return false;
		}

		if (seconds > GetSoundLength(ss.Name) )
		{
			return false;
		}

		pSound->setSecondOffset(seconds);
		return true;
	}


	float SoundManager::GetSoundPitch(const String& soundName)
	{
		OgreAL::Sound*   pSound =  GetSoundImp(soundName);
		if (pSound==NULL)
		{
			return -1;
		}

		return pSound->getPitch();
	}

	void SoundManager::SetSoundPitch(const String& soundName, float fValue)
	{
		SSoundConfig cfg;
		GetConfig(soundName, cfg);

		OgreAL::Sound*   pSound =  GetSoundImp(cfg.Name);
		if (pSound==NULL)
		{
			return;
		}
		
		pSound->setPitch(fValue);
	}

	void SoundManager::SetLoop(const String& soundName,bool bFlag)
	{
		OgreAL::Sound*   pSound =  GetSoundImp(soundName);
		if (pSound==NULL)
		{
			return;
		}

		pSound->setLoop(bFlag);
	}
	
	bool SoundManager::PauseSound(const String& soundName)
	{
		SSoundConfig cfg;
		GetConfig(soundName, cfg);
		OgreAL::Sound*   pSound =  GetSoundImp(cfg.Name);
		if (pSound==NULL)
		{
			return false;
		}

		return  pSound->pause();
	}

	bool SoundManager::IsPlaying(const String& soundName)
	{
		SSoundConfig cfg;
		if (!this->GetConfig(soundName,cfg) )
		{
			return false;
		}

		OgreAL::Sound*   pSound =  GetSoundImp(cfg.Name);
		if (pSound==NULL)
		{
			return false;
		}

		return  pSound->isPlaying();
	}

	bool SoundManager::IsPaused(const String& soundName)
	{
		OgreAL::Sound*   pSound =  GetSoundImp(soundName);
		if (pSound==NULL)
		{
			return false;
		}

		return  pSound->isPaused();
	}


	bool   SoundManager::IsStoped(const String& soundName)
	{
		OgreAL::Sound*   pSound =  GetSoundImp(soundName);
		if (pSound==NULL)
		{
			return false;
		}

		return  pSound->isStopped();
	}
	
	//////////////////////////////////////////////////////////////////////////

	void SoundManager::PauseAllSounds(void)
	{
		OgreAL::SoundManager::getSingleton().pauseAllSounds();
	}

	void SoundManager::ResumeAllSounds(void)
	{
		OgreAL::SoundManager::getSingleton().resumeAllSounds();
	}

	void SoundManager::SetListenerPositionDirection(const Vector3& position, const Vector3& direction)
	{
		OgreAL::SoundManager::getSingleton().getListener()->setPosition(position);
		OgreAL::SoundManager::getSingleton().getListener()->setDirection(direction);
		OgreAL::SoundManager::getSingleton().setDopplerFactor(0.5f);
	}

	void SoundManager::GetListenerPositionDirection(Vector3& position, Vector3& direction)
	{
		position = OgreAL::SoundManager::getSingleton().getListener()->getPosition();
		direction = OgreAL::SoundManager::getSingleton().getListener()->getDirection();
	}

	void SoundManager::SetListenerGain(float gain)
	{
		OgreAL::SoundManager::getSingleton().getListener()->setGain(gain);
	}

	float SoundManager::GetListenerGain(void)
	{
		return OgreAL::SoundManager::getSingleton().getListener()->getGain();
	}

	void SoundManager::AttachSoundToNode(const String& soundName, SceneNode * node)
	{
		OgreAL::Sound *pSnd = GetSoundImp(soundName);
		if( node && pSnd)
		{
			node->attachObject(pSnd);
		}
	}

	//String SoundManager::Play(const String& name, float gain , float fadeInTime, Vector3 position)
	//{
	//	OgreAL::Sound* pSnd = 0;
	//	SoundConfigMap::iterator i = _soundCfgMap.find(name);
	//	if(i != _soundCfgMap.end())
	//	{
	//		SSoundConfig& cfg = i->second;
	//		String sndID = FormatString(("%s[%d]"), cfg.Name.c_str(), _autoID);
	//		++_autoID;

	//		int nSoundType=-1;
	//		if (cfg.FileName.find(".ogg")!=String::npos )
	//		{
	//			nSoundType =1;
	//		}


	//		if (cfg.FileName.find(".wav")!=String::npos )
	//		{
	//			nSoundType =2;
	//		}

	//		pSnd = OgreAL::SoundManager::getSingleton()
	//			.createSound(sndID, cfg.FileName,nSoundType ,cfg.IsLoop, cfg.IsStream);

	//		//pSnd = OgreAL::SoundManager::getSingleton()
	//		//		.createSound(sndID, cfg.FileName, cfg.IsLoop, cfg.IsStream);

	//		if (pSnd)
	//		{
	//			_soundMap[sndID] = pSnd;
	//			if (!position.isZeroLength())
	//			{
	//				pSnd->setPosition(position);
	//			}
	//			if (fadeInTime > 0.0f)
	//			{
	//				_effectManager->AddEffect(OGRE_NEW SoundEffect_FadeIn(sndID, fadeInTime, cfg.Volume * gain));
	//			}
	//			else
	//			{
	//				try 
	//				{
	//					pSnd->setGain(cfg.Volume * gain);
	//					pSnd->play();
	//				}
	//				catch (...)
	//				{
	//				}
	//			}
	//		}

	//		// 非循环播放并且指定自动控制生存期
	//		if(pSnd && !cfg.IsLoop)
	//		{
	//			_effectManager->AddEffect(OGRE_NEW SoundLifeController(sndID));
	//		}
	//		return sndID;
	//	}
	//	else
	//	{
	//		LogErrorf(("[SoundManager::PlaySound] '%s' not found, check sound config file."), name.c_str());
	//		assert(false);
	//	}		
	//	return ("");	
	//}

	void SoundManager::SetSoundPosition(const String& soundName, const Vector3& pos)
	{
		SSoundConfig cfg;
		if (!this->GetConfig(soundName,cfg) )
		{
			return;
		}

		OgreAL::Sound* pSnd = NULL;
		pSnd = this->GetSoundImp(cfg.Name);

		if (pSnd)
		{
			pSnd->setPosition(pos);
			/*pSnd->setReferenceDistance(100.0f);
			pSnd->setInnerConeAngle(10);
			pSnd->setOuterConeAngle(180);
			pSnd->setOuterConeGain(0.2);
			pSnd->setRelativeToListener(false);*/
		}
	}

	bool SoundManager::Play(const String& name, float gain , float fadeInTime, Vector3 position)
	{
		if (name.c_str()==NULL)
		{
			return false;
		}

		SSoundConfig cfg;
		memset(&cfg,0,sizeof(SSoundConfig) );
		if (!this->GetConfig(name,cfg) )
		{
			return false;
		}

		OgreAL::Sound* pSnd = NULL;
		pSnd = this->GetSoundImp(cfg.Name);

		if (!pSnd)
		{
			pSnd = OgreAL::SoundManager::getSingleton()
				.createSound(cfg.Name, cfg.FileName, cfg.IsLoop, cfg.IsStream);
			_soundMap[cfg.Name] = pSnd;
			pSnd->setGain(cfg.Volume * gain);
		}

		if (pSnd)
		{
			if (!position.isZeroLength())
			{
				pSnd->setPosition(position);
			}

			pSnd->setGain(cfg.Volume * gain);
			pSnd->play();

// 			if (fadeInTime > 0.0f)
// 			{
// 				FadeInSound(name, fadeInTime);
// 				//_effectManager->AddEffect(OGRE_NEW SoundEffect_FadeIn(cfg.Name, fadeInTime, cfg.Volume * gain));
// 				return true;
// 			}
// 			else
// 			{
// 				try 
// 				{
// 					pSnd->setGain(cfg.Volume * gain);
// 					pSnd->play();
// 
// 					// 非循环播放并且指定自动控制生存期
// 					if(pSnd && !cfg.IsLoop)
// 					{
// 						_effectManager->AddEffect(OGRE_NEW SoundLifeController(cfg.Name));
// 					}
// 
// 					return true;
// 
// 				}
// 				catch (...)
// 				{
// 					return false;
// 				}
// 			}

			return true;
		}
		else
		{
			return false;
		}
	}

	void SoundManager::FadeInSound(const String& soundName, float duartion)
	{
		SSoundConfig cfg;
		if (!this->GetConfig(soundName,cfg) )
		{
			return;
		}

		OgreAL::Sound* pS = GetSoundImp(cfg.Name);
		if(pS == 0)
		{
			Play(soundName);
			pS = GetSoundImp(cfg.Name);
			pS->stop();
		}

		pS->fadeIn(duartion);
	}

	void SoundManager::FadeOutSound(const String& soundName, float duartion)
	{
		SSoundConfig cfg;
		if (!this->GetConfig(soundName,cfg) )
		{
			return;
		}

		OgreAL::Sound* pS = GetSoundImp(cfg.Name);
		if(pS != 0)
		{
			pS->fadeOut(duartion);
		}
	}

	//-------------------------------------------------------------------------------------------------

	SoundLifeController::SoundLifeController(const String& soundName)
		: _soundName(soundName)
	{
	}

	SoundLifeController::~SoundLifeController()
	{
		SoundManager::getSingleton().StopSound(_soundName);
	}

	bool SoundLifeController::IsDead() const
	{
		OgreAL::Sound *pSnd = SoundManager::getSingleton().GetSoundImp(_soundName);
		return !pSnd || pSnd->isStopped();
	}

	void SoundLifeController::Init()
	{
	}

	void SoundLifeController::Update(float deltaTime)
	{
	}
	
#else

using namespace XAudio2Ex;

//-------------------------------------------------------------------------------------------------

SoundManager::SoundManager()
{
	XAudioSoundManager::getSingleton();
	_effectManager = OGRE_NEW EffectManager();
	GameApp::getSingleton().AddUpdateable(_effectManager);
	//_autoID = 0;
	//SetListenerGain(0.5);

// 	WAVEFORMATEX wf;
// 	wf.wFormatTag = WAVE_FORMAT_PCM;
// 	wf.nChannels = 2;
// 	wf.nSamplesPerSec = 8000 * 1000;
// 	wf.wBitsPerSample = 8;
// 	wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
// 	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
// 	wf.cbSize = 0;
// 
// 	UINT c = waveOutGetNumDevs();
// 	for (UINT id = 0; id < c; id++)
// 	{
// 		if (waveOutOpen(&g_hwo, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL) 
// 			== MMSYSERR_NOERROR) 
// 		{
// 			break;
// 		}
// 	}
}

void SoundManager::ShutDown()
{
	GameApp::getSingleton().RemoveUpdateable(_effectManager);
	_effectManager->RemoveAll();
	XAudioSoundManager::getSingleton().ShutDown();
	OGRE_DELETE this;
}


void SoundManager::ResetSound(const String& soundName)
{	
	IXAudioSound* snd = GetSoundImp(soundName);
	snd->SetOffset(0.0);
}

IXAudioSound* SoundManager::GetSoundImp(const String& soundName)
{
	if(!XAudioSoundManager::getSingleton().HasSound(soundName))
	{
		SSoundConfig cfg;
		memset(&cfg,0,sizeof(SSoundConfig) );
		if (!this->GetConfig(soundName, cfg) )
		{
			//assert(false && "Sound config not found!");
			LogWarningIDf("Engine", "Sound config not found [%s], create as non-stream sound!", soundName.c_str());
			IXAudioSound* snd = XAudioSoundManager::getSingleton().CreateSound(soundName, soundName, false);
			snd->SetLoop(false);
			snd->SetVolumn(1.0f);
			return snd;
		}

		IXAudioSound* snd = XAudioSoundManager::getSingleton().CreateSound(soundName, cfg.FileName, cfg.IsStream);
		snd->SetLoop(cfg.IsLoop);
		snd->SetVolumn(cfg.Volume);
	}

	return XAudioSoundManager::getSingleton().GetSound(soundName);
}

void SoundManager::StopSound(const String& soundName)
{
	GetSoundImp(soundName);
	XAudioSoundManager::getSingleton().StopSound(soundName);
}

void SoundManager::StopAllSound()
{
	XAudioSoundManager::getSingleton().StopAllSound();
}

Real SoundManager::GetSoundLength(const String& soundName)
{
	IXAudioSound* snd = GetSoundImp(soundName);
	return Real(snd->GetLengthInSeconds());
}


Real SoundManager::GetSoundCurPos(const String& soundName)
{
	IXAudioSound* snd = GetSoundImp(soundName);
	return Real(snd->GetOffset());
}

void SoundManager::SetSoundVolume(const String& soundName, float gain)
{
	GetSoundImp(soundName);
	XAudioSoundManager::getSingleton().SetSoundVolume(soundName, gain);
}

float SoundManager::GetSoundPitch(const String& soundName)
{
	GetSoundImp(soundName);
	return XAudioSoundManager::getSingleton().GetSoundPitch(soundName);
}

void SoundManager::SetSoundPitch(const String& soundName, float fValue)
{
	GetSoundImp(soundName);
	XAudioSoundManager::getSingleton().SetSoundPitch(soundName, fValue);
}

void SoundManager::SetLoop(const String& soundName,bool bFlag)
{
	IXAudioSound* snd = GetSoundImp(soundName);
	snd->SetLoop(bFlag);
}

void SoundManager::PauseSound(const String& soundName)
{
	IXAudioSound* snd = GetSoundImp(soundName);
	snd->Pause();
}

bool SoundManager::IsPlaying(const String& soundName)
{
	IXAudioSound* snd = GetSoundImp(soundName);
	return snd->IsPlaying();
}

bool SoundManager::IsPaused(const String& soundName)
{
	IXAudioSound* snd = GetSoundImp(soundName);
	return snd->IsPaused();
}


bool SoundManager::IsStoped(const String& soundName)
{
	IXAudioSound* snd = GetSoundImp(soundName);
	return !snd->IsPlaying();
}

void SoundManager::PauseAllSounds(void)
{
	XAudioSoundManager::getSingleton().PauseAllSounds();
}

void SoundManager::ResumeAllSounds(void)
{
	XAudioSoundManager::getSingleton().ResumeAllSounds();
}

void SoundManager::Play(const String& name)
{
	GetSoundImp(name);
	XAudioSoundManager::getSingleton().Play(name);
}

void SoundEffect_FadeOut::Update(float deltaTime)
{
	EffectTimeDurative::Update(deltaTime);
	IXAudioSound* _sound = SoundManager::getSingleton().GetSoundImp(_soundName);
	if (_sound)
	{
		float v = _sound->GetVolumn() - _fadeSpeed * deltaTime;
		_sound->SetVolumn(v);
	}
}

void SoundManager::SetSoundVolume(const String& soundName, float vol, int channelID)
{
	GetSoundImp(soundName)->SetVolumn(channelID, vol);
}

float SoundManager::GetSoundVolumn(const String& soundName, int channelID)
{
	return GetSoundImp(soundName)->GetVolumn(channelID);
}

void SoundManager::FadeInSound(const String& soundName, float duartion)
{
	_effectManager->AddEffect(
		OGRE_NEW SoundEffect_FadeIn(soundName, duartion));
}

void SoundManager::FadeOutSound(const String& soundName, float duartion)
{
 	_effectManager->AddEffect(
 		OGRE_NEW SoundEffect_FadeOut(soundName, duartion));	
}

void SoundManager::SetSoundCurPos(const String& soundName,Real seconds)
{
	IXAudioSound* snd = GetSoundImp(soundName);
	snd->SetOffset(seconds);
}

void SoundManager::StopAllStreamSound()
{
	XAudioSoundManager::getSingleton().StopAllStreamSound();
}

//-------------------------------------------------------------------------------------------------

SoundLifeController::SoundLifeController(const String& soundName)
: _soundName(soundName)
{
}

SoundLifeController::~SoundLifeController()
{
	SoundManager::getSingleton().StopSound(_soundName);
}

bool SoundLifeController::IsDead() const
{
	IXAudioSound* pSnd = SoundManager::getSingleton().GetSoundImp(_soundName);
	return !pSnd || !pSnd->IsPlaying();
}

void SoundLifeController::Init()
{
}

void SoundLifeController::Update(float deltaTime)
{
}

SoundEffect_FadeIn::SoundEffect_FadeIn(const String& soundName, float duration,
	float finalGain, bool startFromZero)
	: EffectTimeDurative(duration)
	, _soundName(soundName)
	, _finalGain(finalGain)
{
	IXAudioSound* _sound = SoundManager::getSingleton().GetSoundImp(_soundName);
	_fadeSpeed = fabs(_sound->GetVolumn() - finalGain) / duration;
	if (_sound)
	{
		if(startFromZero)
		{
			_sound->SetVolumn(0.0f);
		}

		_sound->Play();
	}
}

SoundEffect_FadeIn::~SoundEffect_FadeIn()
{
	IXAudioSound* _sound = SoundManager::getSingleton().GetSoundImp(_soundName);
	if (_sound)
	{
		_sound->SetVolumn(_finalGain);
		//_sound->play();
	}

}


void SoundEffect_FadeIn::Init()
{
}

void SoundEffect_FadeIn::Update(float deltaTime)
{
	EffectTimeDurative::Update(deltaTime);
	IXAudioSound* _sound = SoundManager::getSingleton().GetSoundImp(_soundName);
	if (_sound)
	{
		float v = _sound->GetVolumn() + _fadeSpeed * deltaTime;
		_sound->SetVolumn(std::min(_finalGain, v));
	}
}

//-------------------------------------------------------------------------------------------------

SoundEffect_FadeOut::SoundEffect_FadeOut(const String& soundName, float duration, float finalGain, bool stopSound)
	: EffectTimeDurative(duration)
	, _soundName(soundName)
	, _finalGain(finalGain)
	, _stopSound(stopSound)
{
	IXAudioSound* _sound = SoundManager::getSingleton().GetSoundImp(_soundName);
	{
		float startGain = 1.0f;
		if (_sound)
			startGain = _sound->GetVolumn();
		_fadeSpeed = fabs(_finalGain - startGain) / duration;
	}
}

SoundEffect_FadeOut::~SoundEffect_FadeOut()
{
	if(_stopSound)
	{
		SoundManager::getSingleton().StopSound(_soundName);
	}
}


void SoundEffect_FadeOut::Init()
{
}

//-------------------------------------------------------------------------------------------------

EffectVibrate::EffectVibrate()
: _enable(false)
, _wavFreqRatio(10.0f)
, _wavSwingMin(0.5f)
, _wavSwingMax(1.3f)
, _timeElapsed(0.0f)
, _channel(-1)
, _restoreVol(1.0f)
{
}

EffectVibrate::~EffectVibrate()
{
}

void EffectVibrate::Update(float deltaTime)
{
	if(_enable)
	{
		_timeElapsed += deltaTime;

		if(!_soundName.empty())
		{
			float sinVal = (_restoreVol + sin(_timeElapsed * _wavFreqRatio));
			float v = sinVal * 0.5f * (_wavSwingMax - _wavSwingMin) + _wavSwingMin;

#if 0
			String sinStr;
			for(int i = 0; i < sinVal * 20; ++i)
			{
				sinStr += "_";
			}

			LogConsoleIDf("Engine", "%f v[%f] [%f] %s", _timeElapsed, v, sinVal,  sinStr.c_str());
#endif
			if(_channel == -1)
			{
				SoundManager::getSingleton().SetSoundVolume(_soundName, v);
			}
			else
			{
				SoundManager::getSingleton().SetSoundVolume(_soundName, v, _channel);
			}
		}
	}
}

void EffectVibrate::SetEnable(bool on)
{
	_enable = on;

	if(!on)
	{
		_timeElapsed = 0.0f;

		if(!_soundName.empty())
		{
			if(_channel == -1)
			{
				SoundManager::getSingleton().SetSoundVolume(_soundName, _restoreVol);
			}
			else
			{
				SoundManager::getSingleton().SetSoundVolume(_soundName, _restoreVol, _channel);
			}
		}
	}
}

#endif
