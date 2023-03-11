#pragma once

#ifdef EDITORPLUGIN_OBJECTEDITOR_EXPORTS
#	define _ObjectEditExport __declspec(dllexport)
#else
#	define _ObjectEditExport __declspec(dllimport)
#endif


#include "EditorCore/EditorCore.h"
