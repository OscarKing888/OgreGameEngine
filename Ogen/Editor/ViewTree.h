
#pragma once

/////////////////////////////////////////////////////////////////////////////
// ViewTree 窗口

class ViewTree : public CTreeCtrl
{
// 构造
public:
	ViewTree();

// 重写
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// 实现
public:
	virtual ~ViewTree();

protected:

	DECLARE_MESSAGE_MAP()


	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDBClick(NMHDR *pNMHDR, LRESULT *pResult);
};
