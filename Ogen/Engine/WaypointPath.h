// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-12-03
// *************************************************************************************

#pragma once

#include "VisualObject.h"
#include "DynamicLines.h"
#include "TransformedMesh.h"

struct _EngineExport Waypoint
{
	Vector3 Position;
	Vector3 YawPitchRoll;

	Waypoint()
		: Position(Vector3::ZERO)
		, YawPitchRoll(Vector3::ZERO)
	{
	}

	Waypoint(const Vector3& position,
		const Vector3& yawPitchRoll = Vector3::ZERO)
		: Position(position)
		, YawPitchRoll(yawPitchRoll)
	{
	}
};

typedef std::vector<Waypoint> WaypointList;

class _EngineExport WaypointPath : public VisualObject
{
public:

	WaypointPath(void);
	virtual ~WaypointPath(void);

	virtual const String& GetTypeName() const;

	const WaypointList& GetWaypointList() const { return _waypointList; }
	void SetWaypointList(const WaypointList& lst);

	size_t GetWaypointCount() const { return _waypointList.size(); }
	const Waypoint& GetWaypoint(size_t idx) const;

	void UpdateWaypoint(size_t idx, const Waypoint& wp);

	virtual void SetPosition(const Vector3& pos){}
	virtual void SetScale(const Vector3& scale){}
	virtual void SetOrientation(const Quaternion& orient){}

protected:

	virtual void BuildPropertyMap();
	virtual void PostCreateNode();
	virtual void PreDestroyNode();

	void CreateWaypoints();
	void DestroyWaypoints();

	virtual void OnEditorSelect();
	virtual void OnEditorDeselect();

	DynamicLines _line;
	WaypointList _waypointList;

	typedef std::vector<TransformedMesh*> MeshList;
	MeshList _nodeEntityList;

	Vector3List GetPositionList() const;
};

