// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-24
// *************************************************************************************

#include "stdafx.h"
#include "Log.h"

LogIDMap g_LogIDMap;

void Log_SystemID(const String& logID, LPCTSTR msg)
{
	if(!LogManager::getSingletonPtr())
	{
		return;
	}

	LogIDMap::iterator it = g_LogIDMap.find(logID);
	if(it != g_LogIDMap.end())
	{
		if(it->second)
		{
			LogManager::getSingleton().logMessage(("[System]\t") + String(msg), LML_CRITICAL);
		}
	}
	else
	{
		EnableLogID(logID, false);
	}
}

void Log_SystemIDf(const String& logID, LPCTSTR lpcFmt, ...)
{		
	if(!LogManager::getSingletonPtr())
	{
		return;
	}

	LogIDMap::iterator it = g_LogIDMap.find(logID);
	if(it != g_LogIDMap.end())
	{
		if(it->second)
		{
			OGE_FARMAT_ARG(msg, lpcFmt);
			LogManager::getSingleton().logMessage(("[System]\t") + String(msg), LML_CRITICAL);
		}
	}
	else
	{
		EnableLogID(logID, false);
	}
}


void Log_DebugID(const String& logID, LPCTSTR msg)
{
	if(!LogManager::getSingletonPtr())
	{
		return;
	}

	LogIDMap::iterator it = g_LogIDMap.find(logID);
	if(it != g_LogIDMap.end())
	{
		if(it->second)
		{
			LogManager::getSingleton().logMessage(("[Debug]\t") + String(msg), LML_TRIVIAL);
		}
	}
	else
	{
		EnableLogID(logID, false);
	}
}

void Log_DebugIDf(const String& logID, LPCTSTR lpcFmt, ...)
{		
	if(!LogManager::getSingletonPtr())
	{
		return;
	}

	LogIDMap::iterator it = g_LogIDMap.find(logID);
	if(it != g_LogIDMap.end())
	{
		if(it->second)
		{
			OGE_FARMAT_ARG(msg, lpcFmt);
			LogManager::getSingleton().logMessage(("[Debug]\t") + String(msg), LML_TRIVIAL);
		}
	}
	else
	{
		EnableLogID(logID, false);
	}
}

void Log_InfoID(const String& logID, LPCTSTR msg)
{
	if(!LogManager::getSingletonPtr())
	{
		return;
	}

	LogIDMap::iterator it = g_LogIDMap.find(logID);
	if(it != g_LogIDMap.end())
	{
		if(it->second)
		{
			LogManager::getSingleton().logMessage(("[Info]\t") + String(msg), LML_NORMAL);
		}
	}
	else
	{
		EnableLogID(logID, false);
	}
}

void Log_InfoIDf(const String& logID, LPCTSTR lpcFmt, ...)
{
	if(!LogManager::getSingletonPtr())
	{
		return;
	}

	LogIDMap::iterator it = g_LogIDMap.find(logID);
	if(it != g_LogIDMap.end())
	{
		if(it->second)
		{
			OGE_FARMAT_ARG(msg, lpcFmt);
			LogManager::getSingleton().logMessage(("[Info]\t") + String(msg), LML_NORMAL);
		}
	}
	else
	{
		EnableLogID(logID, false);
	}
}

void Log_ErrorID(const String& logID, LPCTSTR msg)
{
	if(!LogManager::getSingletonPtr())
	{
		return;
	}

	LogIDMap::iterator it = g_LogIDMap.find(logID);
	if(it != g_LogIDMap.end())
	{
		if(it->second)
		{
			LogManager::getSingleton().logMessage(("[Error]\t") + String(msg), LML_CRITICAL);
		}
	}
	else
	{
		EnableLogID(logID, false);
	}
}

void Log_ErrorIDf(const String& logID, LPCTSTR lpcFmt, ...)
{
	if(!LogManager::getSingletonPtr())
	{
		return;
	}

	LogIDMap::iterator it = g_LogIDMap.find(logID);
	if(it != g_LogIDMap.end())
	{
		if(it->second)
		{
			OGE_FARMAT_ARG(msg, lpcFmt);
			LogManager::getSingleton().logMessage(("[Error]\t") + String(msg), LML_CRITICAL);
		}
	}
	else
	{
		EnableLogID(logID, false);
	}
}

void Log_WarningID(const String& logID, LPCTSTR msg)
{
	if(!LogManager::getSingletonPtr())
	{
		return;
	}

	LogIDMap::iterator it = g_LogIDMap.find(logID);
	if(it != g_LogIDMap.end())
	{
		if(it->second)
		{
			LogManager::getSingleton().logMessage(("[Warning]\t") + String(msg), LML_CRITICAL);
		}
	}
	else
	{
		EnableLogID(logID, false);
	}
}

void Log_WarningIDf(const String& logID, LPCTSTR lpcFmt, ...)
{
	if(!LogManager::getSingletonPtr())
	{
		return;
	}

	LogIDMap::iterator it = g_LogIDMap.find(logID);
	if(it != g_LogIDMap.end())
	{
		if(it->second)
		{
			OGE_FARMAT_ARG(msg, lpcFmt);
			LogManager::getSingleton().logMessage(("[Warning]\t") + String(msg), LML_CRITICAL);
		}
	}
	else
	{
		EnableLogID(logID, false);
	}
}

void Log_ConsoleID(const String& logID, LPCTSTR msg)
{
	if(!LogManager::getSingletonPtr())
	{
		return;
	}

	LogIDMap::iterator it = g_LogIDMap.find(logID);
	if(it != g_LogIDMap.end())
	{
		if(it->second)
		{
			std::cout << ("[") << logID << ("]") << msg << std::endl;
		}
	}
	else
	{
		EnableLogID(logID, false);
	}
}

void Log_ConsoleIDf(const String& logID, LPCTSTR lpcFmt, ...)
{
	if(!LogManager::getSingletonPtr())
	{
		return;
	}

	LogIDMap::iterator it = g_LogIDMap.find(logID);
	if(it != g_LogIDMap.end())
	{
		if(it->second)
		{
			OGE_FARMAT_ARG(msg, lpcFmt);
			std::cout << ("[") << logID << ("]") << msg << std::endl;
		}
	}
	else
	{
		EnableLogID(logID, false);
	}
}

void EnableLogID(const String& logID, bool on)
{
	g_LogIDMap[logID] = on;
}
