// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-10-21
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"
#include "EffectTimeDurative.h"

#if OGE_USE_OPENAL
#else
namespace XAudio2Ex
{
	class IXAudioSound;
}
#endif



	struct SSoundConfig
	{
		String	Name;
		String	FileName;
		//String    SoundID;
		float			Volume;
		bool			IsLoop;
		bool			IsStream;

		SSoundConfig()
			: IsLoop(false)
			, IsStream(false)
		{
		}
	};

#if OGE_USE_OPENAL
	class _EngineExport SoundManager
		: public SingletonMyers<SoundManager>
		, public OgeAlloc
	{
		friend class SoundLifeController;
// 		friend class SoundEffect_FadeIn;
// 		friend class SoundEffect_FadeOut;
		
	public:

		SoundManager();
		virtual ~SoundManager();

		typedef std::map<String, SSoundConfig> SoundConfigMap;
		typedef std::map<String, OgreAL::Sound*> SoundMap;

		/** 直接从磁盘文件加载，LoadDirect("../Data/Config/SomeConfig.xml") */
		void LoadDirect(const String& xmlFile);

		/** 通过Ogre的文件系统加载，使用默认的ResourceGroup(General) */
		void LoadFromResourceSystem(const String& xmlFile);

		/** 直接从xml字符串加载 */
		void LoadFromXML(const String& xmlString);

		void AddConfig(const SSoundConfig& cfg);

		/*
		PlaySound					播放声音：
		name						声音配置文件中的声音名字
		gain						声音增益
		fadeInTime					淡入（渐响）时间（秒），缺省为无淡入效果
		position					声音位置（世界坐标），和Listener的位置方向共同影响声音效果
		返回值：					soundName，实际播放的声音标识，用于其他声音函数的参数
		*/
		//String Play(const String& name, float gain = 1.0, float fadeInTime = 0.0f, Vector3 position = Vector3::ZERO);
		bool  Play(const String& name, float gain = 1.0, float fadeInTime = 0.0f, Vector3 position = Vector3::ZERO);

		void ResetSound(const String& soundName);
		void SetSoundPosition(const String& soundName, const Vector3& pos);


		/*
		AttachSoundToNode			把声音绑定到一个 SceneNode上
		soundName						声音标识，用PlaySound的返回值
		node						绑定的目标 SceneNode
		*/
		void AttachSoundToNode(const String& soundName, SceneNode * node);
		
		/*
		StopSound					停止声音播放
		soundName						声音标识，用PlaySound的返回值
		fadeOutTime					淡出（渐弱）时间（秒），缺省为无淡出效果
		*/
		void StopSound(const String& soundName, float fadeOutTime = 0.0f);
		void _StopSound(const String& soundName);
		
		void StopAllSound();
		
		/*
		SetListenerPositionDirection	设置Listener的位置方向
		position						位置（世界坐标）
		direction						方向（世界坐标）
		*/
		void SetListenerPositionDirection(const Vector3& position, const Vector3& direction);
		/*
		GetListenerPositionDirection	取得Listener的位置方向
		position						位置（世界坐标）
		direction						方向（世界坐标）
		*/
		void GetListenerPositionDirection(Vector3& position, Vector3& direction);
		
		/*
		SetListenerGain					设置Listener增益，相当于设置全局音量
		gain							增益值, 0.0 - 1.0
		*/
		void SetListenerGain(float gain);

		/*
		GetListenerGain					取得Listener增益
		返回值：						增益值, 0.0 - 1.0
		*/
		float GetListenerGain(void);

		/*
		PauseAllSounds					暂停播放所有声音
		*/
		void PauseAllSounds(void);

		/*
		PauseAllSounds					继续播放所有声音
		*/
		void ResumeAllSounds(void);

		void ShutDown();

		Real GetSoundLength(const String& soundName); //得到声音的播放长度
		Real GetSoundCurPos(const String& soundName); //得到声音的播放位置
		bool       SetSoundCurPos(const String& soundName,Real seconds); //设置声音的播放起始位置
		float GetSoundPitch(const String& soundName);

		void   SetSoundPitch(const String& soundName,float fValue);
		bool   SetSoundVolume(const String& soundName, float gain);
		bool   SetSoundVolumeRange(const String& soundName, float minGain, float maxGain);

		void SetSoundVolume(const String& soundName, float vol, int channelID){}

		float GetSoundVolumn(const String& soundName, int channelID){ return 1.0f; }

		void   SetLoop(const String& soundName,bool bFlag);//循环播放
		
		
		bool   PauseSound(const String& soundName);//暂停一个声音
		bool   IsPlaying(const String& soundName);
		bool   IsPaused(const String& soundName);
		bool   IsStoped(const String& soundName);
		//////////////////////////////////////////////////////////////////////////

		IEffectManager& GetEffectManager() { return *_effectManager; }

		void FadeInSound(const String& soundName, float duartion = 1.0f);
		void FadeOutSound(const String& soundName, float duartion = 1.0f);

		/**	0xffff - 0x0000 */
		void SetSoundVolume(int rightChannelVol = 0xffff, int leftChannelVol = 0xffff);

	protected:

		bool GetConfig(const String& Name,SSoundConfig& cfg);
		OgreAL::Sound* GetSoundImp(const String& soundName);
		
		SoundConfigMap					_soundCfgMap;
		SoundMap						_soundMap;
		int								_autoID;
		IEffectManager*					_effectManager;

		void _LoadConfig(XMLElement& rootNode);
	};

