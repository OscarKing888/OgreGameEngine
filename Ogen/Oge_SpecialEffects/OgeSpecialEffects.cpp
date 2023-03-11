// Oge_SpecialEffects.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "OgeSpecialEffects.h"
#include "SpecialEffectsManager.h"

#include "EffectElement_BillBoard.h"
#include "EffectElement_Entity.h"
#include "EffectElement_Particle.h"
#include "EffectElement_PointLight.h"
#include "EffectElement_RibbonTrail.h"

class EffectElement_BillboardFactory : public TEffectElementFactory<EffectElement_Billboard>
{
};

class EffectElement_EntityFactory : public TEffectElementFactory<EffectElement_Entity>
{
};

class EffectElement_ParticleFactory : public TEffectElementFactory<EffectElement_Particle>
{
};

class EffectElement_PointLightFactory : public TEffectElementFactory<EffectElement_PointLight>
{
};

class EffectElement_RibbonTrailFactory : public TEffectElementFactory<EffectElement_RibbonTrail>
{
};

namespace Ogre 
{
	SpecialEffectsPlugin* plugin = 0;
	const String sPluginName = "SpecialEffects";

	//-----------------------------------------------------------------------
	extern "C" void _OgeSFXExport dllStartPlugin(void) throw()
	{
		plugin = OGRE_NEW SpecialEffectsPlugin();
		Root::getSingleton().installPlugin(plugin);
	}

	//-----------------------------------------------------------------------
	extern "C" void _OgeSFXExport dllStopPlugin(void)
	{
		Root::getSingleton().uninstallPlugin(plugin);
		OGRE_DELETE plugin;
	}
	//---------------------------------------------------------------------
	SpecialEffectsPlugin::SpecialEffectsPlugin()
		: OgePlugin(sPluginName)
	{

	}
	//---------------------------------------------------------------------
	const String& SpecialEffectsPlugin::getName() const
	{
		return sPluginName;
	}
	//---------------------------------------------------------------------
	void SpecialEffectsPlugin::install()
	{
		OGRE_NEW SpecialEffectsManager();

		M_REG_EFFECTS_ELEMENT_FACTORY(RibbonTrail, EffectElement_RibbonTrailFactory)
		M_REG_EFFECTS_ELEMENT_FACTORY(PointLight, EffectElement_PointLightFactory)
		M_REG_EFFECTS_ELEMENT_FACTORY(ParticleSystem, EffectElement_ParticleFactory)
		M_REG_EFFECTS_ELEMENT_FACTORY(Entity, EffectElement_EntityFactory)
		M_REG_EFFECTS_ELEMENT_FACTORY(Billboard, EffectElement_BillboardFactory)
	
	}
	//---------------------------------------------------------------------
	void SpecialEffectsPlugin::initialise()
	{
		// nothing to do
	}
	//---------------------------------------------------------------------
	void SpecialEffectsPlugin::shutdown()
	{
	}
	//---------------------------------------------------------------------
	void SpecialEffectsPlugin::uninstall()
	{
		// nothing to do
		OGRE_DELETE SpecialEffectsManager::getSingletonPtr();
	}
}