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


_EngineExport extern void Log_SystemID(const String& logID, LPCTSTR msg);
_EngineExport extern void Log_SystemIDf(const String& logID, LPCTSTR lpcFmt, ...);

_EngineExport extern void Log_DebugID(const String& logID, LPCTSTR msg);
_EngineExport extern void Log_DebugIDf(const String& logID, LPCTSTR lpcFmt, ...);

_EngineExport extern void Log_InfoID(const String& logID, LPCTSTR msg);
_EngineExport extern void Log_InfoIDf(const String& logID, LPCTSTR lpcFmt, ...);

_EngineExport extern void Log_ErrorID(const String& logID, LPCTSTR msg);
_EngineExport extern void Log_ErrorIDf(const String& logID, LPCTSTR lpcFmt, ...);

_EngineExport extern void Log_WarningID(const String& logID, LPCTSTR msg);
_EngineExport extern void Log_WarningIDf(const String& logID, LPCTSTR lpcFmt, ...);

_EngineExport extern void Log_ConsoleID(const String& logID, LPCTSTR msg);
_EngineExport extern void Log_ConsoleIDf(const String& logID, LPCTSTR lpcFmt, ...);

_EngineExport extern void EnableLogID(const String& logID, bool on);

typedef std::map<String, bool, NoCaseLess<String> > LogIDMap;
extern LogIDMap g_LogIDMap;
// 

#if !defined(_PRODUCT_) && 1
#	define LogSysID			Log_SystemID
#	define LogSysIDf		Log_SystemIDf
#	define LogConsoleID		Log_ConsoleID
#	define LogConsoleIDf	Log_ConsoleIDf
#	define LogDebugID		Log_DebugID
#	define LogDebugIDf		Log_DebugIDf
#	define LogInfoID		Log_InfoID
#	define LogInfoIDf		Log_InfoIDf
#	define LogErrorID		Log_ErrorID
#	define LogErrorIDf		Log_ErrorIDf
#	define LogWarningID		Log_WarningID
#	define LogWarningIDf	Log_WarningIDf
#else
#	define LogSysID(x, y)
#	define LogSysIDf
#	define LogConsoleID(x, y)
#	define LogConsoleIDf
#	define LogDebugID(x, y)
#	define LogDebugIDf
#	define LogInfoID(x, y)
#	define LogInfoIDf
#	define LogErrorID(x, y)
#	define LogErrorIDf
#	define LogWarningID(x, y)
#	define LogWarningIDf
#endif