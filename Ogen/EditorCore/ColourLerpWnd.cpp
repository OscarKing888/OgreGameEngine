// ColourLerpWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ColourLerpWnd.h"
#include "MemDC.h"

#define M_TIME_BUTTON_WIDTH 7
#define M_TIME_BUTTON_HEIGHT 15
#define M_TIME_TXT_HEIGHT 20

#define M_ALPHA_BLOCK_SZ 8

#define M_ALPHA_AREA_HEIGHT (M_TIME_BUTTON_HEIGHT + M_TIME_TXT_HEIGHT)

// ColourLerpWnd message handlers

void SetAValue(COLORREF& src, byte a)
{
	DWORD c = 0x00ffffff & src;
	src = c | (((DWORD)(BYTE)(a))<<24);
}

void SetAValue(COLORREF& src, float a)
{
	SetAValue(src, (BYTE)(a * 255));
}


COLORREF RGBLerp(COLORREF src, COLORREF dest, float s)
{
	float r1 = GetRValue(src);
	float g1 = GetGValue(src);
	float b1 = GetBValue(src);
	//float a1 = GetAValue(src);

	float r2 = GetRValue(dest);
	float g2 = GetGValue(dest);
	float b2 = GetBValue(dest);
	//float a2 = GetAValue(dest);

	float r = r1 * s + r2 * (1.0f - s);
	float g = g1 * s + g2 * (1.0f - s);
	float b = b1 * s + b2 * (1.0f - s);
	//float a = a1 * s + a2 * (1.0f - s);

	return RGB(r, g, b);
}
// ColourLerpWnd

IMPLEMENT_DYNAMIC(ColourLerpWnd, CWnd)

