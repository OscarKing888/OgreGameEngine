#pragma once
#include "EditorCorePrerequisites.h"

// ColourLerpWnd
typedef std::pair<float, COLORREF> TimeColorPair;
typedef vector<TimeColorPair>::type TimeColorList;

#define GetAValue(rgb)      (rgb >> 24)
#define ARGB32(a,r,g,b)		((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24))

void SetAValue(COLORREF& src, byte a);
void SetAValue(COLORREF& src, float a);
COLORREF RGBLerp(COLORREF src, COLORREF dest, float s);


class _EditorCoreExport ColourLerpWnd : public CWnd
{
	DECLARE_DYNAMIC(ColourLerpWnd)

public:
	ColourLerpWnd();
	virtual ~ColourLerpWnd();

	const TimeColorList& GetColourList() const { return _clrs; }
	void SetColourList(const TimeColorList& val) { _clrs = val; Invalidate(FALSE); }

	bool GetDrawTimeLine() const { return _drawTimeLine; }
	void SetDrawTimeLine(bool val) { _drawTimeLine = val; }

	CRect GetBorderSize() const { return _borderSize; }
	void SetBorderSize(CRect val) { _borderSize = val; }



protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	void DrawVLineWithAlpha(COLORREF* dat, COLORREF clr, float alpha,
		int x, int y1, int y2, int w);

	CRect GetButtonRect( int x );

protected:
	// ʱ������ɫ�����飬ʱ�䷶Χ��[0.0, 1.0]
	TimeColorList _clrs;
	/// �Ƿ����ʱ����
	bool _drawTimeLine;

	/// �Ƿ����϶��༭��ť
	bool _isDraging;
	bool _isDragingAlpha;
	/// �Ƿ�ѡ�а�ť
	bool _hitButton;
	bool _selButton;
	/// ѡ�а�ť�������е�λ��
	int _hitBtnIdx;
	int _selBtnIdx;

	CFont _defaultFont;

	/// ��������
	CRect _borderSize;

	/// ���ƻ���
	CBitmap _clrBmp;
	CBitmap _bgBmp;

	/// ֪ͨ��������������ʵ��
	virtual void OnColorChange(int idx, COLORREF currentClr);

	/// ĳ��ʱ��ı�
	virtual void OnTimeChange(int idx, float timeChange, COLORREF currentClr);

public:

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// �����꾭��ʱ���еİ�ť�������ذ�ť�������е�����
	bool HitTest(const CPoint& pt, int& idx);
	void DragTime(const CPoint& pt, int idx);
	CRect GetDrawArea() const;
	float GetMouseTime(const CPoint& pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	CRect GetDrawAlphaArea() const;
	void DrawAlphaCanvas(CDC& dc, const CRect& rc);

	CRect GetDrawColorArea() const;
	CRect GetDrawAlphaColorArea() const;


	afx_msg void OnSize(UINT nType, int cx, int cy);
};


