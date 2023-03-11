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
#include "ICameraController.h"


class _EngineExport AbstractCameraController
	: public ICameraController
{
public:

	AbstractCameraController(void);
	virtual ~AbstractCameraController(void);

	virtual void SetCamera(GameCamera* cam);
	virtual GameCamera* GetCamera() const;

	virtual void SetLookAt(const Vector3& v){ _lookAt = v; }
	virtual const Vector3& GetLookAt() const { return _lookAt; }
	virtual void SetCameraMoveSpeed(float speed){ _cameraSpeed = speed; }

	virtual const Vector3& GetPosition() const;
	virtual void SetPosition(const Vector3& v);

	virtual void SetToPanCamera(bool on){ _panCamera = on; }
	bool		 IsPanCamera() const { return _panCamera; }

	virtual void Reset();
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	virtual void MoveForward(float deltaTime);
	virtual void MoveBackward(float deltaTime);
	virtual void Rise(float deltaTime);
	virtual void Down(float deltaTime);
	virtual void Zoom(float delta);
	virtual void LookAround(const Radian& deltaYaw, const Radian& deltaPitch);
	Ray GetCameraRay();

	void SetOrientation(const Quaternion& orient);
	const Quaternion& GetOrientation();
	Vector3 GetDirection();

	virtual void SetYaw(const Radian& r);
	virtual void SetPitch(const Radian& r);
	virtual void SetRoll(const Radian& r);

	virtual const Radian& GetYaw() const;
	virtual const Radian& GetPitch() const;
	virtual const Radian& GetRoll() const;

	virtual void SetYaw(const Degree& d);
	virtual void SetPitch(const Degree& d);
	virtual void SetRoll(const Degree& d);

	Camera* GetOgreCamera() const;

protected:

	GameCamera*	_camera;
	Vector3	_lookAt;			// 当前相机的观察点
	float			_cameraSpeed;
	bool			_panCamera;	// 当使用移动键进行平面移动时是否在Y轴上上下移动

	Vector3 GetDeltaPointOnCameraRay(float deltaDistance);		
};

SmartPointer(AbstractCameraController);

