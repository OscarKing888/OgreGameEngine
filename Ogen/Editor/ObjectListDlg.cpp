// ObjectListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectListDlg.h"

IMPLEMENT_DYNAMIC(ObjectListDlg, CDialogEx)

template<> ObjectListDlg* Singleton<ObjectListDlg>::ms_Singleton = 0;

ObjectListDlg::ObjectListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ObjectListDlg::IDD, pParent)
{
	Create(IDD);

	HICON hicon = ::AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hicon, TRUE);
	SetIcon(hicon, FALSE);

	//ShowWindow(SW_SHOW);
}

ObjectListDlg::~ObjectListDlg()
{
}

void ObjectListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_OBJECTS, _objectTree);
}


BEGIN_MESSAGE_MAP(ObjectListDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_OBJECTS, &ObjectListDlg::OnClickObjectName)

END_MESSAGE_MAP()

BOOL ObjectListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

void ObjectListDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
	//CDialog::OnClose();
}

void ObjectListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	const int C_Space = 4;
	if(_objectTree.GetSafeHwnd())
	{
		_objectTree.MoveWindow(C_Space, C_Space, cx - C_Space * 2, cy - C_Space * 2);
	}
}

void ObjectListDlg::OnClickObjectName(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;

	HTREEITEM child = _objectTree.GetChildItem(hItem);
	HTREEITEM parent = _objectTree.GetParentItem(hItem);
	HTREEITEM root = _objectTree.GetRootItem();
	
	CString name = _objectTree.GetItemText(hItem);

	if((child == NULL) /*&& (parent != root)*/)
	{
		CString objName = _objectTree.GetItemText(hItem);

		String val = (LPCTSTR)objName;
		IGameObject* obj = GameApp::getSingleton().GetGame()->GetGameObject(val);
		//EditState_Select::getSingleton().SetSelectObject(obj);
		Camera* cam = EditorApp::getSingleton().GetActiveCamera();
		VisualObject* vo = dynamic_cast<VisualObject*>(obj);
		if(vo)
		{
			AxisAlignedBox aabb = vo->GetNode()->_getWorldAABB();
			cam->setPosition(aabb.getCenter() + Vector3(1.5f, 0.2f, 1.5f) * aabb.getHalfSize().length());
			cam->lookAt(aabb.getCenter());
		}
		Root::getSingleton().renderOneFrame();
	}
}

void ObjectListDlg::RefreshObjectList()
{
	ClearList();
	IGame* game = GameApp::getSingleton().GetGame();

	// Global object
	HTREEITEM hglobal = _objectTree.InsertItem("GlobalObject");	
	if(SceneEnvironment::getSingletonPtr())
	{
		_objectTree.InsertItem(SceneEnvironment::getSingleton()
			.GetGUID().toString().c_str(), hglobal);
	}

	if(SkyBox::getSingletonPtr())
	{
		_objectTree.InsertItem(SkyBox::getSingleton()
			.GetGUID().toString().c_str(), hglobal);
	}

	if(MainLight::getSingletonPtr())
	{
		_objectTree.InsertItem(MainLight::getSingleton()
			.GetGUID().toString().c_str(), hglobal);
	}

	// Logic object
	HTREEITEM hlogic = _objectTree.InsertItem("LogicObject");
	const LogicObjectList& lst = LogicObjectManager::getSingleton().GetLoagicObjectList();
	LogicObjectList::const_iterator it = lst.begin();
	for(; it != lst.end(); ++it)
	{
		_objectTree.InsertItem((*it)->GetGUID().toString().c_str(), hlogic);
	}

	const TypedGameObjectMap& typeObjects = game->GetAllGameObjects();
	for (TypedGameObjectMap::const_iterator i = typeObjects.begin(); i != typeObjects.end(); ++i)
	{
		const GameObjectMap& objects = i->second;
		HTREEITEM hgrp = _objectTree.InsertItem(i->first.c_str());
		for (GameObjectMap::const_iterator j = objects.begin(); j != objects.end(); ++j)
		{
			HTREEITEM harc = _objectTree.InsertItem(j->first.toString().c_str(), hgrp);
		}
	}

	_objectTree.Expand(hlogic, TVE_EXPAND);
}

void ObjectListDlg::ClearList()
{
	_objectTree.DeleteAllItems();
}

void ObjectListDlg::Show()
{
	RefreshObjectList();
	ShowWindow(SW_RESTORE);
}

void ObjectListDlg::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	OGRE_DELETE this;
}