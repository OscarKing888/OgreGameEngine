// Oge_OpenAL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Oge_OpenAL.h"


// This is an example of an exported variable
OGE_OPENAL_API int nOge_OpenAL=0;

// This is an example of an exported function.
OGE_OPENAL_API int fnOge_OpenAL(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see Oge_OpenAL.h for the class definition
COge_OpenAL::COge_OpenAL()
{
	return;
}
