#include "StdAfx.h"
#include "Cmd_AddEmitter.h"
#include "Cmd_RemoveEmitter.h"
#include "ParticleEditorEvents.h"

Cmd_AddEmitter::Cmd_AddEmitter(ParticleSystem* ps, const String& emitterType, const NameValuePairList& parms)
: _particleSys(ps)
, _emitterType(emitterType)
, _params(parms)
{
}

Cmd_AddEmitter::~Cmd_AddEmitter(void)
{
}

ICommandXPtr Cmd_AddEmitter::Execute()
{
	ParticleEmitter* pe = _particleSys->addEmitter(_emitterType);
	pe->setParameterList(_params);

	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::UpdateEmitterList,
		Any(_particleSys));
	
	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::SelectEmitter,
		Any(_particleSys->getNumEmitters() - 1));

	return OGRE_NEW Cmd_RemoveEmitter(_particleSys,
		_particleSys->getNumEmitters() - 1);
}
