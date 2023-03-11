#pragma once
#include "OgeEnginePluginPrerequisites.h"

class Cinema;
class IGameObject;
class ICinemaTrack;

//-----------------------------------------------------------------------------
class _OgeEnginePluginExport CinemaKeyFrame : public Ogre::AnimationAlloc
{
public:

	CinemaKeyFrame(ICinemaTrack* parent, float time);
	virtual ~CinemaKeyFrame();

	float GetTime() const { return _time; }
	void SetTime(float val) { _time = val; }

	virtual String ToString() const;
	virtual void FromString(const String& val);

protected:

	float _time;
	ICinemaTrack* _parent;
};
//-----------------------------------------------------------------------------
struct _OgeEnginePluginExport SKeyFrameTip
{
	float Time;
	String TipText;
};

class _OgeEnginePluginExport ICinemaTrack : public PropertyObject, public OgeAlloc
{
public:

	typedef Ogre::vector<SKeyFrameTip>::type KeyFrameTipList;

	virtual const String& GetTypeName() const = 0;

	virtual Cinema* GetParent() = 0;

	virtual IGameObject* GetTargetObjct() = 0;
	virtual void SetTargetObjct(IGameObject* object) = 0;

	virtual const XGUID& GetTargetObjectID() const = 0;
	virtual void SetTargetObjectID(const XGUID& val) = 0;

	virtual void SetTimePosition(float timePos) = 0;

	virtual CinemaKeyFrame* CreateKeyFrame(float timePos) = 0;
	virtual void DestroyKeyFrame(CinemaKeyFrame* kf) = 0;
	virtual void DestroyKeyFrame(size_t idx) = 0;
	virtual void DestroyAllKeyFrames() = 0;
	//virtual CinemaKeyFrame* AutoSnapshot(float timePos) = 0;
	virtual void AutoUpdateKeyFrame(CinemaKeyFrame* kf) = 0;

	virtual void AddTip(const SKeyFrameTip& tip) = 0;
	virtual size_t GetTipCount() const = 0;
	virtual const SKeyFrameTip& GetTip(size_t idx) = 0;
	virtual void RemoveTip(size_t idx) = 0;
	virtual void RemoveAllTip() = 0;
	virtual const KeyFrameTipList& GetTips() const = 0;
	virtual void SetTips(const KeyFrameTipList& tips) = 0;

	template<class T>
	T* CreateKeyFrame(float timePos)
	{
		return dynamic_cast<T*>(CreateKeyFrame(timePos));
	}

	virtual bool IsEnabled() const = 0;
	virtual void SetEnable(bool on) = 0;

	virtual bool NeedLoad() const = 0;
	virtual bool HasLoad() const = 0;
	virtual void Load(bool forceSynchronous = false) = 0;
	virtual void CancelLoad() = 0;

	virtual void _OnKeyFrameDataChanged() = 0;

	virtual size_t GetKeyFrameCount() const = 0;
	virtual CinemaKeyFrame* GetKeyFrameByIndex(int idx) = 0;

	virtual float GetKeyFramesAtTime(const Ogre::TimeIndex& timeIndex,
		CinemaKeyFrame** keyFrame1, CinemaKeyFrame** keyFrame2,
		unsigned short* firstKeyIndex = 0) const = 0;

	virtual void _CollectKeyFrameTimes(vector<Real>::type& keyFrameTimes) = 0;
	virtual void _BuildKeyFrameIndexMap(const Ogre::vector<Real>::type& keyFrameTimes) = 0;

	virtual String ToString() const = 0;
};
//-----------------------------------------------------------------------------
class _OgeEnginePluginExport ICinemaTrackFactory : public Ogre::AnimationAlloc
{
public:

	virtual const String& GetTypeName() const = 0;
	virtual ICinemaTrack* Create(Cinema* parent, IGameObject* object) = 0;
	virtual void Destroy(ICinemaTrack* track) = 0;
};
//-----------------------------------------------------------------------------
template<class T>
class TCinemaTrackFactory : public ICinemaTrackFactory
{
public:

	virtual ICinemaTrack* Create(Cinema* parent, IGameObject* object)
	{
		return OGRE_NEW T(parent, object);
	}

	virtual void Destroy(ICinemaTrack* track)
	{
		OGRE_DELETE track;
	}
};
//-----------------------------------------------------------------------------
class _OgeEnginePluginExport CinemaManager
	: public Singleton<CinemaManager>
	, public IEventLisitener
	, public IUpdateAble
{
public:

	CinemaManager();
	virtual ~CinemaManager();

	static String CinemaXMLFileName;

	typedef Ogre::map<String, ICinemaTrackFactory*>::type FactoryMap;
	void AddTrackFactory(const String& typeName, ICinemaTrackFactory* factory, bool overrideExist = true);
	ICinemaTrackFactory* GetTrackFactory(const String& typeName) const;
	const FactoryMap& GetAllTrackFactory() const;

	typedef Ogre::map<String, Cinema*>::type CinemaMap;
	Cinema* CreateCinema(const String& name, float timeLength);
	Cinema* GetCinema(const String& name);
	const CinemaMap& GetAllCinema() const;
	bool HasCinema() const;
	void DestroyCinema(const String& name);
	void DestroyCinema(Cinema* cinema);	
	void DestroyAllCinema();

	void LoadCinemas(const String& cinemaFile);
	void LoadCinemasFromResourceSystem(const String& cinemaFile,
		const String& resGrp = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	void LoadCinemasFromXMLString(const String& cinemaFile);
	void LoadCinemasFromXML(XMLDoc& doc);
	void SaveCinemas(const String& cinemaFile);

	Ogre::Camera* GetCamera() const { return _camera; }
	void SetCamera(Ogre::Camera* val) { _camera = val; }

	virtual void Update(float deltaTime);

protected:

	FactoryMap _factoryMap;
	CinemaMap _cinemaMap;
	Ogre::Camera* _camera;

	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast);
};
//-----------------------------------------------------------------------------
struct _OgeEnginePluginExport SCinemaTracFactoryRgeHelper
{
	SCinemaTracFactoryRgeHelper(const String& typeName, ICinemaTrackFactory* factory, bool overrideExist = true)
	{
		CinemaManager::getSingleton().AddTrackFactory(typeName, factory, overrideExist);
	}
};

