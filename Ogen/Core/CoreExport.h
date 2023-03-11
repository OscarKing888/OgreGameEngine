#pragma once

#ifdef CORE_EXPORTS
#	define _CoreExport __declspec(dllexport)
#	define CORE_TEMPLATE_API
#else
#	define _CoreExport __declspec(dllimport)
#	define CORE_TEMPLATE_API extern
#endif

#ifndef CORE_EXPORTS
#	ifdef _DEBUG
#		pragma comment(lib, "Core_Debug.lib")
#	else
#		pragma comment(lib, "Core_Release.lib")
#	endif
#endif