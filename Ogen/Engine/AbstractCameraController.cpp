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
#include "AbstractCameraController.h"


AbstractCameraController::AbstractCameraController(void)
: _camera(0)
, _cameraSpeed(U_CM(50.0f)) // 500cm/s
, _lookAt(0, 0, 0)
, _panCamera(false)
{
}

AbstractCameraController::~AbstractCameraController(void)
{
}

void AbstractCameraController::SetCamera(GameCamera* cam)
{
	_camera = cam;
}

GameCamera* AbstractCameraController::GetCamera() const
{
	assert(_camera);
	return _camera;
}


Ray AbstractCameraController::GetCameraRay()
{
	Camera* cam = GetOgreCamera();
	float tx = (float)_camera->GetPosition().x / cam->getViewport()->getActualWidth();
	float ty = (float)_camera->GetPosition().y / cam->getViewport()->getActualHeight();

	Ray ray = cam->getCameraToViewportRay(tx, ty);
	return ray;
}

Vector3 AbstractCameraController::GetDeltaPointOnCameraRay(float deltaDistance)
{
	Ray ray = GetCameraRay();
	Vector3 o = ray.getOrigin();
	Vector3 target = ray.getPoint(deltaDistance);
	Vector3 d = target - o;
	return d;
}

void AbstractCameraController::Reset()
{
	Camera* cam = GetOgreCamera();
	cam->setNearClipDistance(5);
	cam->setFarClipDistance(1500000.0f);
	_camera->SetPosition(Vector3(0.0f, 0.0f, 2.0f));
	_camera->SetLookat(Vector3(0, 0, 0));
	cam->getViewMatrix();
	cam->getProjectionMatrix();
}

void AbstractCameraController::MoveLeft(float deltaTime)
{
	Camera* cam = GetOgreCamera();
	Vector3 right = cam->getRight();
	right.normalise();
	Vector3 d = right * _cameraSpeed * deltaTime;	
	_camera->Move(-d);
	cam->getViewMatrix();
	cam->getProjectionMatrix();
}

void AbstractCameraController::MoveRight(float deltaTime)
{
	Camera* cam = GetOgreCamera();
	Vector3 right = cam->getRight();
	right.normalise();
	Vector3 d = right * _cameraSpeed * deltaTime;	
	_camera->Move(d);
	cam->getViewMatrix();
	cam->getProjectionMatrix();
}

void AbstractCameraController::MoveForward(float deltaTime)
{
	Camera* cam = GetOgreCamera();
	Vector3 camDir = cam->getRealDirection();
	Vector3 camDirNormalized = camDir.normalisedCopy();

	Vector3 d = camDirNormalized * _cameraSpeed * deltaTime;
	if(_panCamera)
	{
		d.y = 0; // 不要上下移动
	}
	_camera->Move(d);
	cam->getViewMatrix();
	cam->getProjectionMatrix();
}

void AbstractCameraController::MoveBackward(float deltaTime)
{
	Camera* cam = GetOgreCamera();
	Vector3 camDir = cam->getRealDirection();
	Vector3 camDirNormalized = camDir.normalisedCopy();

	Vector3 d = camDirNormalized * _cameraSpeed * deltaTime;	
	if(_panCamera)
	{
		d.y = 0; // 不要上下移动
	}
	_camera->Move(-d);
	cam->getViewMatrix();
	cam->getProjectionMatrix();
}

void AbstractCameraController::Rise(float deltaTime)
{
	Camera* cam = GetOgreCamera();
	_camera->Move(Vector3(0, _cameraSpeed * deltaTime, 0));
	cam->getViewMatrix();
	cam->getProjectionMatrix();
}

void AbstractCameraController::Down(float deltaTime)
{
	Camera* cam = GetOgreCamera();
	_camera->Move(Vector3(0, -_cameraSpeed * deltaTime, 0));
	cam->getViewMatrix();
	cam->getProjectionMatrix();
}

void AbstractCameraController::Zoom(float delta)
{
	Camera* cam = GetOgreCamera();
	Vector3 camDir = cam->getRealDirection();
	Vector3 camDirNormalized = camDir.normalisedCopy();

	Vector3 v = camDirNormalized * delta;
	_camera->Move(v);
	cam->getViewMatrix();
	cam->getProjectionMatrix();
}

void AbstractCameraController::LookAround(const Radian& deltaYaw, const Radian& deltaPitch)
{
	Camera* cam = GetOgreCamera();
	_camera->Yaw(deltaYaw);
	_camera->Pitch(deltaPitch);
	cam->getViewMatrix();
	cam->getProjectionMatrix();
}

const Vector3& AbstractCameraController::GetPosition() const
{
	return _camera->GetPosition();
}

void AbstractCameraController::SetPosition(const Vector3& v)
{
	_camera->SetPosition(v);
}

void AbstractCameraController::SetYaw(const Radian& r)
{
	_camera->SetYaw(r);
}

void AbstractCameraController::SetPitch(const Radian& r)
{
	_camera->SetPitch(r);
}

void AbstractCameraController::SetRoll(const Radian& r)
{
	_camera->SetRoll(r);
}

const Radian& AbstractCameraController::GetYaw() const
{
	return _camera->GetYaw();
}

const Radian& AbstractCameraController::GetPitch() const
{
	return _camera->GetPitch();
}

const Radian& AbstractCameraController::GetRoll() const
{
	return _camera->GetRoll();
}

void AbstractCameraController::SetYaw(const Degree& d)
{
	_camera->SetYaw(d);
}

void AbstractCameraController::SetPitch(const Degree& d)
{
	_camera->SetPitch(d);
}

void AbstractCameraController::SetRoll(const Degree& d)
{
	_camera->SetRoll(d);
}

void AbstractCameraController::SetOrientation(const Quaternion& orient)
{
	_camera->SetOrientation(orient);
}

const Quaternion& AbstractCameraController::GetOrientation()
{
	return _camera->GetOrientation();
}

Vector3 AbstractCameraController::GetDirection()
{	
	return GetOgreCamera()->getDirection();
}

Camera* AbstractCameraController::GetOgreCamera() const
{
	return _camera->GetCamera();
}