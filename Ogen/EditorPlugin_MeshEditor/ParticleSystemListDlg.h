#pragma once


// ParticleSystemListDlg dialog

class IParticleTempalteListener
{
public:

	virtual void OnSelectParticleSystem(const CString& name) = 0;
};

class ParticleTemplateListCtrl : public CListCtrl
{
	DECLARE_MESSAGE_MAP()

public:

	ParticleTemplateListCtrl();
	virtual ~ParticleTemplateListCtrl();

protected:

	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};


class ParticleSystemListDlg : public CDialog, public IParticleTempalteListener
{
	DECLARE_DYNAMIC(ParticleSystemListDlg)

public:
	ParticleSystemListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ParticleSystemListDlg();
	const String& GetSelectedParticleSystemTemplate() const { return _selectedListItemName; }
	void RefreshListItem();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	ParticleTemplateListCtrl _listCtrl;
	String _selectedListItemName;
	CString _selItem;

	virtual BOOL OnInitDialog();

	virtual void OnOK();

	afx_msg void OnListItemClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnListItemDBClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual void OnSelectParticleSystem(const CString& name);
};
