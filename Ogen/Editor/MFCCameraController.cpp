#include "stdafx.h"
#include "MFCCameraController.h"

template<> MFCCameraController* Singleton<MFCCameraController>::ms_Singleton = 0;

MFCCameraController::MFCCameraController()
: _cam(0)
, _cameraSpeed(500) // cm/s
, _moveBackword(false)
, _moveForward(false)
, _moveLeft(false)
, _moveRight(false)
, _rise(false)
, _enableRise(false)
{
	MFCInputManager::getSingleton().AddKeyListener(this);
	GameApp::getSingleton().AddUpdateable(this);
	EventsManager::getSingleton().AddListener(this);
}

MFCCameraController::~MFCCameraController()
{
	MFCInputManager::getSingleton().RemoveKeyListener(this);
	GameApp::getSingleton().RemoveUpdateable(this);
	EventsManager::getSingleton().AddListener(this);
}

void MFCCameraController::SetCamera( Camera* cam )
{
	_cam = cam;
}

Camera* MFCCameraController::GetCamera() const
{
	return _cam;
}

BOOL MFCCameraController::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	switch(nChar)
	{
	case 'W':
	case 'w':
		{
			_moveForward = true;
			return TRUE;
		}

	case 'S':
	case 's':
		{
			_moveBackword = true;
			return TRUE;
		}

	case 'A':
	case 'a':
		{
			_moveLeft = true;
			return TRUE;
		}

	case 'D':
	case 'd':
		{
			_moveRight = true;
			return TRUE;
		}

	case VK_SPACE:
		{
			_rise = true;
			return TRUE;
		}

	}

	return FALSE;
}

BOOL MFCCameraController::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	switch(nChar)
	{
	case 'W':
	case 'w':
		{
			_moveForward = false;
			return TRUE;
		}

	case 'S':
	case 's':
		{
			_moveBackword = false;
			return TRUE;
		}

	case 'A':
	case 'a':
		{
			_moveLeft = false;
			return TRUE;
		}

	case 'D':
	case 'd':
		{
			_moveRight = false;
			return TRUE;
		}

	case VK_SPACE:
		{
			_rise = false;
			return TRUE;
		}
	}

	return FALSE;
}

void MFCCameraController::Update( float deltaTime )
{
	if(!_cam)
	{
		return;
	}

	EditorConfig& cfg = EditorConfig::getSingleton();
	ECameraSpeed camSpeed = cfg.GetCameraMoveSpeed();

	float camSpd = _cameraSpeed;
	if(camSpeed == eCS_Slow)
	{
		camSpd *= 0.5f;
	}
	else if(camSpeed == eCS_Fast)
	{
		camSpd *= 5.0f;
	}

	camSpd *= deltaTime;

	Vector3 right = _cam->getRealRight().normalisedCopy();
	Vector3 dir = _cam->getRealDirection().normalisedCopy();
	Vector3 d = Vector3::ZERO;

	if(_moveLeft)
	{
		d -= right * camSpd;
	}

	if(_moveRight)
	{
		d += right * camSpd;
	}

	if(_moveForward)
	{
		d += dir * camSpd;
	}

	if(_moveBackword)
	{
		d -= dir * camSpd;
	}

	if(_enableRise && _rise)
	{
		d.y += camSpd;
	}

	_cam->move(d);
	if (cfg.GetClampCameraToTerrain())
	{
		Vector3 pos = _cam->getPosition();
		float tpos = GameApp::getSingleton().GetGame()->GetTerrainHeight(pos);
		pos.y = tpos + cfg.GetCameraHeight();
		_cam->setPosition(pos);
	}
}

void MFCCameraController::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(eventName == "EnableRiseCamera")
	{
		_enableRise = any_cast<bool>(eventContex);
	}
}