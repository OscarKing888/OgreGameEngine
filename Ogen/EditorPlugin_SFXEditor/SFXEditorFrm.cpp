#include "stdafx.h"
#include "SFXEditorFrm.h"
#include "Resource.h"
#include "EditorCore/ListSelectDlg.h"
#include "EditorCore/PropertyEditor_MeshFile.h"
#include "EditorCore/InputDlg.h"
#include "EditorCore/Grid.h"
#include "SFXEditor.h"
#include "SFXEditorEvents.h"

#define ID_PARTCILE_PROPERTIESWND           2

#define COMPOSITOR_COUNT 100
UINT SFXEditorFrm::CompositorStartID = GetAutoWMID();

bool AdvanceWMID()
{
	for (int i = 0; i < COMPOSITOR_COUNT; ++i)
	{
		GetAutoWMID();
	}
	return true;
}

bool callFun = AdvanceWMID();

UINT SFXEditorFrm::CompositorEndID = CompositorStartID + COMPOSITOR_COUNT;

//--------------------------------------------------------------------------------

template<> SFXEditorFrm* SingletonPhoenix<SFXEditorFrm>::ms_Singleton = 0;

const uint32 C_ViewportVisibilityFlag = 0xFFFFFFFF;//0x10000000;

const String& SFXEditorFrm::GetTypeName() const
{
	static String typeName = "SFXEditorFrm";
	return typeName;
}

IMPLEMENT_DYNCREATE(SFXEditorFrm, CFrameWndEx)

SFXEditorFrm::SFXEditorFrm()
{
	EventsManager::getSingleton().AddListener(this);
}
//--------------------------------------------------------------------------------
SFXEditorFrm::~SFXEditorFrm()
{
	EventsManager::getSingleton().RemoveListener(this);
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::DoDataExchange(CDataExchange* pDX)
{
	CFrameWndEx::DoDataExchange(pDX);	
}
//--------------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable : 4407)

BEGIN_MESSAGE_MAP(SFXEditorFrm, CFrameWndEx)
	ON_WM_SIZE()

	ON_COMMAND(ID_SFX_OPTION, OnShowOptions)

	ON_COMMAND(ID_NEW_SFX, OnNewSFX)

	ON_COMMAND(ID_SAVE_SFX, OnSaveSFX)
	ON_UPDATE_COMMAND_UI(ID_SAVE_SFX, OnUpdateSaveSFX)

	
	ON_COMMAND(ID_SHOW_WIREFRAME, OnShowWireframe)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WIREFRAME, OnUpdateShowWireframe)

	ON_WM_CLOSE()
	//ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	
	ON_COMMAND(ID_SFX_EDITOR_CAPTURE, OnCaptureThumbImage)

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

	ON_COMMAND(ID_SFX_UNDO, OnUndo)
	ON_UPDATE_COMMAND_UI(ID_SFX_UNDO, OnUpdateUndo)

	ON_COMMAND(ID_SFX_REDO, OnRedo)
	ON_UPDATE_COMMAND_UI(ID_SFX_REDO, OnUpdateRedo)
	
END_MESSAGE_MAP()

