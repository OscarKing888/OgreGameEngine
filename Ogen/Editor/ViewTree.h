
#pragma once

/////////////////////////////////////////////////////////////////////////////
// ViewTree ����

class ViewTree : public CTreeCtrl
{
// ����
public:
	ViewTree();

// ��д
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// ʵ��
public:
	virtual ~ViewTree();

protected:

	DECLARE_MESSAGE_MAP()


	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDBClick(NMHDR *pNMHDR, LRESULT *pResult);
};
