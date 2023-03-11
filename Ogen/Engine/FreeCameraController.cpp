// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-30
// *************************************************************************************

#include "StdAfx.h"
#include "FreeCameraController.h"
#include "OISInputManager.h"
#include "GameApp.h"

using namespace OIS;


// Rotate the camera by PI/5
const float C_RotSpeed = Math::PI / 3.0f;

FreeCameraController::FreeCameraController(void)
: _bRMBDown(false)
, _bLMBDown(false)
, _lastDown(0.0f, 0.0f)
{
	SetHandleInput(true);
	GameApp::getSingleton().AddUpdateable(this);
}

FreeCameraController::~FreeCameraController(void)
{
	SetHandleInput(false);
	GameApp::getSingleton().RemoveUpdateable(this);
}

const String& FreeCameraController::GetTypeName() const
{
	static String C_TypeName = "FreeCameraController";
	return C_TypeName;
}

void FreeCameraController::SetHandleInput(bool on)
{
	if(on)
	{
		OISInputManager::getSingleton().AddKeyListener(this);
		OISInputManager::getSingleton().AddMouseListener(this);
	}
	else
	{
		OISInputManager::getSingleton().RemoveKeyListener(this);
		OISInputManager::getSingleton().RemoveMouseListener(this);
	}
}


bool FreeCameraController::mouseMoved( const MouseEvent &arg )
{
#ifndef _PRODUCT_
	float d = arg.state.Z.rel * 0.1f;

	if(d != 0.0f)
	{
		Zoom(d);
		return true;
	}

	if(!_bRMBDown && !_bLMBDown)
	{
		_lastDown.x = arg.state.X.abs;
		_lastDown.y = arg.state.Y.abs;
		return false;
	}

	Vector2 pt(arg.state.X.abs, arg.state.Y.abs);

	if(arg.state.buttonDown(OIS::MB_Right)) // ÓÒ¼ü×ÔÓÉÐý×ª
	{
		Radian dx((float)-arg.state.X.rel * 0.003f);
		Radian dy((float)-arg.state.Y.rel * 0.003f);

		LookAround(dx, dy);
	}
#endif

	return true;
}

bool FreeCameraController::mousePressed( const MouseEvent &arg, MouseButtonID id )
{
#ifndef _PRODUCT_
	_bRMBDown = arg.state.buttonDown(OIS::MB_Right);
	_bLMBDown = arg.state.buttonDown(OIS::MB_Left);

	if(_bLMBDown || _bRMBDown)
	{
		_lastDown.x = arg.state.X.abs;
		_lastDown.y = arg.state.Y.abs;

		//_arcBall.OnBegin(arg.state.X.abs, arg.state.Y.abs);
		return true;
	}
#endif

	return false;
}

bool FreeCameraController::mouseReleased( const MouseEvent &arg, MouseButtonID id )
{
#ifndef _PRODUCT_
	_bRMBDown = arg.state.buttonDown(OIS::MB_Right);
	_bLMBDown = arg.state.buttonDown(OIS::MB_Left);
	
	if(!(_bLMBDown || _bRMBDown))
	{
		//_arcBall.OnEnd(arg.state.X.abs, arg.state.Y.abs);
		return true;
	}
#endif

	return false;
}

bool FreeCameraController::keyPressed( const KeyEvent &arg )
{
#ifndef _PRODUCT_
	if(arg.key == OIS::KC_T)
	{
		SetToPanCamera(!IsPanCamera());
		return true;
	}
#endif
	return false;
}

bool FreeCameraController::keyReleased( const KeyEvent &arg )
{
	return false;
}

void FreeCameraController::Update(float deltaTime)
{
#ifndef _PRODUCT_

	deltaTime /= GameApp::getSingleton().GetUpdateSpeed();

	OISInputManager& im = OISInputManager::getSingleton();
	if(im.IsKeyDown(OIS::KC_W))
	{
		MoveForward(deltaTime);
	}

	if(im.IsKeyDown(OIS::KC_S))
	{
		MoveBackward(deltaTime);
	}

	if(im.IsKeyDown(OIS::KC_A))
	{
		MoveLeft(deltaTime);
	}

	if(im.IsKeyDown(OIS::KC_D))
	{
		MoveRight(deltaTime);
	}

	if(im.IsKeyDown(OIS::KC_Q))
	{
		Rise(deltaTime);
	}

	if(im.IsKeyDown(OIS::KC_E))
	{
		Down(deltaTime);
	}

	if(im.IsKeyDown(OIS::KC_LEFT))
	{
		GetOgreCamera()->yaw(Radian(C_RotSpeed * deltaTime));
	}

	if(im.IsKeyDown(OIS::KC_RIGHT))
	{
		GetOgreCamera()->yaw(Radian(-C_RotSpeed * deltaTime));
	}

	if(im.IsKeyDown(OIS::KC_UP))
	{
		
		GetOgreCamera()->pitch(Radian(C_RotSpeed * deltaTime));
	}

	if(im.IsKeyDown(OIS::KC_DOWN))
	{
		GetOgreCamera()->pitch(Radian(-C_RotSpeed * deltaTime));
	}
#endif
}
