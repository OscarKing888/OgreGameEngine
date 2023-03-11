#pragma once

class MFCCameraController
	: public IMFCKeyListener
	, public IUpdateAble
	, public Singleton<MFCCameraController>
	, public IEventLisitener
{
public:

	MFCCameraController();
	virtual ~MFCCameraController();

	void SetCamera(Camera* cam);
	Camera* GetCamera() const;

	float CameraSpeed() const { return _cameraSpeed; }
	void CameraSpeed(float val) { _cameraSpeed = val; }

protected:

	Camera* _cam;
	bool _moveLeft;
	bool _moveRight;
	bool _moveForward;
	bool _moveBackword;
	bool _rise;
	bool _enableRise;
	float _cameraSpeed;

	virtual void Update(float deltaTime);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast);
};