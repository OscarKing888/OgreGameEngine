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

CEGUI::MouseButton _OgeCEGUIExports ConvertOISMouseButtonToCegui(int buttonID);

class _OgeCEGUIExports CEGUIGame
	: public GeneralGame
	, public OIS::KeyListener
	, public OIS::MouseListener
	, public WindowEventListener
{
public:
	CEGUIGame(void);
	~CEGUIGame(void);

	void SetGUIRootWindow(CEGUI::Window* rootWnd);

	CEGUI::Renderer* getUIRender(){ return _guiRender; }

protected:

	virtual void InitUI();
	virtual void InitScriptModule();
	virtual void InitStates();
	virtual void PostInit();

	virtual void InitImp();
	virtual void ShutDown();
	virtual CEGUI::Window* CreateDefaultGUIRootWnd();

	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	virtual void Update(float deltaTime);

	CEGUI::Renderer*		_guiRender;
	CEGUI::System*			_guiSystem;

	virtual void windowResized(RenderWindow* rw);
	virtual void windowFocusChange(RenderWindow* rw);
	virtual bool windowOnChar(uint32 uc);
};

