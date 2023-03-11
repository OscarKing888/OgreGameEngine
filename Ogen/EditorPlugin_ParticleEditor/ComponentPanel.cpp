#include "stdafx.h"
#include "ComponentPanel.h"
#include "Resource.h"
#include "ParticleEditorEvents.h"

template<> ComponentPanel* Singleton<ComponentPanel>::ms_Singleton = 0;

//--------------------------------------------------------------------------------
// ComponentDlg
//--------------------------------------------------------------------------------
ComponentDlg::ComponentDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(IDD_DIALOG_PARTICLE_COMPONENT, pParent)
, _enableEditParSysBtn(false)
, _enableEditParSysRendererBtn(false)
, _enableAddEmitter(false)
, _enableRemoveEmitter(false)
, _enableAddAffector(false)
, _enableRemoveAffector(false)
{
}

void ComponentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EMITTER, _emitterLst);	
	DDX_Control(pDX, IDC_LIST_AFFECTOR, _affectorLst);
	DDX_Control(pDX, IDC_LIST_PARTICLE_SYS_RENDERER, _rendererLst);
	

	DDX_Control(pDX, IDC_BUTTON_EDIT_PARTICLESYS_PROP, _btEditParSys);
	DDX_Control(pDX, IDC_BUTTON_EDIT_PARTICLERENDERER_PROP, _btEditParSysRenderer);

	DDX_Control(pDX, IDC_BUTTON_ADD_EMITTER, _btAddEmitter);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_EMITTER, _btRemoveEmitter);

	DDX_Control(pDX, IDC_BUTTON_ADD_AFFECTOR, _btAddAffector);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_AFFECTOR, _btRemoveAffector);	
}

BEGIN_MESSAGE_MAP(ComponentDlg, CDialogEx)

	//ON_LBN_DBLCLK(IDC_LIST_EMITTER, OnSelectEmitter)
	ON_LBN_SELCHANGE(IDC_LIST_EMITTER, OnSelectEmitter)
	
	//ON_LBN_DBLCLK(IDC_LIST_AFFECTOR, OnSelectAffector)
	ON_LBN_SELCHANGE(IDC_LIST_AFFECTOR, OnSelectAffector)

	ON_LBN_SELCHANGE(IDC_LIST_PARTICLE_SYS_RENDERER, OnChangeRenderer)	
	
	ON_COMMAND(IDC_BUTTON_EDIT_PARTICLESYS_PROP, OnEditParticleSysProp)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_EDIT_PARTICLESYS_PROP, OnUpdateEditParticleSysProp)

	ON_COMMAND(IDC_BUTTON_EDIT_PARTICLERENDERER_PROP, OnEditParticleSysRendererProp)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_EDIT_PARTICLERENDERER_PROP, OnUpdateEditParticleSysRendererProp)

	ON_COMMAND(IDC_BUTTON_ADD_EMITTER, OnAddEmitter)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ADD_EMITTER, OnUpdateAddEmitter)

	ON_COMMAND(IDC_BUTTON_REMOVE_EMITTER, OnRemoveEmitter)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_REMOVE_EMITTER, OnUpdateRemoveEmitter)

	ON_COMMAND(IDC_BUTTON_ADD_AFFECTOR, OnAddAffector)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ADD_AFFECTOR, OnUpdateAddAffector)

	ON_COMMAND(IDC_BUTTON_REMOVE_AFFECTOR, OnRemoveAffector)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_REMOVE_AFFECTOR, OnUpdateRemoveAffector)

END_MESSAGE_MAP()


// ComponentDlg message handlers

BOOL ComponentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_btEditParSys.SetWindowText(_UIDisplayString("IDC_BUTTON_EDIT_PARTICLESYS_PROP").c_str());
	_btEditParSysRenderer.SetWindowText(_UIDisplayString("IDC_BUTTON_EDIT_PARTICLERENDERER_PROP").c_str());

	_btAddEmitter.SetWindowText(_UIDisplayString("IDC_BUTTON_ADD_EMITTER").c_str());
	_btRemoveEmitter.SetWindowText(_UIDisplayString("IDC_BUTTON_REMOVE_EMITTER").c_str());

	_btAddAffector.SetWindowText(_UIDisplayString("IDC_BUTTON_ADD_AFFECTOR").c_str());
	_btRemoveAffector.SetWindowText(_UIDisplayString("IDC_BUTTON_REMOVE_AFFECTOR").c_str());


	_btEditParSys.EnableWindow(FALSE);
	_btEditParSysRenderer.EnableWindow(FALSE);

	_btAddEmitter.EnableWindow(FALSE);
	_btRemoveEmitter.EnableWindow(FALSE);

	_btAddAffector.EnableWindow(FALSE);
	_btRemoveAffector.EnableWindow(FALSE);

	_rendererLst.EnableWindow(FALSE);

	ParticleSystemManager::ParticleRendererFactoryIterator it =
		ParticleSystemManager::getSingleton().getRendererFactoryIterator();
	while(it.hasMoreElements())
	{
		_rendererNameLst.push_back(it.peekNextKey());
		_rendererLst.AddString(it.peekNextKey().c_str());
		it.moveNext();
	}

	// TODO: Add extra initialization here
	//VERIFY(_emitterLst.SubclassDlgItem(IDC_LIST_EMITTER, this));
	//VERIFY(_affectorLst.SubclassDlgItem(IDC_LIST_AFFECTOR, this));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void ComponentDlg::OnSelectAffector()
{
	int s = _affectorLst.GetCurSel();
	if(s == -1)
	{
		return;
	}


	// 通知编辑选中的Affector，按其在particle中的索引
	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::SelectAffector,
		Any(s));
}

