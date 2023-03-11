#pragma once


// SplashDlg dialog

class SplashDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SplashDlg)

public:
	SplashDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SplashDlg();

// Dialog Data
//	enum { IDD = IDD_DIALOG_SPLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
