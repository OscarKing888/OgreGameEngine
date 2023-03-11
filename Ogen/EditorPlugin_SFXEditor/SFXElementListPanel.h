#pragma once

class SFXElementList : public CListCtrl
{
	DECLARE_MESSAGE_MAP()

public:

	SFXElementList();
	virtual ~SFXElementList();

protected:

	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};

class SFXElementListPanel : public CDockablePane
{
	DECLARE_MESSAGE_MAP()

public:

	SFXElementListPanel();
	virtual ~SFXElementListPanel();

	CListCtrl& GetListCtrl() { return _listCtrl; }

protected:

	SFXElementList _listCtrl;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

