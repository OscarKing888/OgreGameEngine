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
	*	��Ļ��ʾ������Ϣ����
	*/
	class _EngineExport DisplaySettings : public SingletonMyers<DisplaySettings>
	{
	public:

		DisplaySettings();
		virtual ~DisplaySettings(){}

		virtual String GetName() const{ return "DisplaySettings"; }

		/** ��ʾ������Ϣ */
		struct SDisplaySetting
		{
			/** ��ʾ���豸���� */
			String DeviceName;

			/** ��ǰ��ɫ��� */
			int BitsPerPixel;

			/** ��ǰ��Ļ��� */
			int PixelWidth;

			/** ��ǰ��Ļ�߶� */
			int PixelHeight;

			/** ˢ���� */
			int DisplayFrequency;

			SDisplaySetting()
			{
				BitsPerPixel = 0;
				PixelWidth = 0;
				PixelHeight = 0;
				DisplayFrequency = 0;
			}
		};

		/** ȡ�õ�ǰ��ʾ���� */
		const SDisplaySetting& GetCurrentDisplaySettings() const { return _currentDisplayMode; }

		typedef std::vector<SDisplaySetting> DisplaySettingsList;

		/** ȡ�����еĿ�����ʾ�����б�*/
		DisplaySettingsList GetAllDisplaySettings() const { return _settingList; }

		// ��鵱ǰ�ֱ����Ƿ�֧��
		bool IsSupportResolution( int width, int height );

	protected:

		SDisplaySetting _currentDisplayMode;
		DisplaySettingsList _settingList;
	};
