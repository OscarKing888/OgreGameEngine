#pragma once

class MaterialEditorFrm
	: public CFrameWndEx
	, public SingletonPhoenix<MaterialEditorFrm>
	, public ResEditorBase
	, public IEventLisitener
{
	DECLARE_DYNCREATE(MaterialEditorFrm)

public:

	virtual ~MaterialEditorFrm();

	void Init();

	virtual BOOL DestroyWindow();

	virtual const String& GetTypeName() const;

protected:

	MaterialEditorFrm();   // standard constructor
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	PropertyGrid	_propGrid;
	
	//CMFCToolBar     _wndToolBar;
	CMFCRibbonBar	_wndRibbonBar;

	static UINT CompositorStartID;
	static UINT CompositorEndID;

	CompositorStatusMap _compositorEnableStatus;


	CMFCRibbonCategory* _viewCategory;
	CMFCRibbonColorButton* _bkClrButton;

	virtual void OnEvents( const String& eventName,
		const Any& eventContex, bool& stopBroadCast );

	void RefreshParticleList();
	virtual void BuildPropertyMap();

	virtual CWnd* _GetActiveView() { return GetActiveView(); }

	virtual void PostNcDestroy();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnShowOptions();

	afx_msg void OnNewSFX();

	afx_msg void OnSaveSFX();
	afx_msg void OnUpdateSaveSFX(CCmdUI* pCmdUI);

	afx_msg void OnShowWireframe();
	afx_msg void OnUpdateShowWireframe(CCmdUI* pCmdUI);

	afx_msg void OnClose();

	afx_msg void OnCaptureThumbImage();

	afx_msg void OnCheckCompositor(UINT id);
	afx_msg void OnUpdateCheckCompositor(CCmdUI* pCmdUI);

	afx_msg void OnShowAxes();
	afx_msg void OnUpdateShowAxes(CCmdUI* pCmdUI);

	afx_msg void OnShowBoundingBox();
	afx_msg void OnShowBoundingBox(CCmdUI* pCmdUI);

	afx_msg void OnMaterialScheme();
	afx_msg void OnUpdateMaterialScheme(CCmdUI* pCmdUI);

	afx_msg void OnShowGrid();
	afx_msg void OnUpdateShowGrid(CCmdUI* pCmdUI);

	afx_msg void OnSetBkClr();

	afx_msg void ResetCamera();
	afx_msg void OnUpdateResetCamera(CCmdUI* pCmdUI);

	afx_msg void OnUndo();
	afx_msg void OnUpdateUndo(CCmdUI* pCmdUI);

	afx_msg void OnRedo();
	afx_msg void OnUpdateRedo(CCmdUI* pCmdUI);
};