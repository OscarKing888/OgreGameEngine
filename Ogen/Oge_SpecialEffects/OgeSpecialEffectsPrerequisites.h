#pragma once
#include <Engine/Engine.h>

#ifdef OGE_SPECIALEFFECTS_EXPORTS
#	define _OgeSFXExport __declspec(dllexport)
#else
#	define _OgeSFXExport __declspec(dllimport)
#endif

extern _OgeSFXExport const String SFXLogID;

#ifndef OGE_SPECIALEFFECTS_EXPORTS
#	ifdef _DEBUG
#		pragma comment(lib, "Oge_SpecialEffects_Debug.lib")
#	else
#		pragma comment(lib, "Oge_SpecialEffects_Release.lib")
#	endif
#endif