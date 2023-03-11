
// Editor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Editor.h"
#include "MainFrm.h"

#include "EditorDoc.h"
#include "EditorView.h"
#include "EditorGameApp.h"
#include "ObjectListDlg.h"

#include "DefaultView.h"

#include "NewMapDlg.h"
#include "OpenMapDlg.h"
#include "EditorConfig.h"
#include "SplashDlg.h"

HACCEL g_hAccel = 0;
// MFCEditorApp

BEGIN_MESSAGE_MAP(MFCEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &MFCEditorApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &MFCEditorApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &MFCEditorApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// MFCEditorApp ����

template<> MFCEditorApp* Singleton<MFCEditorApp>::ms_Singleton = 0;

MFCEditorApp::MFCEditorApp()
: _quit(false)
, _ogreMapDoc(0)
, _particleDoc(0)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	m_bHiColorIcons = TRUE;

	InitQueryMask();

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� MFCEditorApp ����

MFCEditorApp theApp;

MFCEditorApp::~MFCEditorApp()
{
	OGRE_DELETE EditorApp::getSingletonPtr();

	CoUninitialize();
}

// MFCEditorApp ��ʼ��

BOOL MFCEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	DefaultLoadingListener listener;

	CWinAppEx::InitInstance();

	// ��ʼ�� OLE ��
// 	if (!AfxOleInit())
// 	{
// 		AfxMessageBox(IDP_OLE_INIT_FAILED);
// 		return FALSE;
// 	}

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(("_Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

#ifndef _DEBUG
	SplashDlg splashDlg;
#endif

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	
	g_hAccel = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));

	// create game object
	WindowEventUtilities::messagePump();
	EditorGame* game = OGRE_NEW EditorGame();
	EditorGameApp* app = OGRE_NEW EditorGameApp(game, "Editor/Main.cfg");

	OGRE_NEW EditorApp();
	EditorApp::getSingleton().SetMFCApp(this);
	EditorApp::getSingleton().SetGame(game);

	app->AppConstruct();

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������

	CDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(EditorDoc),
		RUNTIME_CLASS(MainFrame),
		RUNTIME_CLASS(EditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	_ogreMapDoc = pDocTemplate;
	
	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	// ������/��
	m_pMainWnd->DragAcceptFiles();
	MainFrame* pMainWnd = dynamic_cast<MainFrame*>(m_pMainWnd);	
	EditorView* mainView = dynamic_cast<EditorView*>(pMainWnd->GetActiveView());

	app->SetMainWnd(mainView->GetSafeHwnd());

	app->Init();

	new ObjectListDlg();// objectList;

	pMainWnd->InitPluginsToolbar();

	EventsManager::getSingleton().AddListener(pMainWnd);
	EventsManager::getSingleton().AddListener(ObjectPropertiesWnd::getSingletonPtr());

	pMainWnd->PostGameInit();
	// startup game framework
	app->Run(mainView->GetSafeHwnd());

	return TRUE;
}

int MFCEditorApp::Run()
{
	while (!_quit)
	{
		_AFX_THREAD_STATE* pState = AfxGetThreadState();

		while(::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_REMOVE))
		{
			::TranslateMessage(&(pState->m_msgCur));
			::DispatchMessage(&(pState->m_msgCur));

			if(pState->m_msgCur.message == WM_QUIT)
			{
				return 0;
			}
		}

		OnIdle(0);

		Root::getSingleton().renderOneFrame();

		Sleep(1);
	}

	return 0;
}

int MFCEditorApp::ExitInstance()
{
	ObjectListDlg::getSingleton().DestroyWindow();
	delete ObjectListDlg::getSingletonPtr();

	return CWinAppEx::ExitInstance();
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void MFCEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// MFCEditorApp �Զ������/���淽��

void MFCEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void MFCEditorApp::LoadCustomState()
{
}

void MFCEditorApp::SaveCustomState()
{
}

// MFCEditorApp ��Ϣ�������

void MFCEditorApp::OnFileNew()
{
	CWinAppEx::OnFileNew();

	static bool inited = false;
	if(inited)
	{		
		NewMapDlg dlg;
		if(dlg.DoModal() == IDOK)
		{
			EditorGame::getSingleton().CreateMap((LPCTSTR)dlg.GetMapName(),
				dlg.GetTileSize(), dlg.GetWorldSize(), dlg.GetTerrainXCnt(),
				dlg.GetTerrainZCnt(), dlg.GetTexWorldSize(),
				dlg.GetBlendMapSize(), dlg.GetColourMapSize(),
				dlg.GetLightMapSize(), dlg.GetCompositeMapDistance(),
				dlg.GetSkirtSize(), dlg.GetCompositeMapSize(),
				dlg.GetMaxPixelError(), dlg.GetCastDynamicShadows());

			CFrameWnd* frm = dynamic_cast<CFrameWnd*>(AfxGetApp()->GetMainWnd());
			frm->GetActiveDocument()->SetTitle(dlg.GetMapName());
		}
	}
	else
	{
		inited = true;
	}
}

void MFCEditorApp::OnFileOpen()
{
	OpenMapDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString mapName = dlg.GetMapName();
		EditorGame::getSingleton().LoadMap((LPCSTR)mapName);
	}
}
//--------------------------------------------------------------------------------
BOOL MFCEditorApp::OnCmdMsg(UINT nID, int nCode, void* pExtra,
					  AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if(FALSE == CWinAppEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	{
		if(EditorApp::getSingletonPtr())
		{
			return EditorApp::getSingleton().NotifyWndMsg(nID, nCode, pExtra, pHandlerInfo);
		}
	}

	return FALSE;
}
