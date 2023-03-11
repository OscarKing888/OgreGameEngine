#include "stdafx.h"
#include "ListSelectDlg.h"
#include "Resource.h"

ListSelectDlg::ListSelectDlg(const CString& title, const StringVector& lst, CWnd* parent)
: CDialogEx(IDD_DIALOG_LIST_SELECT, parent)
, _list(lst)
, _title(title)
{
}

ListSelectDlg::~ListSelectDlg()
{
}

void ListSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//DDX_Control(pDX, IDC_COMBO_LIST_SEL, _comboBox);
	//DDX_Text(pDX, IDC_COMBO_LIST_SEL, _selected);
	DDX_Control(pDX, IDC_LIST_SELECT_ITEMS, _listBox);	
}


BEGIN_MESSAGE_MAP(ListSelectDlg, CDialogEx)
	ON_COMMAND(IDOK, OnOK)
	ON_LBN_DBLCLK(IDC_LIST_SELECT_ITEMS, OnSelect)
END_MESSAGE_MAP()


// ListSelectDlg message handlers

BOOL ListSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (size_t i = 0; i < _list.size(); ++i)
	{
		String s = _list[i];
		if(i == 0)
		{
			_selected = s.c_str();
		}
		_listBox.AddString(s.c_str());
	}

	_listBox.SetCurSel(0);

	UpdateData(FALSE);

	this->SetWindowText(_title);

	this->CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

String ListSelectDlg::GetSelectedItem()
{
	return (LPCTSTR)_selected;
}

void ListSelectDlg::OnSelect()
{
	OnOK();
}

void ListSelectDlg::OnOK()
{
	UpdateData();
	_selected = _list[_listBox.GetCurSel()].c_str();
	CDialogEx::OnOK();
}