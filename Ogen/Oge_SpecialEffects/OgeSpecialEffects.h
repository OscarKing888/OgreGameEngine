// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OGE_SPECIALEFFECTS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OGE_SPECIALEFFECTS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once

#include "OgeSpecialEffectsPrerequisites.h"
#include "Engine/OgePlugin.h"

namespace Ogre
{

	/** Plugin instance for ParticleFX Manager */
	class SpecialEffectsPlugin : public OgePlugin
	{
	public:
		SpecialEffectsPlugin();


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

	};
}
