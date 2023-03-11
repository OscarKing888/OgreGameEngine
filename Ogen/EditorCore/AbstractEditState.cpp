#include "stdafx.h"
#include "AbstractEditState.h"
#include "EditorApp.h"

String ObjectEditStateManager::GetEditStateID(IGameObject* obj) const
{
	ObjectEditStateMap::const_iterator it = _objEditStateMap.find(obj->GetTypeName());
	if(it != _objEditStateMap.end())
	{
		return it->second;
	}

	return "";
}
//--------------------------------------------------------------------------------
void ObjectEditStateManager::AddEditState(const String& objectTypeName, String editState)
{
	_objEditStateMap.insert(std::make_pair(objectTypeName, editState));
}
//--------------------------------------------------------------------------------
void ObjectEditStateManager::ActiveObjectEditState(IGameObject* obj)
{
	String stateID = GetEditStateID(obj);
	if(!stateID.empty())
	{
		EditorApp::getSingleton().ActiveState(stateID);
	}
}
//--------------------------------------------------------------------------------
// AbstractEditState
//--------------------------------------------------------------------------------
AbstractEditState::AbstractEditState(IStateManager* pMgr)
: IState(pMgr)
, _isLMBDown(false)
, _isMMBDown(false)
, _isRMBDown(false)
{
}

