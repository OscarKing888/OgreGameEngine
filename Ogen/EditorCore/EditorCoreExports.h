#pragma once

#ifdef EDITORCORE_EXPORTS
#	define _EditorCoreExport __declspec(dllexport)
#else
#	define _EditorCoreExport __declspec(dllimport)
#endif

extern _EditorCoreExport const String EditorLogID;
extern _EditorCoreExport const String EditorResourceGroup;

#ifndef EDITORCORE_EXPORTS
#	ifdef _DEBUG
#		pragma comment(lib, "EditorCore_Debug.lib")
#	else
#		pragma comment(lib, "EditorCore_Release.lib")
#	endif
#endif