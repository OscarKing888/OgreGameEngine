// EditorPlugin_SFXEditor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SFXEditorMsgHandler.h"
#include "Resource.h"
#include "SFXEditor.h"

///
/// Plugin_SFXEditor
///
class Plugin_SFXEditor : public EditorPlugin
{
public:
	Plugin_SFXEditor();

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
//--------------------------------------------------------------------------------
Plugin_SFXEditor* plugin = 0;
//--------------------------------------------------------------------------------
extern "C" void _SFXEditorExport dllStartPlugin(void) throw()
{
	plugin = OGRE_NEW Plugin_SFXEditor();
	Root::getSingleton().installPlugin(plugin);
}
//--------------------------------------------------------------------------------
extern "C" void _SFXEditorExport dllStopPlugin(void)
{
	Root::getSingleton().uninstallPlugin(plugin);
	OGRE_DELETE plugin;
}
//--------------------------------------------------------------------------------
Plugin_SFXEditor::Plugin_SFXEditor()
: EditorPlugin("SFXEditor")
{
	_description = "Special Effects Editor Plugin.";
}
//--------------------------------------------------------------------------------
void Plugin_SFXEditor::install()
{
	OGRE_NEW SFXEditor();
	new SFXEditorMsgHandler();

	//M_REG_TYPEUIEDITOR_FACTORY(TerrainBrush, PropertyEditor_TerrainBrushFactory);
}
//--------------------------------------------------------------------------------
void Plugin_SFXEditor::initialise()
{
	// nothing to do
}
//--------------------------------------------------------------------------------
void Plugin_SFXEditor::shutdown()
{
	// nothing to do
	//TerrainBrushManager::getSingleton().ShutDown();
}
//--------------------------------------------------------------------------------
void Plugin_SFXEditor::uninstall()
{

}
//--------------------------------------------------------------------------------
void Plugin_SFXEditor::onCreateToolsCategory(
	CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel )
{
	HICON icon = AfxGetApp()->LoadIcon(IDR_SFXEDITOR);
	CMFCRibbonButton* btn = new CMFCRibbonButton(ShowHideSFXEditor, 
		_UIDisplayString("ID_SFX_EDITOR").c_str(), icon);

	mainPanel->Add(btn);
}
//--------------------------------------------------------------------------------
void Plugin_SFXEditor::OnAppShutdown()
{
	delete SFXEditorMsgHandler::getSingletonPtr();
	OGRE_DELETE SFXEditor::getSingletonPtr();
}
//--------------------------------------------------------------------------------