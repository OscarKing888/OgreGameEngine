// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-24
// *************************************************************************************

#include "StdAfx.h"
#include "DebugUtil.h"
#include "OSAPI.h"



void debugTrace(const String& msg)
{
	OutputDebugString(("---" + msg + "\n").c_str());
}
