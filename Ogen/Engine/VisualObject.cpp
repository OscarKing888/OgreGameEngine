// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************
#include "stdafx.h"
#include "VisualObject.h"
#include "GameApp.h"
#include "GameObjectFactoryManager.h"
#include "IGame.h"
#include "Property_Vector3.h"
#include "Property_Quaternion.h"
#include "Property_Bool.h"
#include "GameObjectFactoryManager.h"

AnyGameObject::AnyGameObject(IGameObject* obj)
: GameObject(obj)
{

}

uint32 VisualObject::DefaultVisibilityFlag = 0x00000001;

VisualObject::VisualObject(void)
: _node(0)
, _visible(true)
, _displayEditorObjectInGame(false)
, _isEditorDislpayOnly(false)
, _isShowBoundingBox(false)
, _visibilityFlag(VisualObject::DefaultVisibilityFlag)
, _yaw(0)
, _pitch(0)
, _roll(0)
{
}

VisualObject::~VisualObject(void)
{
}

void VisualObject::SetGUID(const XGUID& guid)
{
	if(guid != GetGUID())
	{
		//DestroyNode();
		AbstractGameObject::SetGUID(guid);

		if(!_node)
		{
			CreateNode();
		}
	}
}

void VisualObject::PostLoad()
{
	if(!_node)
	{
		CreateNode();
	}
}

String VisualObject::GetNodeName()
{
	return GetGUID().toString();// + ".Node";
}

void VisualObject::BuildPropertyMap()
{
	AbstractGameObject::BuildPropertyMap();

	AddPropEx1(Property_Vector3, Position, VisualObject, const Vector3&, "Position", "Transform");
	AddPropEx1(Property_Vector3, Scale, VisualObject, const Vector3&, "Scale", "Transform");
	//AddPropEx1(Property_Quaternion, Orientation, VisualObject, const Quaternion&, "", "Visual");
	AddProp(Property_Bool, Visible, VisualObject, "Visible", "Visual");
	AddProp(Property_Bool, DisplayEditorObjectInGame, VisualObject, "DisplayEditorObjectInGame", "Visual");
	AddReadonlyProp(Property_Bool, EditorDislpayOnly, VisualObject, "EditorDislpayOnly", "Visual");
	AddProp(Property_Bool, ShowBoundingBox, VisualObject, "ShowBoundingBox", "Visual");
	AddProp(Property_IntHex, VisibilityFlag, VisualObject, "VisibilityFlag", "Visual");

	AddPropEx1(Property_Radian, Yaw, VisualObject, const Radian&, "Yaw", "Transform");
	AddPropEx1(Property_Radian, Pitch, VisualObject, const Radian&, "Pitch", "Transform");
	AddPropEx1(Property_Radian, Roll, VisualObject, const Radian&, "Roll", "Transform");
}

void VisualObject::DestroyNode()
{
	if(_node == 0)
	{
		return;
	}

	PreDestroyNode();
	DestroyNodeImp();
	PostDestroyNode();
}

void VisualObject::CreateNode()
{
	PreCreateNode();
	CreateNodeImp();
	PostCreateNode();

	SetVisible(_visible);
	SetShowBoundingBox(_isShowBoundingBox);
}

void VisualObject::DestroyNodeImp()
{
	IGame* game = GameApp::getSingleton().GetGame();
	SceneManager* sm = game->GetVisualScene();
	sm->destroySceneNode(GetNodeName());
	_node = 0;
}

void VisualObject::CreateNodeImp()
{
	_visible = true;
	IGame* game = GameApp::getSingleton().GetGame();
	SceneManager* sm = game->GetVisualScene();
	_node = sm->createSceneNode(GetNodeName());
}


void VisualObject::PreCreateNode()
{
}

void VisualObject::PostCreateNode()
{

}

void VisualObject::PreDestroyNode()
{
	if(_node != 0)
	{
		_node->detachAllObjects();
	}
}

void VisualObject::PostDestroyNode()
{

}

void VisualObject::Destroy()
{
	AbstractGameObject::Destroy();
	DestroyNode();
}


const Vector3& VisualObject::GetPosition() const
{
	assert(_node != 0);
	return _node->getPosition();
}

void VisualObject::SetPosition(const Vector3& pos)
{
	assert(_node != 0);
	_node->setPosition(pos);
}

const Vector3& VisualObject::GetScale() const
{
	assert(_node != 0);
	return _node->getScale();
}

void VisualObject::SetScale(const Vector3& scale)
{
	assert(_node != 0);
	_node->setScale(scale);
}

const Quaternion& VisualObject::GetOrientation() const
{
	assert(_node != 0);
	return _node->getOrientation();
}

void VisualObject::SetOrientation(const Quaternion& orient)
{
	assert(_node != 0);
	_node->setOrientation(orient);

	Matrix3 mat;
	orient.ToRotationMatrix(mat);
	// yaw	pitch	roll
	// y	x		z
	// up	right	dir

	mat.ToEulerAnglesYXZ(_yaw, _pitch, _roll);
}

// void VisualObject::SetOrientation(float yawDegree, float pitchDegree, float rollDegree)

