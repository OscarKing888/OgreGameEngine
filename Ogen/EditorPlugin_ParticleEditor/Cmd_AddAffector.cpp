#include "StdAfx.h"
#include "Cmd_AddAffector.h"
#include "Cmd_RemoveAffector.h"
#include "ParticleEditorEvents.h"

Cmd_AddAffector::Cmd_AddAffector(ParticleSystem* ps, const String& affectorType, const NameValuePairList& parms)
: _particleSys(ps)
, _affectorType(affectorType)
, _params(parms)

{
}

Cmd_AddAffector::~Cmd_AddAffector(void)
{
}

ICommandXPtr Cmd_AddAffector::Execute()
{
	ParticleAffector* pa = _particleSys->addAffector(_affectorType);
	pa->setParameterList(_params);

	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::UpdateAffectorList,
		Any(_particleSys));

	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::SelectAffector,
		Any(_particleSys->getNumAffectors() - 1));

	return OGRE_NEW Cmd_RemoveAffector(_particleSys,
		_particleSys->getNumAffectors() - 1);
}
