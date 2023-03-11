#pragma once
#include "EnginePrerequisites.h"

#define MESHDATA_EXT String(".MeshData")

struct _EngineExport SFrameEvent
{
	Real AnimationTime;
	String EventName;

	SFrameEvent()
		: AnimationTime(0.0f)
	{
	}
};

struct _EngineExport SAttachPoint
{
	String BoneName;
	String AttachPointName;
	Vector3 PositionOffset;
	Vector3 ScaleOffset;
	Vector3 OrientationOffset;

	SAttachPoint()
		: PositionOffset(Vector3::ZERO)
		, ScaleOffset(Vector3::ZERO)
		, OrientationOffset(Vector3::ZERO)
	{
	}
};


class _EngineExport MeshData : public Resource
{
public:

	typedef std::vector<SFrameEvent> FrameEventList;
	typedef std::map<String, FrameEventList> AnimationFrameEventMap;

	typedef std::list<SAttachPoint> AttachPointList;

	typedef std::vector<int> BoneHandleList;
	typedef std::map<String, BoneHandleList> BoneGroupMap;

	// Effects AttachPoint

	struct EffectAttachPointPair
	{
		String EffectTemplateName;
		String AttachPointName;
	};

	typedef std::map<String, EffectAttachPointPair> EffectsAttachPointMap;

	MeshData();
	MeshData(ResourceManager* creator, const String& name, ResourceHandle handle,
		const String& group, bool isManual = false, ManualResourceLoader* loader = 0);
	virtual ~MeshData();

	void LoadFromDataStream(DataStreamPtr ds);
	void SaveToFile(const String& fileName) const;

	bool HasFrameEvent(const String& animationName) const;
	bool HasFrameEvent(const String& animationName, const String& eventName) const;
	FrameEventList& GetAnimationFrameEventList(const String& animationName);

	void AddFrameEvent(const String& animationName, const SFrameEvent& evt);
	void RemoveFrameEvent(const String& animationName, const String& eventName);
	SFrameEvent* GetFrameEvent(const String& animationName, const String& eventName);

	bool HasAttachPoint(const String& attachPointName);
	const SAttachPoint& GetAttachPoint(const String& attachPointName);

	void AddAttchPoint(const SAttachPoint& atp);
	void RemoveAttachPoint(const String& attachPointName);
	const AttachPointList& GetAttachPointList() const { return _attachPointList; }

	const BoneGroupMap& GetBoneGroupMap() const { return _boneGroupMap; }
	const BoneHandleList& GetBoneGroup(const String& groupName) const;
	BoneHandleList& GetBoneGroup(const String& groupName);
	bool HasBoneGroup(const String& groupName) const;
	void AddBoneGroup(const String& groupName, const BoneHandleList& handleList);
	void RemoveBoneGroup(const String& groupName);

	const EffectsAttachPointMap& GetAttachEffectsMap() const { return _effectsMap; }
	void AddAttachEffect(const String& groupName, const EffectAttachPointPair& effectPair);
	void RemoveAttachEffect(const String& groupName);
	bool HasAttachEffect(const String& groupName) const;
	const String& GetAttachEffectAttachPoint(const String& groupName) const;
	const String& GetAttachEffectTemplate(const String& groupName) const;

	const StringVector& GetLinkedSkeletonList() const { return _linkedSkeletonList; }
	size_t GetLinkedSkeletonCount() const { return _linkedSkeletonList.size(); }
	void AddLinkedSkeleton(const String& skeletonName);
	void RemoveLinkedSkeleton(size_t idx);
	void RemoveLinkedSkeleton(const String& skeletonName);

	const String GetLeftFootBone() const { return _leftFootBone; }
	void SetLeftFootBone(const String& bone) { _leftFootBone = bone; }

	const String GetRightFootBone() const { return _rightFootBone; }
	void SetRightFootBone(const String& bone) { _rightFootBone = bone; }

	const String GetLeftBackFootBone() const { return _leftBackFootBone; }
	void SetLeftBackFootBone(const String& bone) { _leftBackFootBone = bone; }

	const String GetRightBackFootBone() const { return _rightBackFootBone; }
	void SetRightBackFootBone(const String& bone) { _rightBackFootBone = bone; }

	bool Is4Legs() const { return _is4Legs; }
	void Set4Legs(bool on) { _is4Legs = on; }

