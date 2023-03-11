#pragma once

#ifdef CINEMAEDITOR_EXPORTS
#	define _CinemaEditorExport __declspec(dllexport)
#else
#	define _CinemaEditorExport __declspec(dllimport)
#endif


#include "EditorCore/EditorCore.h"
#include "Oge_EnginePlugin/CinemaExports.h"
