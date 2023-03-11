#include "StdAfx.h"
#include "Cmd_RemoveAffector.h"
#include "Cmd_AddAffector.h"
#include "ParticleUtil.h"
#include "ParticleEditorEvents.h"

Cmd_RemoveAffector::Cmd_RemoveAffector(ParticleSystem* ps, ushort idx)
: _particleSys(ps)
, _idx(idx)
{
}

Cmd_RemoveAffector::~Cmd_RemoveAffector(void)
{
}

ICommandXPtr Cmd_RemoveAffector::Execute()
{
	ParticleAffector* pa = _particleSys->getAffector(_idx);
	String typeName = pa->getType();

	NameValuePairList parms =
		ParticleUtil::GetParameters(pa);

	_particleSys->removeAffector(_idx);

	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::UpdateAffectorList,

		Any(_particleSys));
	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::UpdateRemoveAffectorUI,
		Any(false));

	return OGRE_NEW Cmd_AddAffector(_particleSys, typeName, parms);
}
