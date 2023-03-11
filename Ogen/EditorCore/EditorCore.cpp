// EditorCore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EditorCore.h"
#include <afxwin.h>
#include <afxdllx.h>

// The one and only application object
//
static AFX_EXTENSION_MODULE EditorCoreDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		debugTrace("EditorCore.DLL Initializing!\n");

		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(EditorCoreDLL, hInstance))
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

		new CDynLinkLibrary(EditorCoreDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		debugTrace("EditorCore.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(EditorCoreDLL);
	}
	return 1;   // ok
}