#include "stdafx.h"
#include "Resource.h"
#include <Engine/OgreLib.h>
#include "EditState_TerrainHeight.h"
#include "TerrainBrush.h"
#include "Cmd_TerrainHeight.h"

#include "EditHeightState_DynamicElevate.h"
#include "EditHeightState_FixedHeight.h"
#include "EditHeightState_Smooth.h"

int EditStateID_TerrainHeight = GetAutoEditStateID();

UINT SubEditHeightID_ElevUp = GetAutoWMID();
UINT SubEditHeightID_Smooth = GetAutoWMID();
UINT SubEditHeightID_FixedHeight = GetAutoWMID();
UINT SubEditHeightID_ElevDown = GetAutoWMID();

UINT SubEditHeightID_FixedFlat = GetAutoWMID();

template<> EditState_TerrainHeight* Singleton<EditState_TerrainHeight>::ms_Singleton = 0;
template<> HeightEditType* Singleton<HeightEditType>::ms_Singleton = 0;
template<> EditState_SubEditHFMsgHandler* Singleton<EditState_SubEditHFMsgHandler>::ms_Singleton = 0;

static HeightEditType _HeightEditTypeInstance;

IMPLEMENT_DYNAMIC(EditState_SubEditHFMsgHandler, CCmdTarget)
BEGIN_MESSAGE_MAP(EditState_SubEditHFMsgHandler, CCmdTarget)

	ON_COMMAND_RANGE(SubEditHeightID_ElevUp, SubEditHeightID_ElevDown, &EditState_SubEditHFMsgHandler::OnActiveSubEditState)
	ON_UPDATE_COMMAND_UI_RANGE(SubEditHeightID_ElevUp, SubEditHeightID_ElevDown, &EditState_SubEditHFMsgHandler::OnUpdateSubEditState)

