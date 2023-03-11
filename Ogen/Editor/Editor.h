
// Editor.h : Editor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// MFCEditorApp:
// �йش����ʵ�֣������ Editor.cpp
//

extern HACCEL g_hAccel;

class MFCEditorApp : public CWinAppEx
	, public Singleton<MFCEditorApp>
{
public:
	MFCEditorApp();
	virtual ~MFCEditorApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance(); 
	virtual int Run();

	bool _quit;

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	CDocTemplate* _ogreMapDoc;
	CDocTemplate* _particleDoc;
	CDocTemplate* _modelEditorDoc;
	CDocTemplate* _materialEditorDoc;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
};

extern MFCEditorApp theApp;
