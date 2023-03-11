// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-31
// *************************************************************************************

#include "StdAfx.h"
#include "UIControllerManager.h"

// using namespace UIController;

UIControllerManager::UIControllerManager(void)
{
}

UIControllerManager::~UIControllerManager(void)
{
}

void UIControllerManager::AddGUIController(IUIControllerPtr spCtrl)
{
	UIControllerMap::iterator i = _mapCtrls.find(spCtrl->GetName());
	if(i != _mapCtrls.end())
	{
		LogErrorIDf("UI", ("[UIControllerManager::AddGUIController][%s] already exist"), spCtrl->GetName().c_str());
		assert(false);
	}
	else
	{
		_mapCtrls.insert(std::make_pair(spCtrl->GetName(), spCtrl));
		spCtrl->LoadWindow();
	}
}

bool UIControllerManager::IsGUIControllerExists(String ctrlName)
{
	UIControllerMap::iterator i = _mapCtrls.find(ctrlName);
	return i != _mapCtrls.end();
}

void UIControllerManager::RemoveGUIController(IUIControllerPtr spCtrl)
{
	UIControllerMap::iterator i = _mapCtrls.find(spCtrl->GetName());
	if(i != _mapCtrls.end())
	{
		_mapCtrls.erase(i);
	}
	else
	{
		LogErrorIDf("UI", ("[UIControllerManager::RemoveGUIController][%s] not found"), spCtrl->GetName().c_str());
		assert(false);
	}
}

IUIControllerPtr UIControllerManager::GetUIController(const String& name)
{
	UIControllerMap::iterator i = _mapCtrls.find(name);
	if(i != _mapCtrls.end())
	{
		return i->second;
	}

	LogErrorIDf("UI", ("[UIControllerManager::GetUIController][%s] not found"), name.c_str());
	assert(false);
	return 0;
}

void UIControllerManager::Relayout(int cx, int cy)
{
	for(UIControllerMap::iterator i = _mapCtrls.begin(); i != _mapCtrls.end(); ++i)
	{
		IUIControllerPtr pCtrl = i->second;
		pCtrl->Relayout(cx, cy);
	}
}

void UIControllerManager::ShutDown()
{
	_mapCtrls.clear();
}


void UIControllerManager::ShowAll(bool on)
{
	for(UIControllerMap::iterator i = _mapCtrls.begin(); i != _mapCtrls.end(); ++i)
	{
		IUIControllerPtr pCtrl = i->second;
		pCtrl->Show(on);
	}
}

void UIControllerManager::Show(const String& name, bool on)
{
	GetUIController(name)->Show(on);
}

StringVector UIControllerManager::GetAllControllerNameList()
{
	StringVector lst;
	for(UIControllerMap::iterator i = _mapCtrls.begin(); i != _mapCtrls.end(); ++i)
	{
		lst.push_back(i->first);
	}

	return lst;
}