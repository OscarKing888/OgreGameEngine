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

class _OgeCEGUIExports AbstractUIController
	: public IUIController
{
public:

	explicit AbstractUIController(const String& name, const String& ceguiLayoutFile,const String& layoutPrefix = (""));
	virtual ~AbstractUIController(void);

	virtual const String& GetName();
	virtual void Show(bool show);
	virtual void SetWindowAlpha(float a);
	virtual float GetWindowAlpha() const;
	virtual CEGUI::Window* GetWindow() const;
	virtual bool IsWindowVisible() const;

	virtual void LoadWindow();
	virtual void DestroyWindow();

	virtual void Relayout(int cx, int cy);

	virtual void OnSetupEvents(){}

	virtual void ShowGUIWindow(CEGUI::Window* pWindow);
	virtual void HideGUIWindow(CEGUI::Window* pWindow);

	virtual bool IsWindowOrChildActive();

	// 此窗口是否有其子控件激活
	bool IsWindowOrChildActive(CEGUI::Window* pWindow);

protected:

	CEGUI::Window*	_window;
	String	_name;
	String	_layoutFileName;
	String	_layoutPrefix;

	void ShowWindow(bool show, CEGUI::Window* wnd);
};

