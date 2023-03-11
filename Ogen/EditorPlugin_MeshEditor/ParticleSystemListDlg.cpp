// ParticleSystemListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ParticleSystemListDlg.h"
#include "Resource.h"

BEGIN_MESSAGE_MAP(ParticleTemplateListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &ParticleTemplateListCtrl::OnNMDblclk)
END_MESSAGE_MAP()

ParticleTemplateListCtrl::ParticleTemplateListCtrl()
{

}

ParticleTemplateListCtrl::~ParticleTemplateListCtrl()
{

}

void ParticleTemplateListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString txt = GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem);
	IParticleTempalteListener* parent = dynamic_cast<IParticleTempalteListener*>(GetParent());
	if(parent)
	{
		parent->OnSelectParticleSystem(txt);
	}
}

//------------------------------------------------------------------------
// ParticleSystemListDlg dialog

IMPLEMENT_DYNAMIC(ParticleSystemListDlg, CDialog)

ParticleSystemListDlg::ParticleSystemListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_PARTICLELIST, pParent)
{

}

ParticleSystemListDlg::~ParticleSystemListDlg()
{
}

void ParticleSystemListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARTICLE_SYSTEM_LIST, _listCtrl);

	DDX_Text(pDX, IDC_PARTICLE_SYSTEM_LIST, _selItem);
}


BEGIN_MESSAGE_MAP(ParticleSystemListDlg, CDialog)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_PARTICLE_SYSTEM_LIST, &ParticleSystemListDlg::OnListItemClick)
	ON_NOTIFY(HDN_ITEMDBLCLICK, IDC_PARTICLE_SYSTEM_LIST, &ParticleSystemListDlg::OnListItemDBClick)
END_MESSAGE_MAP()


// ParticleSystemListDlg message handlers
void ParticleSystemListDlg::OnListItemClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	_selectedListItemName = (LPCTSTR)_listCtrl.GetItemText(phdr->iItem, 0);
	//OnClickListItem(phdr->iItem, _selectedListItemName);
	*pResult = 0;
}

void ParticleSystemListDlg::OnListItemDBClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	_selectedListItemName = (LPCTSTR)_listCtrl.GetItemText(phdr->iItem, 0);
	//OnDBClickListItem(phdr->iItem, _selectedListItemName);
	*pResult = 0;

	OnOK();
}

void ParticleSystemListDlg::RefreshListItem()
{
	_listCtrl.DeleteAllItems();

	ParticleSystemManager& mgr = ParticleSystemManager::getSingleton();
	ParticleSystemManager::ParticleSystemTemplateIterator i = mgr.getTemplateIterator();
	while (i.hasMoreElements())
	{
		_listCtrl.InsertItem(_listCtrl.GetItemCount(), i.peekNextKey().c_str());
		i.moveNext();
	}
}

BOOL ParticleSystemListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rc;
	GetClientRect(&rc);

// 	_listCtrl.Create(WS_CHILD | WS_VISIBLE | /*LVS_ICON |*/ LVS_SINGLESEL | /*LVS_SORTASCENDING |*/ LVS_ALIGNLEFT
// 		| WS_BORDER | WS_TABSTOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
// 		CRect(0, 0, rc.Width(), rc.Height() - 60), this, IDC_PARTICLE_SYSTEM_LIST);

	RefreshListItem();
	return TRUE;
}

void ParticleSystemListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(_listCtrl.GetSafeHwnd())
	{
		_listCtrl.MoveWindow(0, 0, cx, cy - 60);
	}
}

void ParticleSystemListDlg::OnOK()
{
	CDialog::OnOK();

	UpdateData();

	if(_selectedListItemName.empty())
	{
		_selectedListItemName = (LPCTSTR)_selItem;
	}
}

void ParticleSystemListDlg::OnSelectParticleSystem(const CString& name)
{
	_selItem = name;
	_selectedListItemName = (LPCTSTR)name;
}
