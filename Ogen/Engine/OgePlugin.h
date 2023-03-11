#pragma once
#include "EnginePrerequisites.h"
#include <OgrePlugin.h>

class _EngineExport OgePlugin : public Plugin
{
public:

	OgePlugin(const Ogre::String& name);
	virtual ~OgePlugin();
	const Ogre::String& getName() const;
	virtual void install();
	virtual void uninstall();
	virtual void initialise();
	virtual void shutdown();

protected:

	Ogre::String _name;
};