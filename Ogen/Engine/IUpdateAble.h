// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-24
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"



	class _EngineExport  IUpdateAble
	{
	public:

		virtual void Update(float deltaTime) = 0;
	};

	// 不可停止的更新接口
	class _EngineExport  INonPauseUpdateAble : public IUpdateAble
	{
	};

