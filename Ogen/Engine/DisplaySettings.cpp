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
#include "DisplaySettings.h"



	DisplaySettings::SDisplaySetting DEVMODE2Setting(const DEVMODE& dev)
	{
		DisplaySettings::SDisplaySetting s;
		s.BitsPerPixel = dev.dmBitsPerPel;
		s.DeviceName = String((TCHAR*)dev.dmDeviceName);
		s.DisplayFrequency = dev.dmDisplayFrequency;
		s.PixelHeight = dev.dmPelsHeight;
		s.PixelWidth = dev.dmPelsWidth;
		return s;
	}

	DisplaySettings::DisplaySettings()
	{
		DEVMODE dev;
		memset((void*)&dev, 0, sizeof(DEVMODE));
		dev.dmSize = sizeof(DEVMODE);
		dev.dmDriverExtra = OGE_MAX_PATH;

		BOOL r = ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dev);
		assert(r);

		_currentDisplayMode = DEVMODE2Setting(dev);

		r = false;
		int idx = 0;
		do 
		{
			memset((void*)&dev, 0, sizeof(DEVMODE));
			dev.dmSize = sizeof(DEVMODE);
			dev.dmDriverExtra = OGE_MAX_PATH;

			r = ::EnumDisplaySettings(NULL, idx, &dev);					

			++idx;

			if(r)
			{
				SDisplaySetting s = DEVMODE2Setting(dev);

				if((s.PixelWidth < 640) || (s.PixelHeight < 400))
				{
					continue;
				}

				_settingList.push_back(s);
			}

		} while(r);
	}

	bool DisplaySettings::IsSupportResolution( int width, int height )
	{
		DisplaySettingsList::const_iterator itPos = _settingList.begin();
		DisplaySettingsList::const_iterator itEnd = _settingList.end();

		for ( ; itPos != itEnd; itPos++ )
		{
			if ( (*itPos).PixelWidth == width && (*itPos).PixelHeight == height )
			{
				return true;
			}
		}

		return false;
	}