void ComponentDlg::ClearAllAffectors()
{
	_affectorLst.ResetContent();
}

void ComponentDlg::AddAffector( const CString& str )
{
	_affectorLst.AddString(str);
}

void ComponentDlg::OnSelectEmitter()
{
	int s = _emitterLst.GetCurSel();
	if(s == -1)
	{
		return;
	}

	// 通知编辑选中的Emitter，按其在particle中的索引
	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::SelectEmitter,
		Any(s));
}

void ComponentDlg::ClearAllEmitters()
{
	_emitterLst.ResetContent();
}

void ComponentDlg::AddEmitter( const CString& str )
{
	_emitterLst.AddString(str);
}

void ComponentDlg::OnEditParticleSysProp()
{
	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::SelectParticleSysProp, Any());
}

void ComponentDlg::OnUpdateEditParticleSysProp( CCmdUI* ui )
{
	ui->Enable(_enableEditParSysBtn);
}

void ComponentDlg::OnAddEmitter()
{
	StringVector lst;

	ParticleSystemManager::ParticleEmitterFactoryIterator it =
		ParticleSystemManager::getSingleton().getEmitterFactoryIterator();
	while (it.hasMoreElements())
	{
		lst.push_back(it.peekNextKey());
		it.moveNext();
	}

	ListSelectDlg dlg(_UIDisplayString("IDC_BUTTON_ADD_EMITTER").c_str(), lst, this);
	if(dlg.DoModal() == IDOK)
	{
		EventsManager::getSingleton().CreateEvent(
			ParticleEditorEvents::AddEmitter,
			Any(dlg.GetSelectedItem()));
	}
}

void ComponentDlg::OnUpdateAddEmitter( CCmdUI* ui )
{
	ui->Enable(_enableEditParSysBtn);
}

void ComponentDlg::OnRemoveEmitter()
{
	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::RemoveEmitter,
		Any(_emitterLst.GetCurSel()));
}

void ComponentDlg::OnUpdateRemoveEmitter( CCmdUI* ui )
{
	ui->Enable(_enableEditParSysBtn && (_emitterLst.GetCurSel() >= 0));
}

void ComponentDlg::OnAddAffector()
{
	StringVector lst;

	ParticleSystemManager::ParticleAffectorFactoryIterator it =
		ParticleSystemManager::getSingleton().getAffectorFactoryIterator();

	while (it.hasMoreElements())
	{
		lst.push_back(it.peekNextKey());
		it.moveNext();
	}

	ListSelectDlg dlg(_UIDisplayString("IDC_BUTTON_ADD_AFFECTOR").c_str(), lst, this);
	if(dlg.DoModal() == IDOK)
	{
		EventsManager::getSingleton().CreateEvent(
			ParticleEditorEvents::AddAffector,
			Any(dlg.GetSelectedItem()));
	}
}

void ComponentDlg::OnUpdateAddAffector( CCmdUI* ui )
{
	ui->Enable(_enableEditParSysBtn);
}

void ComponentDlg::OnRemoveAffector()
{
	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::RemoveAffector,
		Any(_affectorLst.GetCurSel()));
}

void ComponentDlg::OnUpdateRemoveAffector( CCmdUI* ui )
{
	ui->Enable(_enableEditParSysBtn && (_affectorLst.GetCurSel() >= 0));
}

void ComponentDlg::OnEditParticleSysRendererProp()
{
	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::SelectParticleSysRendererProp, Any());
}

void ComponentDlg::OnUpdateEditParticleSysRendererProp( CCmdUI* ui )
{
	ui->Enable(_enableEditParSysRendererBtn);
}

void ComponentDlg::EnableEditParSysBtn( bool val )
{
	_enableEditParSysBtn = val;
	if(_btEditParSys.GetSafeHwnd())
	{
		_btEditParSys.EnableWindow(val);
	}
}

void ComponentDlg::EnableEditParSysRendererBtn( bool val )
{
	_enableEditParSysRendererBtn = val;

	if(_btEditParSysRenderer.GetSafeHwnd())
	{
		_btEditParSysRenderer.EnableWindow(val);
	}

	if(_rendererLst.GetSafeHwnd())
	{
		_rendererLst.EnableWindow(val);
	}
}

void ComponentDlg::EnableAddEmitter( bool val )
{
	_enableAddEmitter = val;
	if(_btAddEmitter.GetSafeHwnd())
	{
		_btAddEmitter.EnableWindow(val);
	}
}

