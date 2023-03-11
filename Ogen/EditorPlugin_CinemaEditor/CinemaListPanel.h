#pragma once

class CinemaList : public CListCtrl
{
	DECLARE_MESSAGE_MAP()

public:

	CinemaList();
	virtual ~CinemaList();

protected:

	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};

class CinemaListPanel : public CDockablePane, public IEventLisitener
{
	DECLARE_MESSAGE_MAP()

public:

	CinemaListPanel();
	virtual ~CinemaListPanel();

	CListCtrl& GetListCtrl() { return _listCtrl; }

	void RefreshCinemaList();

protected:

	CinemaList _listCtrl;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast);
};

