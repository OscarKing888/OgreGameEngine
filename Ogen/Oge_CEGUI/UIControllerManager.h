// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-31
// *************************************************************************************

#pragma once

#include "IUIController.h"

class _OgeCEGUIExports UIControllerManager
	: public SingletonMyers<UIControllerManager>
{
public:

	UIControllerManager(void);
	~UIControllerManager(void);

	bool IsGUIControllerExists(String ctrlName);
	void AddGUIController(IUIControllerPtr spCtrl);
	void RemoveGUIController(IUIControllerPtr spCtrl);

	IUIControllerPtr GetUIController(const String& name);
	void Relayout(int cx, int cy);

	void ShutDown();

	void ShowAll(bool on);
	void Show(const String& name, bool on);
	StringVector GetAllControllerNameList();

protected:

	typedef std::map<String, IUIControllerPtr>		UIControllerMap;
	UIControllerMap	_mapCtrls;
};
