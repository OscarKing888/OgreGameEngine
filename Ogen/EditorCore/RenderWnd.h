#pragma once
#include "CameraController.h"

class _EditorCoreExport RenderWnd : public CView
{
	DECLARE_DYNCREATE(RenderWnd)

protected:

	RenderWnd();   // standard constructor

public:
	virtual ~RenderWnd();

	void SetMouseListener(IMFCMouseListener* l) { _mouseListener = l; }
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnDraw(CDC* pDC);
	//virtual BOOL OnInitDialog();

	virtual BOOL DestroyWindow();

	void Refresh();

	RenderWindow* GetRenderWindow() const { return _renderWnd; }
	Camera* GetCamera() const { return _camera; }
	Viewport* GetViewport() const { return _viewport; }
	const String& GetName() const { return _wndName; }

	void SetCameraController(IViewportCameraController* ctrl, bool autoDeleteOld = true);
	IViewportCameraController* GetCameraController() const { return _cameraController; }
	void DestroyCameraController();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


	RenderWindow* _renderWnd;
	Camera* _camera;
	Viewport* _viewport;
	String _wndName;
	IViewportCameraController* _cameraController;
	IMFCMouseListener* _mouseListener;

	Viewport* _lastActiveViewPort;
	Camera* _lastActiveCam;

protected:

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
