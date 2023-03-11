// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-09-24
// *************************************************************************************


#include "StdAfx.h"
#include "CmdLineUI.h"
#include "AbstractUIController.h"
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <Elements/CEGUICombobox.h>
#include <CEGUIExceptions.h>

CmdLineUI::CmdLineUI()
: AbstractUIController(("CommandLine"), ("CommandLine.layout"))
{
}

CmdLineUI::~CmdLineUI()
{

}

void CmdLineUI::Show(bool show)
{
	AbstractUIController::Show(show);
	CEGUI::Window* pW = CEGUI_GET_WINDOW(("CommandLineUI/InputBox"));
	pW->activate();
}

void CmdLineUI::OnSetupEvents()
{
	CEGUI_HANDLE_EVENT(("CommandLineUI/InputBox"), CEGUI::Combobox::EventKeyDown, CmdLineUI::OnKeyDown, this);
	CEGUI_HANDLE_EVENT(("CommandLineUI/InputBox"), CEGUI::Combobox::EventTextAccepted, CmdLineUI::OnOK, this);	
}

bool CmdLineUI::OnKeyDown(const CEGUI::EventArgs& e)
{
	const CEGUI::KeyEventArgs& a = (const CEGUI::KeyEventArgs&)e;
	if(a.scancode == 0x29/*OIS::KC_GRAVE*/)
	{
		Show(false);
	}
	else if(a.scancode == CEGUI::Key::ArrowUp)
	{
		if(_currentCmdIdx > 0)
		{
			--_currentCmdIdx;
		}
		
		UpdateCurrentCommandFromHistory();
	}
	else if(a.scancode == CEGUI::Key::ArrowDown)
	{
		if(_currentCmdIdx < (int)_cmdHistory.size() - 1)
		{
			++_currentCmdIdx;
		}

		UpdateCurrentCommandFromHistory();
	}

	return true;
}

bool CmdLineUI::OnOK(const CEGUI::EventArgs& e)
{
	CEGUI::Combobox* box = (CEGUI::Combobox*)CEGUI_GET_WINDOW(("CommandLineUI/InputBox"));
	CEGUI::String txt = box->getText();
	//box->addItem(OGRE_NEW CEGUI::ListboxTextItem(txt));
	String str = txt.c_str();
	if(!str.empty())
	{
		//CommandLineParserManager::getSingleton().Parse(str);
		_cmdHistory.push_back(str);
		box->setText("");
		_currentCmdIdx = _cmdHistory.size() - 1;
	}

	return true;
}

void CmdLineUI::UpdateCurrentCommandFromHistory()
{
	if(!_cmdHistory.empty())
	{
		CEGUI::Combobox* box = (CEGUI::Combobox*)CEGUI_GET_WINDOW(("CommandLineUI/InputBox"));
		box->setText(_cmdHistory[_currentCmdIdx].c_str());
	}
}