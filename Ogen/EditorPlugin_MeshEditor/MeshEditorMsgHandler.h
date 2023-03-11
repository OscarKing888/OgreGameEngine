#pragma once

extern UINT ShowHideMeshEditor;

///
/// MeshEditorMsgHandler
///

class MeshEditorMsgHandler
	: public CCmdTarget
	, public Singleton<MeshEditorMsgHandler>
{
public:

	MeshEditorMsgHandler();
	virtual ~MeshEditorMsgHandler();

	DECLARE_DYNAMIC(MeshEditorMsgHandler);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnShowMeshEditor();
	afx_msg void OnUpdateShowMeshEditor(CCmdUI* ui);

protected:

	CDocTemplate* _modelEditorDoc;
};