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
#include "AbstractUIController.h"
#include <CEGUISystem.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>

AbstractUIController::AbstractUIController(const String& name, const String& ceguiLayoutFile,const String& layoutPrefix)
: _name(name)
, _window(0)
, _layoutFileName(ceguiLayoutFile)
, _layoutPrefix(layoutPrefix)
{
}

AbstractUIController::~AbstractUIController(void)
{
	DestroyWindow();
}

const String& AbstractUIController::GetName()
{
	return _name;
}

void AbstractUIController::Show(bool show)
{
	ShowWindow(show, _window);
}

void AbstractUIController::ShowWindow(bool show, CEGUI::Window* wnd)
{
	assert(wnd);

	CEGUI::Window* pRootWindow = CEGUI::System::getSingleton().getGUISheet();
	assert(pRootWindow);

	if(show)
	{
		if(wnd->getParent() == 0)
		{
			pRootWindow->addChildWindow(wnd);
		}

		wnd->show();
		OnShow();
	}
	else
	{
		wnd->hide();
		OnHide();
		/*if(pRootWindow->getChildCount() > 0)
		{
			pRootWindow->removeChildWindow(wnd);
		}*/
	}
}

void AbstractUIController::SetWindowAlpha(float a)
{
	assert(_window);
	return _window->setAlpha(a);
}

float AbstractUIController::GetWindowAlpha() const
{
	assert(_window);
	return _window->getAlpha();
}

CEGUI::Window* AbstractUIController::GetWindow() const
{
	return _window;
}

bool AbstractUIController::IsWindowVisible() const
{
	assert(_window);
	return _window->isVisible();
}

void AbstractUIController::LoadWindow()
{
	assert(_window == 0);
	_window = CEGUI::WindowManager::getSingleton().loadWindowLayout(_layoutFileName.c_str(), _layoutPrefix);
	assert(_window != NULL);
	_window->hide();

	OnSetupEvents();
}

void AbstractUIController::DestroyWindow()
{
	if(_window != NULL)
	{
		CEGUI::WindowManager::getSingleton().destroyWindow(_window);
		_window = NULL;
	}
}

void AbstractUIController::Relayout(int cx, int cy)
{

}

void AbstractUIController::ShowGUIWindow(CEGUI::Window* pWindow)
{
	ShowWindow(true, pWindow);
}

void AbstractUIController::HideGUIWindow(CEGUI::Window* pWindow)
{
	ShowWindow(false, pWindow);
}

bool AbstractUIController::IsWindowOrChildActive()
{
	assert(_window);
	return IsWindowOrChildActive(_window);
}

bool AbstractUIController::IsWindowOrChildActive(CEGUI::Window* pWindow)
{
	bool r = pWindow->isActive();
	if(!r)
	{
		size_t c = pWindow->getChildCount();
		for (size_t i = 0; i < c; ++i)
		{
			CEGUI::Window* pC = pWindow->getChildAtIdx(i);
			r = IsWindowOrChildActive(pC);

			if(r)
			{
				return true;
			}
		}
	}

	return true;
}