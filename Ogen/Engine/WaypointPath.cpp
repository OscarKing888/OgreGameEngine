// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-12-03
// *************************************************************************************
#include "StdAfx.h"
#include "WaypointPath.h"
#include "GameApp.h"
#include "GameObjectFactoryManager.h"

class WaypointPathFactory : public TGameObjectFactory<WaypointPath>
{
};

M_REG_FACTORY(WaypointPath, WaypointPathFactory, Scene);

//--------------------------------------------------------------------------------
// WaypointPath
//--------------------------------------------------------------------------------
WaypointPath::WaypointPath(void)
{
	_isEditorDislpayOnly = true;

	WaypointList lst;
	lst.push_back(Waypoint(Vector3(0, 32767 + 100, 0)));
	lst.push_back(Waypoint(Vector3(100, 32767 + 0, 100), Vector3(0, 45, 0)));
	lst.push_back(Waypoint(Vector3(200, 32767 + 300, 0), Vector3(0, 0, 45)));
	lst.push_back(Waypoint(Vector3(300, 32767 + 100, 100), Vector3(45, 45, 0)));
	SetWaypointList(lst);
}
//--------------------------------------------------------------------------------
WaypointPath::~WaypointPath(void)
{
}
//--------------------------------------------------------------------------------
void WaypointPath::BuildPropertyMap()
{
	VisualObject::BuildPropertyMap();
	RemoveProperty("Yaw");
	RemoveProperty("Pitch");
	RemoveProperty("Roll");
	RemoveProperty("Scale");
	RemoveProperty("Position");

	//AddProp(Property_Vector3List, WaypointPositionList, WaypointPath, "WaypointPositionList", "WaypointPath");
	//AddProp(Property_Vector3List, WaypointYawPitchRollList, WaypointPath, "WaypointYawPitchRollList", "WaypointPath");
}
//--------------------------------------------------------------------------------
const String& WaypointPath::GetTypeName() const
{
	static String sName("WaypointPath");
	return sName;
}
//--------------------------------------------------------------------------------
void WaypointPath::PostCreateNode()
{
	VisualObject::PostCreateNode();
	CreateWaypoints();
}
//--------------------------------------------------------------------------------
void WaypointPath::PreDestroyNode()
{
	VisualObject::PreDestroyNode();
	DestroyWaypoints();
}
//--------------------------------------------------------------------------------
void WaypointPath::CreateWaypoints()
{
	if(!_node)
	{
		return;
	}

	if((GameApp::CurrentRunType == ERT_Editor) || _displayEditorObjectInGame)
	{
		_line.setPoint(GetPositionList());
		_line.MovableObject::setUserAny(Any(AnyGameObject(this)));
		_line.setUserAny(Any(AnyGameObject(this)));
		_line.setUseParentNodeTransform(false);

		_node->attachObject(&_line);
		_line.setQueryFlags(GetQueryFlag());

		for (size_t i = 0; i < _waypointList.size(); ++i)
		{
			const Waypoint& wp = _waypointList[i];
			String name = GetGUID().toString()
				+ "_Nodes_" + StringConverter::toString(i);

			MeshPtr mesh = MeshManager::getSingleton().load("axes.mesh",
				ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

			TransformedMesh* ent = OGRE_NEW TransformedMesh(name, mesh);
			_node->attachObject(ent);
			ent->setPosition(wp.Position);
			ent->setYawPitchRoll(wp.YawPitchRoll);
			ent->setUserAny(Any(AnyGameObject(this)));

			ent->setQueryFlags(GetQueryFlag());

			_nodeEntityList.push_back(ent);
		}
	}
}
//--------------------------------------------------------------------------------
void WaypointPath::DestroyWaypoints()
{
	if(!_node)
	{
		return;
	}

	if((GameApp::CurrentRunType == ERT_Editor) || _displayEditorObjectInGame)
	{
		for(size_t i = 0; i < _nodeEntityList.size(); ++i)
		{
			TransformedMesh* ent = _nodeEntityList[i];
			OGRE_DELETE ent;
		}

		_nodeEntityList.clear();
	}
}
//--------------------------------------------------------------------------------
void WaypointPath::OnEditorSelect()
{

}
//--------------------------------------------------------------------------------
void WaypointPath::OnEditorDeselect()
{

}
//--------------------------------------------------------------------------------
void WaypointPath::SetWaypointList(const WaypointList& lst)
{
	DestroyWaypoints();
	_waypointList = lst;
	CreateWaypoints();
}
//--------------------------------------------------------------------------------
const Waypoint& WaypointPath::GetWaypoint(size_t idx) const
{
	assert(idx < _waypointList.size());
	return _waypointList[idx];
}
//--------------------------------------------------------------------------------
Vector3List WaypointPath::GetPositionList() const
{
	Vector3List vlst;
	vlst.resize(_waypointList.size());
	for (size_t i = 0; i < _waypointList.size(); ++i)
	{
		vlst[i] = _waypointList[i].Position;
	}

	return vlst;
}
//--------------------------------------------------------------------------------
void WaypointPath::UpdateWaypoint(size_t idx, const Waypoint& wp)
{
	if(idx < _waypointList.size())
	{
		_line.setPoint(idx, wp.Position);
		_line.update();

		TransformedMesh* tm = _nodeEntityList[idx];
		tm->setPosition(wp.Position);
		tm->setYawPitchRoll(wp.YawPitchRoll);
		_waypointList[idx] = wp;
	}
}

