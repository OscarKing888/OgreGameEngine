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
	*	��ʾ�豸(�Կ�)��Ϣ����,��Windows��Ӧh�ļ��и��ƹ�����
	*/
	struct _EngineExport SDisplayDevice
	{
		DWORD  cb;		
		TCHAR  DeviceName[32];
		TCHAR  DeviceString[128];
		DWORD  StateFlags;
		TCHAR  DeviceID[128];
		TCHAR  DeviceKey[128];
	};

	/**
	*	��ʾ�豸��Ϣ������	
	*/
	class _EngineExport DisplayDevice : public SingletonMyers<DisplayDevice>
	{
	public:

		DisplayDevice();
		virtual ~DisplayDevice(){}

		virtual String GetName() const{ return ("DisplayDevice"); }
	
		/**	ȡ������ʾ�豸��Ϣ */
		const SDisplayDevice& GetMainDisplayDevice() const { return _mainDisplayDevice; }

		typedef std::vector<SDisplayDevice> DeviceList;
		/**	ȡ��������ʾ�豸��Ϣ�б� */
		const DeviceList& GetAllDisplayDevices() const { return _deviceList; }			

	protected:

		SDisplayDevice _mainDisplayDevice;

		DeviceList _deviceList;
	};
