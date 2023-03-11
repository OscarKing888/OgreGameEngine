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
#include "EffectCompositor.h"

EffectQueuedCompositor::EffectQueuedCompositor(void)
: _inited(false)
{
}

EffectQueuedCompositor::~EffectQueuedCompositor(void)
{
}

void EffectQueuedCompositor::AddEffect(const IEffectPtr& e)
{
	_effects.push(e);
}

void EffectQueuedCompositor::Update(float deltaTime)
{
	if(_effects.empty())
	{
		return;
	}

	IEffectPtr e = _effects.front();
	if(!_inited)
	{
		_inited = true;
		e->Init();
	}
	
	e->Update(deltaTime);

	if(e->IsDead())
	{
		_effects.pop();
		if(!_effects.empty())
		{
			e = _effects.front();
			e->Init();
		}
	}
}

bool EffectQueuedCompositor::IsDead() const
{
	return _effects.empty();
}

void EffectQueuedCompositor::Init()
{
	
}

//--------------------------------------------------------------------------------------

EffectConcurrentCompositor::EffectConcurrentCompositor(void)
{
}

EffectConcurrentCompositor::~EffectConcurrentCompositor(void)
{
}

void EffectConcurrentCompositor::AddEffect(const IEffectPtr& e)
{
	_effeceMgr.AddEffect(e);
}

void EffectConcurrentCompositor::Update(float deltaTime)
{
	_effeceMgr.Update(deltaTime);
}

bool EffectConcurrentCompositor::IsDead() const
{
	return _effeceMgr.IsEmpty();
}

void EffectConcurrentCompositor::Init()
{
	
}