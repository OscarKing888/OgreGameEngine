// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-27
// *************************************************************************************

#include "StdAfx.h"
#include "Actor.h"
#include "GameObjectFactoryManager.h"
#include "GameApp.h"
#include "ActorComponent.h"
#include "IActorController.h"

//----------------------------------------------------------------------------------------------
class ActorFactory : public TGameObjectFactory<Actor>
{
};
//----------------------------------------------------------------------------------------------
M_REG_FACTORY(Actor, ActorFactory, Actor);
//----------------------------------------------------------------------------------------------
Actor::Actor()
: _actorController(0)
{
	GameApp::getSingleton().AddUpdateable(this);
}
//----------------------------------------------------------------------------------------------
void Actor::BuildPropertyMap()
{
	VisualObject::BuildPropertyMap();
	//AddReadonlyProp(Property_StringVector, AttatchPoints, Actor);
	//AddProp(Property_GameObject, ActiveActorController, Actor, "ActiveActorController", "Controller");
	AddProp(Property_String, ControllerClassTypeName, Actor, "Controller", "Controller");
}
//----------------------------------------------------------------------------------------------
Actor::~Actor(void)
{
}
////----------------------------------------------------------------------------------------------
//void Actor::AttatchGameObject(const XGUID& guid, const String& attatchPointName)
//{
//	AttatchGameObject(guid, attatchPointName, Vector3::ZERO, Quaternion::IDENTITY);
//}
////----------------------------------------------------------------------------------------------
//void Actor::AttatchGameObject(const XGUID& guid, const String& attatchPointName,
//									  const Vector3& offsetPos,
//									  const Quaternion& offsetOrientation)
//{
//	IGame* game = GameApp::getSingleton().GetGame();
//	IGameObject* obj = game->GetGameObject(guid);
//	if(!obj)
//	{
//		LogErrorIDf("GameLogic", "[Actor::AttatchGameObject] [%s] not exist!", guid.toString().c_str());
//		return;
//	}
//
//	StaticModel* model = dynamic_cast<StaticModel*>(obj);
//	if(model != 0)
//	{
//		AttatchObject(model->GetEntity(), attatchPointName, offsetPos, offsetOrientation);
//	}
//	else
//	{
//		LogErrorIDf("GameLogic",
//			"[Actor::AttatchGameObject] [%s] doesn't has entity data!", guid.toString().c_str());
//	}
//}
////----------------------------------------------------------------------------------------------
//void Actor::DetachGameObject(const XGUID& guid)
//{
//	IGame* game = GameApp::getSingleton().GetGame();
//	IGameObject* obj = game->GetGameObject(guid);
//	if(!obj)
//	{
//		LogErrorIDf("GameLogic", 
//			"[Actor::DetachGameObject] [%s] not exist!", guid.toString().c_str());
//		return;
//	}
//
//	StaticModel* model = dynamic_cast<StaticModel*>(obj);
//	if(model != 0)
//	{
//		DetachObject(model->GetEntity());
//	}
//	else
//	{
//		LogErrorIDf("GameLogic",
//			"[Actor::DetachGameObject] [%s] doesn't has entity data!", guid.toString().c_str());
//	}
//}
//----------------------------------------------------------------------------------------------
const String& Actor::GetTypeName() const
{
	static String sName("Actor");
	return sName;
}
//----------------------------------------------------------------------------------------------
void Actor::SetActorController(IActorController* ctrl)
{
	if(_actorController)
	{
		_actorController->OnActorDetach(this);
		_actorController->SetActor(0);
	}

	_actorController = ctrl;

	if(_actorController)
	{
		_actorController->OnActorAttach(this);
		_actorController->SetActor(this);
	}
}
//----------------------------------------------------------------------------------------------
IActorController* Actor::GetActorController() const
{
	return _actorController;
}
//----------------------------------------------------------------------------------------------
const Ogre::String& Actor::GetControllerClassTypeName() const
{
	return _controllerClassTypeName;
}
//----------------------------------------------------------------------------------------------
void Actor::SetControllerClassTypeName( const Ogre::String& val )
{
	_controllerClassTypeName = val;
}
//----------------------------------------------------------------------------------------------
size_t Actor::GetComponentsCount() const
{
	return _components.size();
}
//----------------------------------------------------------------------------------------------
ActorComponent* Actor::GetComponent( size_t idx ) const
{
	if(idx < _components.size())
	{
		return _components[idx];
	}

	return 0;
}
//----------------------------------------------------------------------------------------------
ActorComponent* Actor::GetComponent( const Ogre::String& name ) const
{
	ActorComponentList::const_iterator i = _components.begin();
	for (; i != _components.end(); ++i)
	{
		ActorComponent* ac = *i;
		if(ac->GetName() == name)
		{
			return ac;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------------------------
void Actor::AddComponent( ActorComponent* comp )
{
	_components.push_back(comp);
	comp->SetOwner(this);
	comp->OnAddToActor(this);
}
//----------------------------------------------------------------------------------------------
void Actor::RemoveComponent( ActorComponent* comp )
{
	ActorComponentList::iterator i =
		std::find(_components.begin(), _components.end(), comp);

	if(i != _components.end())
	{
		(*i)->OnRemoveFromActor(this);
		_components.erase(i);
	}
}
//----------------------------------------------------------------------------------------------
void Actor::RemoveComponent( const Ogre::String& name )
{
	ActorComponentList::iterator i = _components.begin();
	for (; i != _components.end(); ++i)
	{
		ActorComponent* ac = *i;
		if(ac->GetName() == name)
		{
			ac->OnRemoveFromActor(this);
			ac->Destroy();
			_components.erase(i);
			OGRE_DELETE ac;
		}
	}
}
//----------------------------------------------------------------------------------------------
void Actor::Update( float deltaTime )
{
	UpdateComponents();
	UpdateComponents(deltaTime);
}
//----------------------------------------------------------------------------------------------
void Actor::UpdateComponents()
{
	ActorComponentList::iterator i = _components.begin();
	for (; i != _components.end(); ++i)
	{
		(*i)->Update();
	}
}
//----------------------------------------------------------------------------------------------
void Actor::UpdateComponents( float deltaTime )
{
	ActorComponentList::iterator i = _components.begin();
	for (; i != _components.end(); ++i)
	{
		(*i)->Update(deltaTime);
	}
}
//----------------------------------------------------------------------------------------------
void Actor::PostSceneLoad()
{
	if((_actorController == 0) && !_controllerClassTypeName.empty())
	{
		IGameObjectFactory* factory =
			GameObjectFactoryManager::getSingleton().GetFactory(_controllerClassTypeName);

		IActorController* ac = dynamic_cast<IActorController*>(
			factory->Create(XGUID::createGUID(), &_controllerCreateParams));

		SetActorController(ac);
	}
	else if((_actorController == 0) && _controllerClassTypeName.empty())
	{
		LogErrorIDf("Engine", "[%s] Controller class type name is null!", __FUNCTION__);
	}

	UpdateComponents();
}
//----------------------------------------------------------------------------------------------
const Ogre::NameValuePairList& Actor::GetControllerCreateParams() const
{
	return _controllerCreateParams;
}
//----------------------------------------------------------------------------------------------
void Actor::SetControllerCreateParams( const Ogre::NameValuePairList& val )
{
	_controllerCreateParams = val;
}
//----------------------------------------------------------------------------------------------
void Actor::Destroy()
{
	GameApp::getSingleton().RemoveUpdateable(this);

	IGame* game = GameApp::getSingleton().GetGame();
	ActorComponentList::iterator i = _components.begin();
	for (; i != _components.end(); ++i)
	{
		(*i)->Destroy();
		OGRE_DELETE (*i);
	}

	_components.clear();

	VisualObject::Destroy();
}