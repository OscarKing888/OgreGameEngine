#pragma once

///
/// ComponentDlg
///
class ComponentDlg : public CDialogEx
{
	// Construction
	DECLARE_MESSAGE_MAP()
public:
	ComponentDlg(CWnd* pParent = NULL);	// standard constructor

	void ClearAllAffectors();
	void AddAffector(const CString& str);

	void ClearAllEmitters();
	void AddEmitter(const CString& str);

	bool EnableEditParSysBtn() const { return _enableEditParSysBtn; }
	void EnableEditParSysBtn(bool val);

	bool EnableEditParSysRendererBtn() const { return _enableEditParSysRendererBtn; }
	void EnableEditParSysRendererBtn(bool val);

	bool EnableAddEmitter() const { return _enableAddEmitter; }
	void EnableAddEmitter(bool val);

	bool EnableRemoveEmitter() const { return _enableRemoveEmitter; }
	void EnableRemoveEmitter(bool val);

	bool EnableAddAffector() const { return _enableAddAffector; }
	void EnableAddAffector(bool val);

	bool EnableRemoveAffector() const { return _enableRemoveAffector; }
	void EnableRemoveAffector(bool val);

	void SelectRenderer(const String& rendererName);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Implementation
protected:
	HICON m_hIcon;
	CListBox _affectorLst;
	CListBox _emitterLst;
	CListBox _rendererLst;
	StringVector _rendererNameLst;

	bool _enableEditParSysBtn;
	bool _enableEditParSysRendererBtn;

	bool _enableAddEmitter;
	bool _enableRemoveEmitter;

	bool _enableAddAffector;
	bool _enableRemoveAffector;

	CButton _btEditParSys;
	CButton _btEditParSysRenderer;
	CButton _btAddEmitter;
	CButton _btRemoveEmitter;
	CButton _btAddAffector;
	CButton _btRemoveAffector;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectEmitter();
	afx_msg void OnSelectAffector();
	afx_msg void OnChangeRenderer();

	afx_msg void OnEditParticleSysProp();
	afx_msg void OnUpdateEditParticleSysProp(CCmdUI* ui);

	afx_msg void OnEditParticleSysRendererProp();
	afx_msg void OnUpdateEditParticleSysRendererProp(CCmdUI* ui);

	afx_msg void OnAddEmitter();
	afx_msg void OnUpdateAddEmitter(CCmdUI* ui);

	afx_msg void OnRemoveEmitter();
	afx_msg void OnUpdateRemoveEmitter(CCmdUI* ui);

	afx_msg void OnAddAffector();
	afx_msg void OnUpdateAddAffector(CCmdUI* ui);

	afx_msg void OnRemoveAffector();
	afx_msg void OnUpdateRemoveAffector(CCmdUI* ui);
};

///
/// 
///
class ComponentPanel
	: public CDockablePane
	, public Singleton<ComponentPanel>
	, public IEventLisitener
{
public:

	ComponentPanel();
	virtual ~ComponentPanel();

	DECLARE_MESSAGE_MAP()

	void Relayout();
	

protected:

	ComponentDlg _dlg;

	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast);

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	void UpdateEmitterList( ParticleSystem* ps );
	void UpdateAffectorList( ParticleSystem* ps );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};