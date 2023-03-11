// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-27
// *************************************************************************************

#pragma once

#include "IEffect.h"

class _EngineExport EffectManager : public IEffectManager
{
public:

	EffectManager(void);
	~EffectManager(void);

	virtual void AddEffect(const IEffectPtr& spEffect);
	virtual void Update(float deltaTime);
	virtual void RemoveAll();
	virtual bool IsEmpty() const;

protected:

	typedef vector<IEffectPtr>::type		GameEffectList;
	GameEffectList	_effects;
	GameEffectList	_addEffects;

	void AddEffects();
};
