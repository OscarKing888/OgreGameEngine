#pragma once
#include "EditorCorePrerequisites.h"

class _EditorCoreExport InputDlg : public CDialogEx
{
public:
	InputDlg(const CString& initStr = "", CWnd* pParent = NULL);   // standard constructor
	virtual ~InputDlg();

	String GetInputText() const { return (LPCTSTR)_inputText; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CString _inputText;
};
