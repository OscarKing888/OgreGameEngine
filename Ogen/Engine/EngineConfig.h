// *************************************************************************************
// 
// 	Copyright (c) 2009 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2009-6-14
// *************************************************************************************

#pragma once

// Don't use windows min/max macro, use std::min/max always
#ifdef _WIN32
#	define NOMINMAX
#endif

//----------------------------------------------------------------------------------------------
// Compile warnings
//----------------------------------------------------------------------------------------------

// warning C4819: The file contains a character that cannot be represented in the current code page (936). Save the file in Unicode format to prevent data loss
#pragma warning(disable : 4819)

// warning C4251: 'Property<SetType,GetType>::_getFun' : class 'std::tr1::function<_Fty>' needs to have dll-interface to be used by clients of class 'Property<SetType,GetType>'
#pragma warning(disable : 4251)

// warning C4661: 'GameApp *Singleton<T>::ms_Singleton' : no suitable definition provided for explicit template instantiation request
#pragma warning(disable : 4661)

// warning C4244: 'return' : conversion from 'Real' to 'int', possible loss of data
#pragma warning(disable : 4244)

// warning C4996: 'sscanf': This function or variable may be unsafe. Consider using sscanf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning(disable : 4996)

//----------------------------------------------------------------------------------------------
// Engine compile configurations
//----------------------------------------------------------------------------------------------

// Default use xaudio sound system
#define OGE_USE_OPENAL	0

// Enable the python script if set to 1
#define OGE_ENABLE_PYTHON	0

// Max string size
#define OGE_MAX_STRING_SIZE	4096

// Max path char size
#define OGE_MAX_PATH		260

// Temp string buffer size
#define OGE_TMP_STRINGBUF_SIZE	1024

//----------------------------------------------------------------------------------------------
// Util macros
//----------------------------------------------------------------------------------------------

// var format to string buffer
#define OGE_FARMAT_ARG(tmpVarName, lpcFmt)	TCHAR tmpVarName[OGE_TMP_STRINGBUF_SIZE];\
	memset(tmpVarName, 0, OGE_TMP_STRINGBUF_SIZE * sizeof(TCHAR));\
	va_list args;\
	va_start(args, lpcFmt);\
	int re = _vsntprintf(tmpVarName, OGE_TMP_STRINGBUF_SIZE, lpcFmt, args);\
	UNREFERENCED_PARAMETER(re);\
	va_end(args);