	float GetAnimationMoveDistance(const String& aniName);
	void SetAnimationMoveDistance(const String& aniName, float dist);

protected:

	AnimationFrameEventMap _animationFrameEventMap;
	bool _enableSort;

	AttachPointList _attachPointList;
	BoneGroupMap	_boneGroupMap;

	EffectsAttachPointMap _effectsMap;

	StringVector _linkedSkeletonList;

	String _leftFootBone;
	String _rightFootBone;

	String _leftBackFootBone;
	String _rightBackFootBone;

	bool _is4Legs;

	typedef std::map<String, float> MoveDistMap;
	MoveDistMap _moveDistMap;

	String GetMeshFile() const;

	/// @copydoc Resource::loadImpl
	void loadImpl(void);
	/// @copydoc Resource::unloadImpl
	void unloadImpl(void);
	/// @copydoc Resource::calculateSize
	size_t calculateSize(void) const;

	void Sort(FrameEventList& lst);
};

//----------------------------------------------------------------------------------------------

/** Specialisation of SharedPtr to allow SharedPtr to be assigned to FontPtr 
@note Has to be a subclass since we need operator=.
We could templatise this instead of repeating per Resource subclass, 
except to do so requires a form VC6 does not support i.e.
ResourceSubclassPtr<T> : public SharedPtr<T>
*/
class _EngineExport MeshDataPtr
	: public SharedPtr<MeshData>
	, public OgeAlloc 
{
public:
	MeshDataPtr() : SharedPtr<MeshData>() {}
	explicit MeshDataPtr(MeshData* rep) : SharedPtr<MeshData>(rep) {}
	MeshDataPtr(const MeshDataPtr& r) : SharedPtr<MeshData>(r) {} 
	MeshDataPtr(const ResourcePtr& r) : SharedPtr<MeshData>()
	{
		// lock & copy other mutex pointer
		OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
		{
			OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
				OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
				pRep = static_cast<MeshData*>(r.getPointer());
			pUseCount = r.useCountPointer();
			if (pUseCount)
			{
				++(*pUseCount);
			}
		}
	}

	/// Operator used to convert a ResourcePtr to a MeshDataPtr
	MeshDataPtr& operator=(const ResourcePtr& r)
	{
		if (pRep == static_cast<MeshData*>(r.getPointer()))
			return *this;
		release();
		// lock & copy other mutex pointer
		OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
		{
			OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
				OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
				pRep = static_cast<MeshData*>(r.getPointer());
			pUseCount = r.useCountPointer();
			if (pUseCount)
			{
				++(*pUseCount);
			}
		}
		else
		{
			// RHS must be a null pointer
			assert(r.isNull() && "RHS must be null if it has no mutex!");
			setNull();
		}
		return *this;
	}
};

//----------------------------------------------------------------------------------------------


class _EngineExport MeshDataManager
	: public ResourceManager
	, public Singleton<MeshDataManager>
{
public:

	MeshDataManager();
	virtual ~MeshDataManager();

	MeshDataPtr createOrRetrieve(const String& name);

protected:

	virtual Resource* createImpl(const String& name, ResourceHandle handle, 
		const String& group, bool isManual, ManualResourceLoader* loader, 
		const NameValuePairList* createParams);
};

/**
*	用于读取列表文件
*	文件内容格式：
*		itemValue1;
*		itemValue2;
*		itemValue3;
*/
class _EngineExport ListFileReader
{
public:

	ListFileReader();
	virtual ~ListFileReader();

	/**	工具函数取得文件列表，用于从文件中读取所有的列表信息，以key为主 */
	StringVector getFileNameList(const String& meshFileName, const String& fileExt);

protected:

	virtual String FormatTargetFileName(const String& meshFileName, const String& fileExt);
};

/**
*	Ofusion骨架动画加载器
*/
class _EngineExport OSMAnimSerializer
	: public Serializer
	, public ListFileReader
{
public:

	OSMAnimSerializer();
	virtual ~OSMAnimSerializer(){}

	/**	从.skani文件加载配置的动画到entity中 */
	void importExternalAnimations(Mesh* pMesh, const String& meshFileName);

	/**	从*.animation文件加载一段动画到骨架中 */
	void addAnimation(Skeleton* skel, const String& fileName);

	/**	取得外部分组动画名列表,传入模型文件名,通过.skani配置的文件列表 */
	StringVector getAllExternalSkeletonAnimationList(const String& meshFileName);
};
