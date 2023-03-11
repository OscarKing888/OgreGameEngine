// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2006-9-8
// *************************************************************************************

#pragma once

#include "IEffect.h"

	class _EngineExport EffectTimeDurative : public IEffect
	{
	public:

		explicit EffectTimeDurative(float fTimeDuration);
		virtual ~EffectTimeDurative(void);

		virtual void Update(float deltaTime);
		virtual bool IsDead() const;
		virtual void Init() {}

	protected:

		float				_duration;
	};

	SmartPointer(EffectTimeDurative);

	class _EngineExport EffectDelay : public EffectTimeDurative
	{
	public:

		explicit EffectDelay(float fTimeToDelay)
			: EffectTimeDurative(fTimeToDelay)
		{

		}

		virtual ~EffectDelay(){}

		virtual void Init(){}
	};

	SmartPointer(EffectDelay);
// } // namespace
