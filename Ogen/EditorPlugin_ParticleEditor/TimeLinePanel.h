#pragma once

class TimeLinePanel
	: public CDockablePane
	, public Singleton<TimeLinePanel>
{
	DECLARE_MESSAGE_MAP()

public:

	TimeLinePanel();
	virtual ~TimeLinePanel();

	CWnd* GetEditorWnd() const { return _editorWnd; }
	void SetEditorWnd(CWnd* val) { _editorWnd = val; }

	void RelayoutEditor();
	
	void ActiveEditor(const String& editorType);
	void RemoveEditor(const String& editorType);
	void AddEditor(const String& editorType, CWnd* editor);

protected:

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CWnd* _editorWnd;

	typedef map<String, CWnd*>::type EditorMap;
	EditorMap _editorMap;
	CWnd* GetEditor(const String& editorType);
};