#pragma once

#ifdef PLUGIN_PARTICEX_EXPORTS
#	define _ParticleXExport __declspec(dllexport)
#else
#	define _ParticleXExport __declspec(dllimport)
#endif
