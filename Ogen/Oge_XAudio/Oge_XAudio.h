// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OGE_XAUDIO_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OGE_XAUDIO_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef OGE_XAUDIO_EXPORTS
#define OGE_XAUDIO_API __declspec(dllexport)
#else
#define OGE_XAUDIO_API __declspec(dllimport)
#endif

// This class is exported from the Oge_XAudio.dll
class OGE_XAUDIO_API COge_XAudio {
public:
	COge_XAudio(void);
	// TODO: add your methods here.
};

extern OGE_XAUDIO_API int nOge_XAudio;

OGE_XAUDIO_API int fnOge_XAudio(void);
