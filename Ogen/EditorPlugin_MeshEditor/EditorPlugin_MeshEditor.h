#pragma once

///
/// Plugin_MeshEditor
///
class Plugin_MeshEditor : public EditorPlugin
{
public:
	Plugin_MeshEditor();

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
};