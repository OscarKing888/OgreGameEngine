#pragma once


// OpenMapDlg dialog

class OpenMapDlg : public CDialog
{
	DECLARE_DYNAMIC(OpenMapDlg)

public:
	OpenMapDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~OpenMapDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPENMAP };

	const CString& GetMapName() const;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	CTreeCtrl _lstCtrl;
	CString _mapName;

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	afx_msg void OnDBClickTree(NMHDR *pNMHDR, LRESULT *pResult);
};
