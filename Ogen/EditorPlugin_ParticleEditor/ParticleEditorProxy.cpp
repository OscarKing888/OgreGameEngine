#include "stdafx.h"
#include "ParticleEditorProxy.h"
#include "EditorCore/PropertyEditor_Material.h"


Ogre::NameValuePairList IParticleComponentEditorFactory::GetInitDefaultParameters()
{
	return NameValuePairList();
}

void ParticleEditorFactoryManager::RegisterParticleRendererEditorFactory(IParticleRenderEditorFactory* render)
{
	_renderEditorFactory.insert(std::make_pair(render->GetTypeName(), render));
}

void ParticleEditorFactoryManager::RegisterEmitterEditorFactory(IParticleEmitterEditorFactory* emitter)
{
	_emitterEditorFactory.insert(std::make_pair(emitter->GetTypeName(), emitter));
}

void ParticleEditorFactoryManager::RegisterAffectorEditorFactory(IParticleAffectorEditorFactory* affector)
{
	_affectorEditorFactory.insert(std::make_pair(affector->GetTypeName(), affector));
}

void ParticleEditorFactoryManager::DestroyParticleRenderEditor(IParticleRenderEditor* renderEditor)
{
	ParticleRenderEditorFactoryMap::iterator i = _renderEditorFactory.find(renderEditor->GetTypeName());
	if(i != _renderEditorFactory.end())
	{
		IParticleRenderEditorFactory* f = i->second;
		f->Destroy(renderEditor);
		return;
	}

	assert(false && "Emitter editor factory not found!");
}

void ParticleEditorFactoryManager::DestroyEmitterEditor(IParticleEmitterEditor* emitterEditor)
{
	EmitterEditorFactoryMap::iterator i = _emitterEditorFactory.find(emitterEditor->GetTypeName());
	if(i != _emitterEditorFactory.end())
	{
		IParticleEmitterEditorFactory* f = i->second;
		f->Destroy(emitterEditor);
		return;
	}

	assert(false && "Emitter editor factory not found!");
}

void ParticleEditorFactoryManager::DestroyAffectorEditor(IParticleAffectorEditor* affectorEditor)
{
	AffectorEditorFactoryMap::iterator i = _affectorEditorFactory.find(affectorEditor->GetTypeName());
	if(i != _affectorEditorFactory.end())
	{
		IParticleAffectorEditorFactory* f = i->second;
		f->Destroy(affectorEditor);
		return;
	}

	assert(false && "Affector editor factory not found!");
}

IParticleRenderEditor* ParticleEditorFactoryManager::CreateParticleRenderEditor(
	const String& typeName, ParticleSystemRenderer* render)
{
	ParticleRenderEditorFactoryMap::iterator i = _renderEditorFactory.find(typeName);
	if(i != _renderEditorFactory.end())
	{
		IParticleRenderEditorFactory* f = i->second;
		IParticleRenderEditor* editor = f->Create(render);
		editor->BuildPropertyMap();
		return editor;
	}

	return 0;
}

IParticleEmitterEditor* ParticleEditorFactoryManager::CreateEmitterEditor(const String& typeName, ParticleEmitter* emitter)
{
	EmitterEditorFactoryMap::iterator i = _emitterEditorFactory.find(typeName);
	if(i != _emitterEditorFactory.end())
	{
		IParticleEmitterEditorFactory* f = i->second;
		IParticleEmitterEditor* editor = f->Create(emitter);
		editor->BuildPropertyMap();
		return editor;
	}

	return 0;
}

IParticleAffectorEditor* ParticleEditorFactoryManager::CreateAffectorEditor(const String& typeName, ParticleAffector* affector)
{
	AffectorEditorFactoryMap::iterator i = _affectorEditorFactory.find(typeName);
	if(i != _affectorEditorFactory.end())
	{
		IParticleAffectorEditorFactory* f = i->second;
		IParticleAffectorEditor* editor = f->Create(affector);
		editor->BuildPropertyMap();
		return editor;
	}

	return 0;
}

IParticleRenderEditorFactory* ParticleEditorFactoryManager::GetRendererFactory( const String& typeName ) const
{
	ParticleRenderEditorFactoryMap::const_iterator i = _renderEditorFactory.find(typeName);
	if(i != _renderEditorFactory.end())
	{
		return i->second;
	}

	return 0;
}

IParticleEmitterEditorFactory* ParticleEditorFactoryManager::GetEmitterFactory( const String& typeName ) const
{
	EmitterEditorFactoryMap::const_iterator i = _emitterEditorFactory.find(typeName);
	if(i != _emitterEditorFactory.end())
	{
		return i->second;
	}

	return 0;
}

IParticleAffectorEditorFactory* ParticleEditorFactoryManager::GetAffectorFactory( const String& typeName ) const
{
	AffectorEditorFactoryMap::const_iterator i = _affectorEditorFactory.find(typeName);
	if(i != _affectorEditorFactory.end())
	{
		return i->second;
	}

	return 0;
}
//------------------------------------------------------------------------

ParticleAttributeEditor::ParticleAttributeEditor()
: _particleSystem(0)
{
	BuildPropertyMap();
}

ParticleAttributeEditor::~ParticleAttributeEditor()
{

}

