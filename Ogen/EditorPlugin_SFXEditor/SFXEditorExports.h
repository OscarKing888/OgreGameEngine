#pragma once


#ifdef SFXEDITOR_EXPORTS
#	define _SFXEditorExport __declspec(dllexport)
#else
#	define _SFXEditorExport __declspec(dllimport)
#endif
