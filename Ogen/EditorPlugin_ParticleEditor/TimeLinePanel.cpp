#include "stdafx.h"
#include "TimeLinePanel.h"

template<> TimeLinePanel* Singleton<TimeLinePanel>::ms_Singleton = 0;

TimeLinePanel::TimeLinePanel()
: _editorWnd(0)
{
}
//--------------------------------------------------------------------------------
TimeLinePanel::~TimeLinePanel()
{
}
//--------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(TimeLinePanel, CDockablePane)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
void TimeLinePanel::OnSize( UINT nType, int cx, int cy )
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	RelayoutEditor();

}
//--------------------------------------------------------------------------------
void TimeLinePanel::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	if(_editorWnd && _editorWnd->GetSafeHwnd())
	{
		_editorWnd->SetFocus();
	}
}
//--------------------------------------------------------------------------------
void TimeLinePanel::RelayoutEditor()
{
	CRect rcClient;
	GetClientRect(rcClient);

	if(_editorWnd && _editorWnd->GetSafeHwnd())
	{
		_editorWnd->MoveWindow(&rcClient);
	}
}
//--------------------------------------------------------------------------------
void TimeLinePanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}
//--------------------------------------------------------------------------------
BOOL TimeLinePanel::OnEraseBkgnd(CDC* pDC)
{

	return CWnd::OnEraseBkgnd(pDC);
}
//--------------------------------------------------------------------------------
void TimeLinePanel::ActiveEditor( const String& editorType )
{
	_editorWnd = GetEditor(editorType);
}
//--------------------------------------------------------------------------------
void TimeLinePanel::AddEditor( const String& editorType, CWnd* editor )
{
	_editorMap[editorType] = editor;
}
//--------------------------------------------------------------------------------
void TimeLinePanel::RemoveEditor( const String& editorType )
{
	if(GetEditor(editorType) == _editorWnd)
	{
		_editorWnd = 0;
	}
	
	EditorMap::iterator it = _editorMap.find(editorType);
	if(it != _editorMap.end())
	{
		_editorMap.erase(it);
	}
}
//--------------------------------------------------------------------------------
CWnd* TimeLinePanel::GetEditor( const String& editorType )
{
	EditorMap::iterator it = _editorMap.find(editorType);
	if(it != _editorMap.end())
	{
		return it->second;
	}

	return 0;
}