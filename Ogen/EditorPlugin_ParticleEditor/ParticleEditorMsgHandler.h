#pragma once

extern UINT ShowHideParticleEditor;

///
/// ParticleEditorMsgHandler
///

class ParticleEditorMsgHandler
	: public CCmdTarget
	, public Singleton<ParticleEditorMsgHandler>
{
public:

	ParticleEditorMsgHandler();
	virtual ~ParticleEditorMsgHandler();

	DECLARE_DYNAMIC(ParticleEditorMsgHandler);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnShowMeshEditor();
	afx_msg void OnUpdateShowMeshEditor(CCmdUI* ui);

protected:

	CDocTemplate* _particleEditorDoc;
};