#pragma once
#include "ActorComponent.h"

class ParticleSys;

class _EngineExport ActorComponent_ParticleSystem : public ActorComponent
{
public:

	ActorComponent_ParticleSystem();
	virtual ~ActorComponent_ParticleSystem();
	virtual const String& GetTypeName() const;

	virtual void Update();
	virtual void Update(float deltaTime);

	virtual void OnAddToActor(Actor* actor);
	virtual void OnRemoveFromActor(Actor* actor);

	Ogre::ParticleSystem* GetParticleSystem() const;

	virtual void Destroy();

	const Ogre::String& GetParticleTemplate() const;
	void SetParticleTemplate(const Ogre::String& val);

	void SetVisible(bool on);
	bool IsVisible() const;

protected:

	Ogre::ParticleSystem* _particleSys;
	Ogre::String _particleTemplate;

	void CreateParSys();
	void DestroyParSys();

	virtual void OnAttachToNode(Ogre::SceneNode* node);
	virtual void OnDetachFromNode(Ogre::SceneNode* node);

	virtual void OnAttachToSkeleton();
	virtual void OnDetachFromSkeleton();

};