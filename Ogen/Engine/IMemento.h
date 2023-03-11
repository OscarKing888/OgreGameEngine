// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2006-10-24
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"



	class _EngineExport IMemento : public RefObject
	{
	public:
		
		IMemento();
		virtual ~IMemento();
		virtual void Set() = 0;
		virtual void Restore() = 0;
	};

	SmartPointer(IMemento);


