// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-29
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"
#include "IUpdateAble.h"

// namespace Input

	class _EngineExport OISInputManager
		: public SingletonMyers<OISInputManager>
		, public WindowEventListener
		, public OIS::MouseListener
		, public OIS::KeyListener
		, public INonPauseUpdateAble
	{
	public:

		OISInputManager(void);
		~OISInputManager(void);

		void Init(RenderWindow* win, bool hideCursor = true);
		void ShutDown();
		void Capture();
		
		void AddMouseListener(OIS::MouseListener* ml);
		void RemoveMouseListener(OIS::MouseListener* ml);

		void AddKeyListener(OIS::KeyListener* kl);
		void RemoveKeyListener(OIS::KeyListener* kl);

		void AddJoyStickListener(OIS::JoyStickListener * jl);
		void RemoveJoyStickListener(OIS::JoyStickListener * jl);
		
		bool IsModifierDown(OIS::Keyboard::Modifier m);
		bool IsKeyDown(OIS::KeyCode k);
// 		OIS::Mouse* GetMouse() const { assert(_mouse != 0); return _mouse; }
// 		OIS::Keyboard* GetKeyboard() const { assert(_keyboard != 0); return _keyboard; }
// 		OIS::InputManager* GetInputManager() const { assert(_inputMgr != 0); return _inputMgr; }
        
		void SetGUIMouseListener(OIS::MouseListener* ml) { _guiMouseListener = ml; }
		void RemoveGUIMouseListener() { _guiMouseListener = 0; }

		void SetGUIKeyListener(OIS::KeyListener* kl) { _guiKeyListener = kl; }
		void RemoveGUIKeyListener() { _guiKeyListener = 0; }

		virtual void Update(float deltaTime);

	protected:
		
		OIS::Mouse*			_mouse;
		OIS::Keyboard*		_keyboard;
		OIS::InputManager*	_inputMgr;
		RenderWindow*	_renderWnd;

		OIS::KeyListener*	_guiKeyListener;
		OIS::MouseListener*	_guiMouseListener;

		virtual void windowResized(RenderWindow* rw);

		typedef std::vector<OIS::MouseListener*> MouseListenerList;
		typedef std::vector<OIS::KeyListener*> KeyListenerList;

		MouseListenerList	_mouseListeners;
		MouseListenerList	_addMouseListeners;
		KeyListenerList		_keyListeners;
		KeyListenerList	_addKeyListeners;

		void ClearMouseListeners();
		void ClearKeyListeners();
		void UpdateListenersModify();

		virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

		virtual bool keyPressed( const OIS::KeyEvent &arg );
		virtual bool keyReleased( const OIS::KeyEvent &arg );
	};
