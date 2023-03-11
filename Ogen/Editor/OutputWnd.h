#pragma once

/////////////////////////////////////////////////////////////////////////////
// OutputList ����

class OutputList : public CListBox
{
// ����
public:
	OutputList();

// ʵ��
public:
	virtual ~OutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class OutputWnd : public CDockablePane
	, public LogListener
	, public Singleton<OutputWnd>
	, public IAppListener
{
// ����
public:
	OutputWnd();

// ����
protected:
	CFont m_Font;

	//CMFCTabCtrl	m_wndTabs;

	OutputList m_wndOutputBuild;

protected:

	void AdjustHorzScroll(CListBox& wndListBox);

	virtual void messageLogged( const String& message, LogMessageLevel lml, bool maskDebug, const String &logName );
	virtual BOOL DestroyWindow();

// ʵ��
public:
	virtual ~OutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()

	virtual void OnAppInitCompleted();
};

