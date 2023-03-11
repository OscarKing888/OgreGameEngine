#include "stdafx.h"
#include "ParticleSystemListPanel.h"
#include "ParticleEditorEvents.h"
#include "ParticleSystemEditor.h"

ParticleSystemListPanel::ParticleSystemListPanel()
{

}

ParticleSystemListPanel::~ParticleSystemListPanel()
{
	OnAppShutdown();
}

BOOL ParticleSystemListPanel::Create( LPCTSTR lpszCaption, CWnd* pParentWnd,
									 const RECT& rect, BOOL bHasGripper, UINT nID, DWORD dwStyle,
									 DWORD dwTabbedStyle /*= AFX_CBRS_REGULAR_TABS*/,
									 DWORD dwControlBarStyle /*= AFX_DEFAULT_DOCKING_PANE_STYLE*/,
									 CCreateContext* pContext /*= NULL*/ )
{
	BOOL r = ResPanel_ParticleSystemBase::Create(lpszCaption, pParentWnd, rect, bHasGripper, nID, dwStyle, dwTabbedStyle, dwControlBarStyle, pContext);
	OnAppInitCompleted();
	return r;
}

void ParticleSystemListPanel::OnDBClickListItem( int item, const CString& resName )
{
	if(resName.IsEmpty())
	{
		return;
	}

	ParticleSystemEditor& editor = ParticleSystemEditor::getSingleton();
	if(editor.IsUndoEnable())
	{
		int r = ::MessageBox(GetSafeHwnd(),
			formatString(_UIDisplayString("IDSF_SAVE_PARTICLE").c_str(),			
			editor.GetCurrentTemplate().c_str()).c_str(),
			_UIDisplayString("IDS_SAVE").c_str(), MB_YESNOCANCEL);

		if(r == IDCANCEL)
		{
			return;
		}

		if(r == IDYES)
		{
			editor.SaveAllParticleTemplate();
		}
	}

	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::EditParticleSystem, Any(String((LPCTSTR)resName)));
}