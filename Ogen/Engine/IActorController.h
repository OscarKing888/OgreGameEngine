// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2009-07-01
// *************************************************************************************

#pragma once

#include "EnginePrerequisites.h"
#include "AbstractGameObject.h"
#include "RefObject.h"

class Actor;

class _EngineExport IActorController
	: public AbstractGameObject
	, public IUpdateAble
{
public:

	virtual void SetActor(Actor* actor) = 0;
	virtual Actor* GetActor() const = 0;
	
	virtual void OnActorAttach(Actor* actor) = 0;
	virtual void OnActorDetach(Actor* actor) = 0;
};
