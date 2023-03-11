
// EditorView.cpp : EditorView 类的实现
//

#include "stdafx.h"
#include "Editor.h"

#include "EditorDoc.h"
#include "EditorView.h"
#include "EditorGameApp.h"


#define M_EVENT_CHECK(fun) if(_leftTopRenderWnd) \
	{ \
		if(EditorGame::getSingleton().GetActiveCamera() == _leftTopRenderWnd->GetCamera()) \
		{ \
		_leftTopRenderWnd->fun; \
		} \
	}

#define M_EVENT_CHECK2(fun) if(_leftTopRenderWnd) \
	{ \
		if(EditorGame::getSingleton().GetActiveCamera() == _leftTopRenderWnd->GetCamera()) \
		{ \
			_leftTopRenderWnd->fun; \
		} \
	} \
	if(_rightTopRenderWnd) \
	{ \
	if(EditorGame::getSingleton().GetActiveCamera() == _rightTopRenderWnd->GetCamera()) \
		{ \
		_rightTopRenderWnd->fun; \
		} \
	} \
	if(_leftBottomRenderWnd) \
	{ \
	if(EditorGame::getSingleton().GetActiveCamera() == _leftBottomRenderWnd->GetCamera()) \
		{ \
		_leftBottomRenderWnd->fun; \
		} \
	} \
	if(_rightBottomRenderWnd) \
	{ \
	if(EditorGame::getSingleton().GetActiveCamera() == _rightBottomRenderWnd->GetCamera()) \
		{ \
		_rightBottomRenderWnd->fun; \
		} \
	} \


// EditorView
template<> EditorView* Singleton<EditorView>::ms_Singleton = 0;

IMPLEMENT_DYNCREATE(EditorView, CView)

BEGIN_MESSAGE_MAP(EditorView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &EditorView::OnFilePrintPreview)
	ON_WM_SIZE()
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
	ON_COMMAND(ID_FILE_SAVE, &EditorView::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &EditorView::OnFileSaveAs)
END_MESSAGE_MAP()

// EditorView 构造/析构

EditorView::EditorView()
{
	// TODO: 在此处添加构造代码
	_leftTopRenderWnd = 0;
// 	_rightTopRenderWnd = 0;
// 	_leftBottomRenderWnd = 0;
// 	_rightBottomRenderWnd = 0;
	_mainRenderWnd = 0;
	_splitStyle = EVS_Max;
}

EditorView::~EditorView()
{
}

BOOL EditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// EditorView 绘制

void EditorView::OnDraw(CDC* pDC)
{
	EditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if(_splitStyle == EVS_Max)
	{
		if(_mainRenderWnd)
		{
			//_mainRenderWnd->resize(cx, cy);
			//_mainRenderWnd->update();
		}
	}
	
	// TODO: 在此处为本机数据添加绘制代码
}


// EditorView 打印


void EditorView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL EditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void EditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void EditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void EditorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// EditorView 诊断

#ifdef _DEBUG
void EditorView::AssertValid() const
{
	CView::AssertValid();
}

void EditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

EditorDoc* EditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(EditorDoc)));
	return (EditorDoc*)m_pDocument;
}
#endif //_DEBUG


// EditorView 消息处理程序

void EditorView::CreateViewports(EViewportStyle style)
{
	/*if(!_mainRenderWnd && (EVS_Max == _splitStyle))
	{
	}
	else 
	*/	
// 	if(_splitStyle == style)
// 	{
// 		return;
// 	}
	
	_splitStyle = style;

	DestroySplitersViewports();

	switch (_splitStyle)
	{
	case EVS_2x2:
		{
			Create2x2Viewports();
			break;
		}

	case EVS_1x2:
		{
			Create1x2Viewports();
			break;
		}

	case EVS_Max:
		{
			Create1Viewports();
			break;
		}
	}

	CRect rc;
	GetClientRect(&rc);
	OnSize(SIZE_MAXSHOW, rc.Width(), rc.Height());
	Invalidate();
}

void EditorView::Create1Viewports()
{
	_mainRenderWnd->setAutoUpdated(true);
	Viewport* vp = _mainRenderWnd->getViewport(0);
	EditorApp::getSingleton().SetActiveCamera(vp->getCamera());
	EditorApp::getSingleton().SetActiveViewport(vp);
	this->SetFocus();
	//_sp
}

