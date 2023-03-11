// SplashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SplashDlg.h"
#include "Resource.h"

// SplashDlg dialog

IMPLEMENT_DYNAMIC(SplashDlg, CDialog)

SplashDlg::SplashDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SPLASH, pParent)
{
	SetBackgroundColor(RGB(175, 197, 249));
	Create(IDD_DIALOG_SPLASH);

	HICON icon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(icon, TRUE);
	SetIcon(icon, FALSE);

	ShowWindow(TRUE);
	CenterWindow();
}

SplashDlg::~SplashDlg()
{
	DestroyWindow();
}

void SplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SplashDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SplashDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SplashDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// SplashDlg message handlers

void SplashDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void SplashDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
}