#pragma warning(pop)
//--------------------------------------------------------------------------------
BOOL SFXEditorFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
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
	if (!_listPanel.Create(_UIDisplayString("ID_SFX_ELEMENT_LIST_PANEL").c_str(),
		this, CRect(0, 0, 200, 230), TRUE, ID_SFX_LIST_PANEL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		// TRACE0("未能创建“SFXElementListPanel”窗口\n");
		return FALSE; // 未能创建
	}
	
	//------------------------------------------------------------------------
	// TimeLine Panel
	//------------------------------------------------------------------------
	//
	if(!_timelinePanel.Create(_UIDisplayString("ID_SFX_TIMELINE_PANEL").c_str(),
		this, CRect(0, 0, 100, 500), TRUE, ID_TIMELINE_PANEL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |  CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		return FALSE;
	}
	_timelinePanel.SetIcon(hPropertiesBarIcon, FALSE);

	//------------------------------------------------------------------------
	// Property Grid Panel
	//------------------------------------------------------------------------
	if (!_propGrid.Create(_UIDisplayString("ID_SFXEDITOR_PROPGRID").c_str(),
		this, CRect(0, 0, 300, 200), TRUE, ID_SFXEDITOR_PROPGRID,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		// TRACE0("未能创建“粒子属性”窗口\n");
		return FALSE; // 未能创建
	}

	_propGrid.SetIcon(hPropertiesBarIcon, FALSE);

	_wndRibbonBar.Create(this);	

	DockPane(&_timelinePanel);
	DockPane(&_listPanel);
	DockPane(&_propGrid);

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
	mainPanel->Add(new CMFCRibbonButton(ID_NEW_SFX,
		_UIDisplayString("ID_NEW_SFX").c_str(), 0, 0));

	// Save
	mainPanel->Add(new CMFCRibbonButton(ID_SAVE_SFX,
		_UIDisplayString("ID_FILE_SAVE").c_str(), 2, 2));

	// Undo
	HICON icon = AfxGetApp()->LoadIcon(ID_SFX_UNDO);
	mainPanel->Add(new CMFCRibbonButton(ID_SFX_UNDO,
		_UIDisplayString("ID_EDIT_UNDO").c_str(), icon));

	// Redo
	icon = AfxGetApp()->LoadIcon(ID_SFX_REDO);
	mainPanel->Add(new CMFCRibbonButton(ID_SFX_REDO,
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
	btn = new CMFCRibbonButton(ID_SFX_EDITOR_CAPTURE,
		_UIDisplayString("ID_SFX_EDITOR_CAPTURETHUMB").c_str());
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
void SFXEditorFrm::Init()
{
	RefreshParticleList();

	//CMFCToolBar::ResetAll();

	ResEditorBase::Init(C_ViewportVisibilityFlag, "__SFX_Editor__");

	_grid->VisibilityFlag(C_ViewportVisibilityFlag);
	_grid->CellSizeOnX(10.0f);
	_grid->CellSizeOnZ(10.0f);


	SetWindowText(_UIDisplayString("IDS_SFX_EDITOR").c_str());


	ResetCamera();
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CFrameWndEx::PostNcDestroy();
	//OGRE_DELETE this;
}
//--------------------------------------------------------------------------------
BOOL SFXEditorFrm::DestroyWindow()
{
	Shutdown();

	SFXEditor::getSingleton().DestroyCurrentSFXInstance();


	BOOL r = CFrameWndEx::DestroyWindow();

	return r;
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnSize(UINT nType, int cx, int cy)
{
	if(_renderWindow)
	{
		_renderWindow->GetRenderWindow()->windowMovedOrResized();
	}

	CFrameWndEx::OnSize(nType, cx, cy);	
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::RefreshParticleList()
{
	//_listPanel.OnRefreshListItem();	
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::BuildPropertyMap()
{
	ResEditorBase::BuildPropertyMap();

	//AddProp(Property_MeshFile, MeshFile, SFXEditorFrm, "MeshFile", "ParticleEditor");
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnShowOptions()
{
	_propGrid.SetObject(this);
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnNewSFX()
{
	EventsManager::getSingleton().CreateEvent(SFXEditorEvents::NewSFX, Any());
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnClose()
{
	//ShowWindow(SW_HIDE);
	CFrameWndEx::OnClose();

	//DestroyWindow();
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnShowWireframe()
{
	SetShowWireframe(!GetShowWireframe());
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnUpdateShowWireframe(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetShowWireframe());
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnCaptureThumbImage()
{
	SFXEditor& editor = SFXEditor::getSingleton();
	bool on = editor.GetShowBounding();
	editor.SetShowBounding(false);
	CaptureThumbImage(EditorUtil::GetParticleThumbFileName(editor.GetCurrentTemplate()));
	editor.SetShowBounding(on);
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnCheckCompositor( UINT id )
{
	Viewport* vp = this->_renderWindow->GetViewport();
	EditorUtil::SelectViewportCompsoitor(vp, _compositorEnableStatus, id);
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnUpdateCheckCompositor( CCmdUI* pCmdUI )
{
	EditorUtil::UpdateCompositorUI(pCmdUI, _compositorEnableStatus);
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnShowBoundingBox()
{
	SFXEditor::getSingleton().SetShowBounding(
		!SFXEditor::getSingleton().GetShowBounding());
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnShowBoundingBox( CCmdUI* pCmdUI )
{
	if(SFXEditor::getSingleton().GetCurrentSFX())
	{
		pCmdUI->SetCheck(SFXEditor::getSingleton().GetShowBounding());
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnUpdateResetCamera( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(TRUE);
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnMaterialScheme()
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
void SFXEditorFrm::OnUpdateMaterialScheme( CCmdUI* pCmdUI )
{
	pCmdUI->Enable((_viewCategory != 0) && _renderWindow
		&& (_renderWindow->GetViewport()));
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnShowGrid()
{
	SetShowGrid(!GetShowGrid());
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnUpdateShowGrid( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(GetShowGrid());
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnSetBkClr()
{
	COLORREF clr = _bkClrButton->GetColor();

	Ogre::ColourValue c;
	c.setAsABGR(clr);

	//this->SetViewportColour(c);
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnShowAxes()
{
	ShowAxes(!IsAxesVisible());
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnUpdateShowAxes( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(IsAxesVisible());
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::ResetCamera()
{
	AxisAlignedBox aabb;
// 	if(SFXEditor::getSingleton().GetCurrentParticles())
// 	{
// 		aabb = SFXEditor::getSingleton().GetCurrentParticles()->getBoundingBox();
// 	}
// 	else
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
void SFXEditorFrm::OnEvents( const String& eventName, const Any& eventContex,
								 bool& stopBroadCast )
{
// 	if(eventName == SFXEditorEvents::ShowParticleObjectProp)
// 	{
// 		IPropertyObject* obj = any_cast<IPropertyObject*>(eventContex);
// 		_propGrid.SetObject(obj);
// 	}
// 	else if((eventName == SFXEditorEvents::ClearObjectProp)
// 		|| (eventName == SFXEditorEvents::RemoveEmitter)
// 		|| (eventName == SFXEditorEvents::RemoveAffector)
// 		|| (eventName == SFXEditorEvents::UpdateRemoveAffectorUI)
// 		|| (eventName == SFXEditorEvents::UpdateRemoveEmitterUI))
// 	{		
// 		_propGrid.SetObject(0);
// 	}
// 	else if(eventName == SFXEditorEvents::UpdateTimeLineEditor)
// 	{
// 		_timelinePanel.RelayoutEditor();
// 	}
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnUndo()
{
	//SFXEditor::getSingleton().Undo();
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnUpdateUndo( CCmdUI* pCmdUI )
{
	//pCmdUI->Enable(SFXEditor::getSingleton().IsUndoEnable());
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnRedo()
{
	//SFXEditor::getSingleton().Redo();
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnUpdateRedo( CCmdUI* pCmdUI )
{
	//pCmdUI->Enable(SFXEditor::getSingleton().IsRedoEnable());
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnSaveSFX()
{
	//SFXEditor::getSingleton().SaveAllParticleTemplate();
}
//--------------------------------------------------------------------------------
void SFXEditorFrm::OnUpdateSaveSFX( CCmdUI* pCmdUI )
{
	//pCmdUI->Enable(SFXEditor::getSingleton().IsUndoEnable());
}
//--------------------------------------------------------------------------------