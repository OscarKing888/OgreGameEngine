#include "stdafx.h"
#include "MeshEditorFrm.h"
#include "Resource.h"
#include "ParticleSystemListDlg.h"
#include "EditorCore/ListSelectDlg.h"
#include <OgreTagPoint.h>
#include "EditorCore/Grid.h"

//----------------------------------------------------------------------------------------------
#define ID_PARTCILE_PROPERTIESWND           2

#define COMPOSITOR_COUNT 100
UINT MeshEditorFrm::CompositorStartID = GetAutoWMID();

bool AdvanceWMID()
{
	for (int i = 0; i < COMPOSITOR_COUNT; ++i)
	{
		GetAutoWMID();
	}
	return true;
}

bool callFun = AdvanceWMID();

UINT MeshEditorFrm::CompositorEndID = CompositorStartID + COMPOSITOR_COUNT;


//----------------------------------------------------------------------------------------------
template<> MeshEditorFrm* SingletonPhoenix<MeshEditorFrm>::ms_Singleton = 0;

const uint32 C_ViewportVisibilityFlag = 0x20000000;
//----------------------------------------------------------------------------------------------
ModelEditor_ModelList::ModelEditor_ModelList(MeshEditorFrm* parent)
: _parent(parent)
{
}
//----------------------------------------------------------------------------------------------
void ModelEditor_ModelList::OnDBClickListItem(int item, const CString& resName)
{
	_parent->EditMeshFile((LPCTSTR)resName);
}

ModelEditor_ModelList::~ModelEditor_ModelList()
{
	OnAppShutdown();
}

BOOL ModelEditor_ModelList::Create( LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect,
								   BOOL bHasGripper, UINT nID, DWORD dwStyle,
								   DWORD dwTabbedStyle /*= AFX_CBRS_REGULAR_TABS*/,
								   DWORD dwControlBarStyle /*= AFX_DEFAULT_DOCKING_PANE_STYLE*/,
								   CCreateContext* pContext /*= NULL*/ )
{
	BOOL r = ResPanel_ResGrouped::Create(lpszCaption, pParentWnd, rect,
		bHasGripper, nID, dwStyle, dwTabbedStyle, dwControlBarStyle, pContext);

	OnAppInitCompleted();
	return r;
}
//----------------------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(MeshEditorFrm, CFrameWndEx)
//----------------------------------------------------------------------------------------------
MeshEditorFrm::MeshEditorFrm()
: _meshFile("axes.mesh")
, _listPanel(this)
, _aniModel(0)
, _isPlay(true)
, _selectedBone(0)
, _selectedAttachPoint(0)
, _showSkeleton(false)
, _viewCategory(0)
{
}
//----------------------------------------------------------------------------------------------
MeshEditorFrm::~MeshEditorFrm()
{
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::DoDataExchange(CDataExchange* pDX)
{
	CFrameWndEx::DoDataExchange(pDX);	
}
//----------------------------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable : 4407)
BEGIN_MESSAGE_MAP(MeshEditorFrm, CFrameWndEx)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	
	ON_COMMAND(ID_MODEL_EDITOR_DELETE, OnDeleteMesh)	
	ON_COMMAND(ID_MODEL_EDITOR_SAVE, OnSaveMesh)
	ON_COMMAND(ID_MODEL_EDITOR_SAVEAS, OnSaveMeshAs)


	ON_COMMAND(ID_SHOW_WIREFRAME, OnShowWireframe)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WIREFRAME, OnUpdateShowWireframe)

	ON_COMMAND(ID_SHOW_SKELETON, OnShowSkeleton)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SKELETON, OnUpdateShowSkeleton)


	ON_COMMAND(ID_MODEL_EDITOR_BUILD_EDGELIST, OnBuildEdigeList)

	ON_COMMAND(ID_MODEL_VIEWPORT_COMPOSITOR, OnCompositorSelect)
	ON_CBN_SELENDOK(ID_MODEL_VIEWPORT_COMPOSITOR, OnCompositorSelect)

	ON_COMMAND(ID_MODEL_EDITOR_CAPTURETHUMB, OnCaptureThumbImage)

	ON_COMMAND(ID_MESHEDIT_SETASLEFTFOOT, SetAsLeftFoot)
	ON_COMMAND(ID_MESHEDIT_SETASLEFTBACKFOOT, SetAsLeftBackFoot)

	ON_COMMAND(ID_MESHEDIT_SETASRIGHTFOOT, SetAsRightFoot)
	ON_COMMAND(ID_MESHEDIT_SETASRIGHTBACKFOOT, SetAsRightBackFoot)

	ON_COMMAND(ID_MESHEDIT_SETAS4LEGS, OnToggle4Legs)
	ON_COMMAND(ID_MESHEDIT_CALCMOVEDISTANCE, CalcMoveDist)

	ON_COMMAND(ID_MESHEDIT_ADDLINKEDSKELETON, OnAddLinkedSkeleton)
	ON_COMMAND(ID_MESHEDIT_REMOVELINKEDSKELETON, OnRemoveLinkedSkeleton)

	ON_COMMAND(ID_MESHEDIT_ADDEFFECTS, OnAddEffects)
	ON_COMMAND(ID_MESHEDIT_DELETEEFFECTS, OnDelEffects)

	ON_COMMAND(ID_MESHEDIT_ADDATTACHPOINT, OnAddAttachPoint)
	ON_COMMAND(ID_MESHEDIT_DELETEATTACHPOINT, OnDelAttachPoint)

	ON_COMMAND(ID_MESHEDIT_ADDBONEGROUP, OnAddBoneGroup)
	ON_COMMAND(ID_MESHEDIT_DELETEBONEGROUP, OnDelBoneGroup)

	ON_COMMAND(ID_MESHEDIT_ADDFRAMEEVENT, OnAddFrameEvent)
	ON_COMMAND(ID_MESHEDIT_DELETEFRAMEEVENT, OnDelFrameEvent)

	ON_COMMAND(ID_MESHEDIT_COPYTOCLIPBOARD, OnCopyPlayingAnimationNameToClipboard)

	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_SETAS4LEGS, OnUpdate4Legs)
	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_CALCMOVEDISTANCE, OnUpdateCalcMoveDistance)
	
	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_ADDLINKEDSKELETON, OnUpdateAddLinkedSkeleton)
	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_REMOVELINKEDSKELETON, OnUpdateRemoveLinkedSkeleton)

	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_ADDEFFECTS, OnUpdateAddEffects)
	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_DELETEEFFECTS, OnUpdateDeleteEffects)

	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_ADDATTACHPOINT, OnUpdateAddAttachPoint)
	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_DELETEATTACHPOINT, OnUpdateDeleteAttachPoint)

	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_ADDBONEGROUP, OnUpdateAddBoneGroup)
	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_DELETEBONEGROUP, OnUpdateDeleteBoneGroup)

	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_ADDFRAMEEVENT, OnUpdateAddFrameEvent)
	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_DELETEFRAMEEVENT, OnUpdateDeleteFrameEvent)

	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_COPYTOCLIPBOARD, OnUpdateCopyToClipboard)
	
	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_ADDLINKEDSKELETON, OnUpdateAddLinkedSkeleton)
	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_REMOVELINKEDSKELETON, OnUpdateRemoveLinkedSkeleton)

	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_SETASLEFTFOOT, OnUpdateSetAsLeftFoot)
	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_SETASLEFTBACKFOOT, OnUpdateSetAsLeftBackFoot)
	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_SETASRIGHTFOOT, OnUpdateSetAsRightFoot)
	ON_UPDATE_COMMAND_UI(ID_MESHEDIT_SETASRIGHTBACKFOOT, OnUpdateSetAsRightBackFoot)

	ON_UPDATE_COMMAND_UI(ID_MODEL_EDITOR_SAVE, OnUpdateSave)
	ON_UPDATE_COMMAND_UI(ID_MODEL_EDITOR_SAVEAS, OnUpdateSaveAs)
	ON_UPDATE_COMMAND_UI(ID_MODEL_EDITOR_DELETE, OnUpdateDeleteMesh)
	ON_UPDATE_COMMAND_UI(ID_MODEL_EDITOR_BUILD_EDGELIST, OnUpdateBuildEdigeList)
	ON_UPDATE_COMMAND_UI(ID_MODEL_EDITOR_CAPTURETHUMB, OnUpdateCaptureThumbImage)

	ON_COMMAND_RANGE(CompositorStartID, CompositorEndID, OnCheckCompositor)
	ON_UPDATE_COMMAND_UI_RANGE(CompositorStartID, CompositorEndID, OnUpdateCheckCompositor)

	ON_COMMAND(ID_SHOW_AXES, OnShowAxes)
	ON_UPDATE_COMMAND_UI(ID_SHOW_AXES, OnUpdateShowAxes)

	ON_COMMAND(ID_RESET_CAMERA, ResetCamera)	
	ON_UPDATE_COMMAND_UI(ID_RESET_CAMERA, OnUpdateResetCamera)

	ON_COMMAND(ID_SHOW_BOUNDINGBOX, OnShowBoundingBox)
	ON_UPDATE_COMMAND_UI(ID_SHOW_BOUNDINGBOX, OnShowBoundingBox)
	
	ON_COMMAND(ID_MATERIAL_SCHEME, OnMaterialScheme)
	ON_UPDATE_COMMAND_UI(ID_MATERIAL_SCHEME, OnUpdateMaterialScheme)

	ON_COMMAND(ID_SHOW_GRID, OnShowGrid)
	ON_UPDATE_COMMAND_UI(ID_SHOW_GRID, OnUpdateShowGrid)

	ON_COMMAND(ID_VIEWPORT_CLR, OnSetBkClr)	

