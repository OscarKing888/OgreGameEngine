#pragma once

#ifdef OGE_CEGUI_EXPORTS
#	define _OgeCEGUIExports __declspec(dllexport)
#else
#	define _OgeCEGUIExports __declspec(dllimport)
#endif

#ifndef OGE_CEGUI_EXPORTS
#	ifdef _DEBUG
#		pragma comment(lib, "Oge_CEGUI_Debug.lib")
#	else
#		pragma comment(lib, "Oge_CEGUI_Release.lib")
#	endif
#endif

#include "Engine/Engine.h"

#include <CEGUI.h>

#ifdef _DEBUG
#	pragma comment(lib, "CEGUIBase_d.lib")
#	pragma comment(lib, "CEGUIOgreRenderer_d.lib")
#else
#	pragma comment(lib, "CEGUIBase.lib")
#	pragma comment(lib, "CEGUIOgreRenderer.lib")
#endif



//
// Handle Event
//
#define CEGUI_HANDLE_EVENT(wndName, eventName, funName, objPtr) \
	try \
{ \
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)wndName)-> \
	subscribeEvent(eventName, CEGUI::Event::Subscriber(&funName, objPtr)); \
} \
	catch (const CEGUI::UnknownObjectException e) \
{\
	LogErrorIDf("UI", ("GUI item %s not found! %s:%d"), (LPCTSTR)wndName, __FILE__, __LINE__); \
}

//
// Handle Event Optional
//
#define CEGUI_HANDLE_EVENT_OPTIONAL(wndName, eventName, funName, objPtr) \
	if (CEGUI::WindowManager::getSingleton().isWindowPresent((CEGUI::utf8*)wndName)) \
{ \
	CEGUI_HANDLE_EVENT(wndName, eventName, funName, objPtr); \
}

//
// Get Window
//
#define CEGUI_GET_WINDOWP(wndName, wndPointer) \
	try \
{ \
	wndPointer = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)wndName); \
	assert(wndPointer != NULL); \
}\
	catch (const CEGUI::UnknownObjectException e) \
{\
	LogErrorIDf("UI", ("Window %s not found! %s:%d"), (LPCTSTR)wndName, __FILE__, __LINE__); \
	wndPointer = NULL; \
}

//
// Get window and return value
//

#define CEGUI_GET_WINDOW(wndName) GetCEGUIWindow(wndName)	

//
// Get Window Optional
//
#define CEGUI_GET_WINDOW_OPTIONAL(wndName, wndPointer) \
	if (CEGUI::WindowManager::getSingleton().isWindowPresent((CEGUI::utf8*)wndName)) \
{ \
	CEGUI_GET_WINDOW(wndName, wndPointer); \
} \
	else \
{ \
	wndPointer = NULL; \
}

//
// Show Window
//
#define CEGUI_SHOW_WINDOW(wndName, isShow) \
	try \
{ \
	CEGUI::Window* wndPointer = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)wndName); \
	assert(wndPointer != NULL); \
	if(isShow){wndPointer->show();}else{wndPointer->hide();}\
}\
	catch (const CEGUI::UnknownObjectException e) \
{\
	LogErrorIDf("UI", ("Window %s not found! %s:%d"), wndName, __FILE__, __LINE__); \
}

//
// Enable Window
//
#define CEGUI_ENABLE_WINDOW(wndName, isEnable) \
	try \
{ \
	CEGUI::Window* wndPointer = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)wndName); \
	assert(wndPointer != NULL); \
	if(isEnable){wndPointer->enable();}else{wndPointer->disable();}\
}\
	catch (const CEGUI::UnknownObjectException e) \
{\
	LogErrorIDf("UI", ("Window %s not found! %s:%d"), wndName, __FILE__, __LINE__); \
}