void ComponentDlg::EnableRemoveEmitter( bool val )
{
	_enableRemoveEmitter = val;
	if(_btRemoveEmitter.GetSafeHwnd())
	{
		_btRemoveEmitter.EnableWindow(val);
	}
}

void ComponentDlg::EnableAddAffector( bool val )
{
	_enableAddAffector = val;
	if(_btAddAffector.GetSafeHwnd())
	{
		_btAddAffector.EnableWindow(val);
	}
}

void ComponentDlg::EnableRemoveAffector( bool val )
{
	_enableRemoveAffector = val;
	if(_btRemoveAffector.GetSafeHwnd())
	{
		_btRemoveAffector.EnableWindow(val);
	}
}

void ComponentDlg::OnChangeRenderer()
{
	int s = _rendererLst.GetCurSel();
	if(s == -1)
	{
		return;
	}
	
	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::ChangeParticleSysRenderer,
		Any(_rendererNameLst[s]));
}

void ComponentDlg::SelectRenderer( const String& rendererName )
{
	_rendererLst.SelectString(-1, rendererName.c_str());
}
//--------------------------------------------------------------------------------
// ComponentPanel
//--------------------------------------------------------------------------------
ComponentPanel::ComponentPanel()
{
	EventsManager::getSingleton().AddListener(this);
}
//--------------------------------------------------------------------------------
ComponentPanel::~ComponentPanel()
{
	EventsManager::getSingleton().RemoveListener(this);
}
//--------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(ComponentPanel, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
int ComponentPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(_dlg.Create(IDD_DIALOG_PARTICLE_COMPONENT, this));

	Relayout();

	return 0;
}
//--------------------------------------------------------------------------------
void ComponentPanel::OnSize( UINT nType, int cx, int cy )
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	Relayout();

}
//--------------------------------------------------------------------------------
void ComponentPanel::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}
//--------------------------------------------------------------------------------
void ComponentPanel::Relayout()
{
	CRect rcClient;
	GetClientRect(rcClient);

	if(_dlg.GetSafeHwnd())
	{
		_dlg.MoveWindow(&rcClient);
	}
}
//--------------------------------------------------------------------------------
void ComponentPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}
//--------------------------------------------------------------------------------
BOOL ComponentPanel::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}
//--------------------------------------------------------------------------------
void ComponentPanel::OnEvents( const String& eventName,
							  const Any& eventContex, bool& stopBroadCast )
{
	if(eventName == ParticleEditorEvents::EditParticleSystemInstance)
	{
		ParticleSystem* ps = any_cast<ParticleSystem*>(eventContex);
		UpdateAffectorList(ps);
		UpdateEmitterList(ps);

		_dlg.EnableEditParSysBtn(true);
		_dlg.EnableEditParSysRendererBtn(true);
		_dlg.EnableAddAffector(true);
		_dlg.EnableAddEmitter(true);
		_dlg.SelectRenderer(ps->getRendererName());
	}
	else if(eventName == ParticleEditorEvents::DestroyParticleSysInstance)
	{

		_dlg.EnableAddAffector(false);
		_dlg.EnableAddEmitter(false);
		_dlg.EnableRemoveAffector(false);
		_dlg.EnableRemoveEmitter(false);
		_dlg.EnableEditParSysRendererBtn(false);
		_dlg.EnableEditParSysBtn(false);
		_dlg.ClearAllAffectors();
		_dlg.ClearAllEmitters();
	}
	else if(eventName == ParticleEditorEvents::UpdateEmitterList)
	{
		ParticleSystem* ps = any_cast<ParticleSystem*>(eventContex);
		UpdateEmitterList(ps);
	}
	else if(eventName == ParticleEditorEvents::UpdateAffectorList)
	{
		ParticleSystem* ps = any_cast<ParticleSystem*>(eventContex);
		UpdateAffectorList(ps);
	}
	else if(eventName == ParticleEditorEvents::SelectAffector)
	{
		_dlg.EnableRemoveAffector(true);
	}
	else if(eventName == ParticleEditorEvents::SelectEmitter)
	{
		_dlg.EnableRemoveEmitter(true);
	}
	else if(eventName == ParticleEditorEvents::UpdateRemoveAffectorUI)
	{
		_dlg.EnableRemoveAffector(false);
	}
	else if(eventName == ParticleEditorEvents::UpdateRemoveEmitterUI)
	{
		_dlg.EnableRemoveEmitter(false);
	}
}

void ComponentPanel::UpdateAffectorList( ParticleSystem* ps )
{
	_dlg.ClearAllAffectors();

	for(ushort i = 0; i < ps->getNumAffectors(); ++i)
	{
		ParticleAffector* aff = ps->getAffector(i);
		_dlg.AddAffector(aff->getType().c_str());
	}
}

void ComponentPanel::UpdateEmitterList( ParticleSystem* ps )
{
	_dlg.ClearAllEmitters();

	for(ushort i = 0; i < ps->getNumEmitters(); ++i)
	{
		ParticleEmitter* emt = ps->getEmitter(i);
		_dlg.AddEmitter(emt->getType().c_str());
	}
}

BOOL ComponentPanel::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	if(FALSE == CDockablePane::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	{
		return _dlg.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}

	return TRUE;
}