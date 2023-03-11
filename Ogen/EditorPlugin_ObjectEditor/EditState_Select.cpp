#include "stdafx.h"
#include "EditState_Select.h"
#include "ActiveEdit.h"
#include "Cmd_CreateObject.h"
#include "resource.h"

int EditStateID_Select = GetAutoEditStateID();

UINT SubEditStateID_Move = GetAutoWMID();
UINT SubEditStateID_Rotate = GetAutoWMID();
UINT SubEditStateID_Scale = GetAutoWMID();

template<> EditState_Select* Singleton<EditState_Select>::ms_Singleton = 0;
template<> EditState_SelectMsgHandler* Singleton<EditState_SelectMsgHandler>::ms_Singleton = 0;

BEGIN_MESSAGE_MAP(EditState_SelectMsgHandler, CCmdTarget)
	// Standard printing commands
	ON_COMMAND(SubEditStateID_Move, &EditState_SelectMsgHandler::OnMove)
	ON_COMMAND(SubEditStateID_Rotate, &EditState_SelectMsgHandler::OnRotate)
	ON_COMMAND(SubEditStateID_Scale, &EditState_SelectMsgHandler::OnScale)

	ON_UPDATE_COMMAND_UI(SubEditStateID_Move, &EditState_SelectMsgHandler::OnUpdateMoveUI)
	ON_UPDATE_COMMAND_UI(SubEditStateID_Rotate, &EditState_SelectMsgHandler::OnUpdateRotateUI)
	ON_UPDATE_COMMAND_UI(SubEditStateID_Scale, &EditState_SelectMsgHandler::OnUpdateScaleUI)

