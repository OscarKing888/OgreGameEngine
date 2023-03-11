// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-25
// *************************************************************************************

#include "stdafx.h"
#include "DisplayDevice.h"



	DisplayDevice::DisplayDevice()
	{
		BOOL r = false;
		int idx = 0;
		do 
		{
			DISPLAY_DEVICE dev;
			memset((void*)&dev, 0, sizeof(DISPLAY_DEVICE));
			dev.cb = sizeof(DISPLAY_DEVICE);
			r = ::EnumDisplayDevices(NULL, idx, &dev, 0);

			SDisplayDevice dd;
			memcpy((void*)&dd, (const void*)&dev, sizeof(SDisplayDevice));

			if(r && (idx == 0)) // 第一次作为主显示设备
			{
				_mainDisplayDevice = dd;
			}

			if(r)
			{
				_deviceList.push_back(dd);
			}

			++idx;
		} while(r);
	}
