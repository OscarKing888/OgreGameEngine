// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PLUGIN_PARTICLEEX_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// _OgreExExport functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once

#include "OgreExPrerequisites.h"
#include "OgrePlugin.h"

namespace Ogre
{

	/** Plugin instance for ParticleFX Manager */
	class OgreEXPlugin : public Plugin
	{
	public:
		OgreEXPlugin();


		/// @copydoc Plugin::getName
		const String& getName() const;

		/// @copydoc Plugin::install
		void install();

		/// @copydoc Plugin::initialise
		void initialise();

		/// @copydoc Plugin::shutdown
		void shutdown();

		/// @copydoc Plugin::uninstall
		void uninstall();
	protected:
		
		vector<ParticleEmitterFactory*>::type mEmitterFactories;
		vector<ParticleAffectorFactory*>::type mAffectorFactories;
		vector<ParticleSystemRendererFactory*>::type mRendererFactories;
		vector<Ogre::MovableObjectFactory*>::type mMovableObjectFactoryList;
	};
}
