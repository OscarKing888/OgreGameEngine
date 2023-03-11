// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-30
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"
#include "GameCamera.h"



	class _EngineExport ICameraController : public AbstractGameObject
	{
	public:

		virtual void SetCamera(GameCamera* cam) = 0;
		virtual GameCamera* GetCamera() const = 0;
	};

	SmartPointer(ICameraController);

