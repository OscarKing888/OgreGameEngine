// FloatValueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FloatValueDlg.h"
#include "Resource.h"


// FloatValueDlg dialog

IMPLEMENT_DYNAMIC(FloatValueDlg, CDialog)

FloatValueDlg::FloatValueDlg(float initVal, CWnd* pParent /*=NULL*/, float minVal, float maxVal, float step)
	: CDialog(IDD_DIALOG_FLOAT_VALUE, pParent)
	, _minVal(minVal)
	, _maxVal(maxVal)
	, _stepVal(step)
	, _currentValue(initVal)
{

}

FloatValueDlg::~FloatValueDlg()
{
}

void FloatValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_INPUT, _spin);
	DDX_Control(pDX, IDC_EDIT_INPUT, _edit);

	DDX_Text(pDX, IDC_EDIT_INPUT, _currentValue);
}


BEGIN_MESSAGE_MAP(FloatValueDlg, CDialog)
	ON_COMMAND(IDOK, OnOK)
	ON_EN_CHANGE(IDC_EDIT_INPUT, OnValueChange)
END_MESSAGE_MAP()

BOOL FloatValueDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_spin.SetRange(_minVal / _stepVal, _maxVal / _stepVal);
	_spin.SetPos(0);

	return TRUE;
}
//--------------------------------------------------------------------------------
float FloatValueDlg::GetValue() const
{
	return _currentValue;
}
//--------------------------------------------------------------------------------
void FloatValueDlg::OnOK()
{
	_currentValue = (float)_spin.GetPos() * _stepVal;
	CDialog::OnOK();
}
//--------------------------------------------------------------------------------
void FloatValueDlg::OnValueChange()
{
	if(!_edit.GetSafeHwnd())
	{
		return;
	}
	 
	if(_spin.GetSafeHwnd())
	{
		_currentValue = (float)_spin.GetPos() * _stepVal;
	}

	//CString str;
	//str.Format("%.2f", _currentValue);
	//_edit.SetWindowText(str);
}
// FloatValueDlg message handlers
