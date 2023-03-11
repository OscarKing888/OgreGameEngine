#pragma once


// FloatValueDlg dialog

class FloatValueDlg : public CDialog
{
	DECLARE_DYNAMIC(FloatValueDlg)

public:
	FloatValueDlg(float initVal, CWnd* pParent = NULL, float minVal = 0.0f, float maxVal = 1.0f, float step = 0.1f);   // standard constructor
	virtual ~FloatValueDlg();

	float GetValue() const;
// Dialog Data
	//enum { IDD = IDD_DIALOG_FLOAT_VALUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CSpinButtonCtrl _spin;
	CEdit _edit;
	float _minVal;
	float _maxVal;
	float _stepVal;

	float _currentValue;

	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnValueChange();
};
