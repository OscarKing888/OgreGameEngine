// Oge_EnginePlugin.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Oge_EnginePlugin.h"
#include "ICinemaTrack.h"

namespace Ogre 
{
	OgeEnginePlugin* plugin = 0;
	const String sPluginName = "OgeEnginePlugin";

	//-----------------------------------------------------------------------
	extern "C" void _OgeEnginePluginExport dllStartPlugin(void) throw()
	{
		plugin = OGRE_NEW OgeEnginePlugin();
		Root::getSingleton().installPlugin(plugin);
	}

	//-----------------------------------------------------------------------
	extern "C" void _OgeEnginePluginExport dllStopPlugin(void)
	{
		Root::getSingleton().uninstallPlugin(plugin);
		OGRE_DELETE plugin;
	}
	//---------------------------------------------------------------------
	OgeEnginePlugin::OgeEnginePlugin()
		: OgePlugin(sPluginName)
	{

	}
	//---------------------------------------------------------------------
	const String& OgeEnginePlugin::getName() const
	{
		return sPluginName;
	}
	//---------------------------------------------------------------------
	void OgeEnginePlugin::install()
	{
	}
	//---------------------------------------------------------------------
	void OgeEnginePlugin::initialise()
	{
		// nothing to do
		EventsManager::getSingleton().AddListener(
			CinemaManager::getSingletonPtr());
	}
	//---------------------------------------------------------------------
	void OgeEnginePlugin::shutdown()
	{
		EventsManager::getSingleton().RemoveListener(
			CinemaManager::getSingletonPtr());
	}
	//---------------------------------------------------------------------
	void OgeEnginePlugin::uninstall()
	{
	}
}