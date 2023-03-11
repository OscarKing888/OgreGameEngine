
// MainFrm.cpp : MainFrame 类的实现
//

#include "stdafx.h"
#include "Editor.h"

#include "MainFrm.h"
#include "EditorView.h"
#include "Resource.h"
#include "EditorGameApp.h"
#include "ObjectListDlg.h"

#define ID_EDITSTATE_END ID_EDITSTATE_TERRAIN_PATH_FINDER

#define ON_UPDATE_COMMAND_UI_RANGEEX(id, idLast, memberFxn) \
	{ WM_COMMAND, CN_UPDATE_COMMAND_UI, (WORD)id, (WORD)idLast, AfxSigCmdUI_RANGE, \
	(AFX_PMSG) \
	(static_cast< void (AFX_MSG_CALL CCmdTarget::*)(CCmdUI*, UINT) > \
	(memberFxn)) },


template<> MainFrame* Singleton<MainFrame>::ms_Singleton = 0;
// MainFrame

IMPLEMENT_DYNCREATE(MainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;


#define COMPOSITOR_COUNT 100
UINT MainFrame::CompositorStartID = GetAutoWMID();

bool AdvanceWMID()
{
	for (int i = 0; i < COMPOSITOR_COUNT; ++i)
	{
		GetAutoWMID();
	}
	return true;
}

bool callFun = AdvanceWMID();

UINT MainFrame::CompositorEndID = CompositorStartID + COMPOSITOR_COUNT;


BEGIN_MESSAGE_MAP(MainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &MainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &MainFrame::OnToolbarCreateNew)
	
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, &MainFrame::OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &MainFrame::OnUpdateFileOpen)

	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &MainFrame::OnApplicationLook)	
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &MainFrame::OnUpdateApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VP_MAX, ID_VP_2x2, &MainFrame::OnUpdateViewport)
	
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &MainFrame::OnUpdateUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &MainFrame::OnUpdateRedo)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &MainFrame::OnUpdateSave)
	
	ON_COMMAND_RANGE(EDITOR_ID_START, EDITOR_ID_END, &MainFrame::OnActiveEditState)
	ON_UPDATE_COMMAND_UI_RANGE(EDITOR_ID_START, EDITOR_ID_END, &MainFrame::OnUpdateEditState)

	ON_COMMAND(ID_VP_MAX, &MainFrame::OnViewportMax)
	ON_COMMAND(ID_VP_1x2, &MainFrame::OnViewport1x2)
	ON_COMMAND(ID_VP_2x2, &MainFrame::OnViewport2x2)
	ON_COMMAND(ID_EDIT_UNDO, &MainFrame::OnUndo)
	ON_COMMAND(ID_EDIT_REDO, &MainFrame::OnRedo)
	ON_COMMAND(ID_SHOW_RES_MANAGER, &MainFrame::OnShowResManager)
	ON_COMMAND(ID_MATERIAL_DLG, &MainFrame::OnShowMaterialManager)
	ON_COMMAND(ID_RESPANEL_TEXTURE, &MainFrame::OnShowTextureManager)
	
	ON_COMMAND(ID_SHOW_OBJECTS, &MainFrame::OnShowObjects)

	ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_CLEAR, ID_EDIT_CUT, &MainFrame::OnUpdateEditCopyDelete)

	ON_UPDATE_COMMAND_UI(ID_SHOW_BOUDNING_BOX, &MainFrame::OnUpdateShowBoundingBox)
	ON_COMMAND(ID_SHOW_BOUDNING_BOX, &MainFrame::OnShowBoundingBox)


	ON_UPDATE_COMMAND_UI(ID_CLAMP_ON_TERRAIN, &MainFrame::OnUpdateToggleClampOnTerrain)
	ON_COMMAND(ID_CLAMP_ON_TERRAIN, &MainFrame::OnToggleClampOnTerrain)

	ON_UPDATE_COMMAND_UI(ID_SHOW_WIRE, &MainFrame::OnUpdateShowWireFrame)
	ON_COMMAND(ID_SHOW_WIRE, &MainFrame::OnToggleShowWireFrame)

	ON_UPDATE_COMMAND_UI(ID_ALIGN_TERRAIN_NORMAL, &MainFrame::OnUpdateAlignTerrainNormal)
	ON_COMMAND(ID_ALIGN_TERRAIN_NORMAL, &MainFrame::OnToggleAlignTerrainNormal)
	
	ON_UPDATE_COMMAND_UI(ID_APPLY_CAMERA, &MainFrame::OnUpdateApplyCamera)
	ON_COMMAND(ID_APPLY_CAMERA, &MainFrame::OnApplyCamera)

	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECT_CAMERA, &MainFrame::OnUpdateViewSelectedCamera)
	ON_COMMAND(ID_VIEW_SELECT_CAMERA, &MainFrame::OnViewSelectedCamera)
	
	
	ON_COMMAND(ID_CURRENT_CAMERA, &MainFrame::OnEditCurrentCamera)
	
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)

	ON_COMMAND(ID_MATERIAL_SCHEME, OnMaterialScheme)
	ON_CBN_SELENDOK(ID_MATERIAL_SCHEME, OnMaterialScheme)

	ON_COMMAND(ID_PICK_FILTER, OnPickFilter)
	ON_CBN_SELENDOK(ID_PICK_FILTER, OnPickFilter)
	

	ON_COMMAND(ID_COMPOSITOR, OnCompositorSelect)
	ON_CBN_SELENDOK(ID_COMPOSITOR, OnCompositorSelect)

	ON_COMMAND(IDC_SNAPSHOT_MINIMAP, &MainFrame::OnSnapshotMiniMap)
	ON_COMMAND(ID_SNAP_CONFIG, &MainFrame::OnSnapshotConfig)
	ON_COMMAND(ID_SNAP_ANGLED, &MainFrame::OnSnapshot)
	

	ON_COMMAND(ID_TOOLS_PARTICLEEDITOR, &MainFrame::OnShowParticleEditor)
	ON_COMMAND(ID_SHOW_MODEL_EDITOR, &MainFrame::OnShowModelEditor)	
	ON_COMMAND(ID_TOOLS_MATERIAL_EDITOR, &MainFrame::OnShowModelMaterialEditor)	

	ON_COMMAND(ID_PICK_FILTER, &MainFrame::OnShowPickFilter)

	ON_COMMAND_RANGE(ID_CAMERASPEED_SLOW, ID_CAMERASPEED_FAST, &MainFrame::OnCameraSpeed)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CAMERASPEED_SLOW, ID_CAMERASPEED_FAST, &MainFrame::OnUpdateCameraSpeed)


	ON_COMMAND(IDRIB_SCENE_PROP, &MainFrame::OnShowProp_Scene)
	ON_UPDATE_COMMAND_UI(IDRIB_SCENE_PROP, &MainFrame::OnUpdateUI_ShowProp_Scene)

	ON_COMMAND(IDRIB_SKYBOX_PROP, &MainFrame::OnShowProp_SkyBox)
	ON_UPDATE_COMMAND_UI(IDRIB_SKYBOX_PROP, &MainFrame::OnUpdateUI_ShowProp_SkyBox)

	ON_COMMAND(IDRIB_MAINLIGHT_PROP, &MainFrame::OnShowProp_MainLight)
	ON_UPDATE_COMMAND_UI(IDRIB_MAINLIGHT_PROP, &MainFrame::OnUpdateUI_ShowProp_MainLight)

	ON_COMMAND(IDRIB_EDITORCFG_PROP, &MainFrame::OnShowProp_EditorConfig)
	

	ON_COMMAND(IDRIB_MOVE_CLAMP_TERRAIN, &MainFrame::OnCheck_Move_ClampTerrain)
	ON_UPDATE_COMMAND_UI(IDRIB_MOVE_CLAMP_TERRAIN, &MainFrame::OnUpdateCheck_Move_ClampTerrain)

	ON_COMMAND(IDRIB_MOVE_ALIGN_NORMAL, &MainFrame::OnCheck_Move_AlignNormal)
	ON_UPDATE_COMMAND_UI(IDRIB_MOVE_ALIGN_NORMAL, &MainFrame::OnUpdateCheck_Move_AlignNormal)

	ON_COMMAND(IDRIB_MOVE_SNAP_GRID, &MainFrame::OnCheck_Move_SnapToGird)
	ON_UPDATE_COMMAND_UI(IDRIB_MOVE_SNAP_GRID, &MainFrame::OnUpdateCheck_Move_SnapToGird)

	ON_COMMAND(IDRIB_MOVECAM_CLAMP_TERRAIN, &MainFrame::OnCheck_MoveCamera_ClampTerrain)
	ON_UPDATE_COMMAND_UI(IDRIB_MOVECAM_CLAMP_TERRAIN, &MainFrame::OnUpdateCheck_MoveCamera_ClampTerrain)

	ON_COMMAND_RANGE(CompositorStartID, CompositorEndID, &MainFrame::OnCheckCompositor)
	ON_UPDATE_COMMAND_UI_RANGE(CompositorStartID, CompositorEndID, &MainFrame::OnUpdateCheckCompositor)


