#include "stdafx.h"
#include "ActorComponent_Mesh.h"
#include "Actor.h"
#include <OgreTagPoint.h>

ActorComponent_Mesh::ActorComponent_Mesh()
: _entity(0)
, _enableAnimation(true)
, _animationSpeed(1.0f)
, _loopAnimation(false)
{

}
//--------------------------------------------------------------------------------
ActorComponent_Mesh::~ActorComponent_Mesh()
{
	Destroy();
}
//--------------------------------------------------------------------------------
const String& ActorComponent_Mesh::GetTypeName() const
{
	static String sName("ActorComponent_StaticMesh");
	return sName;
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::Update()
{
	ActorComponent::Update();
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::Update( float deltaTime )
{
	if(_enableAnimation)
	{
		ConstEnabledAnimationStateIterator asi =
			_entity->getAllAnimationStates()->getEnabledAnimationStateIterator();

		while (asi.hasMoreElements())
		{
			AnimationState* as = asi.peekNext();
			as->addTime(deltaTime * _animationSpeed);			
			asi.moveNext();
		}
	}
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::OnAddToActor( Actor* actor )
{

}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::OnRemoveFromActor( Actor* actor )
{

}
//--------------------------------------------------------------------------------
Ogre::Entity* ActorComponent_Mesh::GetEntity()
{
	return _entity;
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::Destroy()
{
	DestroyEntity();

	ActorComponent::Destroy();
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::CreateEntity()
{
	if(!_entity)
	{
		SceneManager* sm = GetSceneManager();
		String entityName = _owner->GetGUID().toString() + _name + String(".Entity");
		_entity = sm->createEntity(entityName, _meshFile);
		SetVisible(true);
		_entity->setQueryFlags(_owner->GetQueryFlag());
	}
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::DestroyEntity()
{
	if(_entity)
	{
		SceneManager* sm = GetSceneManager();
		sm->destroyEntity(_entity);
		_entity = 0;
	}
}
//--------------------------------------------------------------------------------
const Ogre::String& ActorComponent_Mesh::GetMeshFile() const
{
	return _meshFile;
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::SetMeshFile( const Ogre::String& val )
{
	_meshFile = val;
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::SetVisible( bool on )
{
	if(!_entity)
	{
		return;
	}

	if(on)
	{
		_entity->setVisibilityFlags(_owner->GetVisibilityFlag());
	}
	else
	{
		_entity->setVisibilityFlags(0);
	}
}
//--------------------------------------------------------------------------------
bool ActorComponent_Mesh::IsVisible() const
{
	if(!_entity)
	{
		return false;
	}

	return _entity->getVisibilityFlags() != 0;
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::OnAttachToNode( Ogre::SceneNode* node )
{
	CreateEntity();
	node->attachObject(_entity);
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::OnDetachFromNode( Ogre::SceneNode* node )
{
	if(_entity)
	{
		node->detachObject(_entity);
		DestroyEntity();
	}
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::OnAttachToSkeleton()
{
	ActorComponent::OnAttachToSkeleton();

	CreateEntity();
	Ogre::TagPoint* tp = _attchSource->GetAttachSource()->attachObjectToBone(
		_attachTargetBoneName, _entity, _offsetRotation, _offsetPosition);

	tp->setScale(_offsetScale);
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::OnDetachFromSkeleton()
{
	ActorComponent::OnDetachFromSkeleton();

	_attchSource->GetAttachSource()->detachObjectFromBone(_entity);
	DestroyEntity();
}
//--------------------------------------------------------------------------------
bool ActorComponent_Mesh::GetEnableAnimation() const
{
	return _enableAnimation;
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::SetEnableAnimation( bool val )
{
	_enableAnimation = val;
}
//--------------------------------------------------------------------------------
float ActorComponent_Mesh::GetAnimationSpeed() const
{
	return _animationSpeed;
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::SetAnimationSpeed( float val )
{
	_animationSpeed = val;
}
//--------------------------------------------------------------------------------
bool ActorComponent_Mesh::GetLoopAnimation() const
{
	return _loopAnimation;
}
//--------------------------------------------------------------------------------
void ActorComponent_Mesh::SetLoopAnimation( bool val )
{
	_loopAnimation = val;
}
//--------------------------------------------------------------------------------
// AttachSource_ActorComponentMesh
//--------------------------------------------------------------------------------
Ogre::Entity* AttachSource_ActorComponentMesh::GetAttachSource() const
{
	return _src->GetEntity();
}
//--------------------------------------------------------------------------------
AttachSource_ActorComponentMesh::AttachSource_ActorComponentMesh( ActorComponent_Mesh* src )
: _src(src)
{

}