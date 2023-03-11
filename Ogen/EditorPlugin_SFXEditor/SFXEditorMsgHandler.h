#pragma once

extern UINT ShowHideSFXEditor;

///
/// SFXEditorMsgHandler
///

class SFXEditorMsgHandler
	: public CCmdTarget
	, public Singleton<SFXEditorMsgHandler>
{
public:

	SFXEditorMsgHandler();
	virtual ~SFXEditorMsgHandler();

	DECLARE_DYNAMIC(SFXEditorMsgHandler);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnShowMeshEditor();
	afx_msg void OnUpdateShowMeshEditor(CCmdUI* ui);

protected:

	CDocTemplate* _sfxEditorDoc;
};