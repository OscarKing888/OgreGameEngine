// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputDlg.h"
#include "resource.h"       // Ö÷·ûºÅ

InputDlg::InputDlg(const CString& initStr, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_INPUT, pParent)
	, _inputText(initStr)
{
}

InputDlg::~InputDlg()
{
}

void InputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, _inputText);
}


BEGIN_MESSAGE_MAP(InputDlg, CDialogEx)

END_MESSAGE_MAP()