#include "stdafx.h"
#include "ActiveEdit.h"
#include "Cmd_MoveObject.h"
#include "Cmd_RotateObject.h"
#include "Cmd_ScaleObject.h"

ActiveEdit_MoveObject::ActiveEdit_MoveObject(VisualObject* object)
: _visualObject(object)
{
	_startPosition = object->GetWorldPosition();
	_startOrientation = object->GetOrientation();
	
	_startTile = GameApp::getSingleton().GetGame()->
		GetTileFromPosition(_startPosition);
}

ActiveEdit_MoveObject::~ActiveEdit_MoveObject()
{
}

ICommandXPtr ActiveEdit_MoveObject::GetCommand()
{
	if(!_isDirty)
	{
		return 0;
	}

	// 将起始值传给Command对象以便Undo时正确
	Vector3 pos = _visualObject->GetWorldPosition();
	ITile* endTile = GameApp::getSingleton().GetGame()->
		GetTileFromPosition(pos);
	if(endTile != _startTile)
	{
		_startTile->RemoveGameObject(_visualObject);
		endTile->AddGameObject(_visualObject);
		_visualObject->GetNode()->_setDerivedPosition(pos);
	}

	CommandStack stk;
	stk.Push(OGRE_NEW Cmd_MoveObject(_visualObject, _startPosition, _startTile, endTile), false);
	stk.Push(OGRE_NEW Cmd_RotateObject(_visualObject, _startOrientation), false);
	return OGRE_NEW BatchCommand(stk);
}

void ActiveEdit_MoveObject::Execute(const Any& c)
{
	_isDirty = true;
	SActiveEditExecuteContex contex = any_cast<SActiveEditExecuteContex>(c);

	Vector3 pos = _visualObject->GetPosition();
	Vector3 camRight = contex.CurrentCamera->getRealRight();
	Vector3 camDir = contex.CurrentCamera->getRealDirection();

#if 0
	if(!MFCInputManager::getSingleton().IsKeyDown(VK_LCONTROL))
	{
		pos += contex.MouseMoveDeltaOnX * camRight * 0.2f;
		pos += contex.MouseMoveDeltaOnY * camDir * 0.2f;
	}
	else
	{
		pos.y += contex.MouseMoveDeltaOnY * 0.2f;
	}
#else // 利用XYZ轴移动
	Quaternion objOrientation = _visualObject->GetOrientation();
	Vector3 dirX, dirY, dirZ;
	objOrientation.ToAxes(dirX, dirY, dirZ);
	if(contex.ManipulateFlag == EA_X)
	{
		pos += dirX * 10.0f * contex.MouseMoveDeltaOnX;
		//_visualObject->GetNode()->translate(10.0f * contex.MouseMoveDeltaOnX,
		//	0, 0, Node::TS_LOCAL);
	}
	
	if(contex.ManipulateFlag == EA_Y)
	{
		pos += dirY * 10.0f * contex.MouseMoveDeltaOnY;
	}
	
	if(contex.ManipulateFlag == EA_Z)
	{
		pos += dirZ * 10.0f * contex.MouseMoveDeltaOnY;
	}


	if(contex.ManipulateFlag == EA_XZ)
	{
		if(contex.PickOnTerrainOK)
		{
			pos = contex.PickedPosition;
		}
		else
		{
			Vector3 d = (camRight * 10.0f * contex.MouseMoveDeltaOnX) + (camDir * 10.0f * contex.MouseMoveDeltaOnY);
			d.y = 0.0f;
			pos += d;
		}
	}

#endif

	EditorConfig& cfg = EditorConfig::getSingleton();
	// 自动贴地移动
	if(cfg.GetAutoClampTerrainOnMove() && (contex.ManipulateFlag != EA_Y))
	{
		pos.y = GameApp::getSingleton().GetGame()->GetTerrainHeight(pos);
	}

	if(cfg.GetAutoAlignTerrainNormal() && (contex.ManipulateFlag != EA_Y))
	{
		GeneralGame& game = *EditorApp::getSingleton().GetGame();
		Vector3 here, left, down;
		here.x = pos.x;
		here.y = game.GetTerrainHeight( pos.x, pos.z );
		here.z = pos.z;

		left.x = pos.x - 100;
		left.y = game.GetTerrainHeight( pos.x - 100, pos.z );
		left.z = pos.z;

		down.x = pos.x;
		down.y = game.GetTerrainHeight( pos.x, pos.z + 100 );
		down.z = pos.z + 100;

		left = left - here;
		down = down - here;

		left.normalise();
		down.normalise();

		Vector3 normal  = left.crossProduct( down );
		normal.normalise();
		Vector3 up = Vector3::UNIT_Y;
		Quaternion q = up.getRotationTo(normal);
		_visualObject->SetOrientation(q);
	}

	if(cfg.GetSnapToGrid())
	{
		const Vector2& gridSz = cfg.GetSnapGridSize();
		pos.x = ((int)pos.x / (int)gridSz.x) * (int)gridSz.x;
		pos.z = ((int)pos.z / (int)gridSz.y) * (int)gridSz.y;
	}

	_visualObject->GetNode()->_setDerivedPosition(pos);
	
	EventsManager::getSingleton().CreateEvent(
		PropertyEvents::PropertyChanged, Any(String("Position")));

	Root::getSingleton().renderOneFrame();
}

