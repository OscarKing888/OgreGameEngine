// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-27
// *************************************************************************************

#include "StdAfx.h"
#include "EffectManager.h"
#include <algorithm>

EffectManager::EffectManager(void)
{
}

EffectManager::~EffectManager(void)
{
}

void EffectManager::AddEffect(const IEffectPtr& spEffect)
{
	GameEffectList::iterator i = std::find(_effects.begin(), _effects.end(), spEffect);
	if(i == _effects.end()) // 只加入没在列表中的
	{
		spEffect->Init();
		_addEffects.push_back(spEffect);
	}

	LogDebugIDf("Engine", ("%s"), getClassName(spEffect.GetPointer()).c_str());
}

void EffectManager::Update(float deltaTime)
{
	AddEffects();

	for (GameEffectList::iterator i = _effects.begin(); i != _effects.end(); ++i)
	{
		IEffectPtr e = (*i);
		if(e)
		{
			e->Update(deltaTime);
			if(e->IsDead())
			{
				*i = 0;
			}
		}
	}

	// Remove the died effects

	for (GameEffectList::iterator i = _effects.begin(); i != _effects.end(); )
	{
		IEffectPtr e = (*i);
		if(!e)
		{
			i = _effects.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void EffectManager::RemoveAll()
{
	_addEffects.clear();
	for (GameEffectList::iterator i = _effects.begin(); i != _effects.end(); ++i)
	{
		*i = 0;
	}
}

bool EffectManager::IsEmpty() const
{
	return _effects.empty();
}

void EffectManager::AddEffects()
{
	for (GameEffectList::iterator i = _addEffects.begin(); i != _addEffects.end(); ++i)
	{
		IEffectPtr e = (*i);		
		_effects.push_back(e);
	}
	_addEffects.clear();
}