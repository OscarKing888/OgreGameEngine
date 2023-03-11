#pragma once

class _EditorCoreExport ListSelectDlg
	: public CDialogEx
{
public:

	ListSelectDlg(const CString& title, const StringVector& lst, CWnd* parent);   // standard constructor
	virtual ~ListSelectDlg();

	virtual BOOL OnInitDialog();

	String GetSelectedItem();	

protected:

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//CComboBox	_comboBox;
	CListBox	_listBox;
	CString		_selected;
	StringVector _list;
	CString		_title;

	afx_msg void OnSelect();

	afx_msg void OnOK();
};