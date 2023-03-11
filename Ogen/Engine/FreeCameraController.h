// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-30
// *************************************************************************************

#pragma once
#include "AbstractCameraController.h"
#include "IUpdateAble.h"

class _EngineExport FreeCameraController
	: public AbstractCameraController
	, public OIS::KeyListener
	, public OIS::MouseListener
	, public INonPauseUpdateAble
{
public:

	FreeCameraController(void);
	virtual ~FreeCameraController(void);

	virtual const String& GetTypeName() const;

	void SetHandleInput(bool on);

protected:

	bool			_bRMBDown;
	bool			_bLMBDown;
	Vector2	_lastDown;

	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );

	virtual void Update(float deltaTime);
};

SmartPointer(FreeCameraController);
