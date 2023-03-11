#include "stdafx.h"
#include "CameraController.h"

AbstractEditorCameraController::AbstractEditorCameraController()
: _lastCameraSpeed(eCS_Normal)
, _isLMBDown(false)
, _isRMBDown(false)
, _isMMBDown(false)
, _lastMouseDownPos(0, 0)
, _camera(0)
{

}

AbstractEditorCameraController::~AbstractEditorCameraController()
{

}

BOOL AbstractEditorCameraController::OnLButtonDown(UINT nFlags, CPoint point)
{
	_lastMouseDownPos = point;
	_isLMBDown = true;

	return FALSE;
}


BOOL AbstractEditorCameraController::OnLButtonUp(UINT nFlags, CPoint point)
{
	_isLMBDown = false;
	return FALSE;
}

BOOL AbstractEditorCameraController::OnMButtonDown(UINT nFlags, CPoint point)
{
	_isMMBDown = true;
	_lastMouseDownPos = point;
	return FALSE;
}

BOOL AbstractEditorCameraController::OnMButtonUp(UINT nFlags, CPoint point)
{
	_isMMBDown = false;
	return FALSE;
}


BOOL AbstractEditorCameraController::OnRButtonDown(UINT nFlags, CPoint point)
{
	_lastMouseDownPos = point;
	_isRMBDown = true;
	return FALSE;
}

BOOL AbstractEditorCameraController::OnRButtonUp(UINT nFlags, CPoint point)
{
	_isRMBDown = false;
	return FALSE;
}

bool AbstractEditorCameraController::IsLMBDown()
{
	return _isLMBDown;
}

bool AbstractEditorCameraController::IsRMBDown()
{
	return _isRMBDown;
}

bool AbstractEditorCameraController::IsMMBDown()
{
	return _isMMBDown;
}


BOOL AbstractEditorCameraController::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
// 	if(nChar == VK_SHIFT)
// 	{
// 		_lastCameraSpeed = EditorConfig::getSingleton().CameraMoveSpeed;
// 		EditorConfig::getSingleton().CameraMoveSpeed = eCS_Slow;
// 	}
// 	else if(nChar == VK_CONTROL)
// 	{
// 		_lastCameraSpeed = EditorConfig::getSingleton().CameraMoveSpeed;
// 		EditorConfig::getSingleton().CameraMoveSpeed = eCS_Fast;
// 	}

	return FALSE;
}

BOOL AbstractEditorCameraController::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
// 	EditorConfig::getSingleton().CameraMoveSpeed = _lastCameraSpeed;

	return FALSE;
}

//------------------------------------------------------------------------

FPSCamera::FPSCamera()
{
}

FPSCamera::~FPSCamera()
{
}


BOOL FPSCamera::OnMouseMove(UINT nFlags, CPoint point)
{	
	Camera* cam = _camera;

	float dx = (float)(point.x - _lastMouseDownPos.x);
	float dy = (float)(point.y - _lastMouseDownPos.y);

	if((_lastMouseDownPos.x == 0) && (_lastMouseDownPos.y == 0))
	{
		_lastMouseDownPos = point;
		return FALSE;
	}

	EditorConfig& cfg = EditorConfig::getSingleton();
	ECameraSpeed camSpd = cfg.GetCameraMoveSpeed();

	if(IsMMBDown())
	{
		Vector3 right = cam->getRealRight();
		right.normalise();
		Vector3 d = Vector3::ZERO;
		// 左右平移
		//if(MFCInputManager::getSingleton().IsKeyDown(VK_LSHIFT))
		if(camSpd == eCS_Slow)
		{
			d = -right * dx * 0.5f;
		}
		else if(camSpd == eCS_Normal)
			//else if(MFCInputManager::getSingleton().IsKeyDown(VK_LCONTROL))
		{
			d = -right * dx * 5.0f;
		}
		else
		{
			d = -right * dx * 50.0f;
		}

		d.y = 0.0f;
		cam->move(d);

		// 上下平移
		Vector3 camDir = cam->getRealUp();
		camDir.normalise();

		//if(MFCInputManager::getSingleton().IsKeyDown(VK_LSHIFT))
		if(camSpd == eCS_Slow)
		{
			d = camDir * -dy * 0.5f;
		}
		else if(camSpd == eCS_Normal)
			//else if(MFCInputManager::getSingleton().IsKeyDown(VK_LCONTROL))
		{
			d = camDir * -dy * 5.0f;
		}
		else
		{
			d = camDir * -dy * 50.0f;
		}

		cam->move(-d);

		ClampCameraToTerrain(cam);
	}
	else if(IsRMBDown() && (cam->getProjectionType() == PT_PERSPECTIVE))
	{
		// 左右旋转相机
		Radian dxr(-dx * 0.005f);
		Radian dyr(-dy * 0.005f);

		cam->yaw(dxr);
		cam->pitch(dyr);
	}

	_lastMouseDownPos = point;

	return TRUE;
}

