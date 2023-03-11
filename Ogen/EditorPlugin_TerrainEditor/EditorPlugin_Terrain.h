#pragma once

///
/// Plugin_TerrainEditor
///
class Plugin_TerrainEditor : public EditorPlugin
{
public:
	Plugin_TerrainEditor();

	/// @copydoc Plugin::install
	void install();

	/// @copydoc Plugin::initialise
	void initialise();

	/// @copydoc Plugin::shutdown
	void shutdown();

	/// @copydoc Plugin::uninstall
	void uninstall();

	virtual void onCreateEditStates(IStateManager* sm);

	virtual void onMapCreate(const String& mapName);
	virtual void onMapLoad(const String& mapName);
	virtual void onMapUnload(const String& mapName);

protected:

};