#include "stdafx.h"
#include "MeshInfoPanel.h"
#include "MeshEditorFrm.h"
#include "EditorCore/InputDlg.h"
#include "Resource.h"

//----------------------------------------------------------------------------------------------

#define ICON_GEO_INFO 1
#define ICON_ANIMATION 2
#define ICON_ATTACHPOINT 3
#define ICON_SUBMESH 4
#define ICON_LOD 5
#define ICON_SKELETON 6

//----------------------------------------------------------------------------------------------

class SubMeshMaterialMemento : public IMemento
{
public:

	SubMeshMaterialMemento(SubEntity* subMesh)
		: _subMesh(subMesh)
	{
		_oldMaterial = subMesh->getMaterialName();

		Set();
	}

	virtual ~SubMeshMaterialMemento()
	{
		Restore();
	}

	virtual void Set()
	{
		_subMesh->setMaterialName("__SelectSubMesh__");
	}

	virtual void Restore()
	{
		_subMesh->setMaterialName(_oldMaterial);
	}

protected:

	String _oldMaterial;
	SubEntity* _subMesh;	
};

//---------------------------------------------------------------------------------------------------------------------

class ParticleSystemMaterialMemento : public IMemento
{
public:

	ParticleSystemMaterialMemento(ParticleSystem* particleSys)
		: _particleSys(particleSys)
	{
		_oldMaterial = particleSys->getMaterialName();

		Set();
	}

	virtual ~ParticleSystemMaterialMemento()
	{
		Restore();
	}

	virtual void Set()
	{
		_particleSys->setMaterialName("__SelectSubMesh__");
	}

	virtual void Restore()
	{
		_particleSys->setMaterialName(_oldMaterial);
	}

protected:

	String _oldMaterial;
	ParticleSystem* _particleSys;	
};

//---------------------------------------------------------------------------------------------------------------------

class RenderOperationConverter : public AbstractEnumDescriptor
{
public:

	RenderOperationConverter::RenderOperationConverter()
	{
		M_AddEnumToDescriptorEx(RenderOperation::OT_POINT_LIST, OT_POINT_LIST);
		M_AddEnumToDescriptorEx(RenderOperation::OT_LINE_LIST, OT_LINE_LIST);
		M_AddEnumToDescriptorEx(RenderOperation::OT_LINE_STRIP, OT_LINE_STRIP);
		M_AddEnumToDescriptorEx(RenderOperation::OT_TRIANGLE_LIST, OT_TRIANGLE_LIST);
		M_AddEnumToDescriptorEx(RenderOperation::OT_TRIANGLE_STRIP, OT_TRIANGLE_STRIP);
		M_AddEnumToDescriptorEx(RenderOperation::OT_TRIANGLE_FAN, OT_TRIANGLE_FAN);

		AbstractEnumDescriptor::Init();
	}
};

static RenderOperationConverter g_renderOperationConverter;

//----------------------------------------------------------------------------------------------

class VertexElementSemanticConverter : public AbstractEnumDescriptor
{
public:

	VertexElementSemanticConverter::VertexElementSemanticConverter()
	{
		M_AddEnumToDescriptorEx(VES_POSITION, POSITION);
		M_AddEnumToDescriptorEx(VES_BLEND_WEIGHTS, BLEND_WEIGHTS);
		M_AddEnumToDescriptorEx(VES_BLEND_INDICES, BLEND_INDICES);
		M_AddEnumToDescriptorEx(VES_NORMAL, NORMAL);
		M_AddEnumToDescriptorEx(VES_DIFFUSE, DIFFUSE);
		M_AddEnumToDescriptorEx(VES_SPECULAR, SPECULAR);
		M_AddEnumToDescriptorEx(VES_TEXTURE_COORDINATES, TEXTURE_COORDINATES);
		M_AddEnumToDescriptorEx(VES_BINORMAL, BINORMAL);
		M_AddEnumToDescriptorEx(VES_TANGENT, TANGENT);

		AbstractEnumDescriptor::Init();
	}
};

static VertexElementSemanticConverter g_vertexElementSemanticConverter;

//----------------------------------------------------------------------------------------------

class VertexElementTypeConverter : public AbstractEnumDescriptor
{
public:

