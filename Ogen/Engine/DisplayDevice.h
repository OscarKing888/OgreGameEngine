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
	*	显示设备(显卡)信息描述,从Windows相应h文件中复制过来的
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
	*	显示设备信息工具类	
	*/
	class _EngineExport DisplayDevice : public SingletonMyers<DisplayDevice>
	{
	public:

		DisplayDevice();
		virtual ~DisplayDevice(){}

		virtual String GetName() const{ return ("DisplayDevice"); }
	
		/**	取得主显示设备信息 */
		const SDisplayDevice& GetMainDisplayDevice() const { return _mainDisplayDevice; }

		typedef std::vector<SDisplayDevice> DeviceList;
		/**	取得所有显示设备信息列表 */
		const DeviceList& GetAllDisplayDevices() const { return _deviceList; }			

	protected:

		SDisplayDevice _mainDisplayDevice;

		DeviceList _deviceList;
	};
