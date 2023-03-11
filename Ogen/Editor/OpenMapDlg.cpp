// OpenMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Editor.h"
#include "OpenMapDlg.h"
#include "EditorGameApp.h"

// OpenMapDlg dialog

IMPLEMENT_DYNAMIC(OpenMapDlg, CDialog)

OpenMapDlg::OpenMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_OPENMAP, pParent)
{

}

OpenMapDlg::~OpenMapDlg()
{
}

void OpenMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MAPLIST, _lstCtrl);
}


BEGIN_MESSAGE_MAP(OpenMapDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MAPLIST, &OpenMapDlg::OnDBClickTree)
END_MESSAGE_MAP()


// OpenMapDlg message handlers

BOOL OpenMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString sceneDir = EditorGame::getSingleton().GetSceneDir().c_str();

	CFileFind finder;
	//finder.MatchesMask(FILE_ATTRIBUTE_DIRECTORY);
	BOOL r = finder.FindFile(sceneDir + "*");
	int i = 0;
	while(r)
	{
		r = finder.FindNextFile();
		if(r && !finder.IsDots() && finder.IsDirectory())
		{
			CString fileName = finder.GetFileName();
			_lstCtrl.InsertItem(fileName);
			++i;
		}

	}

	return TRUE;
}

const CString& OpenMapDlg::GetMapName() const
{
	return _mapName;
}

void OpenMapDlg::OnOK()
{
	HTREEITEM h = _lstCtrl.GetSelectedItem();
	_mapName = _lstCtrl.GetItemText(h);

	CDialog::OnOK();
}

void OpenMapDlg::OnDBClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	OnOK();
}