// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-10-27
// *************************************************************************************

#pragma once

#include "VisualObject.h"

class IActorController;
class ActorComponent;

/**
*	抽象角色类实现
*	@para 主要提供简单的动画混合和过度以及装备道具功能
*/
class _EngineExport Actor
	: public VisualObject
	, public IUpdateAble
{
public:

	Actor();
	virtual ~Actor(void);

	virtual const String& GetTypeName() const;

	virtual void BuildPropertyMap();
	virtual void Destroy();

// 	virtual void AttatchGameObject(const XGUID& guid, const String& attatchPointName);
// 	virtual void AttatchGameObject(const XGUID& guid, const String& attatchPointName,
// 		const Vector3& offsetPos, const Quaternion& offsetOrientation);
// 	virtual void DetachGameObject(const XGUID& guid);

	virtual void SetActorController(IActorController* ctrl);
	virtual IActorController* GetActorController() const;

	void SetActiveActorController(IGameObject* ctrl);
	IGameObject* GetActiveActorController() const;

	const Ogre::String& GetControllerClassTypeName() const;
	void SetControllerClassTypeName(const Ogre::String& val);

	const Ogre::NameValuePairList& GetControllerCreateParams() const;
	void SetControllerCreateParams(const Ogre::NameValuePairList& val);

	virtual void Update(float deltaTime);

	size_t GetComponentsCount() const;
	ActorComponent* GetComponent(size_t idx) const;
	ActorComponent* GetComponent(const Ogre::String& name) const;
	void AddComponent(ActorComponent* comp);
	void RemoveComponent(ActorComponent* comp);
	void RemoveComponent(const Ogre::String& name);
	void UpdateComponents();
	void UpdateComponents(float deltaTime);

	virtual void PostSceneLoad();

protected:

	IActorController*		_actorController;
	Ogre::String			_controllerClassTypeName;
	Ogre::NameValuePairList _controllerCreateParams;

	typedef Ogre::vector<ActorComponent*>::type ActorComponentList;
	ActorComponentList _components;
};


