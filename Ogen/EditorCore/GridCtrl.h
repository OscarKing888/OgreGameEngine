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

	/// ��������
	CRect _borderSize;

	/// ������ɫ
	COLORREF _bkColor;

	/// ����ʱ�䷶Χ�ı�����ɫ
	COLORREF _bkInvalidColor;

	/// ������ɫ
	COLORREF _gridColor;

	/// ��������
	CFont _textFont;

	/// ������ɫ
	COLORREF _textColor;

	/// ��ǰʱ������ɫ
	COLORREF _currentTimeLineColor;

	/// ��������ɫ
	COLORREF _axisColor;

	/// X���ϣ�ʱ������ر�����С����������1��
	int _pixelsPerSecond;

	/// Y���ϣ���ֵ�����ر�����С����������1��λ
	int _pixelsPerUnitValue;

	/// ��Сʱ����ֵ��Χ����
	float _minValidTime;

	/// ���ʱ����ֵ��Χ����
	float _maxValidTime;

	/// ��Сy��ֵ��Χ
	float _minValidValue;

	/// ���y��ֵ��Χ
	float _maxValidValue;	

	/// ��ǰ��ʾ��ԭ��
	float _originX;
	float _originY;

	/// ���ű���
	float _zoomScale;

	/// �����ٶ�
	float _zoomSpeed;

	/// ��ǰʱ��
	float _currentTime;

	/// �Ƿ������м��ƶ�ԭ��
	bool _isMovingOrigin;

	/// �ϴ���갴��ʱ�����꣬�����϶�����
	CPoint _lastMouseDownPt;

	/// �Ƿ����ƶ�ʱ����
	bool _isMovingCurrentTime;

	/// �޶��ƶ�ʱ����ʱ����Ч��Χ
	bool _clampToValidRange;

	/// �Ƿ����ʱ����
	bool _hitCurrentTimeLine;

	/// ��x�ϵ��Զ���׽��С
	float _xSnapSz;

	/// ��y�ϵ��Զ���׽��С
	float _ySnapSz;

	/// �Ƿ���Զ���׽
	bool _enableSnap;

	IGridCtrlEventListener* _listener;

	/// �Ƿ�������ʱ����
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


	/// �������ش���ǰʱ��仯
	virtual void OnCurrentTimeChange(float currentTime);
	virtual void OnZoomChange(float scale);
	virtual void OnOriginChange(float xOff, float yOff);
};


