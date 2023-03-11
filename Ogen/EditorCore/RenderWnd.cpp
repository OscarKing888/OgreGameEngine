#include "stdafx.h"
#include "RenderWnd.h"
#include "EditorApp.h"

IMPLEMENT_DYNCREATE(RenderWnd, CView)

RenderWnd::RenderWnd()
	: _renderWnd(0)
	, _wndName()
	, _camera(0)
	, _viewport(0)
	, _cameraController(0)
	, _mouseListener(0)
	, _lastActiveViewPort(0)
	, _lastActiveCam(0)
{
	static int i = 0;
	_wndName = formatString("AutoRenderWnd[%d]", i);
	++i;
	//Create(IDD, pParent);
	//ShowWindow(SW_SHOW);
}

RenderWnd::~RenderWnd()
{
	DestroyCameraController();
}

void RenderWnd::DoDataExchange(CDataExchange* pDX)
{
	CView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RenderWnd, CView)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//ON_MESSAGE_VOID(WM_INITIALUPDATE, RenderWnd::OnInitialUpdate)
END_MESSAGE_MAP()


// RenderWnd message handlers

void RenderWnd::OnInitialUpdate()
{
	//CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	NameValuePairList para;
	String k = "externalWindowHandle";
	String v = StringConverter::toString((uint)GetSafeHwnd());
	para.insert(std::make_pair(k, v));

	k = "vsync";
	v = "true";
	para.insert(std::make_pair(k, v));

	_renderWnd = Root::getSingleton().createRenderWindow(_wndName, 800, 600, false, &para);
	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	//Root::getSingleton().getSceneManager("LandSceneManager"); // 使用自动命名查找
	String camName = String("__auto__camera__") + _wndName;
	try
	{
		_camera = sm->getCamera(camName);
	}
	catch (const Exception&) 
	{
		_camera = sm->createCamera(camName);
	}
	_viewport = _renderWnd->addViewport(_camera);
	_viewport->setBackgroundColour(EditorConfig::getSingleton().GetViewportColour());
	_camera->setPosition(0, 100, 500);
	_camera->lookAt(0, 0, 0);
	_camera->setAutoAspectRatio(true);
	_camera->setNearClipDistance(5.0f);

	ModifyStyle(0, WS_CHILD | WS_VISIBLE);
	
	SetCameraController(OGRE_NEW FPSCamera());

	_lastActiveViewPort = EditorApp::getSingleton().GetActiveViewport();
	_lastActiveCam = EditorApp::getSingleton().GetActiveCamera();

	_renderWnd->windowMovedOrResized();
	EditorApp::getSingleton().NotifywindowResized(_renderWnd);
	//return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL RenderWnd::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	//Root::getSingleton().getSceneManager("LandSceneManager"); // 使用自动命名查找
	sm->destroyCamera(_camera);
	_camera = 0;

	EditorApp::getSingleton().SetActiveCamera(_lastActiveCam);
	EditorApp::getSingleton().SetActiveViewport(_lastActiveViewPort);

	_renderWnd->removeAllViewports();

	Root::getSingleton().getRenderSystem()->destroyRenderWindow(_wndName);
	return CView::DestroyWindow();
}

void RenderWnd::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if((cx > 320) && (cy > 240))
	{
		if(_renderWnd)
		{
			//_renderWnd->resize(cx, cy);
			_renderWnd->windowMovedOrResized();
			
			EditorApp::getSingleton().NotifywindowResized(_renderWnd);

			// TERRAIN:
#if OCT_TERRAIN
			if(Terrain::getSingletonPtr())
			{
				Terrain::getSingletonPtr()->UpdateTerrainData();
			}
#endif
		}
	}
}

void RenderWnd::Refresh()
{
	if(_renderWnd)
	{
		_renderWnd->update();
	}
}

void RenderWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	//Refresh();
}

BOOL RenderWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void RenderWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	// MFCInputManager::getSingleton().OnLButtonUp(nFlags, point);
	CView::OnLButtonUp(nFlags, point);
	if(_mouseListener)
	{
		if(_mouseListener->OnLButtonUp(nFlags, point))
		{
			return;
		}
	}

	if(_cameraController)
	{
		_cameraController->OnLButtonUp(nFlags, point);
	}
}

void RenderWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnLButtonDown(nFlags, point);

	SetFocus();
	::SetCapture(GetSafeHwnd());

	EditorApp::getSingleton().SetActiveCamera(_camera);
	EditorApp::getSingleton().SetActiveViewport(_viewport);
	// MFCInputManager::getSingleton().OnLButtonDown(nFlags, point);

	if(_mouseListener)
	{
		if(_mouseListener->OnLButtonDown(nFlags, point))
		{
			return;
		}
	}


	if(_cameraController)
	{
		_cameraController->OnLButtonDown(nFlags, point);
	}


// 	CWnd* parent = GetParent();
// 	if(parent)
// 	{
// 		parent->OnLButtonDown(nFlags, point);
// 	}
}

void RenderWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
	::SetCapture(GetSafeHwnd());
	CView::OnMButtonDown(nFlags, point);
	// MFCInputManager::getSingleton().OnMButtonDown(nFlags, point);

	if(_mouseListener)
	{
		if(_mouseListener->OnMButtonDown(nFlags, point))
		{
			return;
		}
	}

	if(_cameraController)
	{
		_cameraController->OnMButtonDown(nFlags, point);
	}

}

void RenderWnd::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	// MFCInputManager::getSingleton().OnMButtonUp(nFlags, point);
	CView::OnMButtonUp(nFlags, point);

	if(_mouseListener)
	{
		if(_mouseListener->OnMButtonUp(nFlags, point))
		{
			return;
		}
	}

	if(_cameraController)
	{
		_cameraController->OnMButtonUp(nFlags, point);
	}

}

void RenderWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	// MFCInputManager::getSingleton().OnMouseMove(nFlags, point);
	CView::OnMouseMove(nFlags, point);

	if(_mouseListener)
	{
		if(_mouseListener->OnMouseMove(nFlags, point))
		{
			return;
		}
	}

	if(_cameraController)
	{
		_cameraController->OnMouseMove(nFlags, point);
	}
}

BOOL RenderWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	// MFCInputManager::getSingleton().OnMouseWheel(nFlags, zDelta, pt);
	BOOL r = CView::OnMouseWheel(nFlags, zDelta, pt);

	if(_mouseListener)
	{
		if(_mouseListener->OnMouseWheel(nFlags, zDelta, pt))
		{
			return r;
		}
	}

	if(_cameraController)
	{
		_cameraController->OnMouseWheel(nFlags, zDelta, pt);
	}

	return r;
}

void RenderWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
	::SetCapture(GetSafeHwnd());
	CView::OnRButtonDown(nFlags, point);
	// MFCInputManager::getSingleton().OnRButtonDown(nFlags, point);

	if(_mouseListener)
	{
		if(_mouseListener->OnRButtonDown(nFlags, point))
		{
			return;
		}
	}

	if(_cameraController)
	{
		_cameraController->OnRButtonDown(nFlags, point);
	}

}

void RenderWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();
	CView::OnRButtonUp(nFlags, point);
	// TODO: Add your message handler code here and/or call default
	// MFCInputManager::getSingleton().OnRButtonUp(nFlags, point);

	if(_mouseListener)
	{
		if(_mouseListener->OnRButtonUp(nFlags, point))
		{
			return;
		}
	}

	if(_cameraController)
	{
		_cameraController->OnRButtonUp(nFlags, point);
	}

}

void RenderWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	// MFCInputManager::getSingleton().OnKeyDown(nChar, nRepCnt, nFlags);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	if(_cameraController)
	{
		_cameraController->OnKeyDown(nChar, nRepCnt, nFlags);
	}

}

void RenderWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	// MFCInputManager::getSingleton().OnKeyUp(nChar, nRepCnt, nFlags);
	if(_cameraController)
	{
		_cameraController->OnKeyUp(nChar, nRepCnt, nFlags);
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void RenderWnd::SetCameraController(IViewportCameraController* ctrl, bool autoDeleteOld)
{
	if(autoDeleteOld)
	{
		DestroyCameraController();
	}

	_cameraController = ctrl;
	ctrl->SetCamera(_camera);
}

void RenderWnd::DestroyCameraController()
{
	if(_cameraController)
	{
		OGRE_DELETE _cameraController;
		_cameraController = 0;
	}
}

void RenderWnd::OnDraw(CDC* pDC)
{

}

BOOL RenderWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.cx = 800;
	cs.cy = 600;
	return CView::PreCreateWindow(cs);
}