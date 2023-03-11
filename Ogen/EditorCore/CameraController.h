#pragma once
#include "EditorCorePrerequisites.h"
#include "MFCInputManager.h"
#include "EditorConfig.h"

class _EditorCoreExport IViewportCameraController
	: public IMFCMouseListener
	, public IMFCKeyListener
	, public OgeAlloc
{
public:

	virtual void SetCamera(Camera* cam) = 0;
	virtual Camera* GetCamera() const = 0;
};

class _EditorCoreExport AbstractEditorCameraController
	: public IViewportCameraController
{
public:
	
	AbstractEditorCameraController();
	virtual ~AbstractEditorCameraController();

	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnMButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnMButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnRButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	bool IsLMBDown();
	bool IsRMBDown();
	bool IsMMBDown();

	virtual void SetCamera(Camera* cam) { _camera = cam; }
	virtual Camera* GetCamera() const { return _camera; }

protected:

	Camera* _camera;

	bool		_isLMBDown;
	bool		_isRMBDown;
	bool		_isMMBDown;
	CPoint		_lastMouseDownPos;
	ECameraSpeed _lastCameraSpeed;
};

//------------------------------------------------------------------------

class _EditorCoreExport FPSCamera : public AbstractEditorCameraController
{
public:

	FPSCamera();
	virtual ~FPSCamera();

	virtual BOOL OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	void ClampCameraToTerrain( Camera* cam );
};

//------------------------------------------------------------------------

class _EditorCoreExport LookAtCamera : public AbstractEditorCameraController
{
public:

	LookAtCamera();
	virtual ~LookAtCamera();

	virtual BOOL OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	void SetLookAt(const Vector3& lookAt);
	void SetCameraRadius(float r);

	float GetMinDistance() const { return _minDistance; }
	void SetMinDistance(float val) { _minDistance = val; }

	float GetZoomDelta() const { return _zoomDelta; }
	void SetZoomDelta(float val) { _zoomDelta = val; }

	float GetMaxDistance() const { return _maxDistance; }
	void SetMaxDistance(float val) { _maxDistance = val; }

	float GetYRot() const { return _yRot; }
	void SetYRot(float val) { _yRot = val; }

	float GetXRot() const { return _xRot; }
	void SetXRot(float val) { _xRot = val; }

protected:

	float _radius;
	Vector3 _lookAt;
	float _xRot;
	float _yRot;
	float _minDistance;
	float _maxDistance;
	float _zoomDelta;
	void UpdateCamera();
};