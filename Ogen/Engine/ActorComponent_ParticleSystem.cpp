#include "stdafx.h"
#include "ActorComponent_ParticleSystem.h"
#include "Actor.h"
#include <OgreTagPoint.h>

ActorComponent_ParticleSystem::ActorComponent_ParticleSystem()
: _particleSys(0)
{

}
//--------------------------------------------------------------------------------
ActorComponent_ParticleSystem::~ActorComponent_ParticleSystem()
{
	Destroy();
}
//--------------------------------------------------------------------------------
const String& ActorComponent_ParticleSystem::GetTypeName() const
{
	static String sName("ActorComponent_ParticleSystem");
	return sName;
}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::Update()
{
	ActorComponent::Update();
}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::Update( float deltaTime )
{
}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::OnAddToActor( Actor* actor )
{

}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::OnRemoveFromActor( Actor* actor )
{

}
//--------------------------------------------------------------------------------
Ogre::ParticleSystem* ActorComponent_ParticleSystem::GetParticleSystem() const
{
	return _particleSys;
}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::Destroy()
{
	DestroyParSys();

	ActorComponent::Destroy();
}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::CreateParSys()
{
	if(!_particleSys)
	{
		SceneManager* sm = GetSceneManager();
		String entityName = _owner->GetGUID().toString() + _name + String(".ParticleSys");
		_particleSys = sm->createParticleSystem(entityName, _particleTemplate);
		SetVisible(true);
		_particleSys->setQueryFlags(_owner->GetQueryFlag());
	}
}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::DestroyParSys()
{
	if(_particleSys)
	{
		SceneManager* sm = GetSceneManager();
		sm->destroyParticleSystem(_particleSys);
		_particleSys = 0;
	}
}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::OnAttachToNode( Ogre::SceneNode* node )
{
	CreateParSys();
	node->attachObject(_particleSys);
}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::OnDetachFromNode( Ogre::SceneNode* node )
{
	if(_particleSys)
	{
		node->detachObject(_particleSys);
		DestroyParSys();
	}
}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::OnAttachToSkeleton()
{
	ActorComponent::OnAttachToSkeleton();

	CreateParSys();
	Ogre::TagPoint* tp = _attchSource->GetAttachSource()->attachObjectToBone(
		_attachTargetBoneName, _particleSys, _offsetRotation, _offsetPosition);

	tp->setScale(_offsetScale);
}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::OnDetachFromSkeleton()
{
	ActorComponent::OnDetachFromSkeleton();
	_attchSource->GetAttachSource()->detachObjectFromBone(_particleSys);
	DestroyParSys();
}
//--------------------------------------------------------------------------------
const Ogre::String& ActorComponent_ParticleSystem::GetParticleTemplate() const
{
	return _particleTemplate;
}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::SetParticleTemplate( const Ogre::String& val )
{
	_particleTemplate = val;
}
//--------------------------------------------------------------------------------
void ActorComponent_ParticleSystem::SetVisible( bool on )
{
	if(!_particleSys)
	{
		return;
	}

	if(on)
	{
		_particleSys->setVisibilityFlags(_owner->GetVisibilityFlag());
	}
	else
	{
		_particleSys->setVisibilityFlags(0);
	}
}
//--------------------------------------------------------------------------------
bool ActorComponent_ParticleSystem::IsVisible() const
{
	if(!_particleSys)
	{
		return false;
	}

	return _particleSys->getVisibilityFlags() != 0;
}
