// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-12-03
// *************************************************************************************

#pragma once
#include "VisualObject.h"

class _EngineExport GameCamera : public VisualObject
{
public:

	GameCamera(void);
	virtual ~GameCamera(void);

	Camera* GetCamera() { return _camera; }

	virtual const Vector3& GetLookat() const;
	virtual void SetLookat(const Vector3& lookAt);

	void SetFovDegree(float val);
	float GetFovDegree() const;

	void SetFocalLength(float focalLength);
	float GetFocalLength() const;

	virtual const Vector3& GetPosition() const;
	virtual void SetPosition(const Vector3& pos);

	virtual const Vector3& GetScale() const;
	virtual void SetScale(const Vector3& scale);

	virtual const Quaternion& GetOrientation() const;
	virtual void SetOrientation(const Quaternion& orient);

	virtual bool GetAutoAspectRatio() const;
	virtual void SetAutoAspectRatio(bool val);

	virtual const String& GetTypeName() const;

	virtual void SetNearClipDistance(Real nearDist);
	virtual Real GetNearClipDistance(void) const;

	virtual void SetFarClipDistance(Real nearDist);
	virtual Real GetFarClipDistance(void) const;


	/// ���ǰ���Ϊ��Ⱦ����������������
	void Active();

	/// �ָ�Ϊ�ϴε����
	void Restore();

	/// ����ΪĿ���������Ҫ�������ô�����в������Ա༭������ʹ�á�
	void SetCamera(Camera* cam);

	virtual void Move(const Vector3& deltaPos);
	virtual void Yaw(const Radian& angle);
	virtual void Pitch(const Radian& angle);
	virtual void Roll(const Radian& angle);


protected:

	static Camera*	_mainCamera;
	Camera*	_camera;
	Vector3	_lookAt;
	Entity*	_cameraEditorModel;
	bool			_autoAspectRatio;

	virtual void BuildPropertyMap();
	virtual void PostCreateNode();
	virtual void PreDestroyNode();


	void CreateCamera();
	void DestroyCamera();

	virtual void OnEditorSelect();
	virtual void OnEditorDeselect();

	void CreateCameraEditModel();
	void DestroyCameraEditModel();
	void UpdateCameraEditModel();
};

