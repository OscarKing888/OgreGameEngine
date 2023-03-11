// EditorPlugin_MaterialEditor.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "EditorPlugin_MaterialEditor.h"
#include "MaterialEditor.h"
#include "MaterialEditorMsgHandler.h"
#include "Resource.h"

AFX_EXTENSION_MODULE ObjectEditorDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//TRACE0("ObjectEditor.DLL Initializing!\n");

		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(ObjectEditorDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(ObjectEditorDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		//TRACE0("ObjectEditor.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(ObjectEditorDLL);
	}
	return 1;   // ok
}

///
/// Plugin_MaterialEditor
///
class Plugin_MaterialEditor : public EditorPlugin
{
public:
	Plugin_MaterialEditor();

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
Plugin_MaterialEditor* plugin = 0;
//--------------------------------------------------------------------------------
extern "C" void _MaterialEditorExport dllStartPlugin(void) throw()
{
	plugin = OGRE_NEW Plugin_MaterialEditor();
	Root::getSingleton().installPlugin(plugin);
}
//--------------------------------------------------------------------------------
extern "C" void _MaterialEditorExport dllStopPlugin(void)
{
	Root::getSingleton().uninstallPlugin(plugin);
	OGRE_DELETE plugin;
}
//--------------------------------------------------------------------------------
Plugin_MaterialEditor::Plugin_MaterialEditor()
: EditorPlugin("MaterialEditor")
{
	_description = "Special Effects Editor Plugin.";
}
//--------------------------------------------------------------------------------
void Plugin_MaterialEditor::install()
{
	OGRE_NEW MaterialEditor();
	new MaterialEditorMsgHandler();

	//M_REG_TYPEUIEDITOR_FACTORY(TerrainBrush, PropertyEditor_TerrainBrushFactory);
}
//--------------------------------------------------------------------------------
void Plugin_MaterialEditor::initialise()
{
	// nothing to do
}
//--------------------------------------------------------------------------------
void Plugin_MaterialEditor::shutdown()
{
	// nothing to do
	//TerrainBrushManager::getSingleton().ShutDown();
}
//--------------------------------------------------------------------------------
void Plugin_MaterialEditor::uninstall()
{

}
//--------------------------------------------------------------------------------
void Plugin_MaterialEditor::onCreateToolsCategory(
	CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel )
{
	HICON icon = AfxGetApp()->LoadIcon(IDR_MATERIAL_EDITOR);
	CMFCRibbonButton* btn = new CMFCRibbonButton(ShowHideMaterialEditor, 
		_UIDisplayString("IDR_MATERIAL_EDITOR").c_str(), icon);

	mainPanel->Add(btn);
}
//--------------------------------------------------------------------------------
void Plugin_MaterialEditor::OnAppShutdown()
{
	delete MaterialEditorMsgHandler::getSingletonPtr();
	OGRE_DELETE MaterialEditor::getSingletonPtr();
}
//--------------------------------------------------------------------------------