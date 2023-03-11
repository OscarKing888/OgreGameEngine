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
#include "CorePrerequisites.h"



_CoreExport void debugTrace(const String& msg);



#ifndef _PRODUCT_
#	define DEBUG_TRACE(strArgs) { std::ostringstream os; os << strArgs; debugTrace(os.str()); }
#else
#	define DEBUG_TRACE(strArgs)
#endif