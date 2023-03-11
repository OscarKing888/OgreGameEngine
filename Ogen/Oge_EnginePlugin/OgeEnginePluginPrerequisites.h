#pragma once
#ifdef OGE_ENGINEPLUGIN_EXPORTS
#	define _OgeEnginePluginExport __declspec(dllexport)
#else
#	define _OgeEnginePluginExport __declspec(dllimport)
#endif

#include "Core/Core.h"
#include "Engine/EnginePrerequisites.h"
#include "Engine/Events.h"