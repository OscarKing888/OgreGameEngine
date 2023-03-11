
// MainFrm.h : MainFrame 类的接口
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"

class MainFrame : public CFrameWndEx
	, public Singleton<MainFrame>
	, public ICommandXExecuteObserver
	, public IEventLisitener
{
	
protected: // 仅从序列化创建
	MainFrame();
	DECLARE_DYNCREATE(MainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	void UpdateCompositorList();

	void InitPluginsToolbar();

	void PostGameInit();

// 实现
public:
	virtual ~MainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCRibbonBar     _wndRibbonBar;
	CMFCRibbonMainPanel* _mainToolbarPanel;
	CMFCRibbonCategory* _mainCategoryPanel;
	CMFCRibbonStatusBarPane* _worldPositionPanel;

	CMFCRibbonApplicationButton _mainButton;
	CMFCToolBarImages _panelImages;
	CMFCRibbonStatusBar  _wndStatusBar;

	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCToolBarImages m_UserImages;
	ClassView        m_wndClassView;
	OutputWnd        m_wndOutput;
	ObjectPropertiesWnd    m_wndProperties;

	ResPanel_Material	_materialManagerPanel;
	ResPanel_Mesh	_meshManagerPanel;
	ResPanel_Texture	_textureManagerPanel;
	ResPanel_ParticleSystem	_particleSystemManagerPanel;

	// compositor
	static UINT CompositorStartID;
	static UINT CompositorEndID;

	CompositorStatusMap _compositorEnableStatus;

	CMFCRibbonCategory* _globalObjectCategory;
	CMFCRibbonPanel* _globalObjectCategoryMainPanel;

	CMFCRibbonCategory* _toolsCategory;
	CMFCRibbonPanel* _toolsCategoryMainPanel;

// 生成的消息映射函数
protected:
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);

	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewport(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopyDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowBoundingBox(CCmdUI* pCmdUI);
	
	afx_msg void OnUpdateEditState(CCmdUI* pCmdUI);
	afx_msg void OnActiveEditState(UINT id);

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnViewportMax();
	afx_msg void OnViewport1x2();
	afx_msg void OnViewport2x2();
	afx_msg void OnUndo();
	afx_msg void OnRedo();
	afx_msg void OnClose();
	afx_msg void OnDestroy();

	afx_msg void OnTimer(UINT_PTR timerID);
	afx_msg void OnShowResManager();
	afx_msg void OnShowMaterialManager();
	afx_msg void OnShowTextureManager();
	afx_msg void OnShowObjects();
	afx_msg void OnShowBoundingBox();

	afx_msg void OnUpdateToggleClampOnTerrain(CCmdUI* pCmdUI);
	afx_msg void OnToggleClampOnTerrain();

	afx_msg void OnUpdateShowWireFrame(CCmdUI* pCmdUI);
	afx_msg void OnToggleShowWireFrame();

	afx_msg void OnEditTerrain();

	afx_msg void OnUpdateAlignTerrainNormal(CCmdUI* pCmdUI);
	afx_msg void OnToggleAlignTerrainNormal();

	afx_msg void OnUpdateApplyCamera(CCmdUI* pCmdUI);
	afx_msg void OnApplyCamera();

	afx_msg void OnUpdateViewSelectedCamera(CCmdUI* pCmdUI);
	afx_msg void OnViewSelectedCamera();

	afx_msg void OnEditCurrentCamera();
	
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg void OnMaterialScheme();
	afx_msg void OnCompositorSelect();
	afx_msg void OnPickFilter();

	afx_msg void OnSnapshotMiniMap();
	afx_msg void OnSnapshotConfig();
	afx_msg void OnSnapshot();

	afx_msg void OnShowParticleEditor();
	afx_msg void OnShowModelEditor();
	afx_msg void OnShowModelMaterialEditor();
	
	afx_msg void OnShowPickFilter();

	afx_msg void OnCameraSpeed(UINT id);
	afx_msg void OnUpdateCameraSpeed(CCmdUI* pCmdUI);

	afx_msg void OnShowProp_Scene();
	afx_msg void OnUpdateUI_ShowProp_Scene(CCmdUI* ui);

	afx_msg void OnShowProp_SkyBox();
	afx_msg void OnUpdateUI_ShowProp_SkyBox(CCmdUI* ui);

	afx_msg void OnShowProp_MainLight();
	afx_msg void OnUpdateUI_ShowProp_MainLight(CCmdUI* ui);

	afx_msg void OnShowProp_EditorConfig();

	afx_msg void OnCheck_Move_ClampTerrain();
	afx_msg void OnUpdateCheck_Move_ClampTerrain(CCmdUI* ui);

	afx_msg void OnCheck_Move_AlignNormal();
	afx_msg void OnUpdateCheck_Move_AlignNormal(CCmdUI* ui);

	afx_msg void OnCheck_Move_SnapToGird();
	afx_msg void OnUpdateCheck_Move_SnapToGird(CCmdUI* ui);

	afx_msg void OnCheck_MoveCamera_ClampTerrain();
	afx_msg void OnUpdateCheck_MoveCamera_ClampTerrain(CCmdUI* ui);

	
	afx_msg void OnCheckCompositor(UINT id);
	afx_msg void OnUpdateCheckCompositor(CCmdUI* pCmdUI);


	//afx_msg void On
	DECLARE_MESSAGE_MAP()

	void InitializeRibbon();

	void InitMenu();
	void InitToolbar();
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	virtual void OnCommandExecuted();

	virtual void PostNcDestroy();
	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast);
};


