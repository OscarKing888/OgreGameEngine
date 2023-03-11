// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OGE_OPENAL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OGE_OPENAL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef OGE_OPENAL_EXPORTS
#define OGE_OPENAL_API __declspec(dllexport)
#else
#define OGE_OPENAL_API __declspec(dllimport)
#endif

// This class is exported from the Oge_OpenAL.dll
class OGE_OPENAL_API COge_OpenAL {
public:
	COge_OpenAL(void);
	// TODO: add your methods here.
};

extern OGE_OPENAL_API int nOge_OpenAL;

OGE_OPENAL_API int fnOge_OpenAL(void);
