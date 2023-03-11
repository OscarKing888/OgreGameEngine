// Oge_XAudio.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Oge_XAudio.h"


// This is an example of an exported variable
OGE_XAUDIO_API int nOge_XAudio=0;

// This is an example of an exported function.
OGE_XAUDIO_API int fnOge_XAudio(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see Oge_XAudio.h for the class definition
COge_XAudio::COge_XAudio()
{
	return;
}
