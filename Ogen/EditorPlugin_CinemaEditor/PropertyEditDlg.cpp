// PropertyEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyEditDlg.h"
#include "Resource.h"

// PropertyEditDlg dialog

IMPLEMENT_DYNAMIC(PropertyEditDlg, CDialog)

PropertyEditDlg::PropertyEditDlg(IGameObject* obj, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_PROPERTY_EDIT, pParent)
	, _object(obj)
{

}

PropertyEditDlg::~PropertyEditDlg()
{
}

void PropertyEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PropertyEditDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// PropertyEditDlg message handlers

void PropertyEditDlg::OnSize( UINT nType, int cx, int cy )
{
	Relayout(cx, cy);
}

BOOL PropertyEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!_propGrid.Create(WS_VISIBLE | WS_CHILD, CRect(0, 0, 20, 20), this, 1))
	{
		return FALSE; // Î´ÄÜ´´½¨
	}

	_propGrid.SetObject(_object);

	CRect rc;
	GetClientRect(&rc);
	Relayout(rc.Width(), rc.Height());


	return TRUE;
}

void PropertyEditDlg::Relayout( int cx, int cy )
{
	if(_propGrid.GetSafeHwnd())
	{
		_propGrid.MoveWindow(0, 0, cx, cy - 40);
	}

	CWnd* btn = GetDlgItem(IDOK);
	if(btn)
	{
		CRect rc;
		btn->GetClientRect(&rc);
		btn->MoveWindow(cx - rc.Width(), cy - rc.Height(), rc.Width(), rc.Height());
	}

	btn = GetDlgItem(IDCANCEL);
	if(btn)
	{
		CRect rc;
		btn->GetClientRect(&rc);
		btn->MoveWindow(cx - rc.Width() * 2 - 20, cy - rc.Height(), rc.Width(), rc.Height());
	}
}