
// EditorView.h : EditorView 类的接口
//


#pragma once

#include "EditorCore/RenderWnd.h"


enum EViewportStyle
{
	EVS_Max = 0,	// 最大化一个窗口
	EVS_1x2 = 1,	// 1x2方式
	EVS_2x2 = 2		// 2x2方式窗口
};

class EditorDoc;

class EditorView : public CView
	, public Singleton<EditorView>
{
protected: // 仅从序列化创建
	EditorView();
	DECLARE_DYNCREATE(EditorView)

// 属性
public:

	EditorDoc* GetDocument() const;
	virtual ~EditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void CreateViewports(EViewportStyle style);
	void SetMainRenderWnd(RenderWindow* rw) { _mainRenderWnd = rw; }

	EViewportStyle GetViewportSplitStyle() { return _splitStyle; }

	void SetCursor(HCURSOR cur);

	// 生成的消息映射函数
protected:

	EViewportStyle	_splitStyle;
 	CSplitterWndEx	_spliterV;
// 	CSplitterWndEx	_spliterH;
	RenderWnd*		_leftTopRenderWnd;
// 	RenderWnd*		_rightTopRenderWnd;
// 	RenderWnd*		_leftBottomRenderWnd;
// 	RenderWnd*		_rightBottomRenderWnd;

	RenderWindow*		_mainRenderWnd;

	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	void Create1Viewports();
	void Create2x2Viewports();
	void Create1x2Viewports();
	void DestroySplitersViewports();
	virtual void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
};

#ifndef _DEBUG  // EditorView.cpp 中的调试版本
inline EditorDoc* EditorView::GetDocument() const
   { return reinterpret_cast<EditorDoc*>(m_pDocument); }
#endif

