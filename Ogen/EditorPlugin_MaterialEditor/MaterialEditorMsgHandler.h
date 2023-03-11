#pragma once

extern UINT ShowHideMaterialEditor;

///
/// MaterialEditorMsgHandler
///

class MaterialEditorMsgHandler
	: public CCmdTarget
	, public Singleton<MaterialEditorMsgHandler>
{
public:

	MaterialEditorMsgHandler();
	virtual ~MaterialEditorMsgHandler();

	DECLARE_DYNAMIC(MaterialEditorMsgHandler);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnShowMeshEditor();
	afx_msg void OnUpdateShowMeshEditor(CCmdUI* ui);

protected:

	CDocTemplate* _materialEditorDoc;
};