END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// MainFrame 构造/析构

MainFrame::MainFrame()
: _mainToolbarPanel(0)
, _mainCategoryPanel(0)
, _worldPositionPanel(0)
, _globalObjectCategory(0)
, _globalObjectCategoryMainPanel(0)
, _toolsCategory(0)
, _toolsCategoryMainPanel(0)
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);
}

MainFrame::~MainFrame()
{
}

int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;


	EditorApp::getSingleton().NotifyCreateEditorMainWnd(this);

	BOOL bNameValid = FALSE;
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	_wndRibbonBar.Create(this);
	InitializeRibbon();
	EditorApp::getSingleton().NotifyCreateMainToolbar(&_wndRibbonBar);

	if (!_wndStatusBar.Create(this))
	{
		//TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);

	CMFCRibbonStatusBarPane* panel = new CMFCRibbonStatusBarPane(1,
		"                                            ", TRUE);	
	_wndStatusBar.AddElement(panel, strTitlePane1);

	_worldPositionPanel = new CMFCRibbonStatusBarPane(2, "Pick on terrain Pick on terrain", TRUE);
	_wndStatusBar.AddExtendedElement(_worldPositionPanel, strTitlePane2);
	//_worldPositionPanel->SetText("Pick on terrain");


	EnableDocking(CBRS_ALIGN_ANY);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		// TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	return 0;
}

BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	//cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
	//	 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	cs.style |= WS_MAXIMIZE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	return TRUE;
}

void MainFrame::InitMenu()
{
	if (!m_wndMenuBar.Create(this))
	{
		// TRACE0("未能创建菜单栏\n");
		//return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);



	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
}
//--------------------------------------------------------------------------------
void MainFrame::InitToolbar()
{
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		// TRACE0("未能创建工具栏\n");
	//	return -1;      // 未能创建
	}

	CString strToolBarName;
	BOOL bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);


	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);

	// 允许用户定义的工具栏操作:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load((".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);
}
//--------------------------------------------------------------------------------

BOOL MainFrame::CreateDockingWindows()
{
	BOOL bNameValid = FALSE;

	// 创建类视图
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView,
		this, CRect(0, 0, 220, 250), TRUE, ID_VIEW_CLASSVIEW,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		return FALSE;
	}

	// 创建输出窗口
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd,
		this, CRect(0, 0, 400, 250), TRUE, ID_VIEW_OUTPUTWND,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		return FALSE;
	}

	// 创建属性窗口
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd,
		this, CRect(0, 0, 250, 250), TRUE, ID_VIEW_PROPERTIESWND,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		return FALSE;
	}

	//
	// Create Resource List Panels
	//
	// Material panel
	_materialManagerPanel.Create(NULL,
		this, CRect(0, 0, 400, 250), TRUE, ID_RESPANEL_MATERIAL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI);

	// Mesh panel
	_meshManagerPanel.Create(NULL,
		this, CRect(0, 0, 400, 250), TRUE, ID_RESPANEL_MESH,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI);

	// Texture panel
	_textureManagerPanel.Create(NULL,
		this, CRect(0, 0, 400, 250), TRUE, ID_RESPANEL_TEXTURE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI);

	// Particle system panel
	_particleSystemManagerPanel.Create(NULL,
		this, CRect(0, 0, 400, 250), TRUE, ID_RESPANEL_PARTICLESYSTEM,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI);

	SetDockingWindowIcons(theApp.m_bHiColorIcons);

	CDockablePane* pTabbedBar = NULL;
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);

	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndClassView);

	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	/**
	 *	ClassList				PropList			
	 *
	 *
	 *
	 *
	 *
	 *  Material | Mesh | Particle | Texture
	 */


	DockPane(&_materialManagerPanel);
	_materialManagerPanel.AttachToTabWnd(&m_wndOutput, DM_SHOW, TRUE, &pTabbedBar);

	DockPane(&_meshManagerPanel);
	_meshManagerPanel.AttachToTabWnd(&m_wndOutput, DM_SHOW, TRUE, &pTabbedBar);

	DockPane(&_particleSystemManagerPanel);
	_particleSystemManagerPanel.AttachToTabWnd(&m_wndOutput, DM_SHOW, TRUE, &pTabbedBar);

	DockPane(&_textureManagerPanel);
	_textureManagerPanel.AttachToTabWnd(&m_wndOutput, DM_SHOW, TRUE, &pTabbedBar);

	EditorApp::getSingleton().NotifyCreateDockPanel(this, &m_wndClassView, 
		&m_wndProperties, &_textureManagerPanel);

	return TRUE;
}

void MainFrame::InitializeRibbon()
{

	BOOL bNameValid;

	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_RIBBON_FILE);
	ASSERT(bNameValid);

	// Load panel images:
	_panelImages.SetImageSize(CSize(16, 16));
	_panelImages.Load(IDB_BUTTONS);

	// Init main button:
	_mainButton.SetImage(IDB_MAIN);
	_mainButton.SetText(("\nf"));
	_mainButton.SetToolTipText(strTemp);

	_wndRibbonBar.SetApplicationButton(&_mainButton, CSize (45, 45));
	CMFCRibbonMainPanel* pMainPanel = _wndRibbonBar.AddMainCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);
	_mainToolbarPanel = pMainPanel;


	bNameValid = strTemp.LoadString(IDS_RIBBON_NEW);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW, strTemp, 0, 0));

	bNameValid = strTemp.LoadString(IDS_RIBBON_OPEN);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN, strTemp, 1, 1));

	bNameValid = strTemp.LoadString(IDS_RIBBON_SAVE);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE, strTemp, 2, 2));
// 	bNameValid = strTemp.LoadString(IDS_RIBBON_SAVEAS);
// 	ASSERT(bNameValid);
// 	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE_AS, strTemp, 3, 3));

	pMainPanel->Add(new CMFCRibbonSeparator(TRUE));

	bNameValid = strTemp.LoadString(IDS_RIBBON_CLOSE);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_CLOSE, strTemp, 9, 9));

	bNameValid = strTemp.LoadString(IDS_RIBBON_RECENT_DOCS);
	ASSERT(bNameValid);
	pMainPanel->AddRecentFilesList(strTemp);

	bNameValid = strTemp.LoadString(IDS_RIBBON_EXIT);
	ASSERT(bNameValid);
	pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, strTemp, 15));

	// Add "Home" category with "Clipboard" panel:
	bNameValid = strTemp.LoadString(IDS_RIBBON_HOME);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategoryHome = _wndRibbonBar.AddCategory(strTemp, IDB_TOOLBAR_MAIN, IDB_FILELARGE);
	_mainCategoryPanel = pCategoryHome;

	// Create "Main" panel:
	bNameValid = strTemp.LoadString(IDS_RIBBON_MAIN);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelClipboard = pCategoryHome->AddPanel(strTemp, _panelImages.ExtractIcon(27));

	CMFCRibbonButton* btn = 0;
	// new
	bNameValid = strTemp.LoadString(IDS_RIBBON_NEW);
	ASSERT(bNameValid);
	btn = new CMFCRibbonButton(ID_FILE_NEW, strTemp, 0);
	pPanelClipboard->Add(btn);

	// open
	bNameValid = strTemp.LoadString(IDS_RIBBON_OPEN);
	ASSERT(bNameValid);
	btn = new CMFCRibbonButton(ID_FILE_OPEN, strTemp, 1);
	pPanelClipboard->Add(btn);

	// save
	bNameValid = strTemp.LoadString(IDS_RIBBON_SAVE);
	ASSERT(bNameValid);
	btn = new CMFCRibbonButton(ID_FILE_SAVE, strTemp, 2);
	pPanelClipboard->Add(btn);

	pPanelClipboard->Add(new CMFCRibbonSeparator());

	// cut
	bNameValid = strTemp.LoadString(IDS_RIBBON_CUT);
	ASSERT(bNameValid);
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_CUT, strTemp, 3));

	// copy
	bNameValid = strTemp.LoadString(IDS_RIBBON_COPY);
	ASSERT(bNameValid);
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_COPY, strTemp, 4));

	// paste
	bNameValid = strTemp.LoadString(IDS_RIBBON_PASTE);
	ASSERT(bNameValid);
	btn = new CMFCRibbonButton(ID_EDIT_PASTE, strTemp, 5);
	pPanelClipboard->Add(btn);

	pPanelClipboard->Add(new CMFCRibbonSeparator());

	// undo
	bNameValid = strTemp.LoadString(ID_EDIT_UNDO);
	ASSERT(bNameValid);
	btn = new CMFCRibbonButton(ID_EDIT_UNDO, strTemp, 6, 3);
	pPanelClipboard->Add(btn);


	// redo
	bNameValid = strTemp.LoadString(ID_EDIT_REDO);
	ASSERT(bNameValid);
	btn = new CMFCRibbonButton(ID_EDIT_REDO, strTemp, 7, 4);
	pPanelClipboard->Add(btn);


	//
	// Create and add a "View" panel:
	//
	CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel(_UIDisplayString("IDS_RIBBON_VIEW").c_str(), _panelImages.ExtractIcon (7));

	btn = new CMFCRibbonButton(ID_SHOW_OBJECTS, _UIDisplayString("ID_SHOW_OBJECTS").c_str(), 22);
	pPanelView->Add(btn);

	btn = new CMFCRibbonButton(ID_SHOW_BOUDNING_BOX, _UIDisplayString("ID_SHOW_BOUDNING_BOX").c_str(), 23);
	pPanelView->Add(btn);

	btn = new CMFCRibbonButton(ID_SHOW_WIRE, _UIDisplayString("ID_SHOW_WIRE").c_str(), 24);
	pPanelView->Add(btn);

	CMFCRibbonComboBox* cbtn = new CMFCRibbonComboBox(ID_MATERIAL_SCHEME, FALSE, -1, _UIDisplayString("ID_MATERIAL_SCHEME").c_str());
	pPanelView->Add(cbtn);	

	EditorUtil::InitMaterialSchemeList(cbtn);

	//cbtn = new CMFCRibbonComboBox(ID_COMPOSITOR, FALSE, -1, _UIDisplayString("ID_COMPOSITOR").c_str());
	//pPanelView->Add(cbtn);
	//EditorUtil::InitCompositorList(cbtn);


	//bNameValid = strTemp.LoadString(IDS_RIBBON_STATUSBAR);
	//ASSERT(bNameValid);
	//btn = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, strTemp);
	//pPanelView->Add(btn);


	///
	/// Camera speed
	///
	CMFCRibbonPanel* cameraPanel = pCategoryHome->AddPanel(_UIDisplayString("ID_CAMERA_SPEED").c_str(), _panelImages.ExtractIcon (26));
	
	HICON hicon = AfxGetApp()->LoadIcon(IDI_ICON_CAMSPEED_SLOW);
	btn = new CMFCRibbonButton(ID_CAMERASPEED_SLOW, _UIDisplayString("ID_CAMERASPEED_SLOW").c_str(), 25);
	cameraPanel->Add(btn);

	hicon = AfxGetApp()->LoadIcon(IDI_ICON_CAMSPEED_NORMAL);
	btn = new CMFCRibbonButton(ID_CAMERASPEED_NORMAL, _UIDisplayString("ID_CAMERASPEED_NORMAL").c_str(), 25);
	cameraPanel->Add(btn);

	hicon = AfxGetApp()->LoadIcon(IDI_ICON_CAMSPEED_FAST);
	btn = new CMFCRibbonButton(ID_CAMERASPEED_FAST, _UIDisplayString("ID_CAMERASPEED_FAST").c_str(), 25);
	cameraPanel->Add(btn);

	hicon = AfxGetApp()->LoadIcon(IDI_ICON_PROP);
	btn = new CMFCRibbonButton(ID_CURRENT_CAMERA, _UIDisplayString("ID_CURRENT_CAMERA").c_str(), hicon, TRUE, hicon);
	cameraPanel->Add(btn);
	
	btn = new CMFCRibbonCheckBox(IDRIB_MOVECAM_CLAMP_TERRAIN, _UIDisplayString("IDRIB_MOVECAM_CLAMP_TERRAIN").c_str());
	cameraPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_VIEW_SELECT_CAMERA, _UIDisplayString("ID_VIEW_SELECT_CAMERA").c_str());
	cameraPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_APPLY_CAMERA, _UIDisplayString("ID_APPLY_CAMERA").c_str());
	cameraPanel->Add(btn);
	

	///
	/// Move options
	///

	CMFCRibbonPanel* optionsPanel = pCategoryHome->AddPanel(_UIDisplayString("MoveOptions").c_str());
	
	btn = new CMFCRibbonCheckBox(IDRIB_MOVE_CLAMP_TERRAIN, _UIDisplayString("IDRIB_MOVE_CLAMP_TERRAIN").c_str());
	optionsPanel->Add(btn);	

	btn = new CMFCRibbonCheckBox(IDRIB_MOVE_ALIGN_NORMAL, _UIDisplayString("IDRIB_MOVE_ALIGN_NORMAL").c_str());
	optionsPanel->Add(btn);	

	btn = new CMFCRibbonCheckBox(IDRIB_MOVE_SNAP_GRID, _UIDisplayString("IDRIB_MOVE_SNAP_GRID").c_str());
	optionsPanel->Add(btn);	


	// Add elements to the right side of tabs:
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pVisualStyleButton = new CMFCRibbonButton(-1, strTemp, -1, -1);

	pVisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE /* No default command */, TRUE /* Right align */);

	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_TIP);
	ASSERT(bNameValid);
	pVisualStyleButton->SetToolTipText(strTemp);
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_DESC);
	ASSERT(bNameValid);
	pVisualStyleButton->SetDescription(strTemp);
	_wndRibbonBar.AddToTabs(pVisualStyleButton);

	// Add quick access toolbar commands:
	CList<UINT, UINT> lstQATCmds;

	lstQATCmds.AddTail(ID_FILE_NEW);
	lstQATCmds.AddTail(ID_FILE_OPEN);
	lstQATCmds.AddTail(ID_FILE_SAVE);
	lstQATCmds.AddTail(ID_EDIT_UNDO);
	lstQATCmds.AddTail(ID_EDIT_REDO);
	

	_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);

	_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, ("\na"), _panelImages.ExtractIcon (0)));

	
	//--------------------------------------------------------------------------------

	//
	// Global Object Category Panel
	//
	_globalObjectCategory = _wndRibbonBar.AddCategory(
		_UIDisplayString("Global Object").c_str(),
		IDB_BITMAP_CAMERA_SPEED, IDB_BITMAP_CAMERA_SPEED);
	
	CMFCRibbonPanel* _globalObjectCategoryMainPanel = _globalObjectCategory->AddPanel(
		_UIDisplayString("Global Object").c_str());

	btn = new CMFCRibbonButton(IDRIB_SCENE_PROP, _UIDisplayString("Scene").c_str(), 0);
	_globalObjectCategoryMainPanel->Add(btn);

	btn = new CMFCRibbonButton(IDRIB_SKYBOX_PROP, _UIDisplayString("SkyBox").c_str(), 0);
	_globalObjectCategoryMainPanel->Add(btn);

	btn = new CMFCRibbonButton(IDRIB_MAINLIGHT_PROP, _UIDisplayString("MainLight").c_str(), 0);
	_globalObjectCategoryMainPanel->Add(btn);

	btn = new CMFCRibbonButton(IDRIB_EDITORCFG_PROP, _UIDisplayString("EditorConfig").c_str(), 0);
	_globalObjectCategoryMainPanel->Add(btn);
}

void MainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);
	_meshManagerPanel.SetIcon(hClassViewIcon, FALSE);
	_materialManagerPanel.SetIcon(hClassViewIcon, FALSE);
	_textureManagerPanel.SetIcon(hClassViewIcon, FALSE);
	_particleSystemManagerPanel.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);
}

// MainFrame 诊断

#ifdef _DEBUG
void MainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void MainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// MainFrame 消息处理程序

void MainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT MainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void MainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(("ApplicationLook"), theApp.m_nAppLook);
}

void MainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void MainFrame::OnUpdateViewport(CCmdUI* pCmdUI)
{
	EditorView* view = dynamic_cast<EditorView*>(GetActiveView());
	EViewportStyle vs = view->GetViewportSplitStyle();
	int d = (pCmdUI->m_nID - ID_VP_MAX);
	pCmdUI->SetCheck(vs == d);
}

void MainFrame::OnUpdateUndo(CCmdUI* pCmdUI)
{
	BOOL on = (BOOL)EditorApp::getSingleton().GetCommandHistory().IsUndoEnable();
	pCmdUI->Enable(on);

	GetActiveView()->GetDocument()->SetModifiedFlag(on);

	_wndRibbonBar.UpdateWindow();
}

void MainFrame::OnUpdateRedo(CCmdUI* pCmdUI)
{
	BOOL on = (BOOL)EditorApp::getSingleton().GetCommandHistory().IsRedoEnable();
	pCmdUI->Enable(on);

	GetActiveView()->GetDocument()->SetModifiedFlag(TRUE);

	_wndRibbonBar.UpdateWindow();
}

void MainFrame::OnUpdateSave(CCmdUI* pCmdUI)
{
	BOOL on = GetActiveView()->GetDocument()->IsModified();
	pCmdUI->Enable(on);

	GetActiveView()->GetDocument()->SetModifiedFlag(FALSE);
}

BOOL MainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void MainFrame::OnViewportMax()
{
	EditorView* view = dynamic_cast<EditorView*>(GetActiveView());
	view->CreateViewports(EVS_Max);
}

void MainFrame::OnViewport1x2()
{
	EditorView* view = dynamic_cast<EditorView*>(GetActiveView());
	view->CreateViewports(EVS_1x2);
}

void MainFrame::OnViewport2x2()
{
	EditorView* view = dynamic_cast<EditorView*>(GetActiveView());
	view->CreateViewports(EVS_2x2);
}

BOOL MainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	return CFrameWndEx::OnMouseWheel(nFlags, zDelta, pt);
}

void MainFrame::OnUndo()
{
	EditorApp::getSingleton().GetCommandHistory().Undo();
	GetActiveView()->GetDocument()->SetModifiedFlag();
	Root::getSingleton().renderOneFrame();
}

void MainFrame::OnRedo()
{
	EditorApp::getSingleton().GetCommandHistory().Redo();
	GetActiveView()->GetDocument()->SetModifiedFlag();
	Root::getSingleton().renderOneFrame();
}

void MainFrame::OnClose()
{
	CFrameWndEx::OnClose();
}

void MainFrame::OnDestroy()
{
	//ResourceViewDlg::getSingleton().DestroyWindow();
	//ResManagerDlg_ParticleSystem::getSingleton().DestroyWindow();
	//ResManagerDlg_Material::getSingleton().DestroyWindow();
	//ObjectListDlg::getSingleton().DestroyWindow();

// 	if(ParticleEditorFrm::getSingletonPtr())
// 	{
// 		ParticleEditorFrm::getSingleton().DestroyWindow();
// 	}

	CFrameWndEx::OnDestroy();
	GameApp::getSingleton().Quit();

	EditorGameApp::getSingleton().Shutdown();

	OGRE_DELETE EditorGame::getSingletonPtr();
	OGRE_DELETE GameApp::getSingletonPtr();

}

void MainFrame::PostNcDestroy()
{
	CFrameWndEx::PostNcDestroy();
	//AfxWinTerm();
}

void MainFrame::OnActiveEditState(UINT id)
{
	EditorApp::getSingleton().ActiveState(id);
}

void MainFrame::OnUpdateEditState( CCmdUI* pCmdUI)
{
	int activeStateID = EditorApp::getSingleton().GetActiveStateCommandID();
	pCmdUI->SetCheck(pCmdUI->m_nID == activeStateID ? 1 : 0);
}

void MainFrame::OnTimer(UINT_PTR timerID)
{
	Root::getSingleton().renderOneFrame();
}

void MainFrame::OnShowResManager()
{
	ResPanel_Mesh::getSingleton().Show();
}

void MainFrame::OnShowMaterialManager()
{
	ResPanel_Material::getSingleton().Show();
}

void MainFrame::OnShowTextureManager()
{
	ResPanel_Texture::getSingleton().Show();
	ResPanel_Texture::getSingleton().UndockPane();
}

void MainFrame::OnShowObjects()
{
	ObjectListDlg::getSingleton().Show();
}

void MainFrame::OnUpdateEditCopyDelete(CCmdUI* pCmdUI)
{
// 	pCmdUI->Enable(EditState_Select::getSingletonPtr() && EditState_Select::getSingleton().IsObjectSelected());
}


void MainFrame::OnUpdateShowBoundingBox(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GameApp::getSingleton().GetGame()->GetVisualScene()->getShowBoundingBoxes());
}

void MainFrame::OnShowBoundingBox()
{
	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	sm->showBoundingBoxes(!sm->getShowBoundingBoxes());
	bool on = sm->getShowBoundingBoxes();
	sm->setOption("ShowOctree", (const void*)&on);

// 	EditState_Select::getSingleton().UpdateShowBoundingBox();
}

void MainFrame::OnUpdateToggleClampOnTerrain(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((int)EditorConfig::getSingleton().GetAutoClampTerrainOnMove());
}

void MainFrame::OnToggleClampOnTerrain()
{
	EditorConfig::getSingleton().SetAutoClampTerrainOnMove(
		!EditorConfig::getSingleton().GetAutoClampTerrainOnMove());
}

void MainFrame::OnUpdateAlignTerrainNormal(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((int)EditorConfig::getSingleton().GetAutoAlignTerrainNormal());
}

void MainFrame::OnToggleAlignTerrainNormal()
{
	EditorConfig::getSingleton().SetAutoAlignTerrainNormal(
		!EditorConfig::getSingleton().GetAutoAlignTerrainNormal());
}

void MainFrame::OnUpdateShowWireFrame(CCmdUI* pCmdUI)
{
	Camera* cam = EditorGame::getSingleton().GetActiveCamera();
	PolygonMode currentModel = cam->getPolygonMode();
	bool wire = currentModel == PM_WIREFRAME;
	pCmdUI->SetCheck((int)wire);
}

void MainFrame::OnToggleShowWireFrame()
{
	Camera* cam = EditorGame::getSingleton().GetActiveCamera();
	PolygonMode currentModel = cam->getPolygonMode();
	if(currentModel == PM_WIREFRAME)
	{
		currentModel = PM_SOLID;
	}
	else
	{
		currentModel = PM_WIREFRAME;
	}

	cam->setPolygonMode(currentModel);
}

void MainFrame::OnUpdateApplyCamera(CCmdUI* pCmdUI)
{
	IGameObject* obj = ObjectPropertiesWnd::getSingleton().GetGameObject();
	pCmdUI->Enable(obj && (obj->GetTypeName() == "Camera"));
}

void MainFrame::OnApplyCamera()
{
	IGameObject* obj = ObjectPropertiesWnd::getSingleton().GetGameObject();
	GameCamera* cam = dynamic_cast<GameCamera*>(obj);

	if(cam)
	{
		Camera* activeCam = EditorGame::getSingleton().GetActiveCamera();
		//cam->Active();
		cam->SetFocalLength(activeCam->getFocalLength());
		cam->SetFovDegree(activeCam->getFOVy().valueDegrees());
		cam->SetPosition(activeCam->getPosition());
		cam->SetOrientation(activeCam->getOrientation());
	}
}

void MainFrame::OnUpdateViewSelectedCamera(CCmdUI* pCmdUI)
{
	IGameObject* obj = ObjectPropertiesWnd::getSingleton().GetGameObject();
	pCmdUI->Enable(obj && (obj->GetTypeName() == "Camera"));
}

void MainFrame::OnViewSelectedCamera()
{
	IGameObject* obj = ObjectPropertiesWnd::getSingleton().GetGameObject();
	GameCamera* cam = dynamic_cast<GameCamera*>(obj);

	if(cam)
	{
		Camera* activeCam = EditorGame::getSingleton().GetActiveCamera();
		activeCam->setFocalLength(cam->GetFocalLength());
		activeCam->setFOVy(Radian(Degree(cam->GetFovDegree())));
		activeCam->setPosition(cam->GetPosition());
		activeCam->setOrientation(cam->GetOrientation());
	}
}

void MainFrame::OnEditCurrentCamera()
{
	EditorGame::getSingleton().ShowCameraProperties();
}

void MainFrame::OnMaterialScheme()
{
	if(MaterialManager::getSingletonPtr())
	{

		CMFCRibbonComboBox* combo = (CMFCRibbonComboBox*)_mainCategoryPanel->FindByID(ID_MATERIAL_SCHEME);
		if(combo)
		{
			Viewport* vp = EditorGame::getSingleton().GetActiveViewport();
			String str = (LPCTSTR)combo->GetItem(combo->GetCurSel());
			vp->setMaterialScheme(str);
			LogDebugIDf("Editor", "Active material scheme", str.c_str());
			EditorGame::getSingleton().SetMaterialScheme(str);

		}
#if 0
		CMFCToolBarComboBoxButton* pSrcCombo = NULL;
		CObList listButtons;
		if (CMFCToolBar::GetCommandButtons(ID_MATERIAL_SCHEME, listButtons) > 0)
		{
			for (POSITION posCombo = listButtons.GetHeadPosition();
				pSrcCombo == NULL && posCombo != NULL;)
			{
				CMFCToolBarComboBoxButton* pCombo = DYNAMIC_DOWNCAST(CMFCToolBarComboBoxButton, listButtons.GetNext(posCombo));

				if (pCombo != NULL && CMFCToolBar::IsLastCommandFromButton(pCombo))
				{
					pSrcCombo = pCombo;
				}
			}
		}

		if (pSrcCombo != NULL)
		{
			ASSERT_VALID(pSrcCombo);

			LPCTSTR lpszSelItem = pSrcCombo->GetItem();
			CString strSelItem = (lpszSelItem == NULL) ? ("") : lpszSelItem;
			Viewport* vp = EditorGame::getSingleton().GetActiveViewport();
			String str = (LPCTSTR)strSelItem;
			vp->setMaterialScheme(str);
			LogDebugIDf("Editor", "Active material scheme", str.c_str());
			EditorGame::getSingleton().SetMaterialScheme(str);
			//MaterialManager::getSingleton().setActiveScheme((LPCTSTR)strSelItem);
			//AfxMessageBox(strSelItem);
		}
		else
		{
			AfxMessageBox(("Show \"Set Active Configuration\" dialog...."));
		}
#endif

	}
}

void MainFrame::OnCompositorSelect()
{
	static CompositorEnabledMap	compositorMap;
	Viewport* vp = EditorGame::getSingleton().GetActiveViewport();
	CMFCRibbonComboBox* combo =
		(CMFCRibbonComboBox*)_mainCategoryPanel->FindByID(ID_COMPOSITOR);

	if(combo)
	{
		EditorUtil::SelectViewportCompsoitor(vp, combo, compositorMap);
	}
}

void MainFrame::OnPickFilter()
{
	if(MaterialManager::getSingletonPtr())
	{
		CMFCToolBarComboBoxButton* pSrcCombo = NULL;

		CObList listButtons;
		if (CMFCToolBar::GetCommandButtons(ID_PICK_FILTER, listButtons) > 0)
		{
			for (POSITION posCombo = listButtons.GetHeadPosition();
				pSrcCombo == NULL && posCombo != NULL;)
			{
				CMFCToolBarComboBoxButton* pCombo = DYNAMIC_DOWNCAST(CMFCToolBarComboBoxButton, listButtons.GetNext(posCombo));

				if (pCombo != NULL && CMFCToolBar::IsLastCommandFromButton(pCombo))
				{
					pSrcCombo = pCombo;
				}
			}
		}

		if (pSrcCombo != NULL)
		{
			ASSERT_VALID(pSrcCombo);

			LPCTSTR lpszSelItem = pSrcCombo->GetItem();
			CString strSelItem = (lpszSelItem == NULL) ? ("All") : lpszSelItem;
			if(strSelItem == "All")
			{
				EditorConfig::getSingleton().PickFilter = 0xFFFFFFFF;
			}
			else
			{
				EditorConfig::getSingleton().PickFilter =
					GameObjectFactoryManager::getSingleton().GetQueryFlag((LPCTSTR)strSelItem);
			}
		}
		else
		{
			AfxMessageBox(("Show \"Set Active Configuration\" dialog...."));
		}

	}
}

LRESULT MainFrame::OnToolbarReset(WPARAM wp, LPARAM lp)
{
	UINT uiToolBarId = (UINT) wp;

	if(!ResourceGroupManager::getSingletonPtr())
	{
		return 0;
	}

	switch (uiToolBarId)
	{
	case IDR_MAINFRAME:
	case IDR_MAINFRAME_256:
		{
			{
				CMFCToolBarComboBoxButton comboButton(ID_MATERIAL_SCHEME, GetCmdMgr()->GetCmdImage(ID_MATERIAL_SCHEME, FALSE), CBS_DROPDOWNLIST);
				ConfigFile cfg;
				cfg.loadFromResourceSystem("Material.cfg", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
				String val = cfg.getSetting("Count", "Schemes");
				int cnt = StringConverter::parseInt(val);
				for (int i = 0; i < cnt; ++i)
				{
					String key = formatString("S[%d]", i);
					val = cfg.getSetting(key, "Schemes");
					comboButton.AddItem(val.c_str());
				}

				comboButton.SelectItem(0);
				m_wndToolBar.ReplaceButton (ID_MATERIAL_SCHEME, comboButton);
			}

			UpdateCompositorList();

			{
				CMFCToolBarComboBoxButton comboButton(ID_PICK_FILTER, GetCmdMgr()->GetCmdImage(ID_PICK_FILTER, FALSE), CBS_DROPDOWNLIST);
				comboButton.AddItem("All");

				const GameObjectFactoryManager::FactoryMap& fmap = GameObjectFactoryManager::getSingleton().GetFactoryMap();
				for (GameObjectFactoryManager::FactoryMap::const_iterator i = fmap.begin(); i != fmap.end(); ++i)
				{
					String objTypeName = i->first;
					if(GameObjectFactoryManager::getSingleton().GetFactory(objTypeName)->IsShownInEditorClassList())
					{
						if(objTypeName != "Terrain")
						{
							comboButton.AddItem(objTypeName.c_str());
						}
					}
				}			
				comboButton.SelectItem(0);
				m_wndToolBar.ReplaceButton (ID_PICK_FILTER, comboButton);
			}

			{
				CMenu menu;
				menu.LoadMenu(IDR_MENU_CAMERA_SPEED);
				m_wndToolBar.ReplaceButton(ID_CAMERA_SPEED,
					CMFCToolBarMenuButton(ID_CAMERA_SPEED, menu.GetSubMenu(0)->GetSafeHmenu(),
					GetCmdMgr()->GetCmdImage(ID_CAMERA_SPEED, FALSE)));
			}
		}
		break;
	}

// 	if(ParticleEditorFrm::getSingletonPtr())
// 	{
// 		ParticleEditorFrm::getSingleton().OnToolbarReset(wp, lp);
// 	}

	return 0;
}

void MainFrame::OnCommandExecuted()
{
	GetActiveView()->GetDocument()->SetModifiedFlag();
}

void MainFrame::UpdateCompositorList()
{
	//EditorUtil::InitCompositorList(m_wndToolBar, ID_COMPOSITOR);

	CMFCRibbonComboBox* combo =
		(CMFCRibbonComboBox*)_mainCategoryPanel->FindByID(ID_COMPOSITOR);

	if(combo)
	{
		EditorUtil::InitCompositorList(combo);
	}
}


void MainFrame::OnSnapshotMiniMap()
{
	EditorGame::getSingleton().SnapshotMiniMap();
}

void MainFrame::OnSnapshotConfig()
{
	ObjectPropertiesWnd::getSingleton().SetObject(&EditorConfig::getSingleton());
}

void MainFrame::OnSnapshot()
{
	EditorGame::getSingleton().Snapshot();
}

void MainFrame::OnShowParticleEditor()
{
// 	//if(!ParticleEditorFrm::getSingletonPtr())
// 	{
// 		//ParticleEditorFrm* dlg = OGRE_NEW ParticleEditorFrm();
// 		delete ParticleEditorFrm::getSingletonPtr();
// 		MFCCreateNewWindow(MFCEditorApp::getSingleton()._particleDoc);
// 		ParticleEditorFrm::getSingleton().Init();
// 	}
// 	
// 	ParticleEditorFrm::getSingleton().ShowWindow(SW_RESTORE);
// 	ParticleEditorFrm::getSingleton().ShowWindow(SW_SHOWMAXIMIZED);
}

void MainFrame::OnShowModelEditor()
{
	//if(!ModelEditorFrm::getSingletonPtr())
// 	{
// 		//ParticleEditorFrm* dlg = OGRE_NEW ParticleEditorFrm();
// 		delete ModelEditorFrm::getSingletonPtr();
// 		MFCCreateNewWindow(MFCEditorApp::getSingleton()._modelEditorDoc);
// 		ModelEditorFrm::getSingleton().Init();
// 	}
// 
// 	ModelEditorFrm::getSingleton().ShowWindow(SW_RESTORE);
// 	ModelEditorFrm::getSingleton().ShowWindow(SW_SHOWMAXIMIZED);
}

void MainFrame::OnShowModelMaterialEditor()
{
// 	//if(!ModelEditorFrm::getSingletonPtr())
// 	{
// 		//ParticleEditorFrm* dlg = OGRE_NEW ParticleEditorFrm();
// 		delete MaterialEditorFrm::getSingletonPtr();
// 		MFCCreateNewWindow(MFCEditorApp::getSingleton()._materialEditorDoc);
// 		MaterialEditorFrm::getSingleton().Init();
// 	}
// 
// 	MaterialEditorFrm::getSingleton().ShowWindow(SW_RESTORE);
// 	MaterialEditorFrm::getSingleton().ShowWindow(SW_SHOWMAXIMIZED);
}



void MainFrame::OnShowPickFilter()
{

}

void MainFrame::OnCameraSpeed(UINT id)
{
	EditorConfig& cfg = EditorConfig::getSingleton();
	ECameraSpeed camSpd = cfg.GetCameraMoveSpeed();

	switch(id)
	{
	case ID_CAMERASPEED_FAST:
		camSpd = eCS_Fast;
		break;

	case ID_CAMERASPEED_SLOW:
		camSpd = eCS_Slow;
		break;

	case ID_CAMERASPEED_NORMAL:
		camSpd = eCS_Normal;
		break;
	}

	cfg.SetCameraMoveSpeed(camSpd);
}

void MainFrame::InitPluginsToolbar()
{	
	//CMFCRibbonPanel* panel = _mainCategoryPanel->AddPanel(
	//	_UIDisplayString("IDS_EDIT_STATE").c_str(), m_PanelImages.ExtractIcon (0));

	const StateNameMap& stats = EditorApp::getSingleton().GetStateList();
	StateNameMap::const_iterator i = stats.begin();
	for (; i != stats.end(); ++i)
	{
		AbstractEditState* as = i->second;
		as->CreateToolbar(_mainCategoryPanel);
	}

	EditorApp::getSingleton().NotifyPostCreateEditStateCategory(_mainCategoryPanel);
}

void MainFrame::OnUpdateCameraSpeed( CCmdUI* pCmdUI)
{
	EditorConfig& cfg = EditorConfig::getSingleton();
	ECameraSpeed camSpd = cfg.GetCameraMoveSpeed();

	switch(camSpd)
	{
	case eCS_Fast:
		pCmdUI->SetCheck(pCmdUI->m_nID == ID_CAMERASPEED_FAST);
		break;

	case eCS_Slow:
		pCmdUI->SetCheck(pCmdUI->m_nID == ID_CAMERASPEED_SLOW);
		break;

	case eCS_Normal:
		pCmdUI->SetCheck(pCmdUI->m_nID == ID_CAMERASPEED_NORMAL);
		break;
	}
}

void MainFrame::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(eventName == "PickOnTerrain")
	{
		Vector3 pos = any_cast<Vector3>(eventContex);
// 		EditorGame::getSingleton().DrawString(
// 			eventName, StringConverter::toString(pos), 5, 5);

		_worldPositionPanel->SetText(StringConverter::toString(pos).c_str());
		_worldPositionPanel->Redraw();
	}
}

void MainFrame::OnShowProp_Scene()
{
	EventsManager::getSingleton().CreateEvent(
		"ShowObjectProp", Any((IPropertyObject*)SceneEnvironment::getSingletonPtr()));
}

void MainFrame::OnUpdateUI_ShowProp_Scene( CCmdUI* ui )
{
	ui->Enable(SceneEnvironment::getSingletonPtr() ? TRUE : FALSE);
}

void MainFrame::OnShowProp_SkyBox()
{
	EventsManager::getSingleton().CreateEvent(
		"ShowObjectProp", Any((IPropertyObject*)SkyBox::getSingletonPtr()));
}

void MainFrame::OnUpdateUI_ShowProp_SkyBox( CCmdUI* ui )
{
	ui->Enable(SkyBox::getSingletonPtr() ? TRUE : FALSE);
}

void MainFrame::OnShowProp_MainLight()
{
	EventsManager::getSingleton().CreateEvent(
		"ShowObjectProp", Any((IPropertyObject*)MainLight::getSingletonPtr()));
}

void MainFrame::OnUpdateUI_ShowProp_MainLight( CCmdUI* ui )
{
	ui->Enable(MainLight::getSingletonPtr() ? TRUE : FALSE);
}

void MainFrame::OnShowProp_EditorConfig()
{
	EventsManager::getSingleton().CreateEvent(
		"ShowObjectProp", Any((IPropertyObject*)&EditorConfig::getSingleton()));
}

void MainFrame::OnCheck_Move_ClampTerrain()
{
	EditorConfig::getSingleton().SetAutoClampTerrainOnMove(
		!EditorConfig::getSingleton().GetAutoClampTerrainOnMove());
}

void MainFrame::OnCheck_Move_AlignNormal()
{
	EditorConfig::getSingleton().SetAutoAlignTerrainNormal(
		!EditorConfig::getSingleton().GetAutoAlignTerrainNormal());
}

void MainFrame::OnCheck_Move_SnapToGird()
{
	EditorConfig::getSingleton().SetSnapToGrid(
		!EditorConfig::getSingleton().GetSnapToGrid());
}

void MainFrame::OnUpdateCheck_Move_ClampTerrain( CCmdUI* ui )
{	
	ui->SetCheck(EditorConfig::getSingleton().GetAutoClampTerrainOnMove());
}

void MainFrame::OnUpdateCheck_Move_AlignNormal( CCmdUI* ui )
{
	ui->SetCheck(EditorConfig::getSingleton().GetAutoAlignTerrainNormal());
}

void MainFrame::OnUpdateCheck_Move_SnapToGird( CCmdUI* ui )
{
	ui->SetCheck(EditorConfig::getSingleton().GetSnapToGrid());
}

void MainFrame::OnCheck_MoveCamera_ClampTerrain()
{
	EditorConfig::getSingleton().SetClampCameraToTerrain(
		!EditorConfig::getSingleton().GetClampCameraToTerrain());
}

void MainFrame::OnUpdateCheck_MoveCamera_ClampTerrain( CCmdUI* ui )
{
	ui->SetCheck(EditorConfig::getSingleton().GetClampCameraToTerrain());
}

void MainFrame::OnUpdateFileNew( CCmdUI* pCmdUI )
{
	pCmdUI->Enable();
}

void MainFrame::OnUpdateFileOpen( CCmdUI* pCmdUI )
{
	pCmdUI->Enable();
}

void MainFrame::PostGameInit()
{
	UpdateCompositorList();

	EditorApp::getSingleton().NotifyCreateGlobalObjectsCategory(
		_globalObjectCategory, _globalObjectCategoryMainPanel);

	///
	/// Tools category panel
	///

	_toolsCategory =
		_wndRibbonBar.AddCategory(_UIDisplayString("ID_TOOLS_PANEL").c_str(),
		IDB_TOOLBAR_MAIN, IDB_FILELARGE);

	_toolsCategoryMainPanel = _toolsCategory->AddPanel(
		_UIDisplayString("ID_TOOLS_PANEL").c_str());

	EditorApp::getSingleton().NotifyCreateToolsCategory(
		_toolsCategory, _toolsCategoryMainPanel);


	///
	/// Compositor category panel
	///
	CMFCRibbonCategory* cat =
		_wndRibbonBar.AddCategory(_UIDisplayString("ID_COMPOSITOR").c_str(),
		IDB_TOOLBAR_MAIN, IDB_FILELARGE);

	CMFCRibbonPanel* panel = cat->AddPanel(
		_UIDisplayString("ID_COMPOSITOR").c_str());

	EditorUtil::InitCompositorList(panel, CompositorStartID, _compositorEnableStatus);
}
//--------------------------------------------------------------------------------
void MainFrame::OnCheckCompositor( UINT id )
{
	Viewport* vp = EditorGame::getSingleton().GetActiveViewport();
	EditorUtil::SelectViewportCompsoitor(vp, _compositorEnableStatus, id);
}
//--------------------------------------------------------------------------------
void MainFrame::OnUpdateCheckCompositor( CCmdUI* pCmdUI )
{
	EditorUtil::UpdateCompositorUI(pCmdUI, _compositorEnableStatus);
}
