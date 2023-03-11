// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************

#pragma once
#include "VisualObject.h"
#include "WireBox.h"

class Trigger;

struct _EngineExport STriggerResult
{
	SceneQueryResult RawResult;
	Trigger* TriggerObject;
	GameObjectList TouchedObjects;
};

class _EngineExport ITriggerEventHandler
{
public:

	virtual void OnTouch(Trigger* trigger, IGameObject* object);
	virtual void OnStay(Trigger* trigger, IGameObject* object);
	virtual void OnLeave(Trigger* trigger, IGameObject* object);
};

//--------------------------------------------------------------------------------
///
///  AABB Trigger 
///
class _EngineExport Trigger : public VisualObject
{
public:

	static const String C_TypeName;

	Trigger(void);
	virtual ~Trigger(void);

	AxisAlignedBox GetBoundingBox() const;

	void SetSize(const Vector3& sz);
	const Vector3& GetSize() const;

	void SetFlag(const String& val){ _flag = val; }
	const String& GetFlag() const { return _flag; }

	virtual const String& GetTypeName() const;

	STriggerResult CheckCollision(ITriggerEventHandler* handler);

	void SetShowCollisionBox(bool on);
	bool GetShowCollisionBox() const;

	typedef map<Trigger*, Trigger*>::type TriggerMap;
	static TriggerMap	S_Triggers;

protected:

	WireBox*		_aabbBox;
	Vector3		_size;
	String		_flag;
	bool				_showCollisionBox;

	AxisAlignedBoxSceneQuery* _query;

	typedef map<IGameObject*, IGameObject*>::type ObjectTriggerStateMap;
	ObjectTriggerStateMap	_triggerStateMap;


	virtual void BuildPropertyMap();

	virtual void DestroyEntityImp();
	virtual void CreateEntityImp();

	virtual void PreCreateEntity();
	virtual void PostCreateEntity();

	virtual void PreDestroyEntity();
	virtual void PostDestroyEntity();

	virtual void PostCreateNode();
	virtual void PreDestroyNode();

private:

	void DestroyEntity();
	void CreateEntity();
};

