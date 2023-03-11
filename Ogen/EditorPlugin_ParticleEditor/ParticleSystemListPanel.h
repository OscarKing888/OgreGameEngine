#pragma once

class ParticleSystemListPanel : public ResPanel_ParticleSystemBase
{
public:

	ParticleSystemListPanel();
	virtual ~ParticleSystemListPanel();


	virtual BOOL Create(LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect,
		BOOL bHasGripper, UINT nID, DWORD dwStyle, DWORD dwTabbedStyle = AFX_CBRS_REGULAR_TABS,
		DWORD dwControlBarStyle = AFX_DEFAULT_DOCKING_PANE_STYLE, CCreateContext* pContext = NULL);

	virtual void OnDBClickListItem(int item, const CString& resName);
};