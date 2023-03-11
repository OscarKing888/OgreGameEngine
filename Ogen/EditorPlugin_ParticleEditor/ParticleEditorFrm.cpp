#include "stdafx.h"
#include "ParticleEditorFrm.h"
#include "Resource.h"
#include "EditorCore/ListSelectDlg.h"
#include "EditorCore/PropertyEditor_MeshFile.h"
#include "EditorCore/InputDlg.h"
#include "ParticleUtil.h"
#include "EditorCore/Grid.h"
#include "ParticleSystemEditor.h"
#include "ParticleEditorEvents.h"

#define ID_PARTCILE_PROPERTIESWND           2

#define COMPOSITOR_COUNT 100
UINT ParticleEditorFrm::CompositorStartID = GetAutoWMID();

bool AdvanceWMID()
{
	for (int i = 0; i < COMPOSITOR_COUNT; ++i)
	{
		GetAutoWMID();
	}
	return true;
}

bool callFun = AdvanceWMID();

UINT ParticleEditorFrm::CompositorEndID = CompositorStartID + COMPOSITOR_COUNT;

//--------------------------------------------------------------------------------

template<> ParticleEditorFrm* SingletonPhoenix<ParticleEditorFrm>::ms_Singleton = 0;

const uint32 C_ViewportVisibilityFlag = 0xFFFFFFFF;//0x10000000;

const String& ParticleEditorFrm::GetTypeName() const
{
	static String typeName = "ParticleEditorFrm";
	return typeName;
}

IMPLEMENT_DYNCREATE(ParticleEditorFrm, CFrameWndEx)

ParticleEditorFrm::ParticleEditorFrm()
{
	EventsManager::getSingleton().AddListener(this);
}
//--------------------------------------------------------------------------------
ParticleEditorFrm::~ParticleEditorFrm()
{
	EventsManager::getSingleton().RemoveListener(this);
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::DoDataExchange(CDataExchange* pDX)
{
	CFrameWndEx::DoDataExchange(pDX);	
}
//--------------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable : 4407)

BEGIN_MESSAGE_MAP(ParticleEditorFrm, CFrameWndEx)
	ON_WM_SIZE()

	ON_COMMAND(ID_PARTICLE_OPTION, OnShowOptions)

	ON_COMMAND(ID_NEW_PARTICLE, OnNewParticle)

	ON_COMMAND(ID_SAVE_PARTICLE, OnSaveParticle)
	ON_UPDATE_COMMAND_UI(ID_SAVE_PARTICLE, OnUpdateSaveParticle)

	
	ON_COMMAND(ID_SHOW_WIREFRAME, OnShowWireframe)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WIREFRAME, OnUpdateShowWireframe)

	ON_WM_CLOSE()
	//ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	
	ON_COMMAND(ID_PARTICLE_EDITOR_CAPTURE, OnCaptureThumbImage)

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

	ON_COMMAND(ID_PARTICLEEDIT_UNDO, OnUndo)
	ON_UPDATE_COMMAND_UI(ID_PARTICLEEDIT_UNDO, OnUpdateUndo)

	ON_COMMAND(ID_PARTICLEEDIT_REDO, OnRedo)
	ON_UPDATE_COMMAND_UI(ID_PARTICLEEDIT_REDO, OnUpdateRedo)
	
END_MESSAGE_MAP()

