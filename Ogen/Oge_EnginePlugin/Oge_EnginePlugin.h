#include "OgeEnginePluginPrerequisites.h"
#include "Engine/OgePlugin.h"

namespace Ogre
{

	/** Plugin instance for ParticleFX Manager */
	class OgeEnginePlugin : public OgePlugin
	{
	public:
		OgeEnginePlugin();


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
