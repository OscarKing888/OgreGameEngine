#include "StdAfx.h"
#include "EffectElement_Particle.h"
#include "SpecialEffectsManager.h"


EffectElement_Particle::EffectElement_Particle(const String& name, ISpecialEffects* parent)
: AbstractEffectElement(name, parent)
, _particleSys(0)
, _particleTemplateName("Core/Default")
{
}

EffectElement_Particle::~EffectElement_Particle(void)
{
}

const String& EffectElement_Particle::GetTypeName() const
{
	static const String C_TypeName = "ParticleSystem";
	return C_TypeName;
}

void EffectElement_Particle::SetName(const String& name)
{
	OnDestroyParticle();
	_name = name;
	OnCreateParticle();
}

void EffectElement_Particle::OnStart()
{
	if(_particleSys)
	{
		_particleSys->setVisible(true);
		_particleSys->clear();
	}
}

void EffectElement_Particle::OnEnd()
{
	if(_particleSys)
	{
		_particleSys->setVisible(false);
	}
}

void EffectElement_Particle::BuildPropertyMap()
{
	AbstractEffectElement::BuildPropertyMap();
	AddProp(Property_String, ParticleTemplateName, EffectElement_Particle, "ParticleTemplateName", "Particle");
}

void EffectElement_Particle::OnCreateParticle()
{
	SceneManager* sm = GetSceneManager();
	if(_name.empty())
	{
		return;
	}

	if(!_particleTemplateName.empty())
	{
		_particleSys = sm->createParticleSystem(_parent->GetName() + "/" + GetName(), _particleTemplateName);
		_particleSys->setQueryFlags(0);
		_particleSys->setVisible(false);
	}

	if(!_node)
	{
		String nodeName = _name;
		_node = _parent->GetNode()->createChildSceneNode(nodeName);
	}

	if(_particleSys)
	{
		_node->attachObject(_particleSys);
	}
}

void EffectElement_Particle::OnDestroyParticle()
{
	if(_particleSys)
	{	
		SceneManager* sm = GetSceneManager();
		sm->destroyParticleSystem(_particleSys);
		_particleSys = 0;
	}
}

void EffectElement_Particle::SetParticleTemplateName(const String& tempName)
{
	_particleTemplateName = tempName;
	if(!_particleTemplateName.empty())
	{
		OnDestroyParticle();
		OnCreateParticle();
	}
}

void EffectElement_Particle::OnDestroy()
{
	OnDestroyParticle();
}

Node* EffectElement_Particle::GetNode() const
{
	if(_particleSys)
	{
		return _particleSys->getParentNode();
	}
	return 0;
}