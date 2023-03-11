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

		/** ֱ�ӴӴ����ļ����أ�LoadDirect("../Data/Config/SomeConfig.xml") */
		void LoadDirect(const String& xmlFile);

		/** ͨ��Ogre���ļ�ϵͳ���أ�ʹ��Ĭ�ϵ�ResourceGroup(General) */
		void LoadFromResourceSystem(const String& xmlFile);

		/** ֱ�Ӵ�xml�ַ������� */
		void LoadFromXML(const String& xmlString);

		void AddConfig(const SSoundConfig& cfg);

		/*
		PlaySound					����������
		name						���������ļ��е���������
		gain						��������
		fadeInTime					���루���죩ʱ�䣨�룩��ȱʡΪ�޵���Ч��
		position					����λ�ã��������꣩����Listener��λ�÷���ͬӰ������Ч��
		����ֵ��					soundName��ʵ�ʲ��ŵ�������ʶ�������������������Ĳ���
		*/
		//String Play(const String& name, float gain = 1.0, float fadeInTime = 0.0f, Vector3 position = Vector3::ZERO);
		bool  Play(const String& name, float gain = 1.0, float fadeInTime = 0.0f, Vector3 position = Vector3::ZERO);

		void ResetSound(const String& soundName);
		void SetSoundPosition(const String& soundName, const Vector3& pos);


		/*
		AttachSoundToNode			�������󶨵�һ�� SceneNode��
		soundName						������ʶ����PlaySound�ķ���ֵ
		node						�󶨵�Ŀ�� SceneNode
		*/
		void AttachSoundToNode(const String& soundName, SceneNode * node);
		
		/*
		StopSound					ֹͣ��������
		soundName						������ʶ����PlaySound�ķ���ֵ
		fadeOutTime					������������ʱ�䣨�룩��ȱʡΪ�޵���Ч��
		*/
		void StopSound(const String& soundName, float fadeOutTime = 0.0f);
		void _StopSound(const String& soundName);
		
		void StopAllSound();
		
		/*
		SetListenerPositionDirection	����Listener��λ�÷���
		position						λ�ã��������꣩
		direction						�����������꣩
		*/
		void SetListenerPositionDirection(const Vector3& position, const Vector3& direction);
		/*
		GetListenerPositionDirection	ȡ��Listener��λ�÷���
		position						λ�ã��������꣩
		direction						�����������꣩
		*/
		void GetListenerPositionDirection(Vector3& position, Vector3& direction);
		
		/*
		SetListenerGain					����Listener���棬�൱������ȫ������
		gain							����ֵ, 0.0 - 1.0
		*/
		void SetListenerGain(float gain);

		/*
		GetListenerGain					ȡ��Listener����
		����ֵ��						����ֵ, 0.0 - 1.0
		*/
		float GetListenerGain(void);

		/*
		PauseAllSounds					��ͣ������������
		*/
		void PauseAllSounds(void);

		/*
		PauseAllSounds					����������������
		*/
		void ResumeAllSounds(void);

		void ShutDown();

		Real GetSoundLength(const String& soundName); //�õ������Ĳ��ų���
		Real GetSoundCurPos(const String& soundName); //�õ������Ĳ���λ��
		bool       SetSoundCurPos(const String& soundName,Real seconds); //���������Ĳ�����ʼλ��
		float GetSoundPitch(const String& soundName);

		void   SetSoundPitch(const String& soundName,float fValue);
		bool   SetSoundVolume(const String& soundName, float gain);
		bool   SetSoundVolumeRange(const String& soundName, float minGain, float maxGain);

		void SetSoundVolume(const String& soundName, float vol, int channelID){}

		float GetSoundVolumn(const String& soundName, int channelID){ return 1.0f; }

		void   SetLoop(const String& soundName,bool bFlag);//ѭ������
		
		
		bool   PauseSound(const String& soundName);//��ͣһ������
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

	/** ֱ�ӴӴ����ļ����أ�LoadDirect("../Data/Config/SomeConfig.xml") */
	void LoadDirect(const String& xmlFile);

	/** ͨ��Ogre���ļ�ϵͳ���أ�ʹ��Ĭ�ϵ�ResourceGroup(General) */
	void LoadFromResourceSystem(const String& xmlFile);

	/** ֱ�Ӵ�xml�ַ������� */
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

	Real GetSoundLength(const String& soundName); //�õ������Ĳ��ų���
	Real GetSoundCurPos(const String& soundName); //�õ������Ĳ���λ��
	void       SetSoundCurPos(const String& soundName,Real seconds); //���������Ĳ�����ʼλ��
	float GetSoundPitch(const String& soundName);
	void   SetSoundPitch(const String& soundName,float fValue);
	void   SetSoundVolume(const String& soundName, float gain);

	void SetSoundVolume(const String& soundName, float vol, int channelID);
	float GetSoundVolumn(const String& soundName, int channelID);

	void   SetLoop(const String& soundName, bool bFlag);//ѭ������

	void   PauseSound(const String& soundName);//��ͣһ������
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

		// ������ر��񶯣���ʼΪ�ر�״̬
		void SetEnable(bool on);
		bool IsEnabled() const { return _enable; }

		// ����Ӱ��������-1Ϊȫ��
		void SetAffectChannel(int channel) { _channel = channel; }

		// ����Ƶ�ʱ��ʣ�Ӱ�������1.0Ϊԭʼʱ��ֵ��sint(timeElapsed)
		void SetWaveFrequencyRatio(float freq) { _wavFreqRatio = freq; }
		float GetWaveFrequencyRatio() const { return _wavFreqRatio; }

		// ����������ʣ�Ӱ�������仯��С���������ֵΪ�ڴ����䣬Ĭ��Ϊ[0.5, 1.3]
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
	

