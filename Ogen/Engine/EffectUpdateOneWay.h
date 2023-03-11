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

	class _EngineExport EffectUpdateOneWay : public IEffect
	{
	public:

		EffectUpdateOneWay();
		virtual ~EffectUpdateOneWay(void);

		virtual void Update(float deltaTime);
		virtual bool IsDead() const;
		virtual void Init(){}

	protected:

		bool _isDead;
	};
