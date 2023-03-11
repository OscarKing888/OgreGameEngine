#include "stdafx.h"
#include "ActorComponent_Light.h"
#include "Actor.h"
#include <OgreTagPoint.h>

ActorComponent_Light::ActorComponent_Light()
: _light(0)
{

}
//--------------------------------------------------------------------------------
ActorComponent_Light::~ActorComponent_Light()
{
	Destroy();
}
//--------------------------------------------------------------------------------
const String& ActorComponent_Light::GetTypeName() const
{
	static String sName("ActorComponent_Light");
	return sName;
}
//--------------------------------------------------------------------------------
void ActorComponent_Light::Update()
{
	ActorComponent::Update();
}
//--------------------------------------------------------------------------------
void ActorComponent_Light::Update( float deltaTime )
{
}
//--------------------------------------------------------------------------------
void ActorComponent_Light::OnAddToActor( Actor* actor )
{

}
//--------------------------------------------------------------------------------
void ActorComponent_Light::OnRemoveFromActor( Actor* actor )
{

}
//--------------------------------------------------------------------------------
Ogre::Light* ActorComponent_Light::GetLight()
{
	return _light;
}
//--------------------------------------------------------------------------------
void ActorComponent_Light::Destroy()
{
	DestroyLight();

	ActorComponent::Destroy();
}
//--------------------------------------------------------------------------------
void ActorComponent_Light::OnAttachToNode( Ogre::SceneNode* node )
{
	CreateLight();
	_node->attachObject(_light);
}
//--------------------------------------------------------------------------------
void ActorComponent_Light::OnDetachFromNode( Ogre::SceneNode* node )
{
	if(_light)
	{
		node->detachObject(_light);
	}
}
//--------------------------------------------------------------------------------
void ActorComponent_Light::CreateLight()
{
	if(!_light)
	{
		SceneManager* sm = GetSceneManager();
		String entityName = _owner->GetGUID().toString() + _name + String(".Light");
		_light = sm->createLight(entityName);
		SetOn(true);
		_light->setQueryFlags(_owner->GetQueryFlag());
	}
}
//--------------------------------------------------------------------------------
void ActorComponent_Light::DestroyLight()
{
	if(_light)
	{
		SceneManager* sm = GetSceneManager();
		sm->destroyLight(_light);
		_light = 0;
	}
}
//--------------------------------------------------------------------------------
void ActorComponent_Light::OnAttachToSkeleton()
{
	ActorComponent::OnAttachToSkeleton();

	CreateLight();
	Ogre::TagPoint* tp = _attchSource->GetAttachSource()->attachObjectToBone(
		_attachTargetBoneName, _light, _offsetRotation, _offsetPosition);

	tp->setScale(_offsetScale);
}
//--------------------------------------------------------------------------------
void ActorComponent_Light::OnDetachFromSkeleton()
{
	ActorComponent::OnDetachFromSkeleton();
	_attchSource->GetAttachSource()->detachObjectFromBone(_light);
	DestroyLight();
}
//--------------------------------------------------------------------------------
void ActorComponent_Light::SetOn( bool on )
{
	if(!_light)
	{
		return;
	}

	_light->setVisible(on);
}
//--------------------------------------------------------------------------------
bool ActorComponent_Light::IsOn() const
{
	if(!_light)
	{
		return false;
	}

	return _light->getVisible();
}