BOOL FPSCamera::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta != 0)
	{
		int wheelCnt = zDelta / 120;

		EditorConfig& cfg = EditorConfig::getSingleton();
		ECameraSpeed camSpd = cfg.GetCameraMoveSpeed();

		// 推拉
		Vector3 dir = _camera->getRealDirection().normalisedCopy();
		if(camSpd == eCS_Slow)
		{
			dir *= wheelCnt * 10.0f;
		}
		else if(camSpd == eCS_Normal)
		{
			dir *= wheelCnt * 100.0f;
		}
		else
		{
			dir *= wheelCnt * 1000.0f;
		}

		Camera* cam = _camera;
		if(cam->getProjectionType() == PT_ORTHOGRAPHIC)
		{
			float orthoHeight = cam->getOrthoWindowHeight();
			orthoHeight -= wheelCnt * 1000.0f;
			orthoHeight = std::max(orthoHeight, 2000.0f);
			cam->setOrthoWindowHeight(orthoHeight);
			//LogDebugf("Wheel delta %d %f", wheelCnt, orthoHeight);
		}
		else
		{
			cam->move(dir);

			ClampCameraToTerrain(cam);
		}
	}

	return FALSE;
}
//--------------------------------------------------------------------------------
void FPSCamera::ClampCameraToTerrain( Camera* cam )
{
	EditorConfig& cfg = EditorConfig::getSingleton();
	ECameraSpeed camSpd = cfg.GetCameraMoveSpeed();

	if (cfg.GetClampCameraToTerrain())
	{
		Vector3 pos = cam->getPosition();
		float tpos = GameApp::getSingleton().GetGame()->GetTerrainHeight(pos);
		pos.y = tpos + cfg.GetCameraHeight();
		cam->setPosition(pos);
	}
}
//------------------------------------------------------------------------

LookAtCamera::LookAtCamera()
: _radius(300.0f)
, _xRot(0.0f)
, _yRot(0.0f)
, _lookAt(Vector3::ZERO)
, _minDistance(10.0f)
, _maxDistance(1000.0f)
, _zoomDelta(10.0f)
{
}

LookAtCamera::~LookAtCamera()
{
}


BOOL LookAtCamera::OnMouseMove(UINT nFlags, CPoint point)
{	
	Camera* cam = _camera;

	EditorConfig& cfg = EditorConfig::getSingleton();
	ECameraSpeed camSpd = cfg.GetCameraMoveSpeed();

	float dx = (float)(point.x - _lastMouseDownPos.x);
	float dy = (float)(point.y - _lastMouseDownPos.y);

	if((_lastMouseDownPos.x == 0) && (_lastMouseDownPos.y == 0))
	{
		_lastMouseDownPos = point;
		return FALSE;
	}

	if(IsMMBDown())
	{
		// 上下平移
		float d = 0.0f;
		if(camSpd == eCS_Slow)
		{
			d = -dy * 0.5f;
		}
		else if(camSpd == eCS_Normal)
		{
			d = -dy * 5.0f;
		}
		else
		{
			d = -dy * 50.0f;
		}

		_lookAt.y -= d;
	}
	else if(IsRMBDown())
	{
		float d = Math::TWO_PI / 1200.0f;

		if(camSpd == eCS_Slow)
		{
			d = d * 0.5f;
		}
		else if(camSpd == eCS_Normal)
		{
			d = d * 1.0f;
		}
		else
		{
			d = d * 10.0f;
		}

		_xRot -= dx * d;
		_yRot += dy * d;
	}

	_lastMouseDownPos = point;

	UpdateCamera();

	return TRUE;
}

BOOL LookAtCamera::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta != 0)
	{
		int wheelCnt = zDelta / 120;

		//EditorConfig& cfg = EditorConfig::getSingleton();
		//ECameraSpeed camSpd = cfg.GetCameraMoveSpeed();

		// 推拉
		float d = -_zoomDelta * wheelCnt;

		//if(camSpd == eCS_Fast)
		//{
		//	d = d * 10.0f;
		//}
		//else if(camSpd == eCS_Slow)
		//{
		//	d = d * 0.1f;
		//}
		
		_radius = std::max(_radius + d, _minDistance);

		UpdateCamera();
	}

	return FALSE;
}

void LookAtCamera::SetLookAt(const Vector3& lookAt)
{
	_lookAt = lookAt;
	UpdateCamera();
}

void LookAtCamera::SetCameraRadius(float r)
{
	_radius = r;
	UpdateCamera();
}

void LookAtCamera::UpdateCamera()
{
	float tmp = _radius * cosf(_yRot);
	float z = tmp * cosf(_xRot);
	float x = tmp * sinf(_xRot);
	float y = _radius * sinf(_yRot);

	_camera->setPosition(x, y, z);
	_camera->lookAt(_lookAt);
}