#else
class _EngineExport SoundManager
	: public Singleton<SoundManager>
	, public OgeAlloc
{
	friend class SoundLifeController;
	friend class SoundEffect_FadeIn;
	friend class SoundEffect_FadeOut;

public:

	SoundManager();
	virtual ~SoundManager();

	typedef std::map<String, SSoundConfig> SoundConfigMap;

	/** 直接从磁盘文件加载，LoadDirect("../Data/Config/SomeConfig.xml") */
	void LoadDirect(const String& xmlFile);

	/** 通过Ogre的文件系统加载，使用默认的ResourceGroup(General) */
	void LoadFromResourceSystem(const String& xmlFile);

	/** 直接从xml字符串加载 */
	void LoadFromXML(const String& xmlString);

	void AddConfig(const SSoundConfig& cfg);
	
	void  Play(const String& name);
	void ResetSound(const String& soundName);
	void StopSound(const String& soundName);

	void StopAllSound();
	void StopAllStreamSound();
	void PauseAllSounds(void);
	void ResumeAllSounds(void);
	void ShutDown();

	Real GetSoundLength(const String& soundName); //得到声音的播放长度
	Real GetSoundCurPos(const String& soundName); //得到声音的播放位置
	void       SetSoundCurPos(const String& soundName,Real seconds); //设置声音的播放起始位置
	float GetSoundPitch(const String& soundName);
	void   SetSoundPitch(const String& soundName,float fValue);
	void   SetSoundVolume(const String& soundName, float gain);

	void SetSoundVolume(const String& soundName, float vol, int channelID);
	float GetSoundVolumn(const String& soundName, int channelID);

	void   SetLoop(const String& soundName, bool bFlag);//循环播放

	void   PauseSound(const String& soundName);//暂停一个声音
	bool   IsPlaying(const String& soundName);
	bool   IsPaused(const String& soundName);
	bool   IsStoped(const String& soundName);

	IEffectManager& GetEffectManager() { return *_effectManager; }

	void FadeInSound(const String& soundName, float duartion = 1.0f);
	void FadeOutSound(const String& soundName, float duartion = 1.0f);

	/**	0xffff - 0x0000 */
	void SetSoundVolume(int rightChannelVol = 0xffff, int leftChannelVol = 0xffff);

protected:

	bool GetConfig(const String& soundName, SSoundConfig& cfg);
	XAudio2Ex::IXAudioSound* GetSoundImp(const String& soundName);

	SoundConfigMap					_soundCfgMap;
	IEffectManager*					_effectManager;

	void _LoadConfig(XMLElement& rootNode);
};

class _EngineExport SoundEffect_FadeIn : public EffectTimeDurative
{
public:

	SoundEffect_FadeIn(const String& soundName, float duration = 1.0f, float finalGain = 1.0f, bool startFromZero = true);
	virtual ~SoundEffect_FadeIn();

	virtual void Init();
	virtual void Update(float deltaTime);

protected:

	String	_soundName;
	float			_finalGain;
	float			_fadeSpeed;
};

SmartPointer(SoundEffect_FadeIn);

class _EngineExport SoundEffect_FadeOut : public EffectTimeDurative
{
public:

	SoundEffect_FadeOut(const String& soundName, float duration = 1.0f, float finalGain = 0.0f, bool stopSound = true);
	virtual ~SoundEffect_FadeOut();

	virtual void Init();
	virtual void Update(float deltaTime);

protected:

	String	_soundName;
	float			_finalGain;
	float			_fadeSpeed;
	bool			_stopSound;
};

SmartPointer(SoundEffect_FadeOut);

#endif

	class _EngineExport EffectVibrate : public IUpdateAble
	{
	public:

		EffectVibrate();
		virtual ~EffectVibrate();

		virtual void Update(float deltaTime);

		void SetSoundName(const String& soundName) { _soundName = soundName; }
		const String& GetSoundName() const { return _soundName; }

		// 启动或关闭振动，初始为关闭状态
		void SetEnable(bool on);
		bool IsEnabled() const { return _enable; }

		// 设置影响声道，-1为全部
		void SetAffectChannel(int channel) { _channel = channel; }

		// 设置频率倍率，影响快慢，1.0为原始时间值，sint(timeElapsed)
		void SetWaveFrequencyRatio(float freq) { _wavFreqRatio = freq; }
		float GetWaveFrequencyRatio() const { return _wavFreqRatio; }

		// 设置振幅倍率，影响声音变化大小，输出最后的值为在此区间，默认为[0.5, 1.3]
		void SetWaveSwing(float swingMin, float swingMax) { _wavSwingMin = swingMin; _wavSwingMax = swingMax; }
		float GetWaveSwingMin() const { return _wavSwingMin; }
		float GetWaveSwingMax() const { return _wavSwingMax; }

		void SetRestoreVolumn(float v) { _restoreVol = v; }

	protected:

		String _soundName;
		bool _enable;

		float _wavFreqRatio;

		float _wavSwingMin;
		float _wavSwingMax;

		float _timeElapsed;
		int	_channel;
		float _restoreVol;
	};

	class _EngineExport SoundLifeController : public IEffect
	{
	public:

		SoundLifeController(const String& soundName);
		virtual ~SoundLifeController();

		virtual bool IsDead() const;
		virtual void Init();
		virtual void Update(float deltaTime);

	protected:

		String _soundName;
	};

	SmartPointer(SoundLifeController);
	

