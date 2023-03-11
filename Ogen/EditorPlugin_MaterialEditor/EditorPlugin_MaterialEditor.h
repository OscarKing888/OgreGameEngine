// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EDITORPLUGIN_MATERIALEDITOR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EDITORPLUGIN_MATERIALEDITOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef EDITORPLUGIN_MATERIALEDITOR_EXPORTS
#define EDITORPLUGIN_MATERIALEDITOR_API __declspec(dllexport)
#else
#define EDITORPLUGIN_MATERIALEDITOR_API __declspec(dllimport)
#endif

// This class is exported from the EditorPlugin_MaterialEditor.dll
class EDITORPLUGIN_MATERIALEDITOR_API CEditorPlugin_MaterialEditor {
public:
	CEditorPlugin_MaterialEditor(void);
	// TODO: add your methods here.
};

extern EDITORPLUGIN_MATERIALEDITOR_API int nEditorPlugin_MaterialEditor;

EDITORPLUGIN_MATERIALEDITOR_API int fnEditorPlugin_MaterialEditor(void);
