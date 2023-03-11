#pragma once
#include "ActorComponent.h"

class _EngineExport ActorComponent_Mesh : public ActorComponent
{
public:

	ActorComponent_Mesh();
	virtual ~ActorComponent_Mesh();
	virtual const String& GetTypeName() const;

	virtual void Update();
	virtual void Update(float deltaTime);

	virtual void OnAddToActor(Actor* actor);
	virtual void OnRemoveFromActor(Actor* actor);

	Ogre::Entity* GetEntity();

	virtual void Destroy();

	const Ogre::String& GetMeshFile() const;
	void SetMeshFile(const Ogre::String& val);

	void SetVisible(bool on);
	bool IsVisible() const;

	bool GetEnableAnimation() const;
	void SetEnableAnimation(bool val);

	float GetAnimationSpeed() const;
	void SetAnimationSpeed(float val);

	bool GetLoopAnimation() const;
	void SetLoopAnimation(bool val);

protected:

	Ogre::String _meshFile;
	Ogre::Entity* _entity;
	bool _enableAnimation;
	float _animationSpeed;
	bool _loopAnimation;

	void CreateEntity();
	void DestroyEntity();

	virtual void OnAttachToNode(Ogre::SceneNode* node);
	virtual void OnDetachFromNode(Ogre::SceneNode* node);

	virtual void OnAttachToSkeleton();
	virtual void OnDetachFromSkeleton();
};

class _EngineExport AttachSource_ActorComponentMesh : public IAttachSource
{
public:

	AttachSource_ActorComponentMesh(ActorComponent_Mesh* src);
	virtual Ogre::Entity* GetAttachSource() const;

protected:

	ActorComponent_Mesh* _src;
};