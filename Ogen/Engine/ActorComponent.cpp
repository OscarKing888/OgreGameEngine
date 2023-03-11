#include "stdafx.h"
#include "ActorComponent.h"
#include "Actor.h"
#include <OgreTagPoint.h>

ActorComponent::ActorComponent()
: _offsetYawPitchRoll(Ogre::Vector3::ZERO)
, _offsetScale(Ogre::Vector3::UNIT_SCALE)
, _offsetPosition(Ogre::Vector3::ZERO)
, _needUpdateRot(false)
, _node(0)
, _attachToSkeletonMesh(false)
, _attached(false)
{

}
//--------------------------------------------------------------------------------
ActorComponent::~ActorComponent()
{

}
//--------------------------------------------------------------------------------
void ActorComponent::Update( float deltaTime )
{

}
//--------------------------------------------------------------------------------
void ActorComponent::Update()
{
	if(_needUpdateRot)
	{
		UpdateOffsetRotation();
	}

	if(!_attachToSkeletonMesh && !_node)
	{
		CreateNode();
	}
	else if(_attachToSkeletonMesh && !_attached)
	{
		OnAttachToSkeleton();
	}
}
//--------------------------------------------------------------------------------
void ActorComponent::OnAddToActor( Actor* actor )
{

}
//--------------------------------------------------------------------------------
void ActorComponent::OnRemoveFromActor( Actor* actor )
{

}
//--------------------------------------------------------------------------------
void ActorComponent::SetOffsetYawPitchRoll( const Ogre::Vector3& val )
{
	_offsetYawPitchRoll = val;
	_needUpdateRot = true;
}
//--------------------------------------------------------------------------------
const Ogre::Vector3& ActorComponent::GetOffsetYawPitchRoll() const
{
	return _offsetYawPitchRoll;
}
//--------------------------------------------------------------------------------
const Ogre::Vector3& ActorComponent::GetOffsetScale() const
{
	return _offsetScale;
}
//--------------------------------------------------------------------------------
void ActorComponent::SetOffsetScale( const Ogre::Vector3& val )
{
	_offsetScale = val;
	_needUpdateRot = true;
}
//--------------------------------------------------------------------------------
const Ogre::Vector3& ActorComponent::GetOffsetPosition() const
{
	return _offsetPosition;
}
//--------------------------------------------------------------------------------
void ActorComponent::SetOffsetPosition( const Ogre::Vector3& val )
{
	_offsetPosition = val;
	_needUpdateRot = true;
}
//--------------------------------------------------------------------------------
const Ogre::Quaternion& ActorComponent::GetOffsetRotation() const
{
	return _offsetRotation;
}
//--------------------------------------------------------------------------------
void ActorComponent::SetOffsetRotation( const Ogre::Quaternion& val )
{
	_offsetRotation = val;
	
	Matrix3 mat;
	_offsetRotation.ToRotationMatrix(mat);
	Radian y, p, r;
	mat.ToEulerAnglesYXZ(y, p, r);
	
	_offsetYawPitchRoll.x = y.valueRadians();
	_offsetYawPitchRoll.y = p.valueRadians();
	_offsetYawPitchRoll.z = r.valueRadians();
}
//--------------------------------------------------------------------------------
void ActorComponent::UpdateOffsetRotation()
{
	Matrix3 mat;
	// yaw	pitch	roll
	// y	x		z
	// up	right	dir
	mat.FromEulerAnglesYXZ(Radian(_offsetYawPitchRoll.x), 
		Radian(_offsetYawPitchRoll.y), Radian(_offsetYawPitchRoll.z));

	_offsetRotation = Quaternion(mat);
}
//--------------------------------------------------------------------------------
bool ActorComponent::GetAttachToSkeletonMesh() const
{
	return _attachToSkeletonMesh;
}
//--------------------------------------------------------------------------------
void ActorComponent::SetAttachToSkeletonMesh( bool val )
{
	_attachToSkeletonMesh = val;
}
//--------------------------------------------------------------------------------
void ActorComponent::CreateNode()
{
	if(!_node)
	{		
		_node = _owner->GetNode()->createChildSceneNode(
			GetNodeName(), _offsetPosition, _offsetRotation);
		_node->setScale(_offsetScale);

		OnAttachToNode(_node);
	}
}
//--------------------------------------------------------------------------------
void ActorComponent::DestroyNode()
{
	if(_node)
	{
		OnDetachFromNode(_node);

		//_node->detachAllObjects();
		_node->getParentSceneNode()
			->removeAndDestroyChild(_node->getName());
	}
	else if(_attachToSkeletonMesh)
	{
		OnDetachFromSkeleton();
	}
}
//--------------------------------------------------------------------------------
void ActorComponent::PostCreateNode()
{

}
//--------------------------------------------------------------------------------
void ActorComponent::PreDestroyNode()
{

}
//--------------------------------------------------------------------------------
void ActorComponent::Destroy()
{
	DestroyNode();
}
//--------------------------------------------------------------------------------
void ActorComponent::OnAttachToSkeleton()
{
	_attached = true;
}
//--------------------------------------------------------------------------------
void ActorComponent::OnDetachFromSkeleton()
{
	_attached = false;
}
//--------------------------------------------------------------------------------
const Ogre::String& ActorComponent::GetAttachTargetBoneName() const
{
	return _attachTargetBoneName;
}
//--------------------------------------------------------------------------------
void ActorComponent::SetAttachTargetBoneName( const Ogre::String& val )
{
	_attachTargetBoneName = val;
}
//--------------------------------------------------------------------------------
Ogre::String ActorComponent::GetNodeName() const
{
	String ownerName = _owner->GetGUID().toString();
	return ownerName + _name;
}
//--------------------------------------------------------------------------------
void ActorComponent::OnAttachToNode( Ogre::SceneNode* node )
{

}
//--------------------------------------------------------------------------------
void ActorComponent::OnDetachFromNode( Ogre::SceneNode* node )
{

}
//--------------------------------------------------------------------------------
IAttachSourcePtr ActorComponent::GetAttachSource() const
{
	return _attchSource;
}
//--------------------------------------------------------------------------------
void ActorComponent::SetAttachSource( IAttachSourcePtr src )
{
	_attchSource = src;
}
//--------------------------------------------------------------------------------
Ogre::SceneManager* ActorComponent::GetSceneManager() const
{
	return GameApp::getSingleton().GetGame()->GetVisualScene();
}
//--------------------------------------------------------------------------------
