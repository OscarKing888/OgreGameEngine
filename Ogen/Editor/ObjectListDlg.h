#pragma once
#include "afxcmn.h"
#include "resource.h"       // Ö÷·ûºÅ

class ObjectListDlg : public CDialogEx
	,public Singleton<ObjectListDlg>
{
	DECLARE_DYNAMIC(ObjectListDlg)

public:
	ObjectListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ObjectListDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OBJECTS };

	void RefreshObjectList();
	void ClearList();

	void Show();

	virtual void PostNcDestroy();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CTreeCtrl _objectTree;

	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickObjectName(NMHDR *pNMHDR, LRESULT *pResult);
};