#define M_REG_CINEMA_TRACK_FACTORY(name, F) static F name##F##Instance; static SCinemaTracFactoryRgeHelper name##RegHelper(#name, &name##F##Instance);

//-----------------------------------------------------------------------------
class _OgeEnginePluginExport AbstractCinemaTrack
	: public ICinemaTrack
 	, public Ogre::WorkQueue::RequestHandler
 	, public Ogre::WorkQueue::ResponseHandler
{
public:

	typedef Ogre::vector<CinemaKeyFrame*>::type CinemaKeyFrameList;
	typedef Ogre::vector<ushort>::type KeyFrameIndexMap;

	AbstractCinemaTrack(Cinema* parent, IGameObject* object);
	virtual ~AbstractCinemaTrack();

	virtual Cinema* GetParent();

	virtual IGameObject* GetTargetObjct();
	virtual void SetTargetObjct(IGameObject* object);

	virtual bool IsEnabled() const { return _enable; }
	bool GetEnable() const { return _enable; }
	virtual void SetEnable(bool on) { _enable = on; }

	virtual bool NeedLoad() const;
	virtual bool HasLoad() const;
	virtual void Load(bool forceSynchronous = false);
	virtual void CancelLoad();

	virtual CinemaKeyFrame* CreateKeyFrame(float timePos);
	virtual void DestroyKeyFrame(CinemaKeyFrame* kf);
	virtual void DestroyKeyFrame(size_t idx);
	virtual void DestroyAllKeyFrames();
	//virtual CinemaKeyFrame* AutoSnapshot(float timePos);

	virtual void AddTip(const SKeyFrameTip& tip);
	virtual size_t GetTipCount() const;
	virtual const SKeyFrameTip& GetTip(size_t idx);
	virtual void RemoveTip(size_t idx);
	virtual void RemoveAllTip();
	virtual const KeyFrameTipList& GetTips() const;
	virtual void SetTips(const KeyFrameTipList& tips);

	virtual void _OnKeyFrameDataChanged();

	virtual size_t GetKeyFrameCount() const;
	virtual CinemaKeyFrame* GetKeyFrameByIndex(int idx);

	virtual float GetKeyFramesAtTime(const Ogre::TimeIndex& timeIndex,
		CinemaKeyFrame** keyFrame1, CinemaKeyFrame** keyFrame2,
		unsigned short* firstKeyIndex = 0) const;

	virtual void _CollectKeyFrameTimes(vector<Real>::type& keyFrameTimes);
	virtual void _BuildKeyFrameIndexMap(const Ogre::vector<Real>::type& keyFrameTimes);

	void WaitForDerivedProcesses();

	virtual void BuildPropertyMap();
	virtual String ToString() const;

	const String& GetComment() const { return _comment; }
	void SetComment(const String& val) { _comment = val; }

	virtual const XGUID& GetTargetObjectID() const { return _targetObjectID; }
	virtual void SetTargetObjectID(const XGUID& val) { _targetObjectID = val; }

protected:

	Cinema* _parent;
	IGameObject* _target;
	XGUID _targetObjectID;
	bool _enable;
	String _comment;
	bool _needTarget;

	CinemaKeyFrameList _keyframes;

	KeyFrameIndexMap _keyFrameIndexMap;

	KeyFrameTipList _tipList;

	WorkQueue::RequestID _loadingID;
	bool _needLoad;
	bool _hasLoad;

	struct LoadTrackRequest
	{
		AbstractCinemaTrack* track;
		_OgeEnginePluginExport friend std::ostream& operator<<(std::ostream& o, const LoadTrackRequest& r)
		{ return o; }
	}; 

	struct LoadTrackResponse
	{
		_OgeEnginePluginExport friend std::ostream& operator<<(std::ostream& o, const LoadTrackResponse& r)
		{ return o; }
	}; 

	bool _derivedDataLoadingInProgress; 

	/// WorkQueue::RequestHandler override
	virtual bool canHandleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
	/// WorkQueue::RequestHandler override
	virtual WorkQueue::Response* handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
	/// WorkQueue::ResponseHandler override
	virtual bool canHandleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);
	/// WorkQueue::ResponseHandler override
	virtual void handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);

	virtual void LoadImpl();
	virtual void OnLoadCompleted();

	virtual CinemaKeyFrame* CreateKeyFrameImpl(float timePos) = 0;
};