//------------------------------------------------------------------------

ICommandXPtr ActiveEdit_ScaleObject::GetCommand()
{
	if(!_isDirty)
	{
		return 0;
	}

	return OGRE_NEW Cmd_ScaleObject(_visualObject, _startScale);
}

void ActiveEdit_ScaleObject::Execute(const Any& c)
{
	SActiveEditExecuteContex contex = any_cast<SActiveEditExecuteContex>(c);

	_isDirty = true;

	Vector3 scale = _visualObject->GetScale();
	scale += Vector3(contex.MouseMoveDeltaOnX/* - contex.MouseMoveDeltaOnY*/ * 0.05f);
	_visualObject->SetScale(scale);

	EventsManager::getSingleton().CreateEvent(
		PropertyEvents::PropertyChanged, Any(String("Scale")));

	Root::getSingleton().renderOneFrame();
}

//------------------------------------------------------------------------

ICommandXPtr ActiveEdit_RotateObject::GetCommand()
{
	if(!_isDirty)
	{
		return 0;
	}

	return OGRE_NEW Cmd_RotateObject(_visualObject, _startOrientation);
}

void ActiveEdit_RotateObject::Execute(const Any& c)
{
	_isDirty = true;

	SActiveEditExecuteContex contex = any_cast<SActiveEditExecuteContex>(c);

	const float RotatResolution = 360.0f; // 16 directions
	Quaternion q = _visualObject->GetOrientation();
	Vector3 ax = Vector3::UNIT_Y;
	Vector3 camRight = contex.CurrentCamera->getRealRight();
	Vector3 camDir = contex.CurrentCamera->getRealDirection();

	if(contex.ManipulateFlag == EA_X)
	{
		ax = Vector3::UNIT_Y; // yaw
	}

	if(contex.ManipulateFlag == EA_Y)
	{
		ax = Vector3::UNIT_X; // pitch
	}

	if(contex.ManipulateFlag == EA_Z)
	{
		ax = Vector3::UNIT_Z; // roll
	}

	Vector3 delta = camRight * contex.MouseMoveDeltaOnX;
	q = q * Quaternion(Radian(delta.x * Math::TWO_PI / RotatResolution), ax);
	_visualObject->SetOrientation(q);

	EventsManager::getSingleton().CreateEvent(
		PropertyEvents::PropertyChanged, Any(String("Orientation")));

	Root::getSingleton().renderOneFrame();
}
