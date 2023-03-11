// GridControl.cpp : implementation file
//

#include "stdafx.h"
#include "GridCtrl.h"
#include "MemDC.h"
#include <math.h>

#pragma warning(disable : 4244)

// GridControl

IMPLEMENT_DYNAMIC(GridCtrl, CWnd)
//--------------------------------------------------------------------------------
GridCtrl::GridCtrl()
: _borderSize(0, 0, 0, 0)
, _bkColor(RGB(192, 192, 192))
, _bkInvalidColor(RGB(172, 172, 172))
, _gridColor(RGB(128, 128, 128))
, _currentTimeLineColor(RGB(17, 109, 174))
, _axisColor(RGB(110, 110, 110))
, _pixelsPerSecond(200)
, _pixelsPerUnitValue(40)
, _originX(0.3f)
, _originY(0.0f)
, _zoomScale(1.0f)
, _zoomSpeed(0.1f)
, _currentTime(0.0f)
, _minValidTime(0.0f)
, _maxValidTime(1.0f)
, _minValidValue(-1.0f)
, _maxValidValue(1.0f)
, _isMovingOrigin(false)
, _lastMouseDownPt(0, 0)
, _isMovingCurrentTime(false)
, _hitCurrentTimeLine(false)
, _xSnapSz(0.01f)
, _ySnapSz(0.01f)
, _enableSnap(true)
, _clampToValidRange(true)
, _listener(0)
{
	VERIFY(_textFont.CreateFont(
		16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Tahoma"));                 // lpszFacename
}
//--------------------------------------------------------------------------------
GridCtrl::~GridCtrl()
{
	_textFont.DeleteObject();
}
//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(GridCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
END_MESSAGE_MAP()
// GridControl message handlers
//--------------------------------------------------------------------------------
void GridCtrl::OnPaint()
{
	CPaintDC dcSrc(this); // device context for painting
	MemDC dc(&dcSrc);
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	CRect rc = GetDrawArea();
	CRgn rgn;
	rgn.CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
	dc.SelectClipRgn(&rgn);
	//dc.SetViewportOrg(-_startX, -_startY);

	OnDraw(dc, rc);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnMouseMove(nFlags, point);

	OnMouseMove(point);

// 	if(_isDraging)
// 	{
// 		// 限定在绘制区拖动
// 		CRect rc = GetDrawArea();
// 		if((point.x >= rc.left) && (point.x <= rc.right))
// 		{
// 			DragTimeAndValue(point, _hitBtnIdx);
// 		}
// 	}
// 	else
// 	{
// 		_hitButton = HitTest(point, _hitBtnIdx);
// 	}
// 
// 	Invalidate();
	//UpdateWindow();
}
//--------------------------------------------------------------------------------
void GridCtrl::OnMouseMove( const CPoint& point )
{

	if(_isMovingOrigin)
	{
		float px = _pixelsPerSecond * _zoomScale;
		float py = _pixelsPerUnitValue * _zoomScale;
		CPoint d = point - _lastMouseDownPt;
		_lastMouseDownPt = point;
		_originX  += (float)d.x / px;
		_originY  -= (float)d.y / py;

		OnOriginChange(_originX, _originY);

		Invalidate(FALSE);
	}
	else if(_isMovingCurrentTime)
	{
		float x = GetMouseTime(point);
		
		_currentTime = x;

		OnCurrentTimeChange(x);
		Invalidate(FALSE);
	}
	else
	{
		_hitCurrentTimeLine = HitCurrentTimeLine(point);
		Invalidate(FALSE);
	}
}
//--------------------------------------------------------------------------------
void GridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

// 	_hitButton = HitTest(point, _hitBtnIdx);
// 	if(_hitButton)
// 	{
// 		// 不拖第一个和最后一个
// // 		if((_hitBtnIdx > 0) && (_hitBtnIdx < (int)_clrs.size() - 1))
// // 		{
// 		_isDraging = true;
// 		SetCapture();
// // 		}
// 	}
	SetCapture();
	OnLButtonDown(point);
	CWnd::OnLButtonDown(nFlags, point);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnLButtonDown( const CPoint& point )
{
	_isMovingCurrentTime = false;

	if(HitCurrentTimeLine(point))
	{
		_isMovingCurrentTime = true;
		_lastMouseDownPt = point;
		_hitCurrentTimeLine = true;
	}
}
//--------------------------------------------------------------------------------
void GridCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
// 	if(_isDraging)
// 	{
// 		_isDraging = false;
// 		ReleaseCapture();
// 	}

	ReleaseCapture();
	OnLButtonUp(point);
	CWnd::OnLButtonUp(nFlags, point);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnLButtonUp( const CPoint& point )
{
	_isMovingCurrentTime = false;
}
//--------------------------------------------------------------------------------
BOOL GridCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}
//--------------------------------------------------------------------------------
CRect GridCtrl::GetDrawArea() const
{
	CRect rc;
	GetClientRect(&rc);
	rc.DeflateRect(&_borderSize);

	return rc;
}
//--------------------------------------------------------------------------------
float GridCtrl::GetMouseTime( const CPoint& point )
{
	float px = _pixelsPerSecond * _zoomScale;

	float x = point.x / px - _originX;
	if(_enableSnap)
	{
		x = x - fmod(x, _xSnapSz);
	}

	if(_clampToValidRange)
	{
		x = std::max(x, _minValidTime);
		x = std::min(x, _maxValidTime);
	}

	return x;
}
//--------------------------------------------------------------------------------
float GridCtrl::GetMouseValue( const CPoint& point )
{
	float s = _pixelsPerUnitValue * _zoomScale;
	CRect rc = GetDrawArea();
	float v = (rc.Height() - point.y) / s - _originY;
	if(_enableSnap)
	{
		v = v - fmod(v, _ySnapSz);
	}

	if(_clampToValidRange)
	{
		v = std::max(v, _minValidValue);
		v = std::min(v, _maxValidValue);
	}

	return v;
}
//--------------------------------------------------------------------------------
void GridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
// 	if(HitTest(point, i))
// 	{		
// 	}

	OnLButtonDblClk(point);
	CWnd::OnLButtonDblClk(nFlags, point);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnLButtonDblClk( const CPoint& point )
{

}
//--------------------------------------------------------------------------------
BOOL GridCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	OnMouseWheel(zDelta, pt);
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnMouseWheel( short zDelta, const CPoint& pt )
{
	float s1 = _zoomScale;

	float r = zDelta > 0 ? 1.0f : -1.0f;
	_zoomScale += r * _zoomSpeed;
	_zoomScale = std::max(0.1f, _zoomScale);

	float s2 = _zoomScale;

	float px1 = _pixelsPerSecond * s1;
	float py1 = _pixelsPerUnitValue * s1;

	float px2 = _pixelsPerSecond * s2;
	float py2 = _pixelsPerUnitValue * s2;

	CRect rc = GetDrawArea();

	float o1x = _originX * px1;
	float o1y = rc.Height() - _originY * py1;

	float downX = (float)(pt.x/* - rc.left*/);
	float downY = (float)(pt.y/* - rc.Height() - rc.bottom*/);

	int o2x = (float)downX - ((float)(downX - o1x) * s1 / s2);
	int o2y = (float)downY - ((float)(downY - o1y) * s1 / s2);

	_originX = o2x / px2;
	_originY = (rc.Height() - o2y) / py2;

	OnZoomChange(_zoomScale);
	OnOriginChange(_originX, _originY);

	Invalidate(FALSE);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnDraw( CDC& dc, const CRect& rc )
{
	OnDrawBackground(dc, rc);
	OnDrawGridLine(dc, rc);
	OnPostDrawGrid(dc, rc);
	OnDrawGridCoordText(dc, rc);
	OnDrawCurrentTime(dc, rc);
	OnDrawBorder(dc, rc);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnDrawBackground( CDC &dc, const CRect& rc )
{
	// 绘制背景
	CPen pen(PS_SOLID, 1, _bkInvalidColor);
	CBrush invalidBr(_bkInvalidColor);
	CBrush* oldBrush = dc.SelectObject(&invalidBr);
	CPen* oldPen = dc.SelectObject(&pen);
	dc.Rectangle(&rc);
	
	CBrush validBr(_bkColor/*RGB(255, 0, 0)*/);
	CPen pen2(PS_SOLID, 1, _bkColor);
	dc.SelectObject(&pen2);
	dc.SelectObject(&validBr);

	float px = _pixelsPerSecond * _zoomScale;
	float py = _pixelsPerUnitValue * _zoomScale;

	int x0 = (_originX + _minValidTime) * px;
	int x1 = (_originX + _maxValidTime) * px;
	
	int y0 = (_originY + _minValidValue) * py;
	int y1 = (_originY + _maxValidValue) * py;

	CRect r(x0, rc.Height() - y0, x1, rc.Height() - y1);
	dc.Rectangle(&r);

	dc.SelectObject(oldBrush);
	dc.SelectObject(oldPen);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	_isMovingOrigin = true;
	SetCapture();
	_lastMouseDownPt = point;

	CWnd::OnMButtonDown(nFlags, point);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(_isMovingOrigin)
	{
		_isMovingOrigin = false;
		ReleaseCapture();
	}
	CWnd::OnMButtonUp(nFlags, point);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnPostDrawGrid( CDC& dc, const CRect& rc )
{

}
//--------------------------------------------------------------------------------
void GridCtrl::OnDrawGridLine( CDC &dc, const CRect& rc )
{
	//绘制网格
	int idc = dc.SaveDC();

	CPen gridPen;
	gridPen.CreatePen(PS_SOLID, 1, _gridColor);
	CPen* oldPen = dc.SelectObject(&gridPen);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0, 0, 0));

	// Do something with the font just created...
	CFont* def_font = dc.SelectObject(&_textFont);

	float px = _pixelsPerSecond * _zoomScale;
	float py = _pixelsPerUnitValue * _zoomScale;

	int ox = _originX * px;
	int oy = rc.Height() - _originY * py;

	CString txt;
	// 水平线 y down
	for (int y = oy; y <= rc.bottom; y += py)
	{
		dc.MoveTo(rc.left, y);
		dc.LineTo(rc.right, y);
	}

	// y up
	for (int y = oy - py; y >= rc.top; y -= py)
	{
		float v = ceilf((rc.Height() - y) / py - _originY);

		dc.MoveTo(rc.left, y);
		dc.LineTo(rc.right, y);
	}

	// 垂直线 x+
	for (int x = ox; x <= rc.right; x += px)
	{
		dc.MoveTo(x, rc.top);
		dc.LineTo(x, rc.bottom);
	}

	// x-
	for (int x = ox - px; x >= rc.left; x -= px)
	{
		dc.MoveTo(x, rc.top);
		dc.LineTo(x, rc.bottom);
	}

	dc.SelectObject(oldPen);
	dc.SelectObject(def_font);
	dc.RestoreDC(idc);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnDrawGridCoordText( CDC &dc, const CRect& rc )
{
	int idc = dc.SaveDC();

	float px = _pixelsPerSecond * _zoomScale;
	float py = _pixelsPerUnitValue * _zoomScale;

	int ox = _originX * px;
	int oy = rc.Height() - _originY * py;

	// x/y轴
	CPen axisPen(PS_SOLID, 2, _axisColor);
	dc.SelectObject(&axisPen);
	dc.MoveTo(rc.left, oy);
	dc.LineTo(rc.right, oy);

	dc.MoveTo(ox, rc.top);
	dc.LineTo(ox, rc.bottom);


	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0, 0, 0));

	// Do something with the font just created...
	CFont* def_font = dc.SelectObject(&_textFont);

	CString txt;
	// 水平线 y down
	float v = 0.0f;
	for (int y = oy; y <= rc.bottom; y += py)
	{
		if(v == 0.0f)
		{
			txt = "0";
		}
		else
		{
			txt.Format("%.2f", v);
		}
		dc.TextOut(rc.left + 2, y, txt);
		v -= 1.0f;
	}

	// y up
	v = 1.0f;
	for (int y = oy - py; y >= rc.top; y -= py)
	{
		txt.Format("%.2f", v);
		dc.TextOut(rc.left + 2, y, txt);
		v += 1.0f;
	}

	// 垂直线 x+
	v = 0.0f;
	for (int x = ox; x <= rc.right; x += px)
	{
		if(v == 0.0f)
		{
			txt = "0";
		}
		else
		{
			txt.Format("%.2f", v);
		}

		dc.TextOut(x + 2, rc.bottom - 16, txt);

		v += 1.0f;
	}

	// x-
	v = -1.0f;
	for (int x = ox - px; x >= rc.left; x -= px)
	{
		txt.Format("%.2f", v);
		dc.TextOut(x + 2, rc.bottom - 16, txt);
		v -= 1.0f;
	}

	dc.RestoreDC(idc);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnDrawCurrentTime( CDC &dc, const CRect& rc )
{
	int idc = dc.SaveDC();

	float px = _pixelsPerSecond * _zoomScale;

	// 当前时间
	CFont* def_font = dc.SelectObject(&_textFont);

	CPen timePen(PS_SOLID, 2, _hitCurrentTimeLine ? RGB(255, 0, 0) : _currentTimeLineColor);
	CPen* oldPen = dc.SelectObject(&timePen);
	int x = (_originX + _currentTime) * px;
	dc.MoveTo(x, rc.top);
	dc.LineTo(x, rc.bottom);

	CString txt;
	txt.Format("%.2f", _currentTime);
	dc.TextOut(x + 4, rc.top + rc.Height() / 2, txt);

	dc.SelectObject(oldPen);
	dc.SelectObject(def_font);

	dc.RestoreDC(idc);
}
//--------------------------------------------------------------------------------
void GridCtrl::OnDrawBorder( CDC &dc, const CRect& rc )
{
	CPen border(PS_SOLID, 1, RGB(110, 110, 110));
	CPen* oldPen = dc.SelectObject(&border);
	dc.MoveTo(rc.TopLeft());
	dc.LineTo(rc.left, rc.bottom);
	dc.SelectObject(oldPen);
}
//--------------------------------------------------------------------------------
bool GridCtrl::HitCurrentTimeLine( const CPoint& pt )
{
	float px = _pixelsPerSecond * _zoomScale;
	int x = (_originX + _currentTime) * px;
	if(abs(x - pt.x) <= 9)
	{
		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------
void GridCtrl::OnCurrentTimeChange( float currentTime )
{
	if(_listener)
	{
		_listener->OnSetCurrentTime(this, currentTime);
	}
}
//--------------------------------------------------------------------------------
void GridCtrl::OnZoomChange( float scale )
{
	if(_listener)
	{
		_listener->OnZoomChange(this, scale);
	}
}
//--------------------------------------------------------------------------------
void GridCtrl::OnOriginChange( float xOff, float yOff )
{
	if(_listener)
	{
		_listener->OnOriginChange(this, xOff, yOff);
	}
}
//--------------------------------------------------------------------------------
void GridCtrl::Redraw()
{
	if(GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}
//--------------------------------------------------------------------------------
void GridCtrl::ConvertToPixelCoord( float x, float y, int& outX, int& outY )
{
	float px = _pixelsPerSecond * _zoomScale;
	float py = _pixelsPerUnitValue * _zoomScale;

	CRect rc = GetDrawArea();
	outX = (x + _originX) * px;
	outY = rc.Height() - (y + _originY) * py;
}
//--------------------------------------------------------------------------------
