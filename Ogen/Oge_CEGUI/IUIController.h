// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-27
// *************************************************************************************

#pragma once

#include "UIControllerPrerequisites.h"

class _OgeCEGUIExports IUIController
	: public RefObject
{
public:

	IUIController(void);
	virtual ~IUIController(void);

	virtual const String& GetName() = 0;
	virtual void Show(bool on) = 0;
	virtual void SetWindowAlpha(float a) = 0;
	virtual float GetWindowAlpha() const = 0;
	virtual CEGUI::Window* GetWindow() const = 0;
	virtual bool IsWindowVisible() const = 0;

	virtual void LoadWindow() = 0;
	virtual void DestroyWindow() = 0;

	virtual void Relayout(int cx, int cy) = 0;

	virtual void OnShow();
	virtual void OnHide();
};

SmartPointer(IUIController);
