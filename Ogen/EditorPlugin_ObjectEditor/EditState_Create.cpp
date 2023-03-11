#include "stdafx.h"
#include "EditState_Create.h"
#include "Cmd_CreateObject.h"
#include "EditState_Select.h"
#include "Axes.h"

#include "Resource.h"

int EditStateID_Create = GetAutoEditStateID();


EditState_Create::EditState_Create(IStateManager* pMgr)
: AbstractEditState(pMgr)
{
	_stateID = "Create Object";
	_commandID = EditStateID_Create;

	EventsManager::getSingleton().AddListener(this);
}

EditState_Create::~EditState_Create()
{
	EventsManager::getSingleton().RemoveListener(this);
}

BOOL EditState_Create::OnLButtonDown(UINT nFlags, CPoint point)
{
	AbstractEditState::OnLButtonDown(nFlags, point);

	GeneralGame& game = *EditorApp::getSingleton().GetGame();
	String className = _UIKeyString(_createClassName);
	if(className.empty())
	{
		AfxMessageBox("未选择需要创建的游戏对象类别,请在游戏游戏对象列表中选择一个后再创建!", MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	Vector3 pos = Vector3::ZERO;
	TerrainGroup::RayResult rr = EditorApp::getSingleton().PickOnTerrain(point.x, point.y);
	if(!rr.hit)
	{
		Vector3 camPos = GetActiveCamera()->getPosition();

		float tx = (float)point.x / GetActiveCamera()->getViewport()->getActualWidth();
		float ty = (float)point.y / GetActiveCamera()->getViewport()->getActualHeight();

		Ray ray = GetActiveCamera()->getCameraToViewportRay(tx, ty);

		Vector3 pickDir = ray.getDirection();
		pos = camPos + pickDir * 200.0f;
	}
	else
	{
		pos = rr.position;
	}

	//String id = EditorGame::GenerateObjectID(className);

	XGUID guid = XGUID::createGUID();
	IGameObject* obj = game.CreateGameObject(className, guid, false);
	VisualObject* vo = dynamic_cast<VisualObject*>(obj);
	ITile* tile = game.GetTileFromPosition(pos);
	if(vo)
	{
		vo->SetPosition(pos - tile->GetRootSceneNode()->getPosition());
	}

	AddCommand(OGRE_NEW Cmd_AddObject(obj, tile));

	EditState_Select::getSingleton().SetSelectObject(obj);
	//ObjectPropertiesWnd::getSingleton().SetObject(obj);
	LogSysIDf("Editor", "Create object[%s] [%s]", className.c_str(), guid.toString().c_str());

	// 对于模型物体
	if(obj->HasProperty("MeshFile"))
	{
		String meshFile = (LPCTSTR)ResPanel_Mesh::getSingleton().GetSelectedListItemText();
		if(!meshFile.empty())
		{
			obj->SetProperty("MeshFile", meshFile);
		}
		else
		{
			ResPanel_Mesh::getSingleton().ShowWindow(SW_RESTORE);
		}
	}

	// 对于粒子系统
	if(obj->HasProperty("ParticleTemplateName"))
	{
		String fileName = (LPCTSTR)ResPanel_ParticleSystem::getSingleton().GetSelectedListItemText();
		if(!fileName.empty())
		{
			obj->SetProperty("ParticleTemplateName", fileName);
		}
		else
		{
			ResPanel_ParticleSystem::getSingleton().ShowWindow(SW_RESTORE);
		}
	}

	if(!MFCInputManager::getSingleton().IsKeyDown(VK_LCONTROL))
	{
		EditorApp::getSingleton().ActiveState(EditStateID_Select);
	}

	return TRUE;
}

BOOL EditState_Create::OnRButtonDown(UINT nFlags, CPoint point)
{
	AbstractEditState::OnRButtonDown(nFlags, point);
// 	EditorGame& game = EditorGame::getSingleton();
// 	game.GetStateManager().ActiveState(EES_Select);

	return TRUE;
}

BOOL EditState_Create::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if((nChar == 'q') || (nChar == 'Q'))
	{
		EditorApp::getSingleton().ActiveState(EditStateID_Select);
	}

	return AbstractEditState::OnKeyDown(nChar, nRepCnt, nFlags);
}

void EditState_Create::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(eventName == "CreateObject")
	{
		_createClassName = (LPCTSTR)any_cast<CString>(eventContex);
		stopBroadCast = true;
	}
}

void EditState_Create::CreateToolbar( CMFCRibbonCategory* mainCategory )
{
	CMFCRibbonPanel* panel = MFCGetRibbonPanel(
	_UIDisplayString("ID_OBJECT_EDIT_STATE").c_str(), mainCategory);

	if(!panel)	
	{
		panel = mainCategory->AddPanel(
			_UIDisplayString("ID_OBJECT_EDIT_STATE").c_str());
	}

	HICON hicon = AfxGetApp()->LoadIcon(IDI_ICON_CREATE);
	CMFCRibbonButton* btn = new CMFCRibbonButton(GetCommandID(),
		_UIDisplayString("IDS_EDITCREATE").c_str(), hicon,  TRUE, hicon, TRUE, TRUE);
	panel->Add(btn);
}

void EditState_Create::OnActive()
{
	AbstractEditState::OnActive();
	EventsManager::getSingleton().CreateEvent("EnableRiseCamera", Any(true));
}

void EditState_Create::OnInActive()
{
	AbstractEditState::OnInActive();
	EventsManager::getSingleton().CreateEvent("EnableRiseCamera", Any(false));
}