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
#include "EffectManager.h"

class _EngineExport EffectQueuedCompositor : public IEffect
{
public:

	EffectQueuedCompositor(void);
	virtual ~EffectQueuedCompositor(void);

	virtual void Update(float deltaTime);
	virtual bool IsDead() const;
	virtual void Init();

	void AddEffect(const IEffectPtr& e);

protected:

	typedef std::queue<IEffectPtr>		EffectQueue;
	EffectQueue		_effects;
	bool			_inited;
};

SmartPointer(EffectQueuedCompositor);

//--------------------------------------------------------------------------------------
// 所有子对象同步更新,方便在传回接口时组合多个不同的Effect同时进行
class _EngineExport EffectConcurrentCompositor : public IEffect
{
public:

	EffectConcurrentCompositor(void);
	virtual ~EffectConcurrentCompositor(void);

	virtual void Update(float deltaTime);
	virtual bool IsDead() const;
	virtual void Init();

	void AddEffect(const IEffectPtr& e);

protected:

	EffectManager _effeceMgr;
};

SmartPointer(EffectConcurrentCompositor);