	VertexElementTypeConverter::VertexElementTypeConverter()
	{
		M_AddEnumToDescriptorEx(VET_FLOAT1, FLOAT1);
		M_AddEnumToDescriptorEx(VET_FLOAT2, FLOAT2);
		M_AddEnumToDescriptorEx(VET_FLOAT3, FLOAT3);
		M_AddEnumToDescriptorEx(VET_FLOAT4, FLOAT4);
		M_AddEnumToDescriptorEx(VET_COLOUR, COLOUR);
		M_AddEnumToDescriptorEx(VET_SHORT1, SHORT1);
		M_AddEnumToDescriptorEx(VET_SHORT2, SHORT2);
		M_AddEnumToDescriptorEx(VET_SHORT3, SHORT3);
		M_AddEnumToDescriptorEx(VET_SHORT4, SHORT4);
		M_AddEnumToDescriptorEx(VET_UBYTE4, UBYTE4);
		M_AddEnumToDescriptorEx(VET_COLOUR_ARGB, ARGB);
		M_AddEnumToDescriptorEx(VET_COLOUR_ABGR, ABGR);

		AbstractEnumDescriptor::Init();
	}
};

static VertexElementTypeConverter g_vertexElementTypeConverter;

//----------------------------------------------------------------------------------------------

class VertexAnimationTypeConverter : public AbstractEnumDescriptor
{
public:

	VertexAnimationTypeConverter::VertexAnimationTypeConverter()
	{
		M_AddEnumToDescriptorEx(VAT_NONE, None);
		M_AddEnumToDescriptorEx(VAT_MORPH, Morph);
		M_AddEnumToDescriptorEx(VAT_POSE, Pose);
		AbstractEnumDescriptor::Init();
	}
};

static VertexAnimationTypeConverter g_vertexAnimationTypeConverterConverter;

