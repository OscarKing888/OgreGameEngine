// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-09-24
// *************************************************************************************

#pragma once
#include "UIControllerPrerequisites.h"
#include "AbstractUIController.h"

class _OgeCEGUIExports CmdLineUI : public AbstractUIController
{
public:

	CmdLineUI();
	virtual ~CmdLineUI();

	virtual void OnSetupEvents();

protected:

	bool OnKeyDown(const CEGUI::EventArgs& e);
	bool OnOK(const CEGUI::EventArgs& e);

	virtual void Show(bool show);

	StringVector	_cmdHistory;
	int					_currentCmdIdx;

	void UpdateCurrentCommandFromHistory();
};

