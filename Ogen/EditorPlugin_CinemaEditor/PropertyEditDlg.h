#pragma once


// PropertyEditDlg dialog

class PropertyEditDlg : public CDialog
{
	DECLARE_DYNAMIC(PropertyEditDlg)

public:
	PropertyEditDlg(IGameObject* obj, CWnd* pParent = NULL);   // standard constructor
	virtual ~PropertyEditDlg();

	IGameObject* GetObject() const { return _object; }
	void SetObject(IGameObject* val) { _object = val; }


// Dialog Data
	//enum { IDD = IDD_DIALOG_PROPERTY_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	PropertyGridEx _propGrid;
	IGameObject* _object;
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void Relayout( int cx, int cy );
	virtual BOOL OnInitDialog();
};
