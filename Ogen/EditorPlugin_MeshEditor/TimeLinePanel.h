#pragma once
#include <Engine/MeshDataManager.h>
// class  TimeSliderCtrl : public CSliderCtrl

// 	DECLARE_MESSAGE_MAP()
// 
// public:
// 
// 	TimeSliderCtrl();
// 	virtual ~TimeSliderCtrl();
// 
// protected:
// 
// 	afx_msg void OnNMClk(NMHDR *pNMHDR, LRESULT *pResult);
// };


class TimeLinePanel : public CDockablePane
{
	DECLARE_MESSAGE_MAP()

public:

	//friend TimeSliderCtrl;

	TimeLinePanel();
	virtual ~TimeLinePanel();

	void Reset();
	
	virtual BOOL OnBeforeShowPaneMenu(CMenu& m);
	virtual BOOL OnAfterShowPaneMenu(int nMenuResult);

	void SetMeshData(MeshData* meshData)
	{ _meshData = meshData; _selectedFrameEvent = 0; }

	void SetCurrentTime(float t) { _currentTime = t; }
	float GetCurrentTimePos() const { return _currentTime; }

	void SetTotalTime(float t) { _totalTime = t; }
	float GetTotalTime() const { return _totalTime; }

	const SFrameEvent* GetSelectedFrameEvent() const { return _selectedFrameEvent; }

protected:

	//TimeSliderCtrl _slider;
	CMenu _popupMenu;

	float _totalTime;
	float _currentTime;
	int _moveingCursorX;

	COLORREF _currentCursorClr;
	COLORREF _frameEventClr;
	COLORREF _selectedFrameEventClr;

	//CToolTipCtrl _tooltip;

	bool _isMouseLDown;

	CBitmapButton _playBtn;
	CBitmapButton _pauseBtn;
	CBitmapButton _stopBtn;
	CBitmapButton _toBeginBtn;
	CBitmapButton _toEndBtn;

	const SFrameEvent* _selectedFrameEvent;
	MeshData* _meshData;

	void HitTestFrameEvent(const CPoint& mousePt);

	virtual BOOL  PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPlay();
	afx_msg void OnPause();
	afx_msg void OnStop();
	afx_msg void OnToBegin();
	afx_msg void OnToEnd();
};