END_MESSAGE_MAP()
//--------------------------------------------------------------------------------
EditState_Select::EditState_Select(IStateManager* pMgr)
: AbstractEditState(pMgr)
, _selectedObject(0)
, _subEditState(ESES_Move)
, _selectedAxes(EA_Null)
, _axesMove(0)
, _axesRotate(0)
, _axesScale(0)
{
	_stateID = "Select Object";
	_commandID = EditStateID_Select;

	_axesMove = OGRE_NEW Axes_Move();
	_axesMove->SetDescription("__Axes_Move__");
	_axesMove->SetGUID(XGUID::createGUID());

 	_axesRotate = OGRE_NEW Axes_Rotate();
 	_axesRotate->SetDescription("__Axes_Rotate__");
	_axesRotate->SetGUID(XGUID::createGUID());
 
 	_axesScale = OGRE_NEW Axes_Scale();
 	_axesScale->SetDescription("__Axes_Scale__");
	_axesScale->SetGUID(XGUID::createGUID());

	SceneNode* sn = GameApp::getSingleton().GetGame()->GetVisualScene()->getRootSceneNode();
	sn->addChild(_axesMove->GetNode());
	sn->addChild(_axesRotate->GetNode());
	sn->addChild(_axesScale->GetNode());

	new EditState_SelectMsgHandler(this);
	EventsManager::getSingleton().AddListener(this);
}
//--------------------------------------------------------------------------------
EditState_Select::~EditState_Select()
{
	EventsManager::getSingleton().RemoveListener(this);
	delete EditState_SelectMsgHandler::getSingletonPtr();
}
//--------------------------------------------------------------------------------
BOOL EditState_Select::OnLButtonDown(UINT nFlags, CPoint point)
{
	AbstractEditState::OnLButtonDown(nFlags, point);
	static int m=0;

	SPickResult r = Pick(point.x, point.y);
	if(r.GameObject)
	{
		SetSelectObject(r.GameObject);
	}
	else if(r.AxesPicked == EA_Null)
	{
		// 如果没有选中轴就清空
		if(r.IsPickedOnTerrain)
		{
			// TERRAIN:
#if OCT_TERRAIN
			SetSelectObject(Terrain::getSingletonPtr());
#endif
		}
		else
		{
			SetSelectObject(SceneEnvironment::getSingletonPtr());
			/*if (m == 0) SetSelectObject(SceneEnvironment::getSingletonPtr());
			if (m == 1) SetSelectObject(MSky::getSingletonPtr());
			if( m < 1)
				m++;
			else
				m=0;*/
		}
	}

	_selectedAxes = r.AxesPicked;

	return TRUE;
}
//--------------------------------------------------------------------------------
void EditState_Select::SetSelectObject(IGameObject* obj)
{
	if(_selectedObject != obj)
	{
		_selectedObject = dynamic_cast<VisualObject*>(obj);
		ObjectPropertiesWnd::getSingleton().SetObject(obj);
		UpdateEditAxes();
	}

	if(_selectedObject)
	{
		// active the object edit state
		ObjectEditStateManager::getSingleton().ActiveObjectEditState(_selectedObject);
	}

	EventsManager::getSingleton().CreateEvent("PickObject", Any(AnyGameObject(_selectedObject)));
	EditorApp::getSingleton().SetSelectObject(_selectedObject);
}
//--------------------------------------------------------------------------------
void EditState_Select::UpdateEditAxes()
{
	try
	{
		VisualObject* vo = dynamic_cast<VisualObject*>(_selectedObject);
		if(vo != 0)
		{
			if(_axesMove)
			{
				_axesMove->SetVisible(_subEditState == ESES_Move);
				_axesMove->SetPosition(vo->GetWorldPosition());
				//_axesMove->SetOrientation(vo->GetWorldOrientation());
			}

			if(_axesRotate)
			{
				_axesRotate->SetVisible(_subEditState == ESES_Rotate);
				_axesRotate->SetPosition(vo->GetWorldPosition());
			}

			if(_axesScale)
			{
				_axesScale->SetVisible(_subEditState == ESES_Scale);
				_axesScale->SetPosition(vo->GetWorldPosition());
				//_axesScale->SetOrientation(vo->GetWorldOrientation());
			}
		}
		else
		{
			if(_axesMove)
			{
				_axesMove->SetVisible(false);
			}

			if(_axesRotate)
			{
				_axesRotate->SetVisible(false);
			}

			if(_axesScale)
			{
				_axesScale->SetVisible(false);
			}
		}

		if(_axesMove)
		{
			Camera* cam = GetActiveCamera();
			Vector3 camPos = cam->getPosition();
			Vector3 axesPos = _axesMove->GetPosition();
			Vector3 d = axesPos - camPos;
			float l = d.length();
			l *= 0.003f;
			_axesMove->SetScale(l, l, l);
			_axesRotate->SetScale(l, l, l);
			_axesScale->SetScale(l, l, l);
		}
	}
	catch(const std::exception& ex)
	{
		LogErrorIDf("Editor", "[EditState_Select::UpdateEditAxes] error:[%s]", ex.what());
	}
	catch(...)
	{
		LogErrorIDf("Editor", "[EditState_Select::UpdateEditAxes] unknown error!");
	}
}
//--------------------------------------------------------------------------------
SPickResult EditState_Select::Pick(int x, int y)
{
	SPickResult r;
	if((_axesMove && _axesMove->GetVisible())
		|| (_axesRotate && _axesRotate->GetVisible())
		|| (_axesScale && _axesScale->GetVisible()))
	{
		r.AxesPicked = PickAxes(x, y);
		if(r.AxesPicked != EA_Null)
		{
			return r;
		}
	}

	r = EditorApp::getSingleton().PickGameObjects(x, y, ~C_PickMask);
	if((r.GameObject != 0) && (r.GameObject == _selectedObject))
	{
		r.AxesPicked = EA_XZ;
	}	

	TerrainGroup::RayResult tr = EditorApp::getSingleton().PickOnTerrain(x, y);
	r.IsPickedOnTerrain = tr.hit;
	r.TerrainIntersectPoint = tr.position;

	return r;
}
//--------------------------------------------------------------------------------
EAxes EditState_Select::PickAxes(int x, int y)
{
	SPickGameObjectResult r =
		EditorApp::getSingleton().PickGameObjects(x, y, GetQueryFlag());

	if(!r.RawResult.empty())
	{
		RaySceneQueryResultEntry re = r.RawResult[0];
		if(re.worldFragment) // 不处理地形
		{
			if(r.RawResult.size() > 1)
			{
				re = r.RawResult[1];
			}
		}

		// 第一个Pick到的物体应该是轴
		if(re.movable)
		{
			return GetPickAxes(re.movable);
		}
	}

	return EA_Null;
}
//--------------------------------------------------------------------------------
BOOL EditState_Select::OnMouseMove(UINT nFlags, CPoint point)
{	
	Camera* cam = GetActiveCamera();

	float dx = (float)(point.x - _lastMouseDownPos.x);
	float dy = (float)(point.y - _lastMouseDownPos.y);
	if((dx == 0.0f) && (dy == 0.0f))
	{
		return FALSE;
	}

	if((_lastMouseDownPos.x == 0) && (_lastMouseDownPos.y == 0))
	{
		_lastMouseDownPos = point;
		return FALSE;
	}

	if(IsObjectSelected() && IsLMBDown())
	{
		// manipulate object
		if(!IsActiveEditing())
		{
			switch (_subEditState)
			{
			case ESES_Move:
				{
					BeginActiveEdit(OGRE_NEW ActiveEdit_MoveObject(_selectedObject));
					break;
				}

			case ESES_Rotate:
				{
					BeginActiveEdit(OGRE_NEW ActiveEdit_RotateObject(_selectedObject));
					break;
				}

			case ESES_Scale:
				{
					BeginActiveEdit(OGRE_NEW ActiveEdit_ScaleObject(_selectedObject));
					break;
				}

			}
		}
		else
		{
			SActiveEditExecuteContex contex;
			contex.MouseMoveDeltaOnX = dx;
			contex.MouseMoveDeltaOnY = -dy;
			contex.CurrentCamera = cam;
			contex.ManipulateFlag = _selectedAxes;
			TerrainGroup::RayResult r = EditorApp::getSingleton().PickOnTerrain(point.x, point.y);
			contex.PickOnTerrainOK = r.hit;
			contex.PickedPosition = r.position;
			ExecuteActiveEdit(Any(contex));
			UpdateEditAxes();
		}
	}

	// 检查并设置鼠标
	if(cam && !IsLMBDown())
	{
		SPickResult r = Pick(point.x, point.y);
		_selectedAxes = r.AxesPicked;
		if(r.GameObject)
		{
			SetCursor(ECT_Select);
		}
		else
		{
			if(r.AxesPicked != EA_Null)
			{
				switch (_subEditState)
				{
				case ESES_Move:
					{
						SetCursor(ECT_Move);

						if(_selectedAxes != EA_Null)
						{
							switch(_selectedAxes)
							{
							case EA_X:
								SetCursor(ECT_MoveX);
								break;

							case EA_Y:
								SetCursor(ECT_MoveY);
								break;

							case EA_Z:
								SetCursor(ECT_MoveZ);
								break;
							} // switch
						}
					}
					break;

				case ESES_Rotate:
					{
						SetCursor(ECT_Rotate);
						if(_selectedAxes != EA_Null)
						{
							switch(_selectedAxes)
							{
							case EA_X:
								SetCursor(ECT_RotateX);
								break;

							case EA_Y:
								SetCursor(ECT_RotateY);
								break;

							case EA_Z:
								SetCursor(ECT_RotateZ);
								break;
							} // switch
						}
					}
					break;
				case ESES_Scale:
					SetCursor(ECT_Scale);
					break;
				}
			}
			else
			{
				SetCursor(ECT_Normal);
			}
		}
	}

	AbstractEditState::OnMouseMove(nFlags, point);
	UpdateEditAxes();

	return TRUE;
}
//--------------------------------------------------------------------------------
BOOL EditState_Select::OnRButtonDown(UINT nFlags, CPoint point)
{
	AbstractEditState::OnRButtonDown(nFlags, point);
	//SetSelectObject(0);
	return TRUE;
}
//--------------------------------------------------------------------------------
void EditState_Select::SetSubEditState(ESubEditState se)
{
	if(_subEditState == se)
	{
		return;
	}

	CommitActiveEdit();
	_subEditState = se;
	ActiveState(GetID());
	UpdateEditAxes();
}
//--------------------------------------------------------------------------------
BOOL EditState_Select::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	AbstractEditState::OnKeyDown(nChar, nRepCnt, nFlags);

	if(IsObjectSelected() && (nChar == VK_DELETE))
	{
		CommitActiveEdit();

		ITile* tile = GameApp::getSingleton().GetGame()->GetTileFromPosition(
			_selectedObject->GetWorldPosition());
		AddCommand(OGRE_NEW Cmd_RemoveObject(_selectedObject, tile));
		SetSelectObject(0);
		return TRUE;
	}

	switch(nChar)
	{
	case 'Q':
	case 'q':
		{
			SetSubEditState(ESES_Move);
			return TRUE;
			break;
		}

	case 'E':
	case 'e':
		{

			SetSubEditState(ESES_Rotate);
			return TRUE;
			break;
		}

	case 'R':
	case 'r':
		{

			SetSubEditState(ESES_Scale);
			return TRUE;
			break;
		}
	}

	return FALSE;
}
//--------------------------------------------------------------------------------
void EditState_Select::OnActive()
{
	AbstractEditState::OnActive();
	UpdateEditAxes();

	EventsManager::getSingleton().CreateEvent("EnableRiseCamera", Any(true));
}
//--------------------------------------------------------------------------------
void EditState_Select::OnInActive()
{
	AbstractEditState::OnInActive();

	if(_axesMove)
	{
		_axesMove->SetVisible(false);
	}

	if(_axesRotate)
	{
		_axesRotate->SetVisible(false);
	}

	if(_axesScale)
	{
		_axesScale->SetVisible(false);
	}

	EventsManager::getSingleton().CreateEvent("EnableRiseCamera", Any(false));
}
//--------------------------------------------------------------------------------
uint32 EditState_Select::GetQueryFlag()
{
	return C_PickMask;
/*
	switch (_subEditState)
	{
	case ESES_Move:
		return GetQueryMask<Axes_Move>();
	case ESES_Rotate:
		return GetQueryMask<Axes_Rotate>();
	case ESES_Scale:
		return GetQueryMask<Axes_Scale>();
	}

	return GetQueryMask<Axes_Move>();*/
}
//--------------------------------------------------------------------------------
EAxes EditState_Select::GetPickAxes(MovableObject* obj)
{
	switch (_subEditState)
	{
	case ESES_Move:
		return _axesMove->GetSubAxes(obj);
	case ESES_Rotate:
		return _axesRotate->GetSubAxes(obj);
	case ESES_Scale:
		return _axesScale->GetSubAxes(obj);
	}
	
	return EA_Null;
}
//--------------------------------------------------------------------------------
void EditState_Select::UpdateShowBoundingBox()
{
	if(_axesMove)
	{
		_axesMove->SetShowBoundingBox(false);
	}

	if(_axesRotate)
	{
		_axesRotate->SetShowBoundingBox(false);
	}

	if(_axesScale)
	{
		_axesScale->SetShowBoundingBox(false);
	}
}
//--------------------------------------------------------------------------------
BOOL EditState_Select::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UpdateEditAxes();
	BOOL r = AbstractEditState::OnMouseWheel(nFlags, zDelta, pt);
	Root::getSingleton().renderOneFrame();
	return r;
}
//--------------------------------------------------------------------------------
void EditState_Select::SetCursor( ECursorType c )
{
	struct SCursorPair
	{
		ECursorType CursorType;
		UINT ResID;
		HCURSOR hcr;
	};

	static SCursorPair cursors[] = 
	{
		{ECT_Normal,		OCR_NORMAL, 0},
		{ECT_Select,		IDC_CUR_EDIT_SELECT, 0},
		{ECT_Move,			IDC_CUR_EDIT_MOVE, 0},
		{ECT_MoveX,			IDC_CUR_EDIT_MOVE_X, 0},
		{ECT_MoveY,			IDC_CUR_EDIT_MOVE_Y, 0},
		{ECT_MoveZ,			IDC_CUR_EDIT_MOVE_Z, 0},
		{ECT_Rotate,		IDC_CUR_EDIT_ROTATE, 0},
		{ECT_RotateX,		IDC_CUR_EDIT_ROTATE_X, 0},
		{ECT_RotateY,		IDC_CUR_EDIT_ROTATE_Y, 0},
		{ECT_RotateZ,		IDC_CUR_EDIT_ROTATE_Z, 0},
		{ECT_Scale,			IDC_CURSOR_EDIT_SCALE, 0}
	};

	if(cursors[0].hcr == 0)
	{
		int sz = sizeof(cursors) / sizeof(SCursorPair);


		HCURSOR hcur = AfxGetApp()->LoadOEMCursor(OCR_NORMAL);
		cursors[ECT_Normal].hcr = hcur;

		for (int i = 1; i < sz; ++i)
		{
			// 加载鼠标
			cursors[i].hcr = AfxGetApp()->LoadCursor(cursors[i].ResID);
		}
	}

	EditorApp::getSingleton().SetCursor(cursors[c].hcr);
}
//--------------------------------------------------------------------------------
void EditState_Select::CreateToolbar( CMFCRibbonCategory* mainCategory )
{
	CMFCRibbonPanel* panel = MFCGetRibbonPanel(
		_UIDisplayString("ID_OBJECT_EDIT_STATE").c_str(), mainCategory);

	if(!panel)	
	{
		panel = mainCategory->AddPanel(
			_UIDisplayString("ID_OBJECT_EDIT_STATE").c_str());
	}

	CMFCRibbonButton* btn = new CMFCRibbonButton(GetCommandID(),
		_UIDisplayString("IDS_EDITSELECT").c_str(),
		AfxGetApp()->LoadIcon(IDI_ICON_SELECT));
	panel->Add(btn);

	panel->Add(new CMFCRibbonSeparator());

	btn = new CMFCRibbonButton(SubEditStateID_Move, _UIDisplayString("IDS_MOVESTATE").c_str(),
		AfxGetApp()->LoadIcon(IDI_ICON_MOVE));
	panel->Add(btn);

	btn = new CMFCRibbonButton(SubEditStateID_Rotate, _UIDisplayString("IDS_ROTATESTATE").c_str(),
		AfxGetApp()->LoadIcon(IDI_ICON_ROTATE));
	panel->Add(btn);

	btn = new CMFCRibbonButton(SubEditStateID_Scale, _UIDisplayString("IDS_SCALESTATE").c_str(),
		AfxGetApp()->LoadIcon(IDI_ICON_SCALE));
	panel->Add(btn);
}
//--------------------------------------------------------------------------------
void EditState_Select::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if (eventName == "UpdateGizmo")
	{
		UpdateEditAxes();
	}
}
//--------------------------------------------------------------------------------
// EditState_SelectMsgHandler
//--------------------------------------------------------------------------------
EditState_SelectMsgHandler::EditState_SelectMsgHandler( EditState_Select* state )
: _state(state)
{
	EditorApp::getSingleton().RegisterWMHandler(this);
}
//--------------------------------------------------------------------------------
EditState_SelectMsgHandler::~EditState_SelectMsgHandler()
{
	EditorApp::getSingleton().UnRegisterWMHandler(this);
}
//--------------------------------------------------------------------------------
void EditState_SelectMsgHandler::OnMove()
{
	int activeID = EditorApp::getSingleton().GetActiveStateCommandID();

	if(activeID != EditStateID_Select)
	{
		EditorApp::getSingleton().ActiveState(EditStateID_Select);
	}

	_state->SetSubEditState(ESES_Move);
}
//--------------------------------------------------------------------------------
void EditState_SelectMsgHandler::OnRotate()
{
	int activeID = EditorApp::getSingleton().GetActiveStateCommandID();

	if(activeID != EditStateID_Select)
	{
		EditorApp::getSingleton().ActiveState(EditStateID_Select);
	}

	_state->SetSubEditState(ESES_Rotate);
}
//--------------------------------------------------------------------------------
void EditState_SelectMsgHandler::OnScale()
{
	int activeID = EditorApp::getSingleton().GetActiveStateCommandID();

	if(activeID != EditStateID_Select)
	{
		EditorApp::getSingleton().ActiveState(EditStateID_Select);
	}

	_state->SetSubEditState(ESES_Scale);
}
//--------------------------------------------------------------------------------
void EditState_SelectMsgHandler::OnUpdateMoveUI(CCmdUI* ui)
{
	int activeID = EditorApp::getSingleton().GetActiveStateCommandID();

	if(activeID != EditStateID_Select)
	{
		ui->SetCheck(0);
		return;
	}

	ui->SetCheck(_state->GetSubEditState() == ESES_Move ? 1 : 0);
}
//--------------------------------------------------------------------------------
void EditState_SelectMsgHandler::OnUpdateRotateUI(CCmdUI* ui)
{
	int activeID = EditorApp::getSingleton().GetActiveStateCommandID();

	if(activeID != EditStateID_Select)
	{
		ui->SetCheck(0);
		return;
	}

	ui->SetCheck(_state->GetSubEditState() == ESES_Rotate ? 1 : 0);
}
//--------------------------------------------------------------------------------
void EditState_SelectMsgHandler::OnUpdateScaleUI(CCmdUI* ui)
{
	int activeID = EditorApp::getSingleton().GetActiveStateCommandID();

	if(activeID != EditStateID_Select)
	{
		ui->SetCheck(0);
		return;
	}

	ui->SetCheck(_state->GetSubEditState() == ESES_Scale ? 1 : 0);
}