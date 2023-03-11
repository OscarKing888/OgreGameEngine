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

#ifdef ENGINE_EXPORTS
#	define _EngineExport __declspec(dllexport)
#elif defined(_LIB)	// for static lib
#	define _EngineExport
#else
#	define _EngineExport __declspec(dllimport)
#endif


#ifndef ENGINE_EXPORTS
#	ifdef _DEBUG
#		pragma comment(lib, "Engine_Debug.lib")
#	else
#		pragma comment(lib, "Engine_Release.lib")
#	endif
#endif