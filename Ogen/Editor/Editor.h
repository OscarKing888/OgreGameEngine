
// Editor.h : Editor 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// MFCEditorApp:
// 有关此类的实现，请参阅 Editor.cpp
//

extern HACCEL g_hAccel;

class MFCEditorApp : public CWinAppEx
	, public Singleton<MFCEditorApp>
{
public:
	MFCEditorApp();
	virtual ~MFCEditorApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance(); 
	virtual int Run();

	bool _quit;

// 实现
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
