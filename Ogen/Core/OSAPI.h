// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************

#pragma once

#include "CorePrerequisites.h"

#ifdef _WIN32
#	include "Win32.h"
#endif


_CoreExport inline void XInterLockedIncrement(volatile long* val);
_CoreExport inline void XInterLockedDecrement(volatile long* val);
