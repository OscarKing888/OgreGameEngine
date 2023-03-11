#pragma once
#include "AbstractEffectElement.h"

class EffectElement_Particle : public AbstractEffectElement
{
public:

	EffectElement_Particle(const String& name, ISpecialEffects* parent);
	virtual ~EffectElement_Particle(void);

	virtual void SetName(const String& name);

	virtual const String& GetTypeName() const;
	virtual bool IsAttachable() const{ return true; }

	void SetParticleTemplateName(const String& tempName);
	const String& GetParticleTemplateName() const { return _particleTemplateName; }
	virtual void OnDestroy();

protected:

	virtual void OnStart();
	virtual void OnEnd();
	virtual void BuildPropertyMap();

	virtual void OnCreateParticle();
	virtual void OnDestroyParticle();
	virtual Node* GetNode() const;

	ParticleSystem*	_particleSys;
	String			_particleTemplateName;
};

