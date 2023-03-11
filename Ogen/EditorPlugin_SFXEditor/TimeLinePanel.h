#pragma once
class TimeLinePanelController;

class TimeLinePanel
	: public CDockablePane
	, public Singleton<TimeLinePanel>
{
	DECLARE_MESSAGE_MAP()

public:

	TimeLinePanel();
	virtual ~TimeLinePanel();

	void RelayoutEditor();

	TimeblockContainer& GetContainer() { return _container; }
	
protected:

	TimeblockContainer _container;
	TimeLinePanelController* _controller;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};