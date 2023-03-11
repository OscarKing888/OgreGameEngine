#include "stdafx.h"
#include "ParticleUtil.h"
#include "ParticleSerializer.h"

//--------------------------------------------------------------------------------
void ParticleUtil::ReplaceParticleTemplate(ParticleSystem* par, const String& templateName)
{
	ParticleSystemManager& psm = ParticleSystemManager::getSingleton();
	RemoveParticleTemplate(templateName);
	
	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	ParticleSystem* templatePar = sm->createParticleSystem(templateName, par->getParticleQuota());
	*templatePar = *par;
	psm.addTemplate(templateName, templatePar);
}
//--------------------------------------------------------------------------------
void ParticleUtil::SaveAllParticleTemplate(const String& fileName)
{
	File::removeReadonly(fileName);

	std::ofstream outFile;
	outFile.open ( fileName.c_str(), std::ios::out | std::ios::trunc ); // overwrite file
	if (!outFile.is_open())
	{
		return;
	}

	ParticleSystemManager::ParticleSystemTemplateIterator i = ParticleSystemManager::getSingleton().getTemplateIterator();
	while (i.hasMoreElements())
	{
		ParticleSerializer ser;
		ser.SaveParticle(i.peekNextValue(), outFile);
		i.moveNext();
	}

	outFile.close();
}
//--------------------------------------------------------------------------------
void ParticleUtil::RemoveParticleTemplate(const String& templateName)
{
	ParticleSystemManager& psm = ParticleSystemManager::getSingleton();
	if(psm.getTemplate(templateName) != 0)
	{
		psm.removeTemplate(templateName, false);
	}

	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	sm->destroyParticleSystem(templateName);
}
//--------------------------------------------------------------------------------
Ogre::NameValuePairList ParticleUtil::GetParameters( const StringInterface* obj )
{
	const ParameterList& pl = obj->getParameters();

	NameValuePairList parms;
	ParameterList::const_iterator i = pl.begin();
	for (; i != pl.end(); ++i)
	{
		const ParameterDef& pd = *i;
		String value = obj->getParameter(pd.name);
		parms.insert(std::make_pair(pd.name, value));
	}

	return parms;
}