// 	Matrix3 mat;
// 	// yaw	pitch	roll
// 	// y	x		z
// 	// up	right	dir
// 	mat.FromEulerAnglesYXZ(Radian(Degree(yawDegree)),
// 		Radian(Degree(pitchDegree)),
// 		Radian(Degree(rollDegree)));
// 
// 	SetOrientation(Quaternion(mat));
// }

void VisualObject::SetVisible(bool on)
{
	_visible = on;

	/*bool realVisible = _visible;
	if(_isEditorDislpayOnly)
	{
		if(GameApp::CurrentRunType == ERT_Game)
		{
			if(_displayEditorObjectInGame)
			{
				realVisible = _visible;
			}
			else
			{
				realVisible = false;
			}
		}
	}*/

	_node->setVisible(_visible);
}

bool VisualObject::GetVisible() const
{
	return _visible;
}

void VisualObject::SetShowBoundingBox(bool on)
{
	if(_node)
	{
		_node->showBoundingBox(on);
	}

	_isShowBoundingBox = on;
}

bool VisualObject::GetShowBoundingBox() const
{
	return _isShowBoundingBox;
}

const Vector3& VisualObject::GetWorldPosition() const
{
	if(_node)
	{
		return _node->_getDerivedPosition();
	}

	return Vector3::ZERO;
}

const Vector3& VisualObject::GetWorldScale() const
{
	if(_node)
	{
		return _node->_getDerivedScale();
	}

	return Vector3::UNIT_SCALE;
}

const Quaternion& VisualObject::GetWorldOrientation() const
{
	if(_node)
	{
		return _node->_getDerivedOrientation();
	}

	return Quaternion::IDENTITY;
}

void VisualObject::SetVisibilityFlag(uint32 flag)
{
	_visibilityFlag = flag;
}

uint32 VisualObject::GetVisibilityFlag() const
{
	return _visibilityFlag;
}


uint32 VisualObject::GetQueryFlag()
{
	//return GameObjectFactoryManager::getSingleton().GetQueryFlag<StaticModel>();

	String classTypeName = getClassName(this);
	//LogDebugIDf("Pick", "VisualObject::GetQueryFlag for [%s]", classTypeName.c_str());

	return GameObjectFactoryManager::getSingleton().GetQueryFlag(classTypeName);
}

void VisualObject::SetYaw(const Radian& r)
{
	_yaw = r;

	UpdateRawPitchRoll();
}

void VisualObject::SetPitch(const Radian& r)
{
	_pitch = r;

	UpdateRawPitchRoll();
}

void VisualObject::SetRoll(const Radian& r)
{
	_roll = r;
	UpdateRawPitchRoll();
}

const Radian& VisualObject::GetYaw() const
{
	return _yaw;
}

const Radian& VisualObject::GetPitch() const
{
	return _pitch;
}

const Radian& VisualObject::GetRoll() const
{
	return _roll;
}

void VisualObject::UpdateRawPitchRoll()
{
	Matrix3 mat;
	// yaw	pitch	roll
	// y	x		z
	// up	right	dir
	mat.FromEulerAnglesYXZ(_yaw, _pitch, _roll);

	SetOrientation(Quaternion(mat));
}

void VisualObject::SetYaw(const Degree& d)
{
	SetYaw(Radian(d));
}

void VisualObject::SetPitch(const Degree& d)
{
	SetPitch(Radian(d));
}

void VisualObject::SetRoll(const Degree& d)
{
	SetRoll(Radian(d));
}

void VisualObject::Move(const Vector3& deltaPos)
{
	if(_node)
	{
		_node->setPosition(deltaPos + GetPosition());
	}
}

void VisualObject::Yaw(const Radian& angle)
{
	if(_node)
	{
		SetYaw(angle + GetYaw());
	}
}

void VisualObject::Pitch(const Radian& angle)
{
	if(_node)
	{
		SetPitch(angle + GetPitch());
	}
}

void VisualObject::Roll(const Radian& angle)
{
	if(_node)
	{
		SetRoll(angle + GetRoll());
	}
}

SceneManager* VisualObject::GetSceneManager()
{
	if(_node)
	{
		return _node->getCreator();
	}

	IGame* game = GameApp::getSingleton().GetGame();
	return game->GetVisualScene();
}

void VisualObject::OnAddToScene(SceneNode* rootNode)
{
	AbstractGameObject::OnAddToScene(rootNode);
	if(_node->getParentSceneNode())
	{
		_node->getParentSceneNode()->removeChild(_node);
	}

	rootNode->addChild(_node);
}

void VisualObject::OnRemoveFromScene(SceneNode* rootNode)
{
	AbstractGameObject::OnRemoveFromScene(rootNode);	
	rootNode->removeChild(_node);
}
//--------------------------------------------------------------------------------
void VisualObject::SetWorldPosition( const Vector3& pos )
{
	if(_node)
	{
		_node->_setDerivedPosition(pos);
	}
}
//--------------------------------------------------------------------------------
void VisualObject::SetWorldOrientation( const Quaternion& q )
{
	if(_node)
	{
		_node->_setDerivedOrientation(q);
	}
}