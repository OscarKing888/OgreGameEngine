// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-29
// *************************************************************************************

#include "StdAfx.h"
#include "OISInputManager.h"
#include "GameApp.h"
#include "ActionMapManager.h"

using namespace OIS;

OISInputManager::OISInputManager(void)
: _mouse(0)
, _keyboard(0)
, _inputMgr(0)
, _renderWnd(0)
, _guiKeyListener(0)
, _guiMouseListener(0)
{
}

OISInputManager::~OISInputManager(void)
{
	if (_inputMgr) {
		if (_keyboard) 
		{
			_inputMgr->destroyInputObject(_keyboard);
			_keyboard = 0;
		}

		if (_mouse) {
			_inputMgr->destroyInputObject(_mouse);
			_mouse = 0;
		}

		_inputMgr->destroyInputSystem(_inputMgr);
		_inputMgr = 0;

		// Clear Listeners
		_keyListeners.clear();
		_mouseListeners.clear();
	}
}

void OISInputManager::Init(RenderWindow* win, bool hideCursor)
{
	_renderWnd = win;

	OGRE_NEW ActionMapManager();

	OIS::ParamList pl;	
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	win->getCustomAttribute(("WINDOW"), &windowHnd);
	windowHndStr << (unsigned int)windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	//if(!hideCursor)
	{
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	}

	_inputMgr = OIS::InputManager::createInputSystem(pl);

	using namespace OIS;
	_mouse = static_cast<Mouse*>(_inputMgr->createInputObject(OISMouse, true));
	_keyboard = static_cast<Keyboard*>(_inputMgr->createInputObject(OISKeyboard, true));

	_mouse->setEventCallback(this);
	_keyboard->setEventCallback(this);

	WindowEventUtilities::addWindowEventListener(win, this);
	windowResized(win);
}

void OISInputManager::ShutDown()
{
	OGRE_DELETE ActionMapManager::getSingletonPtr();

	if(_inputMgr)
	{
		WindowEventUtilities::removeWindowEventListener(_renderWnd, this);
		_inputMgr->destroyInputObject(_mouse);
		_inputMgr->destroyInputObject(_keyboard);
		OIS::InputManager::destroyInputSystem(_inputMgr);
		_inputMgr = 0;
	}
}

void OISInputManager::Capture()
{
	if(_mouse)
	{
		_mouse->capture();
	}

	if(_keyboard)
	{
		_keyboard->capture();
	}		
}

//-------------------------------------------------------------------------------------

void OISInputManager::windowResized(RenderWindow* rw)
{
	if(_mouse)
	{
		const OIS::MouseState& ms = const_cast<OIS::MouseState&>(_mouse->getMouseState());
		ms.width = rw->getWidth();
		ms.height = rw->getHeight();
	}
}

