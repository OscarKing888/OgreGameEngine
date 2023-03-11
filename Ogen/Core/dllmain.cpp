#include "stdafx.h"
#include "OSAPI.h"

bool initLocale()
{
	DEBUG_TRACE("[Core initLocale()] set LC_ALL to NULL");
	setlocale(LC_ALL, "");
	return true;
}

static bool g_initLocale = initLocale();

BOOL APIENTRY DllMain( HMODULE /*hModule*/,
					  DWORD  ul_reason_for_call,
					  LPVOID /*lpReserved*/
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}