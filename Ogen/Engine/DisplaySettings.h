// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-25
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"



	/**
	*	屏幕显示设置信息工具
	*/
	class _EngineExport DisplaySettings : public SingletonMyers<DisplaySettings>
	{
	public:

		DisplaySettings();
		virtual ~DisplaySettings(){}

		virtual String GetName() const{ return "DisplaySettings"; }

		/** 显示设置信息 */
		struct SDisplaySetting
		{
			/** 显示器设备名字 */
			String DeviceName;

			/** 当前颜色深度 */
			int BitsPerPixel;

			/** 当前屏幕宽度 */
			int PixelWidth;

			/** 当前屏幕高度 */
			int PixelHeight;

			/** 刷新率 */
			int DisplayFrequency;

			SDisplaySetting()
			{
				BitsPerPixel = 0;
				PixelWidth = 0;
				PixelHeight = 0;
				DisplayFrequency = 0;
			}
		};

		/** 取得当前显示设置 */
		const SDisplaySetting& GetCurrentDisplaySettings() const { return _currentDisplayMode; }

		typedef std::vector<SDisplaySetting> DisplaySettingsList;

		/** 取得所有的可用显示设置列表*/
		DisplaySettingsList GetAllDisplaySettings() const { return _settingList; }

		// 检查当前分辨率是否支持
		bool IsSupportResolution( int width, int height );

	protected:

		SDisplaySetting _currentDisplayMode;
		DisplaySettingsList _settingList;
	};