void ParticleAttributeEditor::SetParticleSystem(ParticleSystem* ps)
{
	_particleSystem = ps;
}

void ParticleAttributeEditor::BuildPropertyMap()
{
	AddProp(Property_String, RendererName, ParticleAttributeEditor, "RendererName", "ParticleAttribute");
	AddProp(Property_Int, ParticleQuota, ParticleAttributeEditor, "ParticleQuota", "ParticleAttribute");
	AddProp(Property_Int, EmittedEmitterQuota, ParticleAttributeEditor, "EmittedEmitterQuota", "ParticleAttribute");
	AddProp(Property_Material, MaterialName, ParticleAttributeEditor, "MaterialName", "ParticleAttribute");
	AddProp(Property_Float, SpeedFactor, ParticleAttributeEditor, "SpeedFactor", "ParticleAttribute");
	AddProp(Property_Float, IterationInterval, ParticleAttributeEditor, "IterationInterval", "ParticleAttribute");
	AddProp(Property_Float, NonVisibleUpdateTimeout, ParticleAttributeEditor, "NonVisibleUpdateTimeout", "ParticleAttribute");
	AddProp(Property_Float, DefaultWidth, ParticleAttributeEditor, "DefaultWidth", "ParticleAttribute");
	AddProp(Property_Float, DefaultHeight, ParticleAttributeEditor, "DefaultHeight", "ParticleAttribute");
	AddProp(Property_Bool, CullIndividually, ParticleAttributeEditor, "CullIndividually", "ParticleAttribute");
	AddProp(Property_Bool, SortingEnabled, ParticleAttributeEditor, "SortingEnabled", "ParticleAttribute");
	AddProp(Property_Bool, KeepParticlesInLocalSpace, ParticleAttributeEditor, "KeepParticlesInLocalSpace", "ParticleAttribute");
}

void ParticleAttributeEditor::SetRendererName(const String& typeName)
{
	_particleSystem->setRenderer(typeName);
}

const String& ParticleAttributeEditor::GetRendererName(void) const
{
	return _particleSystem->getRendererName();
}

size_t ParticleAttributeEditor::GetParticleQuota(void) const
{
	return _particleSystem->getParticleQuota();
}

void ParticleAttributeEditor::SetParticleQuota(size_t quota)
{
	_particleSystem->setParticleQuota(quota);
}

size_t ParticleAttributeEditor::GetEmittedEmitterQuota(void) const
{
	return _particleSystem->getEmittedEmitterQuota();
}

void ParticleAttributeEditor::SetEmittedEmitterQuota(size_t quota)
{
	_particleSystem->setEmittedEmitterQuota(quota);
}

void ParticleAttributeEditor::SetMaterialName(const String& name)
{
	_particleSystem->setMaterialName(name);
}

const String& ParticleAttributeEditor::GetMaterialName(void) const
{
	return _particleSystem->getMaterialName();
}

void ParticleAttributeEditor::SetSpeedFactor(Real speedFactor)
{
	_particleSystem->setSpeedFactor(speedFactor);
}

Real ParticleAttributeEditor::GetSpeedFactor(void) const
{
	return _particleSystem->getSpeedFactor();
}

void ParticleAttributeEditor::SetIterationInterval(Real iterationInterval)
{
	_particleSystem->setIterationInterval(iterationInterval);
}

Real ParticleAttributeEditor::GetIterationInterval(void) const
{
	return _particleSystem->getIterationInterval();
}

void ParticleAttributeEditor::SetNonVisibleUpdateTimeout(Real timeout)
{
	_particleSystem->setNonVisibleUpdateTimeout(timeout);
}

Real ParticleAttributeEditor::GetNonVisibleUpdateTimeout(void) const
{
	return _particleSystem->getNonVisibleUpdateTimeout();
}

void ParticleAttributeEditor::SetDefaultWidth(Real width)
{
	_particleSystem->setDefaultWidth(width);
}

Real ParticleAttributeEditor::GetDefaultWidth(void) const
{
	return _particleSystem->getDefaultWidth();
}

void ParticleAttributeEditor::SetDefaultHeight(Real height)
{
	_particleSystem->setDefaultHeight(height);
}

Real ParticleAttributeEditor::GetDefaultHeight(void) const
{
	return _particleSystem->getDefaultHeight();
}

bool ParticleAttributeEditor::GetCullIndividually(void) const
{
	return _particleSystem->getCullIndividually();
}

void ParticleAttributeEditor::SetCullIndividually(bool cullIndividual)
{
	_particleSystem->setCullIndividually(cullIndividual);
}

void ParticleAttributeEditor::SetSortingEnabled(bool enabled)
{
	_particleSystem->setSortingEnabled(enabled);
}

bool ParticleAttributeEditor::GetSortingEnabled(void) const
{
	return _particleSystem->getSortingEnabled();
}

void ParticleAttributeEditor::SetKeepParticlesInLocalSpace(bool keepLocal)
{
	_particleSystem->setKeepParticlesInLocalSpace(keepLocal);
}

bool ParticleAttributeEditor::GetKeepParticlesInLocalSpace(void) const
{
	return _particleSystem->getKeepParticlesInLocalSpace();
}

const String& ParticleAttributeEditor::GetTypeName() const
{
	static String typeName = "ParticleAttributeEditor";
	return typeName;
}