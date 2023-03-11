#pragma once
#include "MeshInfoPanel.h"
#include "Engine/MeshDataManager.h"
#include "TimeLinePanel.h"

class MeshEditorFrm;

class ModelEditor_ModelList : public ResPanel_MeshBase
{
public:

	ModelEditor_ModelList(MeshEditorFrm* parent);

	virtual ~ModelEditor_ModelList();

	virtual void OnDBClickListItem(int item, const CString& resName);

	virtual BOOL Create(LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect,
		BOOL bHasGripper, UINT nID, DWORD dwStyle, DWORD dwTabbedStyle = AFX_CBRS_REGULAR_TABS,
		DWORD dwControlBarStyle = AFX_DEFAULT_DOCKING_PANE_STYLE, CCreateContext* pContext = NULL);


protected:

	MeshEditorFrm* _parent;
};

//----------------------------------------------------------------------------------------------

class MeshEditorFrm
	: public CFrameWndEx
	, public SingletonPhoenix<MeshEditorFrm>
	, public ResEditorBase
	, public IUpdateAble
	, public IMFCMouseListener
{
	DECLARE_DYNCREATE(MeshEditorFrm)

public:

	virtual ~MeshEditorFrm();

	void Init();

	virtual BOOL DestroyWindow();

	void EditMeshFile(const String& meshFile);
	const String& GetMeshFile() const;

	void CreateParticleSystemInstance(const String& templateName);
	void DestroyCurrentParticleSystemInstance();

	void PlayAnimation();
	void PlayAnimation(const String& aniName);
	void PauseAnimation();
	void StopPlayAnimation();
	void SetPlayingAnimationTimePos(float t);

	const String& GetPlayingAnimation() const { return _playingAnim; }

	void AddLod(float distance, const String& meshFile);

	void SetShowSkeleton(bool on);
	bool GetShowSkeleton() const;
	void SetShowAttachPoint(bool on);

	MeshData* GetMeshData();
	AnimationModel* GetActor() const { return _aniModel; }

	bool HasAnimationPalying() const { return !_playingAnim.empty(); }

	String GetMeshKeyName();

	Bone* GetSelectedBone() { return _selectedBone; }
	String GetSelectedBoneName() const;

	void OnSelectBone(Bone* bone);
	void OnSelectAttachPoint(SAttachPoint* atp);
	void OnSelectEffectGroup(const String& effGrpName);

	afx_msg void OnAddAttachPoint();
	afx_msg void OnDelAttachPoint();
	afx_msg void OnAddLOD();
	afx_msg void OnAddDelLOD();
	afx_msg void OnAddAnimation();
	afx_msg void OnDelAnimation();
	afx_msg void OnAddBoneGroup();
	afx_msg void OnDelBoneGroup();
	afx_msg void OnAddFrameEvent();
	afx_msg void OnDelFrameEvent();
	afx_msg void OnAddEffects();
	afx_msg void OnDelEffects();
	
	afx_msg void OnAddLinkedSkeleton();
	afx_msg void OnRemoveLinkedSkeleton();
	afx_msg void OnCopyPlayingAnimationNameToClipboard();

	void SetAs4Legs(bool on);
	bool Is4Legs();

	afx_msg void SetAsLeftFoot();
	afx_msg void SetAsLeftBackFoot();
	afx_msg void SetAsRightFoot();
	afx_msg void SetAsRightBackFoot();
	afx_msg void CalcMoveDist();
	afx_msg void OnToggle4Legs();


protected:

	MeshEditorFrm();   // standard constructor
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//PropertyGrid	_propGrid;
	String	_currentTemplate;
	ModelEditor_ModelList	_listPanel;
	MeshInfoPanel	_infoPanel;
	
	//CMFCToolBar     _wndToolBar;
	
	CMFCRibbonBar	_wndRibbonBar;
	TimeLinePanel	_timelinePanel;

	String			_meshFile;

	AnimationModel*			 _aniModel;
	String			_playingAnim;
	bool			_isPlay;
	Entity*			_boneAxes;
	SceneNode*		_boneNode;
	Bone*			_selectedBone;
	SAttachPoint*	_selectedAttachPoint;
	String			_selectedEffectGroupName;
	bool			_showSkeleton;


	typedef Ogre::map<Ogre::TagPoint*, Ogre::Node*>::type	TagChildBoneMapType;
	TagChildBoneMapType		_boneVisuals;
	void			_UpdateBoneVisualizer();
	void			AddBoneVisualizer();


	typedef Ogre::list<Ogre::Entity*>::type					EntityListType;
	EntityListType			_boneEntitys;


	static UINT CompositorStartID;
	static UINT CompositorEndID;

	CompositorStatusMap _compositorEnableStatus;

	CMFCRibbonCategory* _viewCategory;

	CMFCRibbonColorButton* _bkClrButton;


	Mesh*	GetMesh();

	void Reset();
	void CreateMesh();
	void DestroyMesh();

	void UpdateMeshInfoList();
	virtual void Update(float deltaTime);

	virtual CWnd* _GetActiveView() { return GetActiveView(); }

	virtual void PostNcDestroy();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL OnMButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnMButtonUp(UINT nFlags, CPoint point);

	virtual BOOL OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	
	afx_msg void ResetCamera();
	afx_msg void OnUpdateResetCamera(CCmdUI* pCmdUI);

	afx_msg void OnShowWireframe();
	afx_msg void OnUpdateShowWireframe(CCmdUI* pCmdUI);

	afx_msg void OnShowSkeleton();
	afx_msg void OnUpdateShowSkeleton(CCmdUI* pCmdUI);


	afx_msg void OnSaveMesh();
	afx_msg void OnSaveMeshAs();
	afx_msg void OnDeleteMesh();
	afx_msg void OnBuildEdigeList();
	afx_msg void OnCompositorSelect();
	afx_msg void OnCaptureThumbImage();

	afx_msg void OnUpdate4Legs(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCalcMoveDistance(CCmdUI *pCmdUI);

	afx_msg void OnUpdateAddLinkedSkeleton(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRemoveLinkedSkeleton(CCmdUI *pCmdUI);

	afx_msg void OnUpdateSetAsLeftFoot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetAsLeftBackFoot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetAsRightFoot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetAsRightBackFoot(CCmdUI* pCmdUI);

	afx_msg void OnUpdateSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteMesh(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBuildEdigeList(CCmdUI* pCmdUI);	
	afx_msg void OnUpdateCaptureThumbImage(CCmdUI* pCmdUI);

	afx_msg void OnUpdateAddEffects(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteEffects(CCmdUI* pCmdUI);

	afx_msg void OnUpdateAddBoneGroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteBoneGroup(CCmdUI* pCmdUI);

	afx_msg void OnUpdateAddAttachPoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteAttachPoint(CCmdUI* pCmdUI);

	afx_msg void OnUpdateAddFrameEvent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteFrameEvent(CCmdUI* pCmdUI);

	afx_msg void OnUpdateCopyToClipboard(CCmdUI* pCmdUI);

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
};