//----------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(MeshInfoTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &MeshInfoTreeCtrl::OnNMClk)
END_MESSAGE_MAP()
//----------------------------------------------------------------------------------------------
MeshInfoTreeCtrl::MeshInfoTreeCtrl()
{

}
//----------------------------------------------------------------------------------------------
MeshInfoTreeCtrl::~MeshInfoTreeCtrl()
{

}
//----------------------------------------------------------------------------------------------
void MeshInfoTreeCtrl::OnNMClk(NMHDR *pNMHDR, LRESULT *pResult)
{	
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;
	HTREEITEM item = pNMTreeView->itemNew.hItem;
	HTREEITEM itemParent = GetParentItem(item);

	MeshInfoPanel* p = dynamic_cast<MeshInfoPanel*>(GetParent());
	p->OnItemClick(item, itemParent);
}
//----------------------------------------------------------------------------------------------
MeshInfoPanel::MeshInfoPanel()
: _totalVertices(0)
, _totalIndices(0)
, _entity(0)
, _mesh(0)
{
}
//----------------------------------------------------------------------------------------------
MeshInfoPanel::~MeshInfoPanel()
{
}
//----------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(MeshInfoPanel, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()
//----------------------------------------------------------------------------------------------
int MeshInfoPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT
		| TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!_treeCtrl.Create(dwViewStyle, rectDummy, this, 2))
	{
		// TRACE0("未能创建Tree控件\n");
		return -1;      // 未能创建
	}

	EnableDocking(CBRS_ALIGN_ANY);

	// image list

	CBitmap bmp;
	if (!bmp.LoadBitmap(IDR_MESH_INFO_ICON))
	{
		TRACE(("无法加载位图: %x\n"), IDR_MESH_INFO_ICON);
		ASSERT(FALSE);
		return -1;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= ILC_COLOR24;

	_imageList.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	_imageList.Add(&bmp, RGB(192, 192, 192));

	_treeCtrl.SetImageList(&_imageList, TVSIL_NORMAL);

	_gemoetryInfoRoot = _treeCtrl.InsertItem("Geometry", ICON_GEO_INFO, ICON_GEO_INFO);
	_animationRoot = _treeCtrl.InsertItem("Animation", ICON_ANIMATION, ICON_ANIMATION);
	_attachPointRoot = _treeCtrl.InsertItem("AttachPoint", ICON_ATTACHPOINT, ICON_ATTACHPOINT);
	_effectsGroupRoot = _treeCtrl.InsertItem("Effects", ICON_ATTACHPOINT, ICON_ATTACHPOINT);
	_boneGroupRoot = _treeCtrl.InsertItem("BoneGroup", ICON_SKELETON, ICON_SKELETON);
	_submeshRoot = _treeCtrl.InsertItem("SubMesh", ICON_SUBMESH, ICON_SUBMESH);
	_lodRoot = _treeCtrl.InsertItem("LOD", ICON_LOD, ICON_LOD);
	
	return 0;
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	CRect rectClient;
	GetClientRect(rectClient);
	_treeCtrl.MoveWindow(rectClient);
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	_treeCtrl.SetFocus();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::_RemoveAllChild(HTREEITEM parentItem)
{
	for(;;)
	{
		HTREEITEM item = _treeCtrl.GetChildItem(parentItem);
		if(NULL != item)
		{
			_treeCtrl.DeleteItem(item);
		}
		else
		{
			break;
		}
	}
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::_RemoveChild(HTREEITEM parentItem, const String& childTxt)
{
	HTREEITEM item = _treeCtrl.GetChildItem(parentItem);
	while(NULL != item)
	{
		CString txt = _treeCtrl.GetItemText(item);
		if(txt == childTxt.c_str())
		{
			_treeCtrl.DeleteItem(item);
			break;
		}

		item = _treeCtrl.GetNextSiblingItem(item);
	}
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::AddAnimation(const String& aniName)
{
	_treeCtrl.InsertItem(aniName.c_str(), ICON_ANIMATION, ICON_ANIMATION, _animationRoot);
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::AddAnimation(const StringVector aniList)
{
	for (StringVector::const_iterator i = aniList.begin(); i != aniList.end(); ++i)
	{
		const String& ani = *i;
		AddAnimation(ani);
	}

	_treeCtrl.Expand(_animationRoot, TVE_EXPAND);
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::RemoveAnimation(const String& aniName)
{
	_RemoveChild(_animationRoot, aniName);
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::RemoveAllAnimation()
{
	_RemoveAllChild(_animationRoot);
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnItemClick(HTREEITEM item, HTREEITEM parentItem)
{
	MeshEditorFrm& frm = MeshEditorFrm::getSingleton();

	// data types
	DWORD_PTR ptr = _treeCtrl.GetItemData(item);
	MeshInfoTreeItemData* datPtr = reinterpret_cast<MeshInfoTreeItemData*>(ptr);

	// parent types
	CString itemTxt = _treeCtrl.GetItemText(item);
	if(parentItem == _animationRoot)
	{
		frm.StopPlayAnimation();
		frm.PlayAnimation((LPCTSTR)itemTxt);
	}
	else if(parentItem == _submeshRoot)
	{
		ClearSubMeshSelect();
	}
	else if(parentItem == _effectsGroupRoot)
	{
		frm.OnSelectEffectGroup((LPCTSTR)itemTxt);
		ClearEffectSelect();
		OnSelectEffect((LPCTSTR)itemTxt);
	}

	// visual display operations
	if(item == _animationRoot)
	{
		frm.StopPlayAnimation();
	}
	else if(item == _submeshRoot)
	{
		ClearSubMeshSelect();
	}
	else if(item == _effectsGroupRoot)
	{
		ClearEffectSelect();
	}

	if(datPtr)
	{
		switch(datPtr->_dataType)
		{
		case MeshInfoTreeItemData::eDT_Bone:
			{
				frm.OnSelectBone(datPtr->_data.bone);
				break;
			}

		case MeshInfoTreeItemData::eDT_AttachPoint:
			{
				frm.OnSelectAttachPoint(datPtr->_data.attachPoint);
				break;
			}

		case MeshInfoTreeItemData::eDT_SubEntity:
			{
				SelectSubMesh(datPtr->_data.subEntity);
				break;
			}
		}
	}
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::SetMesh(Mesh* mesh, Entity* ent)
{
	_entity = ent;
	_mesh = mesh;

	// Geometry info
	AddGeometryInfo();

	// Sub mesh info
	AddSubMeshInfo();

	// skeleton
	AddSkeletonInfo();

	AddAttachPointInfo();
	AddEffectsGroupInfo();

	// LOD
	AddLODInfo();

	ExpandAll();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::AddGeometryInfo()
{
	bool buildEdgeList = _mesh->getAutoBuildEdgeLists();
	_treeCtrl.InsertItem(formatString("AuotBuildEdgeList:%s",
		StringConverter::toString(buildEdgeList).c_str()).c_str(), _gemoetryInfoRoot);

	AxisAlignedBox aabb = _mesh->getBounds();
	_treeCtrl.InsertItem(formatString("Max:%s",
		StringConverter::toString(aabb.getMaximum()).c_str()).c_str(), _gemoetryInfoRoot);

	_treeCtrl.InsertItem(formatString("Min:%s",
		StringConverter::toString(aabb.getMaximum()).c_str()).c_str(), _gemoetryInfoRoot);

	_treeCtrl.InsertItem(formatString("Volume:%f", aabb.volume()).c_str(),
		_gemoetryInfoRoot);

	_treeCtrl.InsertItem(formatString("Bounding Radius:%f",
		_mesh->getBoundingSphereRadius()).c_str(), _gemoetryInfoRoot);

	_treeCtrl.InsertItem(formatString("Vertices:%d",
		_totalVertices).c_str(), _gemoetryInfoRoot);

	_treeCtrl.InsertItem(formatString("Indices:%d",
		_totalIndices).c_str(), _gemoetryInfoRoot);

	_treeCtrl.InsertItem(formatString("Faces:%d",
		(_totalIndices == 0 ? _totalVertices / 3 : _totalIndices / 3)).c_str(), _gemoetryInfoRoot);
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::AddSubMeshInfo()
{
	unsigned short cnt = _mesh->getNumSubMeshes();
	for (unsigned short i = 0; i < cnt; ++i)
	{
		SubMesh* s = _mesh->getSubMesh(i);
		AddSubMesh(i, s);
	}
}

void MeshInfoPanel::AddSkeletonInfo()
{
	HTREEITEM skelItem = _treeCtrl.InsertItem(formatString("Skeleton:[%s]",
		_mesh->getSkeletonName().c_str()).c_str(), _gemoetryInfoRoot);

	AddSkeleton(skelItem);

	_treeCtrl.InsertItem(formatString("Vertex animation:[%s]",
		g_vertexAnimationTypeConverterConverter.ToString(
		_mesh->getSharedVertexDataAnimationType()).c_str()).c_str(), _gemoetryInfoRoot);
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::AddAttachPointInfo()
{
	MeshData* md = MeshEditorFrm::getSingleton().GetMeshData();
	const MeshData::AttachPointList& lst = md->GetAttachPointList();
	MeshData::AttachPointList::const_iterator it = lst.begin();
	for (; it != lst.end(); ++it)
	{
		const SAttachPoint& atp = *it;
		HTREEITEM item = _treeCtrl.InsertItem(atp.AttachPointName.c_str(), ICON_ATTACHPOINT, ICON_ATTACHPOINT, _attachPointRoot);
		MeshInfoTreeItemData* d = OGRE_NEW MeshInfoTreeItemData(const_cast<SAttachPoint*>(&atp));
		AddMeshInfoTreeItemData(d);
		_treeCtrl.SetItemData(item, (DWORD_PTR)d);
		_treeCtrl.SetItemText(item, (atp.AttachPointName + " - " + atp.BoneName).c_str());
	}
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::AddEffectsGroupInfo()
{
	MeshData* md = MeshEditorFrm::getSingleton().GetMeshData();
	const MeshData::EffectsAttachPointMap& effMap = md->GetAttachEffectsMap();
	MeshData::EffectsAttachPointMap::const_iterator i = effMap.begin();
	for(; i != effMap.end(); ++i)
	{
		const MeshData::EffectAttachPointPair& effPair = i->second;

		HTREEITEM item = _treeCtrl.InsertItem(i->first.c_str(), ICON_ATTACHPOINT, ICON_ATTACHPOINT, _effectsGroupRoot);
		MeshInfoTreeItemData* d = OGRE_NEW MeshInfoTreeItemData(const_cast<MeshData::EffectAttachPointPair*>(&effPair));
		AddMeshInfoTreeItemData(d);
		_treeCtrl.SetItemData(item, (DWORD_PTR)d);
		_treeCtrl.InsertItem(("AttachPoint:" + effPair.AttachPointName).c_str(), ICON_ATTACHPOINT, ICON_ATTACHPOINT, item);
		_treeCtrl.InsertItem(("EffectTemplate:" + effPair.EffectTemplateName).c_str(), ICON_ATTACHPOINT, ICON_ATTACHPOINT, item);
	}
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::AddLODInfo()
{
	ushort lodCnt = _mesh->getNumLodLevels();
	for (ushort i = 0; i < lodCnt; ++i)
	{
		const MeshLodUsage& lodUsage = _mesh->getLodLevel(i);
		String str = formatString("%f : %s",
			lodUsage.value, lodUsage.manualName.c_str());
		HTREEITEM item = _treeCtrl.InsertItem(str.c_str(), ICON_LOD, ICON_LOD, _lodRoot);
		MeshInfoTreeItemData* d = OGRE_NEW MeshInfoTreeItemData(const_cast<MeshLodUsage*>(&lodUsage));
		AddMeshInfoTreeItemData(d);
		_treeCtrl.SetItemData(item, (DWORD_PTR)&d);
	}
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::ExpandAll()
{
	_treeCtrl.Expand(_submeshRoot, TVE_EXPAND);
	_treeCtrl.Expand(_gemoetryInfoRoot, TVE_EXPAND);
	_treeCtrl.Expand(_lodRoot, TVE_EXPAND);
	_treeCtrl.Expand(_effectsGroupRoot, TVE_EXPAND);
}
//------------------------------------------------------------------------
void MeshInfoPanel::AddSubMesh(unsigned short idx, SubMesh* subMesh)
{
	String txt = formatString("%d : [%s]", idx,
		subMesh->getMaterialName().c_str());

	HTREEITEM subMeshItem = _treeCtrl.InsertItem(txt.c_str(), ICON_SUBMESH, ICON_SUBMESH, _submeshRoot);
	MeshInfoTreeItemData* d = OGRE_NEW MeshInfoTreeItemData(_entity->getSubEntity(idx));
	AddMeshInfoTreeItemData(d);
	_treeCtrl.SetItemData(subMeshItem, (DWORD_PTR)d);

	txt = formatString("Render operation:%s",
		g_renderOperationConverter.ToString(subMesh->operationType).c_str());

	_treeCtrl.InsertItem(txt.c_str(), subMeshItem);

	//
	// Vetex/Index count
	//
	if(subMesh->vertexData)
	{
		txt = formatString("Vertices:%d", subMesh->vertexData->vertexCount);
// 		HTREEITEM vertItem = _treeCtrl.InsertItem(txt.c_str(), subMeshItem);

// 		VertexBufferBinding* vb = subMesh->vertexData->vertexBufferBinding;

		_totalVertices += subMesh->vertexData->vertexCount;
	}

	bool hasIndex = false;
	if(subMesh->indexData)
	{
		IndexData* id = subMesh->indexData;
		HardwareIndexBuffer::IndexType idt = id->indexBuffer->getType();
		txt = formatString("Indices:%d [%s]", id->indexCount,
			(idt == HardwareIndexBuffer::IT_16BIT ? "16Bit" : "32Bit"));

		_treeCtrl.InsertItem(txt.c_str(), subMeshItem);

		_totalIndices += subMesh->indexData->indexCount;
		hasIndex = true;
	}

	if(hasIndex)
	{
		txt = formatString("Faces:%d", subMesh->indexData->indexCount / 3);
		_treeCtrl.InsertItem(txt.c_str(), subMeshItem);
	}
	else
	{
		txt = formatString("Faces:%d", subMesh->vertexData->vertexCount);
		_treeCtrl.InsertItem(txt.c_str(), subMeshItem);
	}

	//
	// Vertex declaration
	//
	if(subMesh->vertexData)
	{
		VertexDeclaration* vd = subMesh->vertexData->vertexDeclaration;
		if(vd)
		{
			size_t cnt = vd->getElementCount();
			HTREEITEM hvd = _treeCtrl.InsertItem(formatString(
				"Vertex Declaration, Count[%d]", cnt).c_str(), subMeshItem);
			
			const VertexDeclaration::VertexElementList& vel = vd->getElements();
			VertexDeclaration::VertexElementList::const_iterator i = vel.begin();
			for (; i != vel.end(); ++i)
			{
				const VertexElement& ve = *i;
				String str = formatString("%s : %s", //  \r\n Idx[%d] Offset[%d] Source[%d] Size[%d]
					g_vertexElementSemanticConverter.ToString(ve.getSemantic()).c_str(),
					g_vertexElementTypeConverter.ToString(ve.getType()).c_str());
					//,ve.getIndex(), ve.getOffset(), ve.getSource(), ve.getSize());

				HTREEITEM hve = _treeCtrl.InsertItem(str.c_str(), hvd);

				// sub item for details
				str = g_vertexElementSemanticConverter.ToString(ve.getSemantic());
				_treeCtrl.InsertItem(("Semantic:" + str).c_str(), hve);

				str = formatString("Index:%d", ve.getIndex());
				_treeCtrl.InsertItem(str.c_str(), hve);

				str = formatString("Offset:%d", ve.getOffset());
				_treeCtrl.InsertItem(str.c_str(), hve);

				str = formatString("Source:%d", ve.getSource());
				_treeCtrl.InsertItem(str.c_str(), hve);

				str = formatString("Size:%d", ve.getSize());
				_treeCtrl.InsertItem(str.c_str(), hve);

				//_treeCtrl.Expand(hve, TVE_EXPAND);
			}

			//_treeCtrl.Expand(hvd, TVE_EXPAND);
		}
	}

	_treeCtrl.Expand(subMeshItem, TVE_EXPAND);
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::RemoveSubMesh(const String& name)
{
	_RemoveChild(_submeshRoot, name);
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::RemoveSubMesh(unsigned short idx)
{

}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::RemoveAllSubMesh()
{
	_RemoveAllChild(_submeshRoot);
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::SelectSubMesh(SubEntity* subMesh)
{
	MementoMap::iterator i = _subMeshSelectMap.find(subMesh->getMaterialName());
	if(i == _subMeshSelectMap.end())
	{
		_subMeshSelectMap.insert(std::make_pair(
			subMesh->getMaterialName(), OGRE_NEW SubMeshMaterialMemento(subMesh)));
	}
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::ClearSubMeshSelect()
{
	_subMeshSelectMap.clear();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::Reset()
{
	RemoveAllAnimation();
	RemoveAllSubMesh();

	_RemoveAllChild(_gemoetryInfoRoot);
	_RemoveAllChild(_lodRoot);
	_RemoveAllChild(_effectsGroupRoot);
	_RemoveAllChild(_attachPointRoot);
	_RemoveAllChild(_boneGroupRoot);

	_totalIndices = 0;
	_totalVertices = 0;

	ClearSubMeshSelect();
	ClearEffectSelect();

	DestroyAllMeshInfoTreeItemData();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::AddSkeleton(HTREEITEM skeletonItem)
{
	if(_mesh->hasSkeleton())
	{
		Skeleton* sk = _mesh->getSkeleton().get();
		if(sk)
		{
			MeshInfoTreeItemData* d = OGRE_NEW MeshInfoTreeItemData(sk);
			AddMeshInfoTreeItemData(d);
			_treeCtrl.SetItemData(skeletonItem, (DWORD_PTR)d);

			Skeleton::BoneIterator bi = sk->getRootBoneIterator();
			while(bi.hasMoreElements())
			{
				HTREEITEM item = RecursiveAddBoneNode(bi.peekNext(), skeletonItem);
				_treeCtrl.Expand(item, TVE_EXPAND);
				bi.moveNext();
			}
		}
		else
		{
			LogErrorID("Editor", "[MeshInfoPanel::AddSkeleton] skeleton not exist?");
		}
	}
}
//----------------------------------------------------------------------------------------------
HTREEITEM MeshInfoPanel::RecursiveAddBoneNode(Bone* bone, HTREEITEM parentBoneItem)
{
	CString str;
	str.Format("%s [%d]", bone->getName().c_str(), bone->getHandle());	
	
	HTREEITEM boneItem = _treeCtrl.InsertItem(str, parentBoneItem);
	MeshInfoTreeItemData* d = OGRE_NEW MeshInfoTreeItemData(bone);
	AddMeshInfoTreeItemData(d);
	_treeCtrl.SetItemData(boneItem, (DWORD_PTR)d);

	//str.Format("IsManuallyControlled : %s", StringConverter::toString(bone->isManuallyControlled()).c_str());	
	//_treeCtrl.InsertItem(str, boneItem);

	Bone::ChildNodeIterator i = bone->getChildIterator();
	while(i.hasMoreElements())
	{
		HTREEITEM item = RecursiveAddBoneNode((Bone*)i.peekNextValue(), boneItem);
		_treeCtrl.Expand(item, TVE_EXPAND);
		i.moveNext();
	}

	return boneItem;
}
//----------------------------------------------------------------------------------------------
BOOL MeshInfoPanel::OnBeforeShowPaneMenu(CMenu& m)
{
	m.DestroyMenu();
	_popupMenu.DestroyMenu();

	BOOL r = _popupMenu.LoadMenu(IDR_MODELEDITOR);
	ASSERT(r);
	CMenu* subM = _popupMenu.GetSubMenu(0);
	m.Attach(subM->GetSafeHmenu());

	MeshEditorFrm& frm = MeshEditorFrm::getSingleton();
	if(frm.Is4Legs())
	{
		subM->CheckMenuItem(ID_MESHEDIT_SETAS4LEGS, MF_BYCOMMAND | MF_CHECKED);
	}

	if(!frm.GetSelectedBone())
	{
		subM->EnableMenuItem(ID_MESHEDIT_SETASLEFTFOOT, MF_BYCOMMAND | MF_DISABLED);
		subM->EnableMenuItem(ID_MESHEDIT_SETASLEFTBACKFOOT, MF_BYCOMMAND | MF_DISABLED);
		subM->EnableMenuItem(ID_MESHEDIT_SETASRIGHTFOOT, MF_BYCOMMAND | MF_DISABLED);
		subM->EnableMenuItem(ID_MESHEDIT_SETASRIGHTBACKFOOT, MF_BYCOMMAND | MF_DISABLED);
		subM->EnableMenuItem(ID_MESHEDIT_SETAS4LEGS, MF_BYCOMMAND | MF_DISABLED);
	}


	if(!frm.GetActor())
	{
		subM->EnableMenuItem(ID_MESHEDIT_ADDLOD, MF_BYCOMMAND | MF_DISABLED);
		subM->EnableMenuItem(ID_MESHEDIT_DELETELOD, MF_BYCOMMAND | MF_DISABLED);
	}

	if(!frm.GetActor() || !frm.GetActor()->HasSkeleton())
	{
		subM->EnableMenuItem(ID_MESHEDIT_ADDLINKEDSKELETON, MF_BYCOMMAND | MF_DISABLED);
		subM->EnableMenuItem(ID_MESHEDIT_REMOVELINKEDSKELETON, MF_BYCOMMAND | MF_DISABLED);

		subM->EnableMenuItem(ID_MESHEDIT_ADDEFFECTS, MF_BYCOMMAND | MF_DISABLED);
		subM->EnableMenuItem(ID_MESHEDIT_DELETEEFFECTS, MF_BYCOMMAND | MF_DISABLED);

		subM->EnableMenuItem(ID_MESHEDIT_ADDATTACHPOINT, MF_BYCOMMAND | MF_DISABLED);
		subM->EnableMenuItem(ID_MESHEDIT_DELETEATTACHPOINT, MF_BYCOMMAND | MF_DISABLED);

		subM->EnableMenuItem(ID_MESHEDIT_ADDBONEGROUP, MF_BYCOMMAND | MF_DISABLED);
		subM->EnableMenuItem(ID_MESHEDIT_DELETEBONEGROUP, MF_BYCOMMAND | MF_DISABLED);		
	}

	if(frm.GetPlayingAnimation().empty())
	{
		subM->EnableMenuItem(ID_MESHEDIT_ADDFRAMEEVENT, MF_BYCOMMAND | MF_DISABLED);
		subM->EnableMenuItem(ID_MESHEDIT_DELETEFRAMEEVENT, MF_BYCOMMAND | MF_DISABLED);
		subM->EnableMenuItem(ID_MESHEDIT_COPYTOCLIPBOARD, MF_BYCOMMAND | MF_DISABLED);			
		subM->EnableMenuItem(ID_MESHEDIT_CALCMOVEDISTANCE, MF_BYCOMMAND | MF_DISABLED);
	}

	//HTREEITEM selItem = _treeCtrl.GetSelectedItem();
	//HTREEITEM selParent = _treeCtrl.GetParentItem(selItem);

	return TRUE;
}
//----------------------------------------------------------------------------------------------
BOOL MeshInfoPanel::OnAfterShowPaneMenu(int nMenuResult)
{
	switch(nMenuResult)
	{
	case ID_MESHEDIT_ADDATTACHPOINT:
		OnAddAttachPoint();
		break;

	case ID_MESHEDIT_DELETEATTACHPOINT:
		OnDelAttachPoint();
		break;

	case ID_MESHEDIT_ADDLOD:
		OnAddLOD();
		break;

	case ID_MESHEDIT_DELETELOD:
		OnAddDelLOD();
		break;

	case ID_MESHEDIT_ADDBONEGROUP:
		OnAddBoneGroup();
		break;

	case ID_MESHEDIT_DELETEBONEGROUP:
		OnDelBoneGroup();
		break;

	case ID_MESHEDIT_ADDFRAMEEVENT:
		OnAddFrameEvent();
		break;

	case ID_MESHEDIT_DELETEFRAMEEVENT:
		OnDelFrameEvent();
		break;

	case ID_MESHEDIT_ADDEFFECTS:
		OnAddEffects();
		break;

	case ID_MESHEDIT_DELETEEFFECTS:
		OnDelEffects();
		break;

	case ID_MESHEDIT_ADDLINKEDSKELETON:
		MeshEditorFrm::getSingleton().OnAddLinkedSkeleton();
		break;

	case ID_MESHEDIT_REMOVELINKEDSKELETON:
		MeshEditorFrm::getSingleton().OnRemoveLinkedSkeleton();
		break;

	case ID_MESHEDIT_COPYTOCLIPBOARD:
		MeshEditorFrm::getSingleton().OnCopyPlayingAnimationNameToClipboard();
		break;

	case ID_MESHEDIT_SETASLEFTFOOT:
		MeshEditorFrm::getSingleton().SetAsLeftFoot();
		break;

	case ID_MESHEDIT_SETASRIGHTFOOT:
		MeshEditorFrm::getSingleton().SetAsRightFoot();
		break;

	case ID_MESHEDIT_SETASLEFTBACKFOOT:
		MeshEditorFrm::getSingleton().SetAsLeftBackFoot();
		break;

	case ID_MESHEDIT_SETASRIGHTBACKFOOT:
		MeshEditorFrm::getSingleton().SetAsRightBackFoot();
		break;

	case ID_MESHEDIT_SETAS4LEGS:
		MeshEditorFrm::getSingleton().OnToggle4Legs();		
		break;

	case ID_MESHEDIT_CALCMOVEDISTANCE:
		MeshEditorFrm::getSingleton().CalcMoveDist();
		break;
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnAddAttachPoint()
{
	MeshEditorFrm::getSingleton().OnAddAttachPoint();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnDelAttachPoint()
{
	MeshEditorFrm::getSingleton().OnDelAttachPoint();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnAddLOD()
{
	MeshEditorFrm::getSingleton().OnAddLOD();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnAddDelLOD()
{
	MeshEditorFrm::getSingleton().OnAddDelLOD();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnAddAnimation()
{
	MeshEditorFrm::getSingleton().OnAddAnimation();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnDelAnimation()
{
	MeshEditorFrm::getSingleton().OnDelAnimation();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnAddBoneGroup()
{
	MeshEditorFrm::getSingleton().OnAddBoneGroup();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnDelBoneGroup()
{
	MeshEditorFrm::getSingleton().OnDelBoneGroup();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnAddFrameEvent()
{
	MeshEditorFrm::getSingleton().OnAddFrameEvent();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnDelFrameEvent()
{
	MeshEditorFrm::getSingleton().OnDelFrameEvent();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnAddEffects()
{
	MeshEditorFrm::getSingleton().OnAddEffects();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnDelEffects()
{
	MeshEditorFrm::getSingleton().OnDelEffects();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::AddMeshInfoTreeItemData(MeshInfoTreeItemData* dat)
{
	_meshInfoTreeItemDataList.push_back(dat);
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::DestroyAllMeshInfoTreeItemData()
{
	struct DeleteItemData
	{
		void operator()(MeshInfoTreeItemData* dat)
		{
			OGRE_DELETE dat;
		}
	};
	std::for_each(_meshInfoTreeItemDataList.begin(), _meshInfoTreeItemDataList.end(), DeleteItemData());
	_meshInfoTreeItemDataList.clear();
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::OnSelectEffect(const String& effectGrp)
{
	MementoMap::iterator i = _effectsSelectMap.find(effectGrp);
	if(i == _effectsSelectMap.end())
	{
		AnimationModel* actor = MeshEditorFrm::getSingleton().GetActor();
		_effectsSelectMap.insert(std::make_pair(
			effectGrp, OGRE_NEW ParticleSystemMaterialMemento(actor->GetAttachEffects(effectGrp))));
	}
}
//----------------------------------------------------------------------------------------------
void MeshInfoPanel::ClearEffectSelect()
{
	_effectsSelectMap.clear();
}