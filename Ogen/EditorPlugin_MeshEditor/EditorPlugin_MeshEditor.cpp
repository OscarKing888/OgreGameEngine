// EditorPlugin_MeshEditor.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EditorPlugin_MeshEditor.h"
#include "MeshEditorMsgHandler.h"
#include <afxdllx.h>
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

Plugin_MeshEditor* plugin = 0;

//----------------------------------------------------------------------------------
extern "C" void _MeshEditorExport dllStartPlugin(void) throw()
{
	plugin = OGRE_NEW Plugin_MeshEditor();
	Root::getSingleton().installPlugin(plugin);
}

//----------------------------------------------------------------------------------
extern "C" void _MeshEditorExport dllStopPlugin(void)
{
	Root::getSingleton().uninstallPlugin(plugin);
	OGRE_DELETE plugin;
}

//--------------------------------------------------------------------------------
Plugin_MeshEditor::Plugin_MeshEditor()
: EditorPlugin("MeshEditor")
{
	_description = "Mesh editor plugin.";
}
//--------------------------------------------------------------------------------
void Plugin_MeshEditor::install()
{
	new MeshEditorMsgHandler();
}
//--------------------------------------------------------------------------------
void Plugin_MeshEditor::initialise()
{
}
//--------------------------------------------------------------------------------
void Plugin_MeshEditor::shutdown()
{
}
//--------------------------------------------------------------------------------
void Plugin_MeshEditor::uninstall()
{
}
//--------------------------------------------------------------------------------
void Plugin_MeshEditor::onCreateToolsCategory(
	CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel )
{
	HICON icon = AfxGetApp()->LoadIcon(IDR_MODELEDITOR);
	CMFCRibbonButton* btn = new CMFCRibbonButton(ShowHideMeshEditor, 
		_UIDisplayString("ID_MESH_EDITOR").c_str(), icon);

	mainPanel->Add(btn);
}
//--------------------------------------------------------------------------------
void Plugin_MeshEditor::OnAppShutdown()
{
	delete MeshEditorMsgHandler::getSingletonPtr();
}
//--------------------------------------------------------------------------------