ColourLerpWnd::ColourLerpWnd()
: _isDraging(false)
, _isDragingAlpha(false)
, _drawTimeLine(true)
, _hitButton(false)
, _selButton(false)
, _hitBtnIdx(0)
, _selBtnIdx(0)
, _borderSize(25, 10, 25, 10)
{
	_clrs.push_back(std::make_pair(0.0f, ARGB32(255, 255, 0, 0)));
	_clrs.push_back(std::make_pair(0.2f, ARGB32(128, 255, 200, 0)));
	_clrs.push_back(std::make_pair(0.4f, ARGB32(255, 255,255, 0)));
	_clrs.push_back(std::make_pair(0.6f, ARGB32(255, 0, 255, 0)));
	_clrs.push_back(std::make_pair(0.8f, ARGB32(128, 0, 255, 255)));
	_clrs.push_back(std::make_pair(0.9f, ARGB32(80, 0, 0, 255)));
	_clrs.push_back(std::make_pair(1.0f, ARGB32(255, 255, 0, 255)));

	VERIFY(_defaultFont.CreateFont(
		12,                        // nHeight
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

ColourLerpWnd::~ColourLerpWnd()
{
	_defaultFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(ColourLerpWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void ColourLerpWnd::OnPaint()
{
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	CPaintDC dcSrc(this); // device context for painting
	MemDC memDC(&dcSrc);

	CRect clientRc;
	GetClientRect(&clientRc);

	COLORREF maskClr = RGB(255, 0, 255);

	CDC dc;
	dc.CreateCompatibleDC(&dcSrc);
	dc.SelectObject(&_bgBmp);
	CBrush brBlackBg(maskClr);
	CBrush* oldBgBr = dc.SelectObject(&brBlackBg);
	dc.Rectangle(&clientRc);
	dc.SelectObject(oldBgBr);
	
	CDC bmpDC;
	bmpDC.CreateCompatibleDC(&dcSrc);
	bmpDC.SelectObject(&_clrBmp);
	CBrush brWhiteBg(RGB(255, 255, 255));
	oldBgBr = bmpDC.SelectObject(&brWhiteBg);
	bmpDC.Rectangle(&clientRc);
	bmpDC.SelectObject(oldBgBr);

	BITMAP bmp;
	_clrBmp.GetBitmap(&bmp);
	int sz = bmp.bmWidth * bmp.bmHeight * bmp.bmBitsPixel / 8;
	COLORREF* dat = (COLORREF*)OGE_ALLOC_T(BYTE, sz);

	CRect rc = GetDrawArea();
	CRect bk = GetDrawColorArea();
	DrawAlphaCanvas(bmpDC, bk);

	CRect arcClr = GetDrawAlphaColorArea();
	DrawAlphaCanvas(bmpDC, arcClr);

	_clrBmp.GetBitmapBits(sz, dat);

	//CBrush grayBr(RGB(127, 127, 127));
	//CBrush* oldBrush = dc.SelectObject(&grayBr);
	//dc.Rectangle(&rc);
	//dc.SelectObject(oldBrush);

	dc.SetBkMode(TRANSPARENT);

	if(!_clrs.empty())
	{
		float alpha = 1.0f;
		for (int x = rc.left; x <= rc.right; ++x)
		{
			float t = (float)(x - rc.left) / (float)rc.Width();
			COLORREF clr = 0;
			bool drawTimeLine = false;
			if(x == rc.left)
			{
				clr = _clrs.front().second;
				drawTimeLine = true;
				alpha = (float)GetRValue(_clrs.front().second) / 255.0f;
			}
			else if(x == rc.right)
			{
				clr = _clrs.back().second;
				drawTimeLine = true;
				alpha = (float)GetRValue(_clrs.back().second) / 255.0f;
			}
			else
			{
				for (int i = 0; i < (int)_clrs.size() - 1; ++i)
				{
					float tprev = _clrs[i].first;
					float tnow = _clrs[i + 1].first;
					if((t >= tprev) && (t < tnow))
					{
						float rt = (t - tprev) / (tnow - tprev);
						float a1 = (float)GetAValue(_clrs[i + 1].second) / 255.0f;
						float a2 = (float)GetAValue(_clrs[i].second) / 255.0f;

						alpha = a1 * rt + a2 * (1.0 - rt);

						clr = RGBLerp(_clrs[i + 1].second, _clrs[i].second, rt);

						if(((int)(tprev * rc.Width()) == (x - rc.left))
							|| ((int)(tnow * rc.Width()) == (x - rc.left)))
						{
							drawTimeLine = true;
						}

						break;
					}
				}
			} // if

			// draw color lines
#if 0
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, clr);
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(x, rc.top);
			dc.LineTo(x, rc.bottom - M_TIME_BUTTON_HEIGHT - M_TIME_TXT_HEIGHT);
			dc.SelectObject(oldPen);
#else

			DrawVLineWithAlpha(dat, clr, alpha, x, bk.top, bk.bottom, bmp.bmWidth);
#endif
			if(drawTimeLine && _drawTimeLine)
			{
				// draw time line
				CPen pen;
				pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				CPen* oldPen = dc.SelectObject(&pen);
				dc.MoveTo(x, (rc.bottom - (M_TIME_BUTTON_HEIGHT * 2 + M_TIME_TXT_HEIGHT)));
				dc.LineTo(x, rc.bottom - M_TIME_BUTTON_HEIGHT - M_TIME_TXT_HEIGHT);
				dc.SelectObject(oldPen);
			}

			if(drawTimeLine)
			{
				// draw button
				dc.Rectangle(GetButtonRect(x - rc.left));

				// draw time value text
				CString str;
				str.Format("%.2f", t);
				CRect rcTxt(x - M_TIME_BUTTON_WIDTH * 4,
					rc.bottom - M_TIME_TXT_HEIGHT,
					x + M_TIME_BUTTON_WIDTH * 4,
					rc.bottom);

				dc.DrawText(str, rcTxt, DT_SINGLELINE | DT_CENTER);
			}
		} // for

		// draw hit button
		if(_hitButton)
		{
			CBrush br(RGB(255, 0, 0));
			CBrush* oldBrush = dc.SelectObject(&br);
			CRect rcHit = GetButtonRect((int)(_clrs[_hitBtnIdx].first * rc.Width()));
			dc.Rectangle(&rcHit);
			dc.SelectObject(oldBrush);
		}

		// draw alpha value

		for (int x = arcClr.left; x < arcClr.right; ++x)
		{
			float a = (1.0 - ((float)(x - arcClr.left) / (float)arcClr.Width()));
#if 0
			CPen apen(PS_SOLID, 1, RGB(a, a, a));
			CPen* oldPen = dc.SelectObject(&apen);
			dc.MoveTo(x, arcClr.top);
			dc.LineTo(x, arcClr.top + M_TIME_BUTTON_HEIGHT);
			dc.SelectObject(oldPen);
#else
			DrawVLineWithAlpha(dat, RGB(a * 255, a * 255, a * 255), 1.0 - a, x,
				arcClr.top, arcClr.top + M_TIME_BUTTON_HEIGHT, bmp.bmWidth);
#endif
		}

		if(_selButton && (_selBtnIdx < (int)_clrs.size()))
		{
			COLORREF c = _clrs[_selBtnIdx].second;
			float r = (float)GetAValue(c) / 255.0f;
			int rx = (int)((float)arcClr.Width() * r) + arcClr.left;
			CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
			CPen* oldPen = dc.SelectObject(&redPen);
			dc.MoveTo(rx, arcClr.top);
			dc.LineTo(rx, arcClr.top + M_TIME_BUTTON_HEIGHT);
			dc.SelectObject(oldPen);

			CRect rcTxt;
			rcTxt.left = rx - M_TIME_BUTTON_WIDTH * 4;
			rcTxt.right = rx + M_TIME_BUTTON_WIDTH * 4;
			rcTxt.top = arcClr.bottom;
			rcTxt.bottom = rcTxt.top + M_TIME_TXT_HEIGHT;
			CString str;
			str.Format("%.2f", r);
			dc.DrawText(str, &rcTxt, DT_CENTER | DT_SINGLELINE);
		}

	} // if

	COLORREF* dat2 = (COLORREF*)OGE_ALLOC_T(BYTE, sz);
	_bgBmp.GetBitmapBits(sz, dat2);
	for (int bx = 0; bx < bmp.bmWidth; ++bx)
	{
		for (int by = 0; by < bmp.bmHeight; ++by)
		{
			COLORREF r = dat2[by * bmp.bmWidth + bx];
			if(r != maskClr)
			{
				dat[by * bmp.bmWidth + bx] = r;
			}
		}
	}

	_clrBmp.SetBitmapBits(sz, dat);

	OGE_FREE(dat);
	OGE_FREE(dat2);
	
	//bmpDC.BitBlt(0, 0,
	//	clientRc.Width(), clientRc.Height(),
	//	&dc, 0, 0, SRCINVERT);

	memDC.BitBlt(0, 0,
		clientRc.Width(), clientRc.Height(),
		&bmpDC, 0, 0, SRCCOPY);
}

void ColourLerpWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnMouseMove(nFlags, point);

	CRect rc;
	GetClientRect(&rc);

	bool update = false;
	if(_isDraging)
	{
		// 限定在绘制区拖动
		//if((point.x >= rc.left - M_TIME_BUTTON_WIDTH) && (point.x <= rc.right + M_TIME_BUTTON_WIDTH))
		if((point.x >= 0) && (point.x <= rc.right))
		{
			DragTime(point, _hitBtnIdx);
			update = true;
		}
	}
	else if(_isDragingAlpha)
	{
		CRect rcA = GetDrawAlphaColorArea();
		if((point.x >= 0) && (point.x <= rc.right))
		{
			// 控制在范围内
			long x = std::max(rcA.left, point.x);
			x = std::min(rcA.right, x);
			float a = (((float)(x - rcA.left) / (float)rcA.Width()));

			SetAValue(_clrs[_selBtnIdx].second, a);
			OnTimeChange(_selBtnIdx, _clrs[_selBtnIdx].first, _clrs[_selBtnIdx].second);
			update = true;
		}
	}
	else
	{
		int idx = 0;
		if(HitTest(point, idx))		
		{
			_hitButton = true;
			_hitBtnIdx = idx;
			update = true;
		}

		if(_hitButton)
		{
			_selButton = true;
			_selBtnIdx = _hitBtnIdx;
		}
	}

	if(update)
	{
		Invalidate(FALSE);
	}
	//UpdateWindow();
}

void ColourLerpWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	// 如果击中Alpha区则开始拖动ALPHA数值
	CRect arc = GetDrawAlphaColorArea();
	if(arc.PtInRect(point))
	{
		_isDragingAlpha = true;
		SetCapture();
	}
	else
	{
		int idx = 0;
		bool hit = HitTest(point, idx);
		if(hit)
		{
			_hitButton = hit;
			_hitBtnIdx = idx;
		// 不拖第一个和最后一个
		if((_hitBtnIdx >= 0) && (_hitBtnIdx < (int)_clrs.size()))
		{
			_isDraging = true;
			SetCapture();
		}

			_selButton = true;
			_selBtnIdx = _hitBtnIdx;
		}
		else
		{
			_selButton = false;
			_selBtnIdx = 0;
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void ColourLerpWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(_isDraging)
	{
		_isDraging = false;
		ReleaseCapture();
	}

	if(_isDragingAlpha)
	{
		_isDragingAlpha = false;
		ReleaseCapture();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

BOOL ColourLerpWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}

CRect ColourLerpWnd::GetButtonRect( int x )
{
	CRect rc = GetDrawArea();

	return CRect(rc.left + x - M_TIME_BUTTON_WIDTH / 2,
		rc.bottom - M_TIME_BUTTON_HEIGHT - M_TIME_TXT_HEIGHT,
		rc.left + x + M_TIME_BUTTON_WIDTH / 2 + 1,
		rc.bottom - M_TIME_TXT_HEIGHT);
}

// 检测鼠标经过时击中的按钮，并返回按钮的矩形
bool ColourLerpWnd::HitTest(const CPoint& pt, int& idx)
{
	CRect rc = GetDrawArea();
	int w = rc.Width();

	for (size_t i = 0; i < _clrs.size(); ++i)
	{
		float t = _clrs[i].first;
		int tx = (int)(t * w);
		CRect trc = GetButtonRect(tx);
		if(trc.PtInRect(pt))
		{
			idx = i;
			return true;
		}
	}

	return false;
}

void ColourLerpWnd::DragTime( const CPoint& pt, int idx )
{
	CRect rc = GetDrawArea();

	float currentT = GetMouseTime(pt);


	bool changed = false;

	// 检查是否要交换相邻两个
	if(idx - 1 > 0)
	{
		// 如果索引在左边的时间值比当前大
		if(_clrs[idx - 1].first > currentT)
		{
			_clrs[idx].first = currentT;

			std::swap(_clrs[idx - 1], _clrs[idx]);
			
			OnTimeChange(idx, _clrs[idx].first, _clrs[idx].second);
			OnTimeChange(idx - 1, _clrs[idx - 1].first, _clrs[idx - 1].second);

			_hitBtnIdx = idx - 1;

			changed = true;
		}
	}
	
	if(idx + 1 < (int)_clrs.size())
	{
		// 如果索引在右边的时间值比当前小
		if(_clrs[idx + 1].first < currentT)
		{
			_clrs[idx].first = currentT;

			std::swap(_clrs[idx + 1], _clrs[idx]);

			OnTimeChange(idx, _clrs[idx].first, _clrs[idx].second);
			OnTimeChange(idx + 1, _clrs[idx + 1].first, _clrs[idx + 1].second);

			_hitBtnIdx = idx + 1;

			changed = true;
		}
	}

	if(!changed)
	{
		_clrs[idx].first = currentT;
		OnTimeChange(idx, currentT, _clrs[idx].second);
	}
}

CRect ColourLerpWnd::GetDrawArea() const
{
	CRect rc;
	GetClientRect(&rc);
	rc.DeflateRect(&_borderSize);
	rc.bottom -= M_ALPHA_AREA_HEIGHT;

	return rc;
}

float ColourLerpWnd::GetMouseTime( const CPoint& pt )
{
	CRect rc = GetDrawArea();
	float t = (float)(pt.x - _borderSize.left) / (float)rc.Width();
	return std::min(1.0f, std::max(0.0f, t));
}

void ColourLerpWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	if(HitTest(point, i))
	{
		CColorDialog dlg(_clrs[i].second, CC_FULLOPEN, this);
		BYTE a = GetAValue(_clrs[i].second);
		if(dlg.DoModal() == IDOK)
		{
			COLORREF clr = dlg.GetColor();
			SetAValue(clr,  a);
			_clrs[i].second = clr;
			OnColorChange(i, clr);

			Invalidate(FALSE);
		}
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void ColourLerpWnd::OnColorChange( int idx, COLORREF currentClr )
{

}

void ColourLerpWnd::OnTimeChange( int idx, float timeChange, COLORREF currentClr )
{

}

CRect ColourLerpWnd::GetDrawAlphaArea() const
{
	CRect rc;
	GetClientRect(&rc);
	rc.DeflateRect(&_borderSize);
	rc.top = rc.bottom - M_ALPHA_AREA_HEIGHT;
	return rc;
}

void ColourLerpWnd::DrawAlphaCanvas( CDC& dc, const CRect& rc )
{
	COLORREF clr1 = RGB(191, 191, 191);
	COLORREF clr2 = RGB(255, 255, 255);
	CBrush br1(clr1);
	CBrush br2(clr2);
	CPen p1(PS_SOLID, 1, clr1);
	CPen p2(PS_SOLID, 1, clr2);

	CBrush* oldBr = dc.SelectObject(&br1);
	CPen* oldPen = dc.SelectObject(&p1);

	LONG xidx = 0;
	for (LONG x = rc.left; x < rc.right; x += M_ALPHA_BLOCK_SZ)
	{
		LONG yidx = 0;
		for (LONG y = rc.top; y < rc.bottom; y += M_ALPHA_BLOCK_SZ)
		{
			CBrush* br = &br1;
			CPen* pen = &p1;

			if((xidx + 1) % 2 == 0)
			{
				if((yidx + 1) % 2 == 0)
				{
					br = &br1;
					pen = &p1;
				}
				else
				{
					br = &br2;
					pen = &p2;
				}
			}
			else
			{
				if((yidx + 1) % 2 == 0)
				{
					br = &br2;
					pen = &p2;
				}
				else
				{
					br = &br1;
					pen = &p1;
				}
			}

			++yidx;

			dc.SelectObject(pen);
			dc.SelectObject(br);
			dc.Rectangle(x, y, std::min(rc.right, x + M_ALPHA_BLOCK_SZ),
				std::min(y + M_ALPHA_BLOCK_SZ, rc.bottom));
		}

		++xidx;
	}

	dc.SelectObject(oldBr);
	dc.SelectObject(oldPen);
}

void ColourLerpWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CPaintDC dc(this);
	// TODO: Add your message handler code here
	if(_clrBmp.GetSafeHandle())
	{
		_clrBmp.DeleteObject();
	}

	_clrBmp.CreateCompatibleBitmap(&dc, cx, cy);

	if(_bgBmp.GetSafeHandle())
	{
		_bgBmp.DeleteObject();
	}

	_bgBmp.CreateCompatibleBitmap(&dc, cx, cy);


	Invalidate(FALSE);
	//if(_alphaBmp.GetSafeHandle())
	//{
	//	_alphaBmp.DeleteObject();
	//}

	//rc = GetDrawAlphaColorArea();
	//_alphaBmp.CreateCompatibleBitmap(&dc, rc.Width() + 1, rc.Height());
}

CRect ColourLerpWnd::GetDrawColorArea() const
{
	CRect rc = GetDrawArea();
	CRect bk = rc;
	bk.bottom -= M_TIME_BUTTON_HEIGHT + M_TIME_TXT_HEIGHT;
	return bk;
}

CRect ColourLerpWnd::GetDrawAlphaColorArea() const
{
	CRect arc = GetDrawAlphaArea();
	arc.bottom = arc.top + M_TIME_BUTTON_HEIGHT;
	return arc;
}

void ColourLerpWnd::DrawVLineWithAlpha(
	COLORREF* dat, COLORREF clr, float alpha, int x,
	int y1, int y2, int w )
{
	for (int y = y1; y <= y2; ++y)
	{
		COLORREF cs = dat[y * w + x];
		cs = RGBLerp(clr, cs, alpha);
		dat[y * w + x] = RGB(GetBValue(cs), GetGValue(cs), GetRValue(cs));
	}
}