void OISInputManager::ClearMouseListeners()
{
	MouseListenerList::iterator i = _mouseListeners.begin();
	while (i != _mouseListeners.end())
	{
		if((*i) == 0)
		{
			i = _mouseListeners.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void OISInputManager::ClearKeyListeners()
{
	KeyListenerList::iterator i = _keyListeners.begin();
	while (i != _keyListeners.end())
	{
		if((*i) == 0)
		{
			i = _keyListeners.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void OISInputManager::AddMouseListener(OIS::MouseListener* ml)
{
	MouseListenerList::iterator i = std::find(_mouseListeners.begin(), _mouseListeners.end(), ml);
	if(i == _mouseListeners.end())
	{
		i = std::find(_addMouseListeners.begin(), _addMouseListeners.end(), ml);
		if(i == _addMouseListeners.end())
		{
			_addMouseListeners.push_back(ml);
			return;
		}
	}

	assert(false);
}

void OISInputManager::RemoveMouseListener(OIS::MouseListener* ml)
{
	MouseListenerList::iterator i = std::find(_mouseListeners.begin(), _mouseListeners.end(), ml);
	if(i != _mouseListeners.end())
	{
		//_mouseListeners.erase(i);
		*i = 0;
	}

	i = std::find(_addMouseListeners.begin(), _addMouseListeners.end(), ml);
	if(i != _addMouseListeners.end())
	{
		*i = 0;
	}
}

void OISInputManager::AddKeyListener(OIS::KeyListener* kl)
{
	KeyListenerList::iterator i = std::find(_keyListeners.begin(), _keyListeners.end(), kl);
	if(i == _keyListeners.end())
	{
		i = std::find(_addKeyListeners.begin(), _addKeyListeners.end(), kl);
		if(i == _addKeyListeners.end())
		{
			_addKeyListeners.push_back(kl);
			return;
		}
	}

	assert(false);
}

void OISInputManager::RemoveKeyListener(OIS::KeyListener* kl)
{
	KeyListenerList::iterator i = std::find(_keyListeners.begin(), _keyListeners.end(), kl);
	if(i != _keyListeners.end())
	{
		//_keyListeners.erase(i);
		*i = 0;
	}

	i = std::find(_addKeyListeners.begin(), _addKeyListeners.end(), kl);
	if(i != _addKeyListeners.end())
	{
		*i = 0;
	}
}


bool OISInputManager::mouseMoved( const MouseEvent &e )
{
	HWND hwnd = GameApp::getSingleton().GetGameWindowHWND();

	POINT ptCur;
	::GetCursorPos(&ptCur);
	::ScreenToClient(hwnd, &ptCur);

	RECT rcC;
	::GetClientRect(hwnd, &rcC);

	//bool showCursor = (::PtInRect(&rcC, ptCur) != TRUE) || (ptCur.x < 0) || (ptCur.y < 0);
	//printf("%d, %d [%d, %d, %d, %d] Show:%d\n", ptCur.x, ptCur.y, rcC.left, rcC.top, rcC.right, rcC.bottom, showCursor);

	//::ShowCursor((BOOL)showCursor);

	// 重新构造事件信息
	MouseState s = e.state;
	MouseEvent arg((OIS::Object*)e.device, s);
	s.X.abs = ptCur.x;
	s.Y.abs = ptCur.y;

	if(_guiMouseListener)
	{
		if(_guiMouseListener->mouseMoved(arg))
		{
			return true;
		}
	}

	UpdateListenersModify();

	MouseListenerList::iterator i = _mouseListeners.begin();
	for (; i != _mouseListeners.end(); ++i)
	{
		if((*i) && (*i)->mouseMoved(arg))
		{
			return true;
		}
	}
	return false;
}

void OISInputManager::UpdateListenersModify()
{
	{
		MouseListenerList::iterator i = _addMouseListeners.begin();
		for (; i != _addMouseListeners.end(); ++i)
		{
			if(*i)
			{
				_mouseListeners.push_back(*i);
			}
		}
	}

	_addMouseListeners.clear();

	{
		KeyListenerList::iterator i = _addKeyListeners.begin();
		for (; i != _addKeyListeners.end(); ++i)
		{
			if(*i)
			{
				_keyListeners.push_back(*i);
			}
		}
	}

	_addKeyListeners.clear();

	ClearMouseListeners();
	ClearKeyListeners();
}

bool OISInputManager::mousePressed( const MouseEvent &arg, MouseButtonID id )
{
	if(_guiMouseListener)
	{
		if(_guiMouseListener->mousePressed(arg, id))
		{
			return true;
		}
	}

	UpdateListenersModify();

	MouseListenerList::iterator i = _mouseListeners.begin();
	for (; i != _mouseListeners.end(); ++i)
	{
		if((*i) && (*i)->mousePressed(arg, id))
		{
			return true;
		}
	}
	return false;
}

bool OISInputManager::mouseReleased( const MouseEvent &arg, MouseButtonID id )
{
	if(_guiMouseListener)
	{
		if(_guiMouseListener->mouseReleased(arg, id))
		{
			return true;
		}
	}

	UpdateListenersModify();

	MouseListenerList::iterator i = _mouseListeners.begin();
	for (; i != _mouseListeners.end(); ++i)
	{
		if((*i) && (*i)->mouseReleased(arg, id))
		{
			return true;
		}
	}
	return false;
}

bool OISInputManager::keyPressed( const KeyEvent &arg )
{
	if(_guiKeyListener)
	{
		if(_guiKeyListener->keyPressed(arg))
		{
			return true;
		}
	}

	if(ActionMapManager::getSingleton().HandleKeyPressed(arg.key))
	{
		return true;
	}

	UpdateListenersModify();

	KeyListenerList::iterator i = _keyListeners.begin();
	for (; i != _keyListeners.end(); ++i)
	{
		if((*i) && (*i)->keyPressed(arg))
		{
			return true;
		}
	}
	return false;
}

bool OISInputManager::keyReleased( const KeyEvent &arg )
{
	if(_guiKeyListener)
	{
		if(_guiKeyListener->keyReleased(arg))
		{
			return true;
		}
	}

	if(ActionMapManager::getSingleton().HandleKeyReleased(arg.key))
	{
		return true;
	}

	UpdateListenersModify();

	KeyListenerList::iterator i = _keyListeners.begin();
	for (; i != _keyListeners.end(); ++i)
	{
		if((*i) && (*i)->keyReleased(arg))
		{
			return true;
		}
	}
	return false;
}

bool OISInputManager::IsModifierDown(OIS::Keyboard::Modifier m)
{
	assert(_keyboard);
	return _keyboard->isModifierDown(m);
}

bool OISInputManager::IsKeyDown(OIS::KeyCode k)
{
	assert(_keyboard);
	return _keyboard->isKeyDown(k);
}

void OISInputManager::Update(float deltaTime)
{
	Capture();
}