#pragma warning(pop)
//--------------------------------------------------------------------------------
BOOL ParticleEditorFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CFrameWndEx::OnCreateClient(lpcs, pContext);
	
	EnableDocking(CBRS_ALIGN_ANY);
	SetMenu(0);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(),
		MAKEINTRESOURCE(IDI_PROPERTIES_WND_HC),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON),
		::GetSystemMetrics(SM_CYSMICON), 0);

	//------------------------------------------------------------------------
	// Particle List Panel
	//------------------------------------------------------------------------
	if (!_listPanel.Create(_UIDisplayString("ID_PARTICLE_LIST_PANEL").c_str(),
		this, CRect(0, 0, 200, 230), TRUE, ID_PARTICLE_LIST_PANEL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		// TRACE0("未能创建“ParticleListPanel”窗口\n");
		return FALSE; // 未能创建
	}

	DockPane(&_listPanel);
	
	//------------------------------------------------------------------------
	// TimeLine Panel
	//------------------------------------------------------------------------
	//
	if(!_timelinePanel.Create(_UIDisplayString("ID_TIMELINE_PANEL").c_str(),
		this, CRect(0, 0, 100, 150), TRUE, ID_TIMELINE_PANEL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |  CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		return FALSE;
	}
	_timelinePanel.SetIcon(hPropertiesBarIcon, FALSE);
	DockPane(&_timelinePanel);

	//------------------------------------------------------------------------
	// Property Grid Panel
	//------------------------------------------------------------------------
	if (!_propGrid.Create(_UIDisplayString("ID_PARTICLEEDITOR_PROPGRID").c_str(),
		this, CRect(0, 0, 300, 200), TRUE, ID_PARTICLEEDITOR_PROPGRID,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		// TRACE0("未能创建“粒子属性”窗口\n");
		return FALSE; // 未能创建
	}

	_propGrid.SetIcon(hPropertiesBarIcon, FALSE);
	DockPane(&_propGrid);

	//------------------------------------------------------------------------
	// Component Panel
	//------------------------------------------------------------------------

	if (!_componentPanel.Create(_UIDisplayString("ID_PARTICLE_COMPONENT_PANEL").c_str(),
		this, CRect(0, 0, 280, 200), TRUE, ID_PARTICLE_COMPONENT_PANEL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		// TRACE0("未能创建“粒子属性”窗口\n");
		return FALSE; // 未能创建
	}
	_componentPanel.SetIcon(hPropertiesBarIcon, FALSE);
	DockPane(&_componentPanel);	

	//------------------------------------------------------------------------
	// Toolbar
	//------------------------------------------------------------------------
	//if (!_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!_wndToolBar.LoadToolBar(IDR_PARTICLE_WND))
	//{
	//	// TRACE0("未能创建粒子编辑器工具栏\n");
	//	return FALSE;      // 未能创建
	//}

	//_wndToolBar.SetPaneStyle(_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	//_wndToolBar.SetPaneStyle(_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	//_wndToolBar.SetOwner(this);

	//// 所有命令将通过此控件路由，而不是通过主框架路由:
	//_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&_wndToolBar);

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

	// New
	mainPanel->Add(new CMFCRibbonButton(ID_NEW_PARTICLE,
		_UIDisplayString("ID_NEW_PARTICLE").c_str(), 0, 0));

	// Save
	mainPanel->Add(new CMFCRibbonButton(ID_SAVE_PARTICLE,
		_UIDisplayString("ID_FILE_SAVE").c_str(), 2, 2));

	// Undo
	HICON icon = AfxGetApp()->LoadIcon(ID_PARTICLEEDIT_UNDO);
	mainPanel->Add(new CMFCRibbonButton(ID_PARTICLEEDIT_UNDO,
		_UIDisplayString("ID_EDIT_UNDO").c_str(), icon));

	// Redo
	icon = AfxGetApp()->LoadIcon(ID_PARTICLEEDIT_REDO);
	mainPanel->Add(new CMFCRibbonButton(ID_PARTICLEEDIT_REDO,
		_UIDisplayString("ID_EDIT_REDO").c_str(), icon));

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
	btn = new CMFCRibbonButton(ID_PARTICLE_EDITOR_CAPTURE,
		_UIDisplayString("ID_MODEL_EDITOR_CAPTURETHUMB").c_str());
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
	/// Compositor category panel
	///
	CMFCRibbonCategory* compCat = mainCat;
		//_wndRibbonBar.AddCategory(_UIDisplayString("ID_COMPOSITOR").c_str(),
		//IDB_FILESMALL, IDB_FILELARGE);

	CMFCRibbonPanel* compositorPanel = compCat->AddPanel(
		_UIDisplayString("ID_COMPOSITOR").c_str());

	EditorUtil::InitCompositorList(compositorPanel, CompositorStartID, _compositorEnableStatus);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::Init()
{
	RefreshParticleList();

	//CMFCToolBar::ResetAll();

	ResEditorBase::Init(C_ViewportVisibilityFlag, "__Particle_Editor__");

	_grid->VisibilityFlag(C_ViewportVisibilityFlag);
	_grid->CellSizeOnX(10.0f);
	_grid->CellSizeOnZ(10.0f);


	SetWindowText(_UIDisplayString("IDS_PARTICLE_EDITOR").c_str());


	ResetCamera();
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CFrameWndEx::PostNcDestroy();
	//OGRE_DELETE this;
}
//--------------------------------------------------------------------------------
BOOL ParticleEditorFrm::DestroyWindow()
{
	Shutdown();

	ParticleSystemEditor::getSingleton().DestroyCurrentParticleSystemInstance();


	BOOL r = CFrameWndEx::DestroyWindow();

	return r;
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnSize(UINT nType, int cx, int cy)
{
	if(_renderWindow)
	{
		_renderWindow->GetRenderWindow()->windowMovedOrResized();
	}

	CFrameWndEx::OnSize(nType, cx, cy);	
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::RefreshParticleList()
{
	_listPanel.OnRefreshListItem();	
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::BuildPropertyMap()
{
	ResEditorBase::BuildPropertyMap();

	//AddProp(Property_MeshFile, MeshFile, ParticleEditorFrm, "MeshFile", "ParticleEditor");
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnShowOptions()
{
	_propGrid.SetObject(this);
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnNewParticle()
{
	ParticleSystemEditor& editor = ParticleSystemEditor::getSingleton();
	if(editor.IsUndoEnable())
	{
		int r = AfxMessageBox(
			formatString(_UIDisplayString("IDSF_SAVE_PARTICLE").c_str(), 
			editor.GetCurrentTemplate().c_str()).c_str(), MB_YESNOCANCEL);

		if(r == IDCANCEL)
		{
			return;
		}

		if(r == IDYES)
		{
			editor.SaveAllParticleTemplate();
		}
	}

	InputDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		String str = dlg.GetInputText();
		if(ParticleSystemManager::getSingleton().getTemplate(str))
		{
			AfxMessageBox(formatString(
				_UIDisplayString("IDSF_PARTICLE_ALREADY_EXIST").c_str(),
				str.c_str()).c_str());

			return;
		}

		EventsManager::getSingleton().CreateEvent(
			ParticleEditorEvents::NewParticleSystem,
			Any(str));

		RefreshParticleList();
	}
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnClose()
{
	//ShowWindow(SW_HIDE);
	CFrameWndEx::OnClose();

	//DestroyWindow();
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnShowWireframe()
{
	SetShowWireframe(!GetShowWireframe());
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnUpdateShowWireframe(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetShowWireframe());
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnCaptureThumbImage()
{
	ParticleSystemEditor& editor = ParticleSystemEditor::getSingleton();
	bool on = editor.GetShowBounding();
	editor.SetShowBounding(false);
	CaptureThumbImage(EditorUtil::GetParticleThumbFileName(editor.GetCurrentTemplate()));
	editor.SetShowBounding(on);
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnCheckCompositor( UINT id )
{
	Viewport* vp = this->_renderWindow->GetViewport();
	EditorUtil::SelectViewportCompsoitor(vp, _compositorEnableStatus, id);
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnUpdateCheckCompositor( CCmdUI* pCmdUI )
{
	EditorUtil::UpdateCompositorUI(pCmdUI, _compositorEnableStatus);
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnShowBoundingBox()
{
	ParticleSystemEditor::getSingleton().SetShowBounding(
		!ParticleSystemEditor::getSingleton().GetShowBounding());
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnShowBoundingBox( CCmdUI* pCmdUI )
{
	if(ParticleSystemEditor::getSingleton().GetCurrentParticles())
	{
		pCmdUI->SetCheck(ParticleSystemEditor::getSingleton().GetShowBounding());
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnUpdateResetCamera( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(TRUE);
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnMaterialScheme()
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
void ParticleEditorFrm::OnUpdateMaterialScheme( CCmdUI* pCmdUI )
{
	pCmdUI->Enable((_viewCategory != 0) && _renderWindow
		&& (_renderWindow->GetViewport()));
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnShowGrid()
{
	SetShowGrid(!GetShowGrid());
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnUpdateShowGrid( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(GetShowGrid());
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnSetBkClr()
{
	COLORREF clr = _bkClrButton->GetColor();

	Ogre::ColourValue c;
	c.setAsABGR(clr);

	//this->SetViewportColour(c);
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnShowAxes()
{
	ShowAxes(!IsAxesVisible());
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnUpdateShowAxes( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(IsAxesVisible());
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::ResetCamera()
{
	AxisAlignedBox aabb;
	if(ParticleSystemEditor::getSingleton().GetCurrentParticles())
	{
		aabb = ParticleSystemEditor::getSingleton().GetCurrentParticles()->getBoundingBox();
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
	lookAtCam->SetYRot(Degree(45.0f).valueRadians());
	lookAtCam->SetXRot(0.0f);
	lookAtCam->SetLookAt(aabb.getCenter());


	Root::getSingleton().renderOneFrame();
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnEvents( const String& eventName, const Any& eventContex,
								 bool& stopBroadCast )
{
	if(eventName == ParticleEditorEvents::ShowParticleObjectProp)
	{
		IPropertyObject* obj = any_cast<IPropertyObject*>(eventContex);
		_propGrid.SetObject(obj);
	}
	else if((eventName == ParticleEditorEvents::ClearObjectProp)
		|| (eventName == ParticleEditorEvents::RemoveEmitter)
		|| (eventName == ParticleEditorEvents::RemoveAffector)
		|| (eventName == ParticleEditorEvents::UpdateRemoveAffectorUI)
		|| (eventName == ParticleEditorEvents::UpdateRemoveEmitterUI))
	{		
		_propGrid.SetObject(0);
	}
	else if(eventName == ParticleEditorEvents::UpdateTimeLineEditor)
	{
		_timelinePanel.RelayoutEditor();
	}
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnUndo()
{
	ParticleSystemEditor::getSingleton().Undo();
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnUpdateUndo( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(ParticleSystemEditor::getSingleton().IsUndoEnable());
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnRedo()
{
	ParticleSystemEditor::getSingleton().Redo();
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnUpdateRedo( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(ParticleSystemEditor::getSingleton().IsRedoEnable());
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnSaveParticle()
{
	ParticleSystemEditor::getSingleton().SaveAllParticleTemplate();
}
//--------------------------------------------------------------------------------
void ParticleEditorFrm::OnUpdateSaveParticle( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(ParticleSystemEditor::getSingleton().IsUndoEnable());
}
//--------------------------------------------------------------------------------