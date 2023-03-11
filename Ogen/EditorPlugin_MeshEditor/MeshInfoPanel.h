#pragma once
#include "Engine/MeshDataManager.h"

class  MeshInfoTreeCtrl : public CTreeCtrl
{
	DECLARE_MESSAGE_MAP()

public:

	MeshInfoTreeCtrl();
	virtual ~MeshInfoTreeCtrl();

protected:

	afx_msg void OnNMClk(NMHDR *pNMHDR, LRESULT *pResult);
};

struct MeshInfoTreeItemData : public OgeAlloc
{
	enum EDataType
	{
		eDT_None,
		eDT_Bone,
		eDT_AttachPoint,
		eDT_LODUsage,
		eDT_EffectAttachPointPair,
		eDT_Skeleton,
		eDT_SubEntity
	};

	union UnionData
	{
		Bone* bone;
		SAttachPoint* attachPoint;
		MeshLodUsage* lodUsage;
		MeshData::EffectAttachPointPair* effectAttachPointPair;
		Skeleton* skeleton;
		SubEntity* subEntity;
	};

	UnionData _data;
	EDataType _dataType;

	MeshInfoTreeItemData()
		: _dataType(eDT_None)
	{
		memset(&_data, 0, sizeof(UnionData));
	}

	MeshInfoTreeItemData(Bone* val) : _dataType(eDT_Bone) { _data.bone = val; }
	MeshInfoTreeItemData(SAttachPoint* val)	: _dataType(eDT_AttachPoint) { _data.attachPoint = val; }
	MeshInfoTreeItemData(MeshLodUsage* val)	: _dataType(eDT_LODUsage) { _data.lodUsage = val; }
	MeshInfoTreeItemData(MeshData::EffectAttachPointPair* val)	: _dataType(eDT_EffectAttachPointPair) { _data.effectAttachPointPair = val; }
	MeshInfoTreeItemData(Skeleton* val)	: _dataType(eDT_Skeleton) { _data.skeleton = val; }
	MeshInfoTreeItemData(SubEntity* val)	: _dataType(eDT_SubEntity) { _data.subEntity = val; }
};


class MeshInfoPanel : public CDockablePane
{
	DECLARE_MESSAGE_MAP()

public:

	friend MeshInfoTreeCtrl;

	MeshInfoPanel();
	virtual ~MeshInfoPanel();

	CTreeCtrl& GetListCtrl() { return _treeCtrl; }

	void Reset();
	
	void AddAnimation(const String& aniName);
	void AddAnimation(const StringVector aniList);
	void RemoveAnimation(const String& aniName);
	void RemoveAllAnimation();

	void SetMesh(Mesh* mesh, Entity* ent);
	void AddSubMesh(unsigned short idx, SubMesh* subMesh);
	void RemoveSubMesh(const String& name);
	void RemoveSubMesh(unsigned short idx);
	void RemoveAllSubMesh();

	void AddLod(const String& aniName);
	void RemoveLod(const String& aniName);

	virtual BOOL OnBeforeShowPaneMenu(CMenu& m);
	virtual BOOL OnAfterShowPaneMenu(int nMenuResult);

protected:

	MeshInfoTreeCtrl _treeCtrl;
	CImageList _imageList;
	CMenu _popupMenu;

	HTREEITEM _submeshRoot;
	HTREEITEM _animationRoot;
	HTREEITEM _attachPointRoot;
	HTREEITEM _lodRoot;
	HTREEITEM _gemoetryInfoRoot;
	HTREEITEM _boneGroupRoot;
	HTREEITEM _effectsGroupRoot;

	int _totalVertices;
	int _totalIndices;

	typedef std::map<String, IMementoPtr> MementoMap;
	MementoMap _subMeshSelectMap;
	MementoMap _effectsSelectMap;

	Mesh* _mesh;
	Entity* _entity;

	typedef std::list<MeshInfoTreeItemData*> MeshInfoTreeItemDataList;
	MeshInfoTreeItemDataList _meshInfoTreeItemDataList;

	void AddMeshInfoTreeItemData(MeshInfoTreeItemData* dat);
	void DestroyAllMeshInfoTreeItemData();

	void OnItemClick(HTREEITEM item, HTREEITEM parentItem);

	void SelectSubMesh(SubEntity* subMesh);
	void ClearSubMeshSelect();

	void OnSelectEffect(const String& effectGrp);
	void ClearEffectSelect();

	void AddSkeleton(HTREEITEM skeletonItem);
	HTREEITEM RecursiveAddBoneNode(Bone* bone, HTREEITEM parentBoneItem);

	void _RemoveAllChild(HTREEITEM parentItem);
	void _RemoveChild(HTREEITEM parentItem, const String& childTxt);

	void AddGeometryInfo();
	void AddSubMeshInfo();
	void AddSkeletonInfo();
	void AddAttachPointInfo();
	void AddEffectsGroupInfo();
	void AddLODInfo();
	void ExpandAll();

	void OnAddAttachPoint();
	void OnDelAttachPoint();
	void OnAddLOD();
	void OnAddDelLOD();
	void OnAddAnimation();
	void OnDelAnimation();
	void OnAddBoneGroup();
	void OnDelBoneGroup();
	void OnAddFrameEvent();
	void OnDelFrameEvent();
	void OnAddEffects();
	void OnDelEffects();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

