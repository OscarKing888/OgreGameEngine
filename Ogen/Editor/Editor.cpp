
// Editor.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &MFCEditorApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &MFCEditorApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// MFCEditorApp 构造

template<> MFCEditorApp* Singleton<MFCEditorApp>::ms_Singleton = 0;

MFCEditorApp::MFCEditorApp()
: _quit(false)
, _ogreMapDoc(0)
, _particleDoc(0)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	m_bHiColorIcons = TRUE;

	InitQueryMask();

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 MFCEditorApp 对象

MFCEditorApp theApp;

MFCEditorApp::~MFCEditorApp()
{
	OGRE_DELETE EditorApp::getSingletonPtr();

	CoUninitialize();
}

// MFCEditorApp 初始化

BOOL MFCEditorApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	DefaultLoadingListener listener;

	CWinAppEx::InitInstance();

	// 初始化 OLE 库
// 	if (!AfxOleInit())
// 	{
// 		AfxMessageBox(IDP_OLE_INIT_FAILED);
// 		return FALSE;
// 	}

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(("_应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

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

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接

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
	
	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	// 启用拖/放
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



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void MFCEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// MFCEditorApp 自定义加载/保存方法

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

// MFCEditorApp 消息处理程序

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
