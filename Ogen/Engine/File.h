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



	class _EngineExport FileEx : public File
	{
	public:

		static String		GetResourceFilePath(const String& fileName, bool withFileName = true);
	};

