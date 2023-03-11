// *************************************************************************************
// 
// 	Copyright (c) 2009 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2009-6-13
// *************************************************************************************

#include "stdafx.h"
#include "PlatformAPI.h"

#ifdef _WIN32
int	g_LowpowerTimeout = 0;
int g_PoweroffTimeout = 0;
#endif

void MessageBox_Error(const String& msg)
{
#ifdef _WIN32
	::MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONEXCLAMATION);
#endif
}

void EnableScreenSaver(bool on)
{
#ifdef _WIN32
	if(!on)
	{
		SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, 0, SPIF_SENDWININICHANGE);
		SystemParametersInfo(SPI_GETLOWPOWERTIMEOUT, 0, &g_LowpowerTimeout, 0);
		SystemParametersInfo(SPI_GETPOWEROFFTIMEOUT, 0, &g_PoweroffTimeout, 0);
		SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, 0, NULL, 0);
		SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, 0, NULL, 0);
	}
	else
	{
		SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, 0, SPIF_SENDWININICHANGE);
		SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, g_LowpowerTimeout, NULL, 0);
		SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, g_PoweroffTimeout, NULL, 0);

		g_LowpowerTimeout = 0;
		g_PoweroffTimeout = 0;
	}
#endif
}