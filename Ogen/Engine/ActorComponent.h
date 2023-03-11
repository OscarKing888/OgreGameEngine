#pragma once
#include "GameComponent.h"

class Actor;

class _EngineExport IAttachSource : public RefObject
{
public:

	virtual Ogre::Entity* GetAttachSource() const = 0;
};

SmartPointer(IAttachSource);

class _EngineExport ActorComponent : public GameComponent
{
public:

	ActorComponent();
	virtual ~ActorComponent();

	Actor* GetOwner() const { return _owner; }
	void SetOwner(Actor* val) { _owner = val; }

	virtual void Update();
	virtual void Update(float deltaTime);

	virtual void OnAddToActor(Actor* actor);
	virtual void OnRemoveFromActor(Actor* actor);
	
	const Ogre::Vector3& GetOffsetYawPitchRoll() const;
	void SetOffsetYawPitchRoll(const Ogre::Vector3& val);
	
	const Ogre::Quaternion& GetOffsetRotation() const;
	void SetOffsetRotation(const Ogre::Quaternion& val);

	const Ogre::Vector3& GetOffsetScale() const;
	void SetOffsetScale(const Ogre::Vector3& val);
	
	const Ogre::Vector3& GetOffsetPosition() const;
	void SetOffsetPosition(const Ogre::Vector3& val);

	bool GetAttachToSkeletonMesh() const;
	void SetAttachToSkeletonMesh(bool val);

	const Ogre::String& GetAttachTargetBoneName() const;
	void SetAttachTargetBoneName( const Ogre::String& val);

	virtual void Destroy();

	Ogre::String GetNodeName() const;

	IAttachSourcePtr GetAttachSource() const;
	void SetAttachSource(IAttachSourcePtr src);

protected:

	Actor* _owner;
	Ogre::Vector3 _offsetPosition;
	Ogre::Vector3 _offsetScale;
	Ogre::Vector3 _offsetYawPitchRoll;
	Ogre::Quaternion _offsetRotation;
	bool _needUpdateRot;
	Ogre::SceneNode* _node;
	bool _attachToSkeletonMesh;
	bool _attached;
	Ogre::String _attachTargetBoneName;
	IAttachSourcePtr _attchSource;

	void UpdateOffsetRotation();
	void CreateNode();
	void DestroyNode();

	virtual void PostCreateNode();
	virtual void PreDestroyNode();

	virtual void OnAttachToSkeleton();
	virtual void OnDetachFromSkeleton();

	virtual void OnAttachToNode(Ogre::SceneNode* node);
	virtual void OnDetachFromNode(Ogre::SceneNode* node);

	Ogre::SceneManager* GetSceneManager() const;
};
