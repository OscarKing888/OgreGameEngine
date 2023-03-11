#pragma once


#ifdef EDITORPLUGIN_TERRAIN_EXPORTS
#	define _TerrainEditorExport __declspec(dllexport)
#else
#	define _TerrainEditorExport __declspec(dllimport)
#endif