AbstractEditState::~AbstractEditState()
{
}
//--------------------------------------------------------------------------------
bool AbstractEditState::IsShiftDown() const
{
	return MFCInputManager::getSingleton().IsShiftDown();
}
//--------------------------------------------------------------------------------
bool AbstractEditState::IsCtrlDown() const
{
	return MFCInputManager::getSingleton().IsCtrlDown();
}
//--------------------------------------------------------------------------------
void AbstractEditState::OnActive()
{
	MFCInputManager::getSingleton().AddMouseListener(this);
	MFCInputManager::getSingleton().AddKeyListener(this);
}
//--------------------------------------------------------------------------------
void AbstractEditState::OnInActive()
{
	MFCInputManager::getSingleton().RemoveMouseListener(this);
	MFCInputManager::getSingleton().RemoveKeyListener(this);
}
//--------------------------------------------------------------------------------
Camera* AbstractEditState::GetActiveCamera()
{
	return EditorApp::getSingleton().GetActiveCamera();
}
//--------------------------------------------------------------------------------
void AbstractEditState::AddCommand(ICommandXPtr cmd, bool callExec)
{
	if(cmd != 0)
	{
		CommandHistory& cmdH = EditorApp::getSingleton().GetCommandHistory();
		cmdH.AddCommand(cmd, callExec);
	}
}
//--------------------------------------------------------------------------------
void AbstractEditState::BeginActiveEdit(IActiveEditPtr activeEdit)
{
	_activeEdit = activeEdit;
}
//--------------------------------------------------------------------------------
void AbstractEditState::CommitActiveEdit()
{
	if(_activeEdit)
	{
		AddCommand(_activeEdit->GetCommand(), false);
		_activeEdit = 0;
	}
}
//--------------------------------------------------------------------------------
bool AbstractEditState::IsActiveEditing() const
{
	return _activeEdit != 0;
}
//--------------------------------------------------------------------------------
void AbstractEditState::ExecuteActiveEdit(const Any& contex)
{
	if(_activeEdit)
	{
		_activeEdit->Execute(contex);
	}
	else
	{
		assert(false && "[AbstractEditState::ExecuteActiveEdit] Active edit is NULL!");
	}
}
//--------------------------------------------------------------------------------
BOOL AbstractEditState::OnLButtonDown(UINT nFlags, CPoint point)
{
	CommitActiveEdit();
	_lastMouseDownPos = point;
	_isLMBDown = true;

	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditState::OnLButtonUp(UINT nFlags, CPoint point)
{
	_isLMBDown = false;
	CommitActiveEdit();
	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditState::OnMButtonDown(UINT nFlags, CPoint point)
{
	_isMMBDown = true;
	_lastMouseDownPos = point;
	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditState::OnMButtonUp(UINT nFlags, CPoint point)
{
	_isMMBDown = false;
	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditState::OnMouseMove(UINT nFlags, CPoint point)
{	
	Camera* cam = GetActiveCamera();

	float dx = (float)(point.x - _lastMouseDownPos.x);
	float dy = (float)(point.y - _lastMouseDownPos.y);
	if((dx == 0.0f) && (dy == 0.0f))
	{
		Root::getSingleton().renderOneFrame();
		return FALSE;
	}

	if((_lastMouseDownPos.x == 0) && (_lastMouseDownPos.y == 0))
	{
		_lastMouseDownPos = point;
		Root::getSingleton().renderOneFrame();
		return FALSE;
	}

	EditorConfig& cfg = EditorConfig::getSingleton();
	ECameraSpeed camSpd = cfg.GetCameraMoveSpeed();

	if(IsMMBDown())
	{
		Vector3 right = cam->getRealRight();
		right.normalise();
		Vector3 d = Vector3::ZERO;
		// 左右平移
		//if(MFCInputManager::getSingleton().IsKeyDown(VK_LSHIFT))
		if(camSpd == eCS_Slow)
		{
			d = -right * dx * 0.5f;
		}
		else if(camSpd == eCS_Normal)
			//else if(MFCInputManager::getSingleton().IsKeyDown(VK_LCONTROL))
		{
			d = -right * dx * 5.0f;
		}
		else
		{
			d = -right * dx * 50.0f;
		}

		d.y = 0.0f;
		cam->move(d);

		// 上下平移
		Vector3 camDir = cam->getRealUp();
		camDir.normalise();

		//if(MFCInputManager::getSingleton().IsKeyDown(VK_LSHIFT))
		if(camSpd == eCS_Slow)
		{
			d = camDir * -dy * 0.5f;
		}
		else if(camSpd == eCS_Normal)
			//else if(MFCInputManager::getSingleton().IsKeyDown(VK_LCONTROL))
		{
			d = camDir * -dy * 5.0f;
		}
		else
		{
			d = camDir * -dy * 50.0f;
		}

		cam->move(-d);

		CheckLoadTerrain(cam->getPosition());
	}
	else if(IsRMBDown() && (cam->getProjectionType() == PT_PERSPECTIVE))
	{
		// 左右旋转相机
		Radian dxr(-dx * 0.005f);
		Radian dyr(-dy * 0.005f);

		cam->yaw(dxr);
		cam->pitch(dyr);
	}

	if(IsLMBDown() || IsRMBDown() || IsMMBDown())
	{
		//::SetCursorPos(_lastMouseDownPos.x, _lastMouseDownPos.y);
	}

	_lastMouseDownPos = point;

#ifndef _DEBUG
	Root::getSingleton().renderOneFrame();
#endif

	return TRUE;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditState::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta != 0)
	{
		int wheelCnt = zDelta / 120;

		EditorConfig& cfg = EditorConfig::getSingleton();
		ECameraSpeed camSpd = cfg.GetCameraMoveSpeed();

		// 推拉
		Vector3 dir = GetActiveCamera()->getRealDirection().normalisedCopy();
		//if(MFCInputManager::getSingleton().IsKeyDown(VK_LSHIFT))
		if(camSpd == eCS_Slow)
		{
			dir *= wheelCnt * 10.0f;
		}
		else if(camSpd == eCS_Normal)
			//else if(MFCInputManager::getSingleton().IsKeyDown(VK_LCONTROL))
		{
			dir *= wheelCnt * 100.0f;
		}
		else
		{
			dir *= wheelCnt * 1000.0f;
		}

		Camera* cam = GetActiveCamera();
		if(cam->getProjectionType() == PT_ORTHOGRAPHIC)
		{
			float orthoHeight = cam->getOrthoWindowHeight();
			orthoHeight -= wheelCnt * 1000.0f;
			orthoHeight = std::max(orthoHeight, 2000.0f);
			cam->setOrthoWindowHeight(orthoHeight);
			//LogDebugf("Wheel delta %d %f", wheelCnt, orthoHeight);
		}
		else
		{
			cam->move(dir);
		}

		CheckLoadTerrain(cam->getPosition());
	}

	return TRUE;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditState::OnRButtonDown(UINT nFlags, CPoint point)
{
	_lastMouseDownPos = point;
	_isRMBDown = true;
	CommitActiveEdit();
	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditState::OnRButtonUp(UINT nFlags, CPoint point)
{
	_isRMBDown = false;
	CommitActiveEdit();
	return FALSE;
}
//--------------------------------------------------------------------------------
bool AbstractEditState::IsLMBDown() const
{
	return _isLMBDown;
}
//--------------------------------------------------------------------------------
bool AbstractEditState::IsRMBDown() const
{
	return _isRMBDown;
}
//--------------------------------------------------------------------------------
bool AbstractEditState::IsMMBDown() const
{
	return _isMMBDown;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditState::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//if(nChar == VK_SHIFT)
	//{
	//	_lastCameraSpeed = EditorConfig::getSingleton().CameraMoveSpeed;
	//	EditorConfig::getSingleton().CameraMoveSpeed = eCS_Slow;
	//}
	//else if(nChar == VK_CONTROL)
	//{
	//	_lastCameraSpeed = EditorConfig::getSingleton().CameraMoveSpeed;
	//	EditorConfig::getSingleton().CameraMoveSpeed = eCS_Fast;
	//}

	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL AbstractEditState::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
// 	EditorConfig::getSingleton().CameraMoveSpeed = _lastCameraSpeed;

	return FALSE;
}
//--------------------------------------------------------------------------------
void AbstractEditState::BeginBatchEdit()
{	
	CommandHistory& cmdH = EditorApp::getSingleton().GetCommandHistory();
	cmdH.BeginBatch();
}
//--------------------------------------------------------------------------------
void AbstractEditState::EndBatchEdit()
{	
	CommandHistory& cmdH = EditorApp::getSingleton().GetCommandHistory();
	cmdH.EndBatch();
}
//--------------------------------------------------------------------------------
bool AbstractEditState::IsBatchEditMode() const
{
	const CommandHistory& cmdH = EditorApp::getSingleton().GetCommandHistory();

	return cmdH.IsBatchMode();
}
//--------------------------------------------------------------------------------
void AbstractEditState::AddMemento(IMementoPtr m)
{
	EditorApp::getSingleton().AddMemento(m);
}
//--------------------------------------------------------------------------------
void AbstractEditState::RemoveAllMemento()
{
	EditorApp::getSingleton().RemoveAllMemeto();
}
//--------------------------------------------------------------------------------
void AbstractEditState::CheckLoadTerrain( const Vector3& pos )
{
	IGame* game = GameApp::getSingleton().GetGame();		
	ITile* t = game->GetTileFromPosition(pos);
	if(t && !t->IsLoaded())
	{
		long x = t->GetX();
		long z = t->GetZ();
		GameApp::getSingleton().GetGame()->LoadTileAndTerrain(x, z);
	}
}