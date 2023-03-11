// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2006-12-30
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"
#include "EnumDescriptorManager.h"
#include "IUpdateAble.h"

namespace Animations
{
	enum _EngineExport EAnimationType
	{
		eAnimationNone		= -1,

		// cycle loop frame type		@---------- >> ---------#
		//								@---------- >> ---------#
		eAnimationLoop		= 0,

		// just play once				@---------- >> ---------#
		eAnimationOnce		= 1,


		// loop reverse					@---------- >> ---------#
		//								@---------- << ---------#
		eAnimationReverse	= 2		
	};

	class _EngineExport FrameAnimationType :
		public AbstractEnumDescriptor,
		public Singleton<FrameAnimationType>
	{
	public:

		FrameAnimationType();
		virtual ~FrameAnimationType();
	};
	
	//--------------------------------------------------------------------------------------

	struct _EngineExport  SAnimationInfo
	{
		typedef std::map<int, float> FPSMap;
		
		String	ImageName;		// 以"set:imageSetName image:ImageName_#"的格式来定位

		int				TotalFrames;

		EAnimationType	FrameAnimationType;

		float			DelayEach;	// default is 25 FPS
		FPSMap			DelayFPSMap;

		int				HFrameCount;		// 水平方向上的图片个数
		int				VFrameCount;		// 

		SAnimationInfo()
		{
			TotalFrames = 0;
			FrameAnimationType = eAnimationLoop;
			DelayEach = 0.04f;
			HFrameCount = 0;
			VFrameCount = 0;
		}

		inline float GetDelayTime(int frameNumber)
		{
			FPSMap::iterator i = DelayFPSMap.find(frameNumber);
			if(i != DelayFPSMap.end())
			{
				return i->second;
			}

			return DelayEach;
		}

		String GetFrameImageName(int frameNumber)
		{
			return formatString(("%s_%d"), ImageName.c_str(), frameNumber);
		}
	};

	//--------------------------------------------------------------------------------------


	class _EngineExport FrameAnimationConfigManager :
		public SingletonMyers<FrameAnimationConfigManager>
	{
	public:

		typedef std::map<String, SAnimationInfo, NoCaseLess<String> >
			FrameAnimationConfigMap;

		//
		// Frame Animation Config
		//
		const SAnimationInfo&				GetFrameAnimationInfo(const String& strAniName);

		bool								AddFrameAnimationInfo(const String& strAniName, const SAnimationInfo& info);
		void								RemoveFrameAnimationInfo(const String& strAniName);

		FrameAnimationConfigMap				GetAnimationConfigMap(){ return _mapFrameAnimationConfig; }

		void LoadConfig(const String& xmlFile);

		virtual ~FrameAnimationConfigManager(){}

	protected:

		FrameAnimationConfigMap		_mapFrameAnimationConfig;
	};

	//--------------------------------------------------------------------------------------

	enum _EngineExport EAnimationDirection
	{
		eAD_Forward,
		eAD_Backward
	};

	class _EngineExport FrameAnimation
		: public RefObject
		, public IUpdateAble
	{
	public:

		explicit FrameAnimation(const String& frameAniConfigName);

		explicit FrameAnimation(const String& frameAniConfigName, const SAnimationInfo& aniInfo);

		virtual ~FrameAnimation(void);

		virtual void			Update(float deltaTime);
		virtual void			SetName(const String& name) { _strName = name; }
		virtual String	GetName() const { return _strName; }

		const SAnimationInfo&	GetAnimationInfo();
		void					SetAnimationInfo(const SAnimationInfo& info);

		void					SetCurrentFrame(int frameNumber);
		int						GetCurrentFrame(){ return _currentFrame; }

		float                   GetFrameAniTime();

		void					Enable(bool on);
		bool					IsEnabled() const { return _enable; }
		
		bool					IsOnceDead(void);
		void					SetEndShow(bool on){ _isEndShow = on;}

	protected:

		SAnimationInfo		_aniInfo;
		int					_currentFrame;
		float				_timeDelta;
		EAnimationDirection	_aniDir;
		String		_strName;
		String		_configName;
		bool				_enable;
		bool				_isOnceDead;
		bool				_isEndShow;
		bool				_needUpdate;

		virtual void				UpdateImage(){}
	};

	
	//-------------------------------------------------------------------------------------------------

	class _EngineExport FrameAnimation_Material
		: public FrameAnimation
	{
	public:

		explicit FrameAnimation_Material(TextureUnitState* tus,
			const String& frameAniConfigName);

		explicit FrameAnimation_Material(TextureUnitState* tus,
			const String& frameAniConfigName, const SAnimationInfo& aniInfo);

		virtual ~FrameAnimation_Material(void);
		TextureUnitState*		GetTextureUnitState(void){ return _textureUnitState; }

	protected:

		TextureUnitState*		_textureUnitState;
		Vector2				_uvUnit;

		virtual void				UpdateImage();
		void Init();
	};

	SmartPointer(FrameAnimation_Material);
}
