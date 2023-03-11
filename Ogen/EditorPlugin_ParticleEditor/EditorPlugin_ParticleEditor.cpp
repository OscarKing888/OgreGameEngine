// EditorPlugin_ParticleEditor.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EditorPlugin_ParticleEditor.h"
#include <afxdllx.h>
#include "ParticleEditorMsgHandler.h"
#include "Resource.h"
#include "ParticleSystemEditor.h"
#include "ParticleEditorProxy.h"

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


Plugin_ParticleEditor* plugin = 0;
ParticleSystemEditor* particleEditor = 0;

//-----------------------------------------------------------------------
extern "C" void _PaticleEditorExport dllStartPlugin(void) throw()
{
	plugin = OGRE_NEW Plugin_ParticleEditor();
	Root::getSingleton().installPlugin(plugin);
}

//-----------------------------------------------------------------------
extern "C" void _PaticleEditorExport dllStopPlugin(void)
{
	Root::getSingleton().uninstallPlugin(plugin);
	OGRE_DELETE plugin;
}

//---------------------------------------------------------------------
Plugin_ParticleEditor::Plugin_ParticleEditor()
: EditorPlugin("MeshEditor")
{
	_description = "Particle editor plugin.";
}
//--------------------------------------------------------------------------------
void Plugin_ParticleEditor::install()
{
	particleEditor = OGRE_NEW ParticleSystemEditor();
	new ParticleEditorMsgHandler();

	ParticleSystemManager& pm = ParticleSystemManager::getSingleton();
	ParticleEditorFactoryManager& pem = ParticleEditorFactoryManager::getSingleton();
	LogSysIDf("Editor", "----------------------------------------------------------");
	LogSysIDf("Editor", "No affector editor:");
	// Check affector editor
	ParticleSystemManager::ParticleAffectorFactoryIterator afi = pm.getAffectorFactoryIterator();
	while(afi.hasMoreElements())
	{
		const String& key = afi.peekNextKey();
		if(!pem.GetAffectorFactory(key))
		{
			LogSysIDf("Editor", "\t%s", key.c_str());
		}

		afi.moveNext();
	}

	LogSysIDf("Editor", "No emitter editor:");
	// Check emitter editor
	ParticleSystemManager::ParticleEmitterFactoryIterator efi = pm.getEmitterFactoryIterator();
	while(efi.hasMoreElements())
	{
		const String& key = efi.peekNextKey();
		if(!pem.GetEmitterFactory(key))
		{
			LogSysIDf("Editor", "\t%s", key.c_str());
		}

		efi.moveNext();
	}

	LogSysIDf("Editor", "No renderer editor:");
	// Check renderer editor
	ParticleSystemManager::ParticleRendererFactoryIterator rfi = pm.getRendererFactoryIterator();
	while(rfi.hasMoreElements())
	{
		const String& key = rfi.peekNextKey();
		if(!pem.GetRendererFactory(key))
		{
			LogSysIDf("Editor", "\t%s", key.c_str());
		}

		rfi.moveNext();
	}
	
	LogSysIDf("Editor", "----------------------------------------------------------");
}
//---------------------------------------------------------------------
void Plugin_ParticleEditor::initialise()
{
}
//---------------------------------------------------------------------
void Plugin_ParticleEditor::shutdown()
{
}
//---------------------------------------------------------------------
void Plugin_ParticleEditor::uninstall()
{
	DestroyParticleEditor();

}
//---------------------------------------------------------------------
void Plugin_ParticleEditor::onCreateToolsCategory(
	CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel )
{
	HICON icon = AfxGetApp()->LoadIcon(IDR_PARTICLE_WND);
	CMFCRibbonButton* btn = new CMFCRibbonButton(ShowHideParticleEditor, 
		_UIDisplayString("IDS_PARTICLE_EDITOR").c_str(), icon);

	mainPanel->Add(btn);
}
//--------------------------------------------------------------------------------
void Plugin_ParticleEditor::OnAppShutdown()
{
	delete ParticleEditorMsgHandler::getSingletonPtr();
	
	DestroyParticleEditor();
}

void Plugin_ParticleEditor::DestroyParticleEditor()
{
	if(particleEditor)
	{
		OGRE_DELETE particleEditor;
		particleEditor = 0;
	}
}
//---------------------------------------------------------------------