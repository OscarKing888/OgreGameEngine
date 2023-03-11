#pragma once
#include "ActorComponent.h"

class _EngineExport ActorComponent_Light : public ActorComponent
{
public:

	ActorComponent_Light();
	virtual ~ActorComponent_Light();
	virtual const String& GetTypeName() const;

	virtual void Update();
	virtual void Update(float deltaTime);

	virtual void OnAddToActor(Actor* actor);
	virtual void OnRemoveFromActor(Actor* actor);

	Ogre::Light* GetLight();

	virtual void Destroy();

	void SetOn(bool on);
	bool IsOn() const;

protected:

	Ogre::Light* _light;

	void CreateLight();
	void DestroyLight();

	virtual void OnAttachToNode(Ogre::SceneNode* node);
	virtual void OnDetachFromNode(Ogre::SceneNode* node);

	virtual void OnAttachToSkeleton();
	virtual void OnDetachFromSkeleton();

};