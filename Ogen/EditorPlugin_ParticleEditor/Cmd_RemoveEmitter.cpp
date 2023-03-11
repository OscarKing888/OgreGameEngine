#include "StdAfx.h"
#include "Cmd_RemoveEmitter.h"
#include "Cmd_AddEmitter.h"
#include "ParticleUtil.h"
#include "ParticleEditorEvents.h"

Cmd_RemoveEmitter::Cmd_RemoveEmitter(ParticleSystem* ps, ushort idx)
: _particleSys(ps)
, _idx(idx)
{
}

Cmd_RemoveEmitter::~Cmd_RemoveEmitter(void)
{
}

ICommandXPtr Cmd_RemoveEmitter::Execute()
{
	ParticleEmitter* pe = _particleSys->getEmitter(_idx);
	String typeName = pe->getType();
	NameValuePairList parms =
		ParticleUtil::GetParameters(pe);

	_particleSys->removeEmitter(_idx);

	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::UpdateEmitterList,
		Any(_particleSys));

	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::UpdateRemoveEmitterUI,
		Any(false));

	return OGRE_NEW Cmd_AddEmitter(_particleSys, typeName, parms);
}
