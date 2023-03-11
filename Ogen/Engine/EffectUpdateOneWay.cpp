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
#include "EffectUpdateOneWay.h"

EffectUpdateOneWay::EffectUpdateOneWay()
: _isDead(false)
{
}

EffectUpdateOneWay::~EffectUpdateOneWay(void)
{
}

bool EffectUpdateOneWay::IsDead() const
{
	return _isDead;
}

void EffectUpdateOneWay::Update(float)
{
	_isDead = true;
}