END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
HeightEditType::HeightEditType()
{		
	M_AddEnumToDescriptorEx(EHET_DynamicElevate, Elevate);
	M_AddEnumToDescriptorEx(EHET_Smooth, Smooth);
	M_AddEnumToDescriptorEx(EHET_FixedHeight, FixedHeight);

	AbstractEnumDescriptor::Init();
}
//--------------------------------------------------------------------------------
HeightEditType::~HeightEditType()
{

}
//--------------------------------------------------------------------------------
void EditState_TerrainHeight::BuildPropertyMap()
{
	AbstractTerrainEditState::BuildPropertyMap();

	//AddEnumProp(HeightEditType, EHeightEditType, HeightEditType, EditState_TerrainHeight, "HeightEditType", "TerrainBrush");
	AddProp(Property_Float, FixedHeight, EditState_TerrainHeight, "FixedHeight", "TerrainBrush");
	AddRangeFloatProp(Strength, 10, 1000, EditState_TerrainHeight, "Strength for elevation", "TerrainBrush");
}
//--------------------------------------------------------------------------------
EditState_TerrainHeight::EditState_TerrainHeight(IStateManager* pMgr)
: AbstractTerrainEditState(pMgr)
, _heightEditType(EHET_DynamicElevate)
, _fixedHeight(0.0f)
, _strength(10.0f)
, _isUpElev(true)
{
	_stateID = "Terrain Height";
	_commandID = EditStateID_TerrainHeight;
	_brushRadius = 600.0f;

	BuildPropertyMap();

	String name = "__Terrain_HF_Brush__";
	CreateBrushModel(name);

	///
	/// Add height edit sub states here
	///
	_stateManager.AddState(OGRE_NEW EditHeightState_DynamicElevate(&_stateManager));
	_stateManager.AddState(OGRE_NEW EditHeightState_Smooth(&_stateManager));
	_stateManager.AddState(OGRE_NEW EditHeightState_FixedHeight(&_stateManager));

	// active default sub state
	_stateManager.ActiveState((int)EHET_DynamicElevate);

	new EditState_SubEditHFMsgHandler(this);

}
//--------------------------------------------------------------------------------
EditState_TerrainHeight::~EditState_TerrainHeight()
{
	delete EditState_SubEditHFMsgHandler::getSingletonPtr();
}
//--------------------------------------------------------------------------------
BOOL EditState_TerrainHeight::OnLButtonDown(UINT nFlags, CPoint point)
{
	AbstractTerrainEditState::OnLButtonDown(nFlags, point);

	GetActiveState()->OnLButtonDown(nFlags, point);
		
	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL EditState_TerrainHeight::OnRButtonDown(UINT nFlags, CPoint point)
{
	AbstractTerrainEditState::OnRButtonDown(nFlags, point);

	GetActiveState()->OnRButtonDown(nFlags, point);

	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL EditState_TerrainHeight::OnMouseMove(UINT nFlags, CPoint point)
{	
	if(AbstractTerrainEditState::OnMouseMove(nFlags, point))
	{
	}
	else if(IsActiveEditing())
	{
// 		SActiveEditExecuteContex contex;
// 		contex.BrushDownPoint = _brushDownPoint;
// 		//contex.PickOnTerrainOK = PickOnTerrain(point.x, point.y, contex.PickedPosition);
// 		contex.MouseMoveDeltaOnX = point.x - _lastMouseDownPos.x;
// 		contex.MouseMoveDeltaOnY = -(point.y - _lastMouseDownPos.y);
// 		//ExecuteActiveEdit(contex);
// 		UpdateBrush(contex.PickedPosition);
		//Root::getSingleton().renderOneFrame();

	}
	else
	{
		GetActiveState()->OnMouseMove(nFlags, point);
	}	

	if(IsActiveEditing())
	{
		_lastMouseDownPos = point;
	}

	return FALSE;
}
//--------------------------------------------------------------------------------
void EditState_TerrainHeight::OnActive()
{
	AbstractTerrainEditState::OnActive();
}
//--------------------------------------------------------------------------------
void EditState_TerrainHeight::OnInActive()
{
	AbstractTerrainEditState::OnInActive();
}
//--------------------------------------------------------------------------------
BOOL EditState_TerrainHeight::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(!AbstractTerrainEditState::OnKeyDown(nChar, nRepCnt, nFlags))
	{
		return GetActiveState()->OnKeyDown(nChar, nRepCnt, nFlags);
	}

	return FALSE;
}
//--------------------------------------------------------------------------------
void EditState_TerrainHeight::SetHeightEditType(EHeightEditType e)
{
	_heightEditType = e;
	_stateManager.ActiveState((int)e);
}
//--------------------------------------------------------------------------------
AbstractEditHeightState* EditState_TerrainHeight::GetActiveState()
{
	IStatePtr s = _stateManager.GetActiveState();
	AbstractEditHeightStatePtr as = AbstractEditHeightStatePtr::DynamicCast(s);
	return as.GetPointer();
}
//--------------------------------------------------------------------------------
void EditState_TerrainHeight::CreateToolbar( CMFCRibbonCategory* mainCategory )
{
	CMFCRibbonPanel* panel = MFCGetRibbonPanel(
		_UIDisplayString("ID_TERRAIN_HF_EDIT_STATE").c_str(), mainCategory);

	if(!panel)	
	{
		panel = mainCategory->AddPanel(
			_UIDisplayString("ID_TERRAIN_HF_EDIT_STATE").c_str());
	}

	HICON hicon = 0;//AfxGetApp()->LoadIcon(IDI_ICON_TERRAIN);
	//CMFCRibbonButton* btn = new CMFCRibbonButton(
	//	GetID(), _UIDisplayString("ID_TERRAIN_HF_EDIT_STATE").c_str(), hicon,  TRUE, hicon, TRUE, TRUE);
	//panel->Add(btn);

	
	hicon = AfxGetApp()->LoadIcon(IDI_ICON_ELEV_UP);
	CMFCRibbonButton* btn = new CMFCRibbonButton(
		SubEditHeightID_ElevUp, _UIDisplayString("ID_TERRAIN_ELEVATE_UP").c_str(), hicon, TRUE, hicon);
	panel->Add(btn);

	hicon = AfxGetApp()->LoadIcon(IDI_ICON_ELEV_DOWN);
	btn = new CMFCRibbonButton(
		SubEditHeightID_ElevDown, _UIDisplayString("ID_TERRAIN_ELEVATE_DOWN").c_str(), hicon, TRUE, hicon);
	panel->Add(btn);

	hicon = AfxGetApp()->LoadIcon(IDI_ICON_ELEV_SMOOTH);
	btn = new CMFCRibbonButton(
		SubEditHeightID_Smooth, _UIDisplayString("ID_TERRAIN_SMOOTH").c_str(), hicon, TRUE, hicon);
	panel->Add(btn);

	hicon = AfxGetApp()->LoadIcon(IDI_ICON_ELEV_FIXED);
	btn = new CMFCRibbonButton(
		SubEditHeightID_FixedHeight, _UIDisplayString("ID_TERRAIN_FIXED").c_str(), hicon, TRUE, hicon);
	panel->Add(btn);

// 	hicon = AfxGetApp()->LoadIcon(IDI_ICON_ELEV_FLAT);
// 	btn = new CMFCRibbonButton(
// 		SubEditHeightID_FixedFlat, _UIDisplayString("IDI_ICON_ELEV_FLAT").c_str(), hicon, TRUE, hicon);
// 	editStatePanel->Add(btn);
// 
}
//--------------------------------------------------------------------------------
// const String& EditState_TerrainHeight::GetTypeName() const
// {
// 	return _name;
// }
//------------------------------------------------------------------------
BOOL EditState_TerrainHeight::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	if(MFCInputManager::getSingleton().IsKeyDown(VK_SHIFT))
	{
		_strength += zDelta;

		EventsManager::getSingleton().CreateEvent(
			PropertyEvents::PropertyChanged, Any(String("Strength")));
	}

	return AbstractTerrainEditState::OnMouseWheel(nFlags, zDelta, pt);
}
//------------------------------------------------------------------------
BOOL EditState_TerrainHeight::OnLButtonUp( UINT nFlags, CPoint point )
{
	GetActiveState()->OnLButtonUp(nFlags, point);

	return AbstractTerrainEditState::OnLButtonUp(nFlags, point);
}
//------------------------------------------------------------------------
// AbstractEditHeightState
//------------------------------------------------------------------------
AbstractEditHeightState::AbstractEditHeightState(IStateManager* pMgr)
: IState(pMgr)
, _isPainting(false)
{

}
//--------------------------------------------------------------------------------
AbstractEditHeightState::~AbstractEditHeightState()
{

}
//--------------------------------------------------------------------------------
BOOL AbstractEditHeightState::OnLButtonDown(UINT nFlags, CPoint point)
{
	_isPainting = true;

	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditHeightState::OnLButtonUp(UINT nFlags, CPoint point)
{
	_isPainting = false;

	EditorApp::getSingleton().GetCommandHistory().BeginBatch();
	CommitHFChange();
	EditorApp::getSingleton().GetCommandHistory().EndBatch();

	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditHeightState::OnRButtonDown(UINT nFlags, CPoint point)
{
	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditHeightState::OnMouseMove(UINT nFlags, CPoint point)
{
	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditHeightState::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return FALSE;
}
//--------------------------------------------------------------------------------
void AbstractEditHeightState::AddOldHF( Terrain* terrain, long x, long y, float val )
{
	TerrainHFData::iterator i = _oldData.find(terrain);
	if (i == _oldData.end())
	{
		i = _oldData.insert(std::make_pair(terrain, TerrainHFMap())).first;
	}

	TerrainHFMap& xMap = i->second;
	TerrainHFMap::iterator ix = xMap.find(x);
	if(ix == xMap.end())
	{
		ix = xMap.insert(std::make_pair(x, FloatMap())).first;
	}

	FloatMap& yMap = ix->second;
	FloatMap::iterator iy = yMap.find(y);
	if(iy == yMap.end())
	{
		yMap.insert(std::make_pair(y, val));
	}
}
//--------------------------------------------------------------------------------
void AbstractEditHeightState::CommitHFChange()
{
	TerrainHFData::iterator i = _oldData.begin();
	for (; i != _oldData.end(); ++i)
	{
		EditorApp::getSingleton().GetCommandHistory()
			.AddCommand(OGRE_NEW Cmd_TerrainHeight(
			i->first, i->second), false);

		i->first->update();
	}

	_oldData.clear();
}
//--------------------------------------------------------------------------------
// EditState_SubEditHFMsgHandler
//--------------------------------------------------------------------------------
EditState_SubEditHFMsgHandler::EditState_SubEditHFMsgHandler( EditState_TerrainHeight* state )
: _state(state)
{
	EditorApp::getSingleton().RegisterWMHandler(this);
}
//--------------------------------------------------------------------------------
EditState_SubEditHFMsgHandler::~EditState_SubEditHFMsgHandler()
{
	EditorApp::getSingleton().UnRegisterWMHandler(this);
}
//--------------------------------------------------------------------------------
void EditState_SubEditHFMsgHandler::OnActiveSubEditState( UINT id )
{
	int activeID = EditorApp::getSingleton().GetActiveStateCommandID();

	if(activeID != EditStateID_TerrainHeight)
	{
		EditorApp::getSingleton().ActiveState(EditStateID_TerrainHeight);
	}

	if(id == SubEditHeightID_ElevDown)
	{
		_state->SetElevUp(false);
		_state->SetHeightEditType(EHET_DynamicElevate);
	}
	else
	{
		if(id == SubEditHeightID_ElevUp)
		{
			_state->SetElevUp(true);
		}

		EHeightEditType ht = (EHeightEditType)(id - SubEditHeightID_ElevUp);
		_state->SetHeightEditType(ht);
	}
}
//--------------------------------------------------------------------------------
void EditState_SubEditHFMsgHandler::OnUpdateSubEditState( CCmdUI* ui )
{
	int activeID = EditorApp::getSingleton().GetActiveStateCommandID();

	if(activeID != EditStateID_TerrainHeight)
	{
		ui->SetCheck(0);
		return;
	}

	if((ui->m_nID == SubEditHeightID_ElevDown))		
	{
		if(_state->GetHeightEditType() == EHET_DynamicElevate)
		{
			ui->SetCheck(_state->IsElevUp() ? 0 : 1);
		}
		else
		{
			ui->SetCheck(0);
		}
	}
	else if(ui->m_nID == SubEditHeightID_ElevUp)
	{
		if(_state->GetHeightEditType() == EHET_DynamicElevate)
		{
			ui->SetCheck(_state->IsElevUp() ? 1 : 0);
		}
		else
		{
			ui->SetCheck(0);
		}
	}
	else
	{
		EHeightEditType ht = (EHeightEditType)(ui->m_nID - SubEditHeightID_ElevUp);
		ui->SetCheck(ht == _state->GetHeightEditType());
	}
}
//--------------------------------------------------------------------------------