END_MESSAGE_MAP()
#pragma warning(pop)

//----------------------------------------------------------------------------------------------
BOOL MeshEditorFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CFrameWndEx::OnCreateClient(lpcs, pContext);
	
	EnableDocking(CBRS_ALIGN_ANY);
	SetMenu(0);

	//------------------------------------------------------------------------
	// Mesh List Panel
	//------------------------------------------------------------------------
	if (!_listPanel.Create(_UIDisplayString("ID_MODELEDITOR_MESHLIST_PANEL").c_str(),
		this, CRect(0, 0, 400, 200), TRUE, ID_MODELEDITOR_PANEL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		// TRACE0("未能创建“MeshListPanel”窗口\n");
		return FALSE; // 未能创建
	}

	DockPane(&_listPanel);
	
	_listPanel.OnViewList();
	_listPanel.OnRefreshTreeItem();

	//------------------------------------------------------------------------
	// Mesh info Panel
	//------------------------------------------------------------------------
	if (!_infoPanel.Create(_UIDisplayString("ID_MODELEDITOR_INFOPANEL").c_str(),
		this, CRect(0, 0, 300, 200), TRUE, ID_MODELEDITOR_INFOPANEL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		// TRACE0("未能创建“MeshEditorFrm”窗口\n");
		return FALSE; // 未能创建
	}

	//------------------------------------------------------------------------
	// Time line Panel
	//------------------------------------------------------------------------
	if (!_timelinePanel.Create(_UIDisplayString("ID_MODELEDITOR_TIMELINE_PANEL").c_str(),
		this, CRect(0, 0, 300, 100), TRUE, ID_MODELEDITOR_TIMELINE_PANEL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		// TRACE0("未能创建“ModelEditorTimeLinePanel”窗口\n");
		return FALSE; // 未能创建
	}


	//------------------------------------------------------------------------
	// Property Grid Panel
	//------------------------------------------------------------------------
	//if (!_propGrid.Create("ModelEditor Properties", this, CRect(0, 0, 200, 200), TRUE, ID_MODELEDITOR_PROPGRID,
	//	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT| CBRS_FLOAT_MULTI))
	//{
	//	// TRACE0("未能创建“模型属性”窗口\n");
	//	return FALSE; // 未能创建
	//}

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(),
		MAKEINTRESOURCE(IDI_PROPERTIES_WND_HC),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);

	_timelinePanel.SetIcon(hPropertiesBarIcon, FALSE);
	_infoPanel.SetIcon(hPropertiesBarIcon, FALSE);
	//_propGrid.SetIcon(hPropertiesBarIcon, FALSE);
	//DockPane(&_propGrid);
	DockPane(&_infoPanel);
	DockPane(&_timelinePanel);


	//------------------------------------------------------------------------
	// Toolbar
	//------------------------------------------------------------------------
	_wndRibbonBar.Create(this);

	///
	/// Main panel
	///

	CMFCRibbonCategory* mainCat =
		_wndRibbonBar.AddCategory(_UIDisplayString("IDS_HOME").c_str(),
		IDB_FILESMALL, IDB_FILELARGE);

	_viewCategory = mainCat;

	CMFCRibbonPanel* mainPanel = mainCat->AddPanel(
		_UIDisplayString("IDS_HOME").c_str());

	//CMFCRibbonButton* btn = new CMFCRibbonButton(ID_MODEL_EDITOR_OPTIONS,
	//	_UIDisplayString("ID_MODEL_EDITOR_OPTIONS").c_str());
	//mainPanel->Add(btn);


	// Save
	mainPanel->Add(new CMFCRibbonButton(ID_MODEL_EDITOR_SAVE,
		_UIDisplayString("ID_FILE_SAVE").c_str(), 2, 2));

	///
	/// View panel
	///

	CMFCRibbonPanel* viewPanel = mainCat->AddPanel(
		_UIDisplayString("IDS_RIBBON_VIEW").c_str());

	// wire frame
	CMFCRibbonCheckBox* chkbox = new CMFCRibbonCheckBox(ID_SHOW_WIREFRAME,
		_UIDisplayString("ID_SHOW_WIRE").c_str());
	viewPanel->Add(chkbox);

	// axes
	chkbox = new CMFCRibbonCheckBox(ID_SHOW_AXES,
		_UIDisplayString("ID_SHOW_AXES").c_str());
	viewPanel->Add(chkbox);

	// bounding box
	chkbox = new CMFCRibbonCheckBox(ID_SHOW_BOUNDINGBOX,
		_UIDisplayString("ID_SHOW_BOUDNING_BOX").c_str());
	viewPanel->Add(chkbox);

	// gird
	chkbox = new CMFCRibbonCheckBox(ID_SHOW_GRID,
		_UIDisplayString("ID_SHOW_GRID").c_str());
	viewPanel->Add(chkbox);


	// reset camera
	CMFCRibbonButton* btn = new CMFCRibbonButton(ID_RESET_CAMERA,
		_UIDisplayString("ID_RESET_CAMERA").c_str());
	viewPanel->Add(btn);	

	// capture
	btn = new CMFCRibbonButton(ID_MODEL_EDITOR_CAPTURETHUMB,
		_UIDisplayString("ID_MODEL_EDITOR_CAPTURETHUMB").c_str());
	viewPanel->Add(btn);

	// show skeleton
	chkbox = new CMFCRibbonCheckBox(ID_SHOW_SKELETON,
		_UIDisplayString("ID_SHOW_SKELETON").c_str());
	viewPanel->Add(chkbox);

	// build edge list
	btn = new CMFCRibbonButton(ID_MODEL_EDITOR_BUILD_EDGELIST,
		_UIDisplayString("ID_MODEL_EDITOR_BUILD_EDGELIST").c_str());
	viewPanel->Add(btn);

	// view port colour
	CMFCRibbonColorButton* clrBtn = new CMFCRibbonColorButton(ID_VIEWPORT_CLR,
		_UIDisplayString("ID_VIEWPORT_CLR").c_str(), FALSE, 0, 0, RGB(127, 127, 127));
	viewPanel->Add(clrBtn);
	clrBtn->EnableAutomaticButton(_UIDisplayString("ID_AUTO_BK_CLR").c_str(), RGB(127, 127, 127));
	clrBtn->EnableOtherButton(_UIDisplayString("ID_OTHER_BK_CLR").c_str());

	_bkClrButton = clrBtn;

	// material scheme
	CMFCRibbonComboBox* cbtn = new CMFCRibbonComboBox(ID_MATERIAL_SCHEME, FALSE, -1,
		_UIDisplayString("ID_MATERIAL_SCHEME").c_str());
	viewPanel->Add(cbtn);	

	EditorUtil::InitMaterialSchemeList(cbtn);


	///
	/// Edit panel
	///

	CMFCRibbonPanel* editPanel = mainCat->AddPanel(
		_UIDisplayString("IDS_MODEL_EDIT").c_str());

	chkbox = new CMFCRibbonCheckBox(ID_MESHEDIT_SETAS4LEGS,
		_UIDisplayString("ID_MESHEDIT_SETAS4LEGS").c_str());
	editPanel->Add(chkbox);

	btn = new CMFCRibbonButton(ID_MESHEDIT_SETASLEFTFOOT,
		_UIDisplayString("ID_MESHEDIT_SETASLEFTFOOT").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_SETASLEFTBACKFOOT,
		_UIDisplayString("ID_MESHEDIT_SETASLEFTBACKFOOT").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_SETASRIGHTFOOT,
		_UIDisplayString("ID_MESHEDIT_SETASRIGHTFOOT").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_SETASRIGHTBACKFOOT,
		_UIDisplayString("ID_MESHEDIT_SETASRIGHTBACKFOOT").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_CALCMOVEDISTANCE,
		_UIDisplayString("ID_MESHEDIT_CALCMOVEDISTANCE").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_ADDLINKEDSKELETON,
		_UIDisplayString("ID_MESHEDIT_ADDLINKEDSKELETON").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_REMOVELINKEDSKELETON,
		_UIDisplayString("ID_MESHEDIT_REMOVELINKEDSKELETON").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_ADDEFFECTS,
		_UIDisplayString("ID_MESHEDIT_ADDEFFECTS").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_DELETEEFFECTS,
		_UIDisplayString("ID_MESHEDIT_DELETEEFFECTS").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_ADDATTACHPOINT,
		_UIDisplayString("ID_MESHEDIT_ADDATTACHPOINT").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_DELETEATTACHPOINT,
		_UIDisplayString("ID_MESHEDIT_DELETEATTACHPOINT").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_ADDBONEGROUP,
		_UIDisplayString("ID_MESHEDIT_ADDBONEGROUP").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_DELETEBONEGROUP,
		_UIDisplayString("ID_MESHEDIT_DELETEBONEGROUP").c_str());
	editPanel->Add(btn);

	btn = new CMFCRibbonButton(ID_MESHEDIT_COPYTOCLIPBOARD,
		_UIDisplayString("ID_MESHEDIT_COPYTOCLIPBOARD").c_str());
	editPanel->Add(btn);

	///
	/// Compositor category panel
	///
	CMFCRibbonCategory* compCat =
		_wndRibbonBar.AddCategory(_UIDisplayString("ID_COMPOSITOR").c_str(),
		IDB_FILESMALL, IDB_FILELARGE);

	CMFCRibbonPanel* compositorPanel = compCat->AddPanel(
		_UIDisplayString("ID_COMPOSITOR").c_str());

	EditorUtil::InitCompositorList(compositorPanel, CompositorStartID, _compositorEnableStatus);
	

	//if (!_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!_wndToolBar.LoadToolBar(IDR_MODELEDITOR))
	//{
	//	// TRACE0("未能创建模型编辑器工具栏\n");
	//	return FALSE;      // 未能创建
	//}

	//_wndToolBar.SetPaneStyle(_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	//_wndToolBar.SetPaneStyle(_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	//_wndToolBar.SetOwner(this);

	//// 所有命令将通过此控件路由，而不是通过主框架路由:
	//_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&_wndToolBar);

	//SetMenu(IDR_POPUP_MESHEDIT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::Init()
{
	//CMFCToolBar::ResetAll();

	ResEditorBase::Init(C_ViewportVisibilityFlag, "__Model_Editor__");
	_grid->VisibilityFlag(C_ViewportVisibilityFlag);
	_grid->CellSizeOnX(10.0f);
	_grid->CellSizeOnZ(10.0f);

	SetWindowText(_UIDisplayString("IDS_MODEL_EDITOR").c_str());

	GameApp::getSingleton().AddUpdateable(this);

	_renderWindow->SetMouseListener(this);

	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	_boneNode = sm->getRootSceneNode()->createChildSceneNode("__MeshEditorSelectBone__");
	_boneAxes = sm->createEntity("__MeshEditorSelectBone__", "axes.mesh");
	_boneAxes->setQueryFlags(0);
	_boneAxes->setVisibilityFlags(C_ViewportVisibilityFlag);
	_boneNode->attachObject(_boneAxes);
	_boneAxes->setVisible(false);

	ResetCamera();
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CFrameWndEx::PostNcDestroy();
	//OGRE_DELETE this;
}
//----------------------------------------------------------------------------------------------
BOOL MeshEditorFrm::DestroyWindow()
{
// 	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();

// 	OGRE_DELETE _renderWindow;
// 	OGRE_DELETE _propGrid;
	DestroyMesh();

	GameApp::getSingleton().RemoveUpdateable(this);
	
	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	sm->destroyEntity(_boneAxes);

	_boneNode->removeAndDestroyAllChildren();
	_boneNode->detachAllObjects();
	sm->destroySceneNode(_boneNode);

	Shutdown();
	DestroyMesh();

	BOOL r = CFrameWndEx::DestroyWindow();

	return r;
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnSize(UINT nType, int cx, int cy)
{
	if(_renderWindow)
	{
		_renderWindow->GetRenderWindow()->windowMovedOrResized();
	}
	CFrameWndEx::OnSize(nType, cx, cy);
}
//----------------------------------------------------------------------------------------------
LRESULT MeshEditorFrm::OnToolbarReset(WPARAM wp, LPARAM)
{
	UINT uiToolBarId = (UINT) wp;

	switch (uiToolBarId)
	{
	case IDR_MODELEDITOR:
		{
			//EditorUtil::InitCompositorList(_wndToolBar, ID_MODEL_VIEWPORT_COMPOSITOR);
		}
		break;
	}

	return 0;
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::EditMeshFile(const String& meshFile)
{
	_meshFile = meshFile;

	Reset();

	DestroyMesh();

	if(!_meshFile.empty())
	{
		CreateMesh();

		UpdateMeshInfoList();

		if(_aniModel)
		{	
			ResetCamera();
			_timelinePanel.SetMeshData(GetMeshData());
		}
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateResetCamera( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(TRUE);
}
//----------------------------------------------------------------------------------------------
const String& MeshEditorFrm::GetMeshFile() const
{
	return _meshFile;
}
//----------------------------------------------------------------------------------------------
Mesh* MeshEditorFrm::GetMesh()
{
	if(_aniModel)
	{
		return _aniModel->GetEntity()->getMesh().getPointer();
	}

	return 0;
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::CreateMesh()
{
	IGame* game = GameApp::getSingleton().GetGame();
	_aniModel = dynamic_cast<AnimationModel*>(game->CreateGameObject(
		"AnimationModel", XGUID::createGUID()));

	_aniModel->SetVisibilityFlag(C_ViewportVisibilityFlag);
	_aniModel->SetMeshFileEx(_meshFile, false);
	_aniModel->SetPosition(0, 0, 0);
	GameApp::getSingleton().RemoveUpdateable(_aniModel);

	AddBoneVisualizer();
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::DestroyMesh()
{
	if(_aniModel)
	{
		IGame* game = GameApp::getSingleton().GetGame();
		SceneManager* sm = game->GetVisualScene();

		EntityListType::reverse_iterator it = _boneEntitys.rbegin();
		for (; it != _boneEntitys.rend(); ++it)
		{
			(*it)->detachFromParent();
			sm->destroyEntity(*it);
		}
		_boneEntitys.clear();
		_boneVisuals.clear();		

		game->DestroyGameObject(_aniModel);
		_aniModel = 0;
	}

	Reset();
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnDeleteMesh()
{
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnBuildEdigeList()
{
	if(_aniModel)
	{
		GetMesh()->buildEdgeList();
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnShowSkeleton()
{
	SetShowSkeleton(!GetShowSkeleton());
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateShowSkeleton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(_aniModel != 0);
	pCmdUI->SetCheck(GetShowSkeleton());
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnSaveMesh()
{
	if(_aniModel)
	{
		String pth = FileEx::GetResourceFilePath(_meshFile, false);
		pth += GetMeshKeyName() + MESHDATA_EXT;
		GetMeshData()->SaveToFile(pth);
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnSaveMeshAs()
{
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnShowWireframe()
{
	static bool wire = false;
	wire = !wire;

	SetShowWireframe(wire);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateShowWireframe(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetShowWireframe());
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnClose()
{
	//ShowWindow(SW_HIDE);
	CFrameWndEx::OnClose();

	//DestroyWindow();
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::PlayAnimation(const String& aniName)
{
	_playingAnim = aniName;
	if(_aniModel && _aniModel->HasAnimation(_playingAnim))
	{
		_aniModel->PlayAnimation(aniName);
		_timelinePanel.SetCurrentTime(0.0f);
		float l = _aniModel->GetAnimationState(aniName)->getLength();
		_timelinePanel.SetTotalTime(l);
		_isPlay = true;
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::PlayAnimation()
{
	PlayAnimation(_playingAnim);
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::PauseAnimation()
{
	if(_aniModel && !_playingAnim.empty())
	{
		if(_aniModel->HasAnimation(_playingAnim))
		{
			_aniModel->PlayAnimation(_playingAnim);
		}

		_isPlay = false;
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::StopPlayAnimation()
{
	if(_aniModel && !_playingAnim.empty())
	{
		_aniModel->StopAnimation(_playingAnim);
		_isPlay = false;
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::SetPlayingAnimationTimePos(float t)
{
	if(_aniModel && !_playingAnim.empty())
	{
		AnimationState* as = _aniModel->GetAnimationState(_playingAnim);
		as->setTimePosition(t);
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::AddLod(float distance, const String& meshFile)
{
	Mesh* mesh = GetMesh();
	if(mesh)
	{
		mesh->createManualLodLevel(distance, meshFile);
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::UpdateMeshInfoList()
{
	if(!_aniModel)
	{
		return;
	}

	Reset();

	_infoPanel.AddAnimation(_aniModel->GetAllAnimations());
	_infoPanel.SetMesh(GetMesh(), _aniModel->GetEntity());
}
//----------------------------------------------------------------------------------------------
MeshData* MeshEditorFrm::GetMeshData()
{
	if(!_aniModel)
	{
		return 0;
	}

	CString str = _aniModel->GetMeshFile().c_str();
	int i = str.ReverseFind('.');
	if(i != -1)
	{
		str = str.Left(i) + MESHDATA_EXT.c_str();
		MeshDataPtr r =
			MeshDataManager::getSingleton().createOrRetrieve((LPCTSTR)str);
		return r.get();
	}
	else
	{
		assert(false && "Mesh file name is not valid!");
		return 0;
	}
}
//----------------------------------------------------------------------------------------------
String MeshEditorFrm::GetMeshKeyName()
{
	if(_aniModel)
	{
		CString str = _aniModel->GetMeshFile().c_str();
		int i = str.ReverseFind('.');
		if(i != -1)
		{
			str = str.Left(i);
			return (LPCTSTR)str;
		}
	}

	return "";
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::Update(float deltaTime)
{
	if(_aniModel)
	{
		if(_isPlay)
		{
			((IUpdateAble*)_aniModel)->Update(deltaTime);
			if(!_playingAnim.empty())
			{
				float t = _aniModel->GetAnimationState(_playingAnim)->getTimePosition();
				_timelinePanel.SetCurrentTime(t);
			}
		}

		_UpdateBoneVisualizer();
		_timelinePanel.Invalidate();
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::SetShowSkeleton(bool on)
{
	_showSkeleton = on;

	EntityListType::reverse_iterator it = _boneEntitys.rbegin();
	for (; it != _boneEntitys.rend(); ++it)
	{
		(*it)->setVisible(on);
	}
}

//--------------------------------------------------------------------------------
bool MeshEditorFrm::GetShowSkeleton() const
{
	return _showSkeleton;
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::SetShowAttachPoint(bool on)
{
	if(_aniModel && _aniModel->HasSkeleton())
	{
		SkeletonInstance* skl = _aniModel->GetSkeletonInstance();
		//const SkeletonInstance::TagPointList& tagPoints = skl->getTagPoint();
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnAddAttachPoint()
{
	if(!_aniModel || !_selectedBone)
	{
		return;
	}

	InputDlg dlg((_selectedBone->getName() + "_AttachPoint").c_str(), this);
	for(;;)
	{
		if(dlg.DoModal() == IDOK)
		{
			String newAtp = dlg.GetInputText();
			MeshData* md = GetMeshData();
			if(md)
			{
				if(md->HasAttachPoint(newAtp))
				{
					CString str;
					str.Format("AttachPoint already exist:%s", newAtp.c_str());
					AfxMessageBox(str);
				}
				else
				{
					SAttachPoint atp;
					atp.AttachPointName = newAtp;
					atp.BoneName = _selectedBone->getName();
					// TODO: offsets
					md->AddAttchPoint(atp);
					UpdateMeshInfoList();
					break;
				}
			}
			else
			{
				AfxMessageBox("MeshData not found!");
				break;
			}
		}
		else
		{
			break;
		}
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnDelAttachPoint()
{

}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnAddLOD()
{

}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnAddDelLOD()
{

}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnAddAnimation()
{

}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnDelAnimation()
{

}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnAddBoneGroup()
{

}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnDelBoneGroup()
{

}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnAddFrameEvent()
{
	if(!_aniModel || _playingAnim.empty())
	{
		return;
	}

	InputDlg dlg("NewFrameEvent", this);
	for(;;)
	{
		if(dlg.DoModal() == IDOK)
		{
			String newEvent = dlg.GetInputText();
			MeshData* md = GetMeshData();
			if(md)
			{
				if(md->HasFrameEvent(_playingAnim, newEvent))
				{
					CString str;
					str.Format("FrameEvent already exist:%s", newEvent.c_str());
					AfxMessageBox(str);
				}
				else
				{
					SFrameEvent frmEvt;
					frmEvt.EventName = newEvent;
					frmEvt.AnimationTime = _timelinePanel.GetCurrentTimePos();
					md->AddFrameEvent(_playingAnim, frmEvt);
					UpdateMeshInfoList();
					//_timelinePanel.SetFrameEventList(md->GetAnimationFrameEventList(_playingAnim));
					break;
				}
			}
			else
			{
				AfxMessageBox("MeshData not found!");
				break;
			}
		}
		else
		{
			break;
		}
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnDelFrameEvent()
{
	const SFrameEvent* evt = _timelinePanel.GetSelectedFrameEvent();
	if(evt)
	{
		MeshData* md = GetMeshData();
		md->RemoveFrameEvent(_playingAnim, evt->EventName);
		//_timelinePanel.SetFrameEventList(md->GetAnimationFrameEventList(_playingAnim));
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnAddEffects()
{
	if(_selectedAttachPoint == 0)
	{
		AfxMessageBox("AttachPoint must select first!");
		return;
	}

	ParticleSystemListDlg dlg1(this);
	if(dlg1.DoModal() == IDOK)
	{
		if(dlg1.GetSelectedParticleSystemTemplate().empty())
		{
			return;
		}

		InputDlg dlg("NewEffectName", this);
		for(;;)
		{
			if(dlg.DoModal() == IDOK)
			{
				String effName = dlg.GetInputText();
				MeshData* md = GetMeshData();
				if(md)
				{
					if(md->HasAttachEffect(effName))
					{
						CString str;
						str.Format("EffectGroup already exist:%s", effName.c_str());
						AfxMessageBox(str);
					}
					else
					{
						MeshData::EffectAttachPointPair eff;
						eff.AttachPointName = _selectedAttachPoint->AttachPointName;
						eff.EffectTemplateName = dlg1.GetSelectedParticleSystemTemplate();
						md->AddAttachEffect(effName, eff);
						_aniModel->DestroyAttachEffects();
						_aniModel->CreateAttachEffects();
						UpdateMeshInfoList();
						break;
					}
				}
				else
				{
					AfxMessageBox("MeshData not found!");
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnDelEffects()
{
	if(!_selectedEffectGroupName.empty())
	{
		MeshData* md = GetMeshData();
		if(md)
		{
			if(md->HasAttachEffect(_selectedEffectGroupName))
			{
				md->RemoveAttachEffect(_selectedEffectGroupName);
				_selectedEffectGroupName.clear();
				_aniModel->DestroyAttachEffects();
				_aniModel->CreateAttachEffects();
				UpdateMeshInfoList();
			}
		}
	}
}
//----------------------------------------------------------------------------------------------
BOOL MeshEditorFrm::OnMButtonDown(UINT nFlags, CPoint point)
{
	return FALSE;
}
//----------------------------------------------------------------------------------------------
BOOL MeshEditorFrm::OnMButtonUp(UINT nFlags, CPoint point)
{
	return FALSE;
}
//----------------------------------------------------------------------------------------------
BOOL MeshEditorFrm::OnLButtonUp(UINT nFlags, CPoint point)
{
	return FALSE;
}
//----------------------------------------------------------------------------------------------
BOOL MeshEditorFrm::OnLButtonDown(UINT nFlags, CPoint point)
{
	return FALSE;
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::Reset()
{
	_infoPanel.Reset();
	_selectedBone = 0;
	_selectedAttachPoint = 0;
	_selectedEffectGroupName.clear();
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnSelectBone(Bone* bone)
{
	_selectedBone = bone;
	_boneNode->setPosition(bone->_getDerivedPosition());
	_boneNode->setOrientation(bone->_getDerivedOrientation());
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnSelectAttachPoint(SAttachPoint* atp)
{
	_selectedAttachPoint = atp;
	Bone* bone = _aniModel->GetAttatchPointBone(atp->AttachPointName);
	_boneNode->setPosition(bone->_getDerivedPosition() + atp->PositionOffset);
	_boneNode->setOrientation(bone->_getDerivedOrientation() + 
		MathUtil::MakeQuaternion(atp->OrientationOffset.x,
		atp->OrientationOffset.y, atp->OrientationOffset.z));
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnSelectEffectGroup(const String& effGrpName)
{
	_selectedEffectGroupName = effGrpName;
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnCompositorSelect()
{
	static CompositorEnabledMap	compositorMap;
	Viewport* vp = _renderWindow->GetViewport();
	EditorUtil::SelectViewportCompsoitor(vp, ID_MODEL_VIEWPORT_COMPOSITOR, compositorMap);
}
//----------------------------------------------------------------------------------------------
void MeshEditorFrm::OnAddLinkedSkeleton()
{
	StringVector lst;
// 	SkeletonManager::ResourceMapIterator it =
// 		SkeletonManager::getSingleton().getResourceIterator();
// 	while(it.hasMoreElements())
// 	{
// 		lst.push_back(it.peekNextValue()->getName());
// 		it.moveNext();
// 	}

	lst = *(ResourceGroupManager::getSingleton().findResourceNames(
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "*.skeleton"));
	
	ListSelectDlg dlg("Add Linked Skeleton", lst, this);
	if(dlg.DoModal() == IDOK)
	{
		MeshData* md = GetMeshData();
		if(md)
		{
			md->AddLinkedSkeleton(dlg.GetSelectedItem());
			_aniModel->GetEntity()->refreshAvailableAnimationState();

			EditMeshFile(_meshFile);
		}
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnRemoveLinkedSkeleton()
{
	MeshData* md = GetMeshData();
	if(md)
	{
		ListSelectDlg dlg("Add Linked Skeleton", md->GetLinkedSkeletonList(), this);
		if(dlg.DoModal() == IDOK)
		{
			md->RemoveLinkedSkeleton(dlg.GetSelectedItem());
			EditMeshFile(_meshFile);
		}
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnCaptureThumbImage()
{
	bool showAxes = IsAxesVisible();
	bool showBoneAxes = _boneAxes->isVisible();
	bool showBounding = _aniModel->GetShowBoundingBox();
	bool showGrid = GetShowGrid();

	_boneAxes->setVisible(false);
	_aniModel->SetShowBoundingBox(false);

	CaptureThumbImage(
		EditorUtil::GetThumbFileName(_aniModel->GetMeshFile()));

	_boneAxes->setVisible(showBoneAxes);
	_aniModel->SetShowBoundingBox(showBounding);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnCopyPlayingAnimationNameToClipboard()
{
	if(TRUE == OpenClipboard())
	{
		EmptyClipboard();
		HGLOBAL hglbCopy = 0;
		hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 
			(_playingAnim.size() + 1) * sizeof(char)); 

		LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hglbCopy); 
		memcpy(lptstrCopy, _playingAnim.c_str(), 
			_playingAnim.size() * sizeof(char)); 
		lptstrCopy[_playingAnim.size()] = (char) 0;    // null character 
		GlobalUnlock(hglbCopy); 


		SetClipboardData(CF_TEXT, hglbCopy);
		CloseClipboard();
	}
}
//--------------------------------------------------------------------------------
String MeshEditorFrm::GetSelectedBoneName() const
{
	if(_selectedBone)
	{
		return _selectedBone->getName();
	}

	return "";
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::SetAs4Legs(bool on)
{
	MeshData* md = GetMeshData();
	if(!md)
	{
		return;
	}

	GetMeshData()->Set4Legs(on);
}
//--------------------------------------------------------------------------------
bool MeshEditorFrm::Is4Legs()
{
	MeshData* md = GetMeshData();
	if(!md)
	{
		return false;
	}

	return GetMeshData()->Is4Legs();
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::SetAsLeftFoot()
{
	MeshData* md = GetMeshData();
	if(!md)
	{
		return;
	}

	if(!GetSelectedBoneName().empty())
	{
		GetMeshData()->SetLeftFootBone(GetSelectedBoneName());
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::SetAsLeftBackFoot()
{
	MeshData* md = GetMeshData();
	if(!md)
	{
		return;
	}

	if(!GetSelectedBoneName().empty())
	{
		GetMeshData()->SetLeftBackFootBone(GetSelectedBoneName());
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::SetAsRightFoot()
{
	MeshData* md = GetMeshData();
	if(!md)
	{
		return;
	}

	if(!GetSelectedBoneName().empty())
	{
		GetMeshData()->SetRightFootBone(GetSelectedBoneName());
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::SetAsRightBackFoot()
{
	MeshData* md = GetMeshData();
	if(!md)
	{
		return;
	}

	if(!GetSelectedBoneName().empty())
	{
		GetMeshData()->SetRightBackFootBone(GetSelectedBoneName());
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::CalcMoveDist()
{
	PauseAnimation();

	MeshData* md = GetMeshData();
	if(!md)
	{
		return;
	}

	if(_playingAnim.empty())
	{
		return;
	}

	// for player actor
	if(!Is4Legs())
	{
		SkeletonInstance* sk = _aniModel->GetSkeletonInstance();
		Bone* leftFoot = sk->getBone(md->GetLeftFootBone());
		Bone* rightFoot = sk->getBone(md->GetRightFootBone());

		Animation* animal = sk->getAnimation(_playingAnim);

		NodeAnimationTrack* leftTrack = animal->getNodeTrack(leftFoot->getHandle() + 1);
		NodeAnimationTrack* rightTrack = animal->getNodeTrack(rightFoot->getHandle() + 1);

		TransformKeyFrame* minLeftKF = 0;
		TransformKeyFrame* maxLeftKF = 0;
		float maxZ = 0.0f;
		float minZ = 0.0f;

		for(size_t i = 0; i < leftTrack->getNumKeyFrames(); ++i)
		{
			TransformKeyFrame* kf = leftTrack->getNodeKeyFrame(i);

			float z = kf->getTranslate().z;

			if(!maxLeftKF)
			{
				maxLeftKF = kf;
				maxZ = z;
			}
			else if(z > maxZ)
			{
				maxZ = z;
				maxLeftKF = kf;
			}

			if(!minLeftKF)
			{
				minLeftKF = kf;
				minZ = z;
			}
			else if(z < minZ)
			{
				minZ = z;
				minLeftKF = kf;
			}
		}

		AnimationState* as = _aniModel->GetAnimationState(_playingAnim);
		
		//animal->apply(sk, minLeftKF->getTime());
// 		as->setTimePosition(minLeftKF->getTime());
// 		_aniModel->GetEntity()->_updateAnimation();
// 		float minLeftKFZ = leftFoot->_getDerivedPosition().z;
// 
// 		//animal->apply(sk, maxLeftKF->getTime());
// 		as->setTimePosition(maxLeftKF->getTime());
// 		_aniModel->GetEntity()->_updateAnimation();
// 		float maxLeftKFZ = leftFoot->_getDerivedPosition().z;
// 
		
		TransformKeyFrame* minRightKF = 0;
		TransformKeyFrame* maxRightKF = 0;
		minZ = maxZ = 0.0f;

		for(size_t i = 0; i < leftTrack->getNumKeyFrames(); ++i)
		{
			TransformKeyFrame* kf = leftTrack->getNodeKeyFrame(i);

			float z = kf->getTranslate().z;

			if(!maxRightKF)
			{
				maxRightKF = kf;
				maxZ = z;
			}
			else if(z > maxZ)
			{
				maxZ = z;
				maxRightKF = kf;
			}

			if(!minRightKF)
			{
				minRightKF = kf;
				minZ = z;
			}
			else if(z < minZ)
			{
				minZ = z;
				minRightKF = kf;
			}
		}

		//animal->apply(sk, minRightKF->getTime());
		as->setTimePosition(minRightKF->getTime());
		_aniModel->GetEntity()->_updateAnimation();
		float minRightKFZR = rightFoot->_getDerivedPosition().z;
		float minRightKFZL = leftFoot->_getDerivedPosition().z;

		//animal->apply(sk, maxRightKF->getTime());
		as->setTimePosition(maxRightKF->getTime());
		_aniModel->GetEntity()->_updateAnimation();
		float maxRightKFZR = rightFoot->_getDerivedPosition().z;
		float maxRightKFZL = leftFoot->_getDerivedPosition().z;

		as->setTimePosition(minLeftKF->getTime());
		_aniModel->GetEntity()->_updateAnimation();
		float minLeftKFZL = leftFoot->_getDerivedPosition().z;
		float minLeftKFZR = rightFoot->_getDerivedPosition().z;

		as->setTimePosition(maxLeftKF->getTime());
		_aniModel->GetEntity()->_updateAnimation();
		float maxLeftKFZL = leftFoot->_getDerivedPosition().z;
		float maxLeftKFZR = rightFoot->_getDerivedPosition().z;

		// max is always right
		if(maxRightKFZL > maxRightKFZR)
		{
			std::swap(maxRightKFZL, maxRightKFZR);
		}

		if(maxLeftKFZL > maxLeftKFZR)
		{
			std::swap(maxLeftKFZL, maxLeftKFZR);
		}

		if(minRightKFZL < minRightKFZR)
		{
			std::swap(minRightKFZL, minRightKFZR);
		}

		if(minLeftKFZL < minLeftKFZR)
		{
			std::swap(minLeftKFZL, minLeftKFZR);
		}


		float maxR = std::max(maxRightKFZR, maxRightKFZL);
		maxR = std::max(maxR, maxLeftKFZL);
		maxR = std::max(maxR, maxLeftKFZR);

		float minL = std::min(minRightKFZR, minRightKFZL);
		minL = std::min(minL, minLeftKFZL);
		minL = std::min(minL, minLeftKFZR);

		float dist = std::abs(maxR - minL);

		md->SetAnimationMoveDistance(_playingAnim, dist);
		_timelinePanel.SetCurrentTime(maxRightKF->getTime());
		_timelinePanel.Invalidate();
	}
	else // for monsters or mount
	{

	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnToggle4Legs()
{
	SetAs4Legs(!Is4Legs());
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdate4Legs(CCmdUI *pCmdUI)
{
	if(!_aniModel)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

	if(Is4Legs())
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateCalcMoveDistance(CCmdUI *pCmdUI)
{
	BOOL on = FALSE;
	if(!_aniModel || !GetMeshData())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		on = TRUE;
	}

	MeshData* md = GetMeshData();

	if(_playingAnim.empty()
		|| md->GetLeftFootBone().empty()
		|| md->GetRightFootBone().empty())
	{
		on = FALSE;
	}
	else if(Is4Legs())
	{
		if(_playingAnim.empty()
			|| md->GetLeftFootBone().empty()
			|| md->GetRightFootBone().empty()
			|| md->GetLeftBackFootBone().empty()
			|| md->GetRightBackFootBone().empty())
		{
			on = FALSE;
		}
	}
	else
	{
		on = TRUE;
	}

	pCmdUI->Enable(on);

}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateAddLinkedSkeleton(CCmdUI *pCmdUI)
{
	if(!_aniModel || !_aniModel->HasSkeleton())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateRemoveLinkedSkeleton(CCmdUI *pCmdUI)
{
	OnUpdateAddLinkedSkeleton(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateSetAsLeftFoot(CCmdUI* pCmdUI)
{
	if(!_aniModel || !_selectedBone)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateSetAsLeftBackFoot(CCmdUI* pCmdUI)
{
	OnUpdateSetAsLeftFoot(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateSetAsRightFoot(CCmdUI* pCmdUI)
{
	OnUpdateSetAsLeftFoot(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateSetAsRightBackFoot(CCmdUI* pCmdUI)
{
	OnUpdateSetAsLeftFoot(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateSave(CCmdUI* pCmdUI)
{
	if(!_aniModel)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateSaveAs(CCmdUI* pCmdUI)
{
	if(!_aniModel)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateDeleteMesh(CCmdUI* pCmdUI)
{
	OnUpdateSaveAs(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateBuildEdigeList(CCmdUI* pCmdUI)
{
	OnUpdateSaveAs(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateCaptureThumbImage(CCmdUI* pCmdUI)
{
	OnUpdateSaveAs(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateAddEffects(CCmdUI* pCmdUI)
{
	OnUpdateAddLinkedSkeleton(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateDeleteEffects(CCmdUI* pCmdUI)
{
	OnUpdateAddLinkedSkeleton(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateAddBoneGroup(CCmdUI* pCmdUI)
{
	if(!_aniModel || _aniModel->HasSkeleton() || !_selectedBone)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateDeleteBoneGroup(CCmdUI* pCmdUI)
{
	OnUpdateAddBoneGroup(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateAddAttachPoint(CCmdUI* pCmdUI)
{
	OnUpdateAddLinkedSkeleton(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateDeleteAttachPoint(CCmdUI* pCmdUI)
{
	OnUpdateAddLinkedSkeleton(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateAddFrameEvent(CCmdUI* pCmdUI)
{
	if(_playingAnim.empty())
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateDeleteFrameEvent(CCmdUI* pCmdUI)
{
	OnUpdateAddLinkedSkeleton(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateCopyToClipboard(CCmdUI* pCmdUI)
{
	OnUpdateAddLinkedSkeleton(pCmdUI);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnCheckCompositor( UINT id )
{
	Viewport* vp = this->_renderWindow->GetViewport();
	EditorUtil::SelectViewportCompsoitor(vp, _compositorEnableStatus, id);
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateCheckCompositor( CCmdUI* pCmdUI )
{
	EditorUtil::UpdateCompositorUI(pCmdUI, _compositorEnableStatus);
}
//--------------------------------------------------------------------------------

void MeshEditorFrm::AddBoneVisualizer()
{
	if(!_aniModel)
	{
		return;
	}

	Entity* ent = _aniModel->GetEntity();
	SkeletonInstance* skeletonInstance = ent->getSkeleton();//_aniModel->GetSkeletonInstance();

	if (!skeletonInstance)
	{
		return;
	}


	Skeleton::BoneIterator it = skeletonInstance->getBoneIterator();
	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	while (it.hasMoreElements())
	{
		Bone* pBone = it.getNext();

		Bone::ChildNodeIterator cit = pBone->getChildIterator();
		int iCount = 0;
		while (cit.hasMoreElements())
		{
			Node* pChild = cit.getNext();
			iCount++;

			String strName = pBone->getName() + "_" + pChild->getName();
			Ogre::Entity* bone = sm->createEntity(strName, "bone.mesh");
			bone->setVisibilityFlags(C_ViewportVisibilityFlag);
			bone->setRenderQueueGroup(RENDER_QUEUE_MAX);

			TagPoint* pTag	= ent->attachObjectToBone(pBone->getName(), bone);

			bone->setVisible(_showSkeleton);

			_boneEntitys.push_back(bone);

			_boneVisuals[pTag] = pChild;
		}

		if (iCount == 0)
		{
			Ogre::Entity* bone = sm->createEntity(pBone->getName(), "bone.mesh");
			TagPoint* pTag	= ent->attachObjectToBone(pBone->getName(), bone);

			bone->setVisible(_showSkeleton);
			bone->setVisibilityFlags(C_ViewportVisibilityFlag);
			bone->setRenderQueueGroup(RENDER_QUEUE_MAX);

			_boneEntitys.push_back(bone);

			_boneVisuals[pTag] = 0;
		}
	}

	_UpdateBoneVisualizer();
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::_UpdateBoneVisualizer()
{
	for (TagChildBoneMapType::iterator it = _boneVisuals.begin(); it != _boneVisuals.end(); ++it)
	{
		TagPoint* pTag		= it->first;
		Node* pChild		= it->second;

		if (pChild)
		{
			Vector3 vChild		= pChild->getPosition();
			Vector3 targetDir	= vChild.normalisedCopy();

			const Quaternion& currentOrient = pTag->getOrientation();
			Vector3 currentDir				= currentOrient * Vector3::UNIT_Y;

			Quaternion rotQuat				= currentDir.getRotationTo(targetDir);
			Quaternion	targetOrientation	= rotQuat * currentOrient;

			pTag->setOrientation(targetOrientation);

			Vector3 vOffset = targetOrientation.UnitInverse() * vChild;
			pTag->setScale(1, fabs(vOffset.y), 1);
		}
		else
		{
			pTag->setScale(1, 5, 1);
		}
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnShowAxes()
{
	ShowAxes(!IsAxesVisible());
	_boneAxes->setVisible(!_boneAxes->isVisible());
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateShowAxes( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(IsAxesVisible());
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::ResetCamera()
{
	AxisAlignedBox aabb;
	if(_aniModel)
	{
		aabb = _aniModel->GetEntity()->getBoundingBox();
	}
	else
	{
		aabb.setExtents(-100, -100, -100, 100, 100, 100);
	}

	IViewportCameraController* camCtrl = _renderWindow->GetCameraController();
	LookAtCamera* lookAtCam = dynamic_cast<LookAtCamera*>(camCtrl);

	Vector3 sz = aabb.getSize();
	float r = sz.length();

	Camera* cam = lookAtCam->GetCamera();
	cam->setPosition(aabb.getCenter() + Vector3(0, 0.0f, r * 1.5f));
	cam->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);

	lookAtCam->SetMinDistance(r * 0.01f);
	lookAtCam->SetMaxDistance(r * 5.0f);
	lookAtCam->SetZoomDelta(r * 0.1f);
	lookAtCam->SetCameraRadius(r * 1.5f);
	lookAtCam->SetYRot(Degree(22.5f).valueRadians());
	lookAtCam->SetXRot(Degree(0.0f).valueRadians());
	lookAtCam->SetLookAt(aabb.getCenter());


	Root::getSingleton().renderOneFrame();
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnShowBoundingBox()
{
	if(_aniModel)
	{
		bool on = _aniModel->GetShowBoundingBox();
		_aniModel->SetShowBoundingBox(!on);
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnShowBoundingBox( CCmdUI* pCmdUI )
{
	if(_aniModel)
	{
		bool on = _aniModel->GetShowBoundingBox();
		pCmdUI->SetCheck(on);
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnMaterialScheme()
{
	if(MaterialManager::getSingletonPtr())
	{
		CMFCRibbonComboBox* combo = (CMFCRibbonComboBox*)
			_viewCategory->FindByID(ID_MATERIAL_SCHEME);

		if(combo)
		{
			Viewport* vp = _renderWindow->GetViewport();
			String str = (LPCTSTR)combo->GetItem(combo->GetCurSel());
			vp->setMaterialScheme(str);
		}
	}
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateMaterialScheme( CCmdUI* pCmdUI )
{
	pCmdUI->Enable((_viewCategory != 0) && _renderWindow
		&& (_renderWindow->GetViewport()));
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnShowGrid()
{
	SetShowGrid(!GetShowGrid());
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnUpdateShowGrid( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(GetShowGrid());
}
//--------------------------------------------------------------------------------
void MeshEditorFrm::OnSetBkClr()
{
	COLORREF clr = _bkClrButton->GetColor();
	
	Ogre::ColourValue c;
	c.setAsABGR(clr);

	this->SetViewportColour(c);
}