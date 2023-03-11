#pragma once

///
/// Plugin_ParticleEditor
///
class Plugin_ParticleEditor : public EditorPlugin
{
public:
	Plugin_ParticleEditor();

	/// @copydoc Plugin::install
	void install();

	/// @copydoc Plugin::initialise
	void initialise();

	/// @copydoc Plugin::shutdown
	void shutdown();

	/// @copydoc Plugin::uninstall
	void uninstall();

	/// @copydoc EditorPlugin::onCreateToolsCategory
	virtual void onCreateToolsCategory(CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel);

	virtual void OnAppShutdown();
	void DestroyParticleEditor();
};