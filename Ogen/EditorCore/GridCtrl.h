#pragma once
#include "EditorCorePrerequisites.h"

// GridControl
class GridCtrl;

class IGridCtrlEventListener
{
public:

	virtual void OnSetCurrentTime(GridCtrl* ctrl, float t) = 0;
	virtual void OnZoomChange(GridCtrl* ctrl, float scale) = 0;
	virtual void OnOriginChange(GridCtrl* ctrl, float xOff, float yOff) = 0;
};


class _EditorCoreExport GridCtrl : public CWnd
{
	DECLARE_DYNAMIC(GridCtrl)

public:

	GridCtrl();
	virtual ~GridCtrl();

	CRect GetBorderSize() const { return _borderSize; }
	void SetBorderSize(CRect val) { _borderSize = val; }

	COLORREF GetBkColor() const { return _bkColor; }
	void SetBkColor(COLORREF val) { _bkColor = val; }

	COLORREF GetGridColor() const { return _gridColor; }
	void SetGridColor(COLORREF val) { _gridColor = val; }

	COLORREF GetCurrentTimeLineColor() const { return _currentTimeLineColor; }
	void SetCurrentTimeLineColor(COLORREF val) { _currentTimeLineColor = val; Redraw(); }

	COLORREF AxisColor() const { return _axisColor; }
	void AxisColor(COLORREF val) { _axisColor = val; }

	int GetPixelPerSecond() const { return _pixelsPerSecond; }
	void SetPixelPerSecond(int val) { _pixelsPerSecond = val; }

	int GetPixelsPerUnitValue() const { return _pixelsPerUnitValue; }
	void SetPixelsPerUnitValue(int val) { _pixelsPerUnitValue = val; }

	float GetMaxValidTime() const { return _maxValidTime; }
	void SetMaxValidTime(float val) { _maxValidTime = val; }

	float GetMinValidTime() const { return _minValidTime; }
	void SetMinValidTime(float val) { _minValidTime = val; }

	float GetMaxValidValue() const { return _maxValidTime; }
	void SetMaxValidValue(float val) { _maxValidTime = val; }

	float GetMinValidValue() const { return _minValidTime; }
	void SetMinValidValue(float val) { _minValidTime = val; }

	float GetZoomScale() const { return _zoomScale; }
	void SetZoomScale(float val) { _zoomScale = val; }

	float GetZoomSpeed() const { return _zoomSpeed; }
	void SetZoomSpeed(float val) { _zoomSpeed = val;  Redraw(); }

	float GetXSnapSz() const { return _xSnapSz; }
	void SetXSnapSz(float val) { _xSnapSz = val; }

	float GetYSnapSz() const { return _ySnapSz; }
	void SetYSnapSz(float val) { _ySnapSz = val; }

	bool GetEnableSnap() const { return _enableSnap; }
	void SetEnableSnap(bool val) { _enableSnap = val; }

	bool GetClampToValidRange() const { return _clampToValidRange; }
	void SetClampToValidRange(bool val) { _clampToValidRange = val; }

	float GetOriginX() const { return _originX; }
	void SetOriginX(float val) { _originX = val;  Redraw(); }
	
	float GetOriginY() const { return _originY; }
	void SetOriginY(float val) { _originY = val;  Redraw(); }

	IGridCtrlEventListener* GetListener() const { return _listener; }
	void SetListener(IGridCtrlEventListener* val) { _listener = val; }

	float GetCurrentTime() const { return _currentTime; }
	void SetCurrentTime(float val) { _currentTime = val; Redraw(); }

	void Redraw();

	void ConvertToPixelCoord(float x, float y, int& outX, int& outY);

protected:
	DECLARE_MESSAGE_MAP()

	/// 四周留边
	CRect _borderSize;

	/// 背景颜色
	COLORREF _bkColor;

	/// 超出时间范围的背景颜色
	COLORREF _bkInvalidColor;

	/// 网格颜色
	COLORREF _gridColor;

	/// 文字字体
	CFont _textFont;

	/// 文字颜色
	COLORREF _textColor;

	/// 当前时间线颜色
	COLORREF _currentTimeLineColor;

	/// 主轴线颜色
	COLORREF _axisColor;

	/// X轴上，时间的像素比例大小，多少像素1秒
	int _pixelsPerSecond;

	/// Y轴上，数值的像素比例大小，多少像素1单位
	int _pixelsPerUnitValue;

	/// 最小时间数值范围，秒
	float _minValidTime;

	/// 最大时间数值范围，秒
	float _maxValidTime;

	/// 最小y数值范围
	float _minValidValue;

	/// 最大y数值范围
	float _maxValidValue;	

	/// 当前显示的原点
	float _originX;
	float _originY;

	/// 缩放比例
	float _zoomScale;

	/// 缩放速度
	float _zoomSpeed;

	/// 当前时间
	float _currentTime;

	/// 是否在用中键移动原点
	bool _isMovingOrigin;

	/// 上次鼠标按下时的坐标，用于拖动操作
	CPoint _lastMouseDownPt;

	/// 是否在移动时间线
	bool _isMovingCurrentTime;

	/// 限定移动时间线时在有效范围
	bool _clampToValidRange;

	/// 是否击中时间线
	bool _hitCurrentTimeLine;

	/// 在x上的自动捕捉大小
	float _xSnapSz;

	/// 在y上的自动捕捉大小
	float _ySnapSz;

	/// 是否打开自动捕捉
	bool _enableSnap;

	IGridCtrlEventListener* _listener;

	/// 是否鼠标击中时间线
	bool HitCurrentTimeLine(const CPoint& pt);

	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);

	CRect GetDrawArea() const;
	float GetMouseTime(const CPoint& pt);
	float GetMouseValue(const CPoint& pt);

	virtual void OnDraw(CDC& dc, const CRect& rc);
	virtual void OnPostDrawGrid(CDC& dc, const CRect& rc);
	virtual void OnDrawBackground( CDC &dc, const CRect& rc );
	virtual void OnDrawGridLine( CDC &dc, const CRect& rc );
	virtual void OnDrawGridCoordText( CDC &dc, const CRect& rc );
	virtual void OnDrawCurrentTime( CDC &dc, const CRect& rc );
	virtual void OnDrawBorder( CDC &dc, const CRect& rc );

	virtual void OnMouseWheel(short zDelta, const CPoint& point);
	virtual void OnMouseMove(const CPoint& point);
	virtual void OnLButtonDown(const CPoint& point);
	virtual void OnLButtonUp(const CPoint& point);
	virtual void OnLButtonDblClk(const CPoint& point);


	/// 子类重载处理当前时间变化
	virtual void OnCurrentTimeChange(float currentTime);
	virtual void OnZoomChange(float scale);
	virtual void OnOriginChange(float xOff, float yOff);
};


