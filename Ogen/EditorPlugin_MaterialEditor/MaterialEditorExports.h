#pragma once


#ifdef EDITORPLUGIN_MATERIALEDITOR_EXPORTS
#	define _MaterialEditorExport __declspec(dllexport)
#else
#	define _MaterialEditorExport __declspec(dllimport)
#endif