void EditorView::Create2x2Viewports()
{
	_mainRenderWnd->setAutoUpdated(false);
// 	_leftTopRenderWnd = OGRE_NEW RenderWnd("LeftTop", this);
// 	_rightTopRenderWnd = OGRE_NEW RenderWnd("RightTop", this);
// 	_leftBottomRenderWnd = OGRE_NEW RenderWnd("LeftBottom", this);
// 	_rightBottomRenderWnd = OGRE_NEW RenderWnd("RightBottom", this);
// 	_rightBottomRenderWnd->SetFocus();
// 	EditorGame::getSingleton().SetActiveCamera(_rightBottomRenderWnd->GetCamera());
// 	EditorGame::getSingleton().SetActiveViewport(_rightBottomRenderWnd->GetViewport());

}

void EditorView::Create1x2Viewports()
{
	_mainRenderWnd->setAutoUpdated(false);
// 	_leftTopRenderWnd = OGRE_NEW RenderWnd("LeftTop", this);
// 	_rightTopRenderWnd = OGRE_NEW RenderWnd("RightTop", this);
// 	_rightBottomRenderWnd = OGRE_NEW RenderWnd("RightBottom", this);

	_leftTopRenderWnd->SetFocus();
	EditorApp::getSingleton().SetActiveCamera(_leftTopRenderWnd->GetCamera());
	EditorApp::getSingleton().SetActiveViewport(_leftTopRenderWnd->GetViewport());
}

void EditorView::DestroySplitersViewports()
{
	//_spliterV.DestroyWindow();
	//_spliterH.DestroyWindow();

	if(_leftTopRenderWnd)
	{
		_leftTopRenderWnd->DestroyWindow();
		_leftTopRenderWnd = 0;
	}

// 	if(_rightTopRenderWnd)
// 	{
// 		_rightTopRenderWnd->DestroyWindow();
// 		_rightTopRenderWnd = 0;
// 	}
// 
// 	if(_leftBottomRenderWnd)
// 	{
// 		_leftBottomRenderWnd->DestroyWindow();
// 		_leftBottomRenderWnd = 0;
// 	}
// 
// 	if(_rightBottomRenderWnd)
// 	{
// 		_rightBottomRenderWnd->DestroyWindow();
// 		_rightBottomRenderWnd = 0;
// 	}
}

void EditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}

void EditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if((cx > 320) && (cy > 240))
	{
		int w = cx / 2;
		int h = cy / 2;

		if(_splitStyle == EVS_Max)
		{
			if(_mainRenderWnd)
			{
				//_mainRenderWnd->resize(cx, cy);
				_mainRenderWnd->windowMovedOrResized();
				EditorApp::getSingleton().NotifywindowResized(_mainRenderWnd);
			}
		}
		else if(_splitStyle == EVS_1x2)
		{
			if(_leftTopRenderWnd)
			{
				_leftTopRenderWnd->MoveWindow(0, 0, w, cy);
			}

// 			if(_rightTopRenderWnd)
// 			{
// 				_rightTopRenderWnd->MoveWindow(w + 2, 0, w, h);
// 			}
// 
// 			if(_rightBottomRenderWnd)
// 			{
// 				_rightBottomRenderWnd->MoveWindow(w + 2, h + 2, w, h);
// 			}
		}
		else if(_splitStyle == EVS_2x2)
		{
			if(_leftTopRenderWnd)
			{
				_leftTopRenderWnd->MoveWindow(0, 0, w, h);
			}

// 			if(_rightTopRenderWnd)
// 			{
// 				_rightTopRenderWnd->MoveWindow(w + 2, 0, w, h);
// 			}
// 
// 			if(_leftBottomRenderWnd)
// 			{
// 				_leftBottomRenderWnd->MoveWindow(0, h + 2, w, h);
// 			}
// 
// 			if(_rightBottomRenderWnd)
// 			{
// 				_rightBottomRenderWnd->MoveWindow(w + 2, h + 2, w, h);
// 			}
		}		
	}
}

BOOL EditorView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//return TRUE;
	//BOOL r = CView::OnEraseBkgnd(pDC);

	CRect rc;
	GetClientRect(&rc);
	int w = rc.Width() / 2;
	int h = rc.Height() / 2;

	if(_splitStyle != EVS_Max)
	{
		// |
		//if(_splitStyle == EVS_1x2)
		{
			pDC->MoveTo(w, 0);
			pDC->LineTo(w, rc.Height());

			pDC->MoveTo(w + 1, 0);
			pDC->LineTo(w + 1, rc.Height());
		}

		// ----
		//if(_splitStyle == EVS_2x2)
		{
			pDC->MoveTo(0, h);
			pDC->LineTo(rc.Width(), h);

			pDC->MoveTo(0, h + 1);
			pDC->LineTo(rc.Width(), h + 1);
		}
	}

	return TRUE;
}


void EditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	MFCInputManager::getSingleton().OnLButtonUp(nFlags, point);

	M_EVENT_CHECK(OnLButtonUp(nFlags, point));

	CView::OnLButtonUp(nFlags, point);
}

void EditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::SetCapture(GetSafeHwnd());
	SetFocus();
	MFCInputManager::getSingleton().OnLButtonDown(nFlags, point);
	M_EVENT_CHECK(OnLButtonDown(nFlags, point));
	CView::OnLButtonDown(nFlags, point);
}

void EditorView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::SetCapture(GetSafeHwnd());
	SetFocus();
	MFCInputManager::getSingleton().OnMButtonDown(nFlags, point);
	M_EVENT_CHECK(OnMButtonDown(nFlags, point));
	CView::OnMButtonDown(nFlags, point);
}

void EditorView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	MFCInputManager::getSingleton().OnMButtonUp(nFlags, point);
	M_EVENT_CHECK(OnMButtonUp(nFlags, point));
	CView::OnMButtonUp(nFlags, point);
}

void EditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	MFCInputManager::getSingleton().OnMouseMove(nFlags, point);
	M_EVENT_CHECK(OnMouseMove(nFlags, point));
	CView::OnMouseMove(nFlags, point);
}

BOOL EditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	MFCInputManager::getSingleton().OnMouseWheel(nFlags, zDelta, pt);
	M_EVENT_CHECK(OnMouseWheel(nFlags, zDelta, pt));
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void EditorView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::SetCapture(GetSafeHwnd());
	SetFocus();
	MFCInputManager::getSingleton().OnRButtonDown(nFlags, point);
	M_EVENT_CHECK(OnRButtonDown(nFlags, point));
	CView::OnRButtonDown(nFlags, point);
}

void EditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();
	ClientToScreen(&point);
	//OnContextMenu(this, point);

	// TODO: Add your message handler code here and/or call default
	MFCInputManager::getSingleton().OnRButtonUp(nFlags, point);
	M_EVENT_CHECK(OnRButtonUp(nFlags, point));
	CView::OnRButtonUp(nFlags, point);
}

void EditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	MFCInputManager::getSingleton().OnKeyDown(nChar, nRepCnt, nFlags);
	M_EVENT_CHECK(OnKeyDown(nChar, nRepCnt, nFlags));
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void EditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	MFCInputManager::getSingleton().OnKeyUp(nChar, nRepCnt, nFlags);
	M_EVENT_CHECK(OnKeyUp(nChar, nRepCnt, nFlags));
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void EditorView::SetCursor(HCURSOR cur)
{
	SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)cur);

	//::SendMessage(GetSafeHwnd(), STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)cur);

// 	if(_leftBottomRenderWnd)
// 	{
// 		SetClassLong(_leftBottomRenderWnd->GetSafeHwnd(), GCL_HCURSOR, (LONG)cur);
// 		//::SendMessage(_leftBottomRenderWnd->GetSafeHwnd(), STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)cur);
// 	}

	if(_leftTopRenderWnd)
	{
		SetClassLong(_leftTopRenderWnd->GetSafeHwnd(), GCL_HCURSOR, (LONG)cur);
		//::SendMessage(_leftTopRenderWnd->GetSafeHwnd(), STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)cur);
	}

// 	if(_rightBottomRenderWnd)
// 	{
// 		SetClassLong(_rightBottomRenderWnd->GetSafeHwnd(), GCL_HCURSOR, (LONG)cur);
// 		//::SendMessage(_rightBottomRenderWnd->GetSafeHwnd(), STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)cur);
// 	}
// 
// 	if(_rightTopRenderWnd)
// 	{
// 		SetClassLong(_rightTopRenderWnd->GetSafeHwnd(), GCL_HCURSOR, (LONG)cur);
// 		//::SendMessage(_rightTopRenderWnd->GetSafeHwnd(), STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)cur);
// 	}

}

//--------------------------------------------------------------------------------
void EditorView::OnFileSave()
{
	CWaitCursor wait;
	EditorGame::getSingleton().SaveScene();
}
//--------------------------------------------------------------------------------
void EditorView::OnFileSaveAs()
{
	CWaitCursor wait;
	EditorGame::getSingleton().SaveScene();
}