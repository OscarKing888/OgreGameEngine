// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2006-9-8
// *************************************************************************************

#include "StdAfx.h"
#include "EffectTimeDurative.h"

EffectTimeDurative::EffectTimeDurative(float fTimeDuration)
: _duration(fTimeDuration)
{
}

EffectTimeDurative::~EffectTimeDurative(void)
{
}

bool EffectTimeDurative::IsDead() const
{
	return _duration <= 0.0f;
}

void EffectTimeDurative::Update(float deltaTime)
{
	_duration -= deltaTime;
}