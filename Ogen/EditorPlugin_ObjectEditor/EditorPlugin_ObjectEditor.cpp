// EditorPlugin_ObjectEditor.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EditorPlugin_ObjectEditor.h"

#include "EditState_Select.h"
#include "EditState_Create.h"
#include <afxdllx.h>

ObjectEditorPlugin* plugin = 0;
const String sPluginName = "ObjectEdit";

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


//-----------------------------------------------------------------------
extern "C" void _ObjectEditExport dllStartPlugin(void) throw()
{
	plugin = OGRE_NEW ObjectEditorPlugin();
	Root::getSingleton().installPlugin(plugin);
}

//-----------------------------------------------------------------------
extern "C" void _ObjectEditExport dllStopPlugin(void)
{
	Root::getSingleton().uninstallPlugin(plugin);
	OGRE_DELETE plugin;
}
//---------------------------------------------------------------------
ObjectEditorPlugin::ObjectEditorPlugin()
: EditorPlugin(sPluginName)
{
	_description = "Game object edit states plugin.";
}
//---------------------------------------------------------------------
ObjectEditorPlugin::~ObjectEditorPlugin()
{

}
//---------------------------------------------------------------------
void ObjectEditorPlugin::install()
{
}
//---------------------------------------------------------------------
void ObjectEditorPlugin::initialise()
{
}
//---------------------------------------------------------------------
void ObjectEditorPlugin::shutdown()
{
}
//---------------------------------------------------------------------
void ObjectEditorPlugin::uninstall()
{
}
//---------------------------------------------------------------------
void ObjectEditorPlugin::onCreateEditorMainWnd( CFrameWndEx* mainWnd )
{

}
//---------------------------------------------------------------------
void ObjectEditorPlugin::onCreateEditorMainToolbar( CMFCRibbonBar* mainToolbar )
{

}
//---------------------------------------------------------------------
void ObjectEditorPlugin::onCreateEditStates( IStateManager* sm )
{
	EditorApp::getSingleton().AddState(OGRE_NEW EditState_Select(sm));
	EditorApp::getSingleton().AddState(OGRE_NEW EditState_Create(sm));

	EditorApp::getSingleton().ActiveState(EditStateID_Select);
}
//---------------------------------------------------------------------
void ObjectEditorPlugin::onWindowResized( RenderWindow* rw )
{

}
//---------------------------------------------------------------------
