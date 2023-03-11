// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-12-03
// *************************************************************************************
#include "StdAfx.h"
#include "GameCamera.h"
#include "GameApp.h"
#include "GameObjectFactoryManager.h"
#include "IGame.h"

Camera* GameCamera::_mainCamera = 0;

class GameCameraFactory : public TGameObjectFactory<GameCamera>
{
};

M_REG_FACTORY(GameCamera, GameCameraFactory, Camera);

GameCamera::GameCamera(void)
: _camera(0)
, _lookAt(Vector3::ZERO)
, _cameraEditorModel(0)
, _autoAspectRatio(true)
{
	_isEditorDislpayOnly = true;
}

GameCamera::~GameCamera(void)
{
}

void GameCamera::BuildPropertyMap()
{
	VisualObject::BuildPropertyMap();

	AddProp(Property_Vector3, Lookat, GameCamera, "Lookat", "Camera");
	AddProp(Property_Float, FovDegree, GameCamera, "FovDegree", "Camera");
	AddProp(Property_Float, FocalLength, GameCamera, "FocalLength", "Camera");
	AddProp(Property_Bool, AutoAspectRatio, GameCamera, "AutoAspectRatio", "Camera");
	AddProp(Property_Float, NearClipDistance, GameCamera, "NearClipDistance", "Camera");
	AddProp(Property_Float, FarClipDistance, GameCamera, "FarClipDistance", "Camera");
	
	RemoveProperty("Scale");
}

const String& GameCamera::GetTypeName() const
{
	static String sName("Camera");
	return sName;
}

void GameCamera::PostCreateNode()
{
	VisualObject::PostCreateNode();
	CreateCamera();
}

void GameCamera::PreDestroyNode()
{
	VisualObject::PreDestroyNode();
	DestroyCamera();
}

void GameCamera::CreateCamera()
{
	SceneManager* sm = GetSceneManager();
	_camera = sm->createCamera(GetGUID().toString());
	_camera->MovableObject::setUserAny(Any(AnyGameObject(this)));
	_node->attachObject(_camera);
	_camera->setAutoAspectRatio(_autoAspectRatio);
	_camera->setQueryFlags(GetQueryFlag());
	CreateCameraEditModel();
}

void GameCamera::DestroyCamera()
{
	Restore();

	SceneManager* sm = GetSceneManager();
	sm->destroyCamera(_camera);
	_camera = 0;
	DestroyCameraEditModel();
}

void GameCamera::OnEditorSelect()
{

}

void GameCamera::OnEditorDeselect()
{

}

const Vector3& GameCamera::GetLookat() const
{
	return _lookAt;
}

void GameCamera::SetLookat(const Vector3& lookAt)
{
	_lookAt = lookAt;
	_camera->lookAt(lookAt);
	UpdateCameraEditModel();
}

void GameCamera::CreateCameraEditModel()
{
	if((GameApp::CurrentRunType == ERT_Editor) || _displayEditorObjectInGame)
	{
		uint32 flag = GetQueryFlag();
		SceneManager* sm = _node->getCreator();
		_cameraEditorModel = sm->createEntity(GetGUID().toString() + "/EditorModel", "CameraModel.mesh");
		_cameraEditorModel->setUserAny(Any(AnyGameObject(this)));
		_cameraEditorModel->setQueryFlags(flag);
		_node->attachObject(_cameraEditorModel);
	}
}

void GameCamera::DestroyCameraEditModel()
{
	if((GameApp::CurrentRunType == ERT_Editor) || _displayEditorObjectInGame)
	{
		_node->getCreator()->destroyEntity(_cameraEditorModel);
		_cameraEditorModel = 0;
	}
}

void GameCamera::UpdateCameraEditModel()
{
	if((GameApp::CurrentRunType == ERT_Editor) || _displayEditorObjectInGame)
	{

	}
}

void GameCamera::SetFovDegree(float val)
{
	_camera->setFOVy(Radian(Degree(val)));
}

float GameCamera::GetFovDegree() const
{
	return _camera->getFOVy().valueDegrees();
}

void GameCamera::SetFocalLength(float focalLength)
{
	_camera->setFocalLength(focalLength);
}

float GameCamera::GetFocalLength() const
{
	return _camera->getFocalLength();
}

void GameCamera::Active()
{
	Viewport* vp = GameApp::getSingleton().GetRenderWindow()->getViewport(0);
	if(_mainCamera == 0)
	{
		_mainCamera = vp->getCamera();
	}

	vp->setCamera(_camera);
}

void GameCamera::Restore()
{
	if(_mainCamera)
	{
		Viewport* vp = GameApp::getSingleton().GetRenderWindow()->getViewport(0);
		vp->setCamera(_mainCamera);
	}
}

void GameCamera::SetCamera(Camera* cam)
{
	_camera = cam;
}

const Vector3& GameCamera::GetPosition() const
{
	if(_node)
	{
		return _node->getPosition();
	}

	return _camera->getPosition();
}

void GameCamera::SetPosition(const Vector3& pos)
{
	if(_node)
	{
		_node->setPosition(pos);
	}
	else
	{
		_camera->setPosition(pos);
	}
}

const Vector3& GameCamera::GetScale() const
{
	return Vector3::UNIT_SCALE;
}

void GameCamera::SetScale(const Vector3& /*scale*/)
{

}

const Quaternion& GameCamera::GetOrientation() const
{
	if(_node)
	{
		return _node->getOrientation();
	}

	return _camera->getOrientation();
}

void GameCamera::SetOrientation(const Quaternion& orient)
{
	if(_node)
	{
		_node->setOrientation(orient);
	}
	else
	{
		_camera->setOrientation(orient);
	}
}

bool GameCamera::GetAutoAspectRatio() const
{
	return _autoAspectRatio;
}

void GameCamera::SetAutoAspectRatio(bool val)
{
	if(_camera)
	{
		_camera->setAutoAspectRatio(val);
	}

	_autoAspectRatio = val;
}

void GameCamera::Move(const Vector3& deltaPos)
{
	if(_node)
	{
		_node->setPosition(deltaPos + GetPosition());
	}
	else if(_camera)
	{
		_camera->move(deltaPos);
	}
}

void GameCamera::Yaw(const Radian& angle)
{
	if(_node)
	{
		SetYaw(angle + GetYaw());
	}
	else if(_camera)
	{
		_camera->yaw(angle);
	}
}

void GameCamera::Pitch(const Radian& angle)
{
	if(_node)
	{
		SetPitch(angle + GetPitch());
	}
	else if(_camera)
	{
		_camera->pitch(angle);
	}
}

void GameCamera::Roll(const Radian& angle)
{
	if(_node)
	{
		SetRoll(angle + GetRoll());
	}
	else if(_camera)
	{
		_camera->roll(angle);
	}
}

void GameCamera::SetNearClipDistance( Real farDist )
{
	if(_camera)
	{
		_camera->setNearClipDistance(farDist);
	}
}

Ogre::Real GameCamera::GetNearClipDistance( void ) const
{
	if(_camera)
	{
		return _camera->getNearClipDistance();
	}

	return 0.0f;
}

void GameCamera::SetFarClipDistance( Real farDist )
{
	if(_camera)
	{
		_camera->setFarClipDistance(farDist);
	}
}

Ogre::Real GameCamera::GetFarClipDistance( void ) const
{
	if(_camera)
	{
		return _camera->getFarClipDistance();
	}

	return 0.0f;
}