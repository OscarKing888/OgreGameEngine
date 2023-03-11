#include "StdAfx.h"
#include "Cmd_SetParticleRenderer.h"
#include <OgreParticleSystemRenderer.h>
#include "ParticleUtil.h"
#include "ParticleSystemEditor.h"

Cmd_SetParticleRenderer::Cmd_SetParticleRenderer(
	ParticleSystem* ps, const String& renderName, const NameValuePairList& parms)
: _particleSys(ps)
, _renderName(renderName)
, _params(parms)
{
}

Cmd_SetParticleRenderer::~Cmd_SetParticleRenderer(void)
{
}

ICommandXPtr Cmd_SetParticleRenderer::Execute()
{
	String oldRenderName = _particleSys->getRendererName();
	ParticleSystemRenderer* oldRender = _particleSys->getRenderer();
	NameValuePairList parms =
		ParticleUtil::GetParameters(oldRender);

	_particleSys->setRenderer(_renderName);
	ParticleSystemRenderer* psr = _particleSys->getRenderer();
	psr->setParameterList(_params);

	ParticleSystemEditor::getSingleton().UpdateRendererEditor();

	return OGRE_NEW Cmd_SetParticleRenderer(_particleSys, oldRenderName, parms);
}
