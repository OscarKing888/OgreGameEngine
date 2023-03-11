#include "StdAfx.h"
#include "ICinemaTrack.h"
#include "Cinema.h"

const uint16 WORKQUEUE_TRACK_LOADING_REQUEST = 1;
String CinemaManager::CinemaXMLFileName = "Cinemas.xml";


struct KeyFrameTimeLess
{
	bool operator() (const CinemaKeyFrame* kf, const CinemaKeyFrame* kf2) const
	{
		return kf->GetTime() < kf2->GetTime();
	}
};
//-----------------------------------------------------------------------------
AbstractCinemaTrack::AbstractCinemaTrack(Cinema* parent, IGameObject* object)
: _parent(parent)
, _target(object)
, _loadingID((WorkQueue::RequestID)-1)
, _needLoad(false)
, _hasLoad(false)
, _enable(true)
, _needTarget(true)
, _derivedDataLoadingInProgress(false)
{

	WorkQueue* wq = Root::getSingleton().getWorkQueue();
	wq->addRequestHandler(Cinema::LoadingWorkQueueChannel, this);
	wq->addResponseHandler(Cinema::LoadingWorkQueueChannel, this); 
}
//-----------------------------------------------------------------------------
AbstractCinemaTrack::~AbstractCinemaTrack()
{
	WaitForDerivedProcesses();

	WorkQueue* wq = Root::getSingleton().getWorkQueue();
	wq->removeRequestHandler(Cinema::LoadingWorkQueueChannel, this);
	wq->removeResponseHandler(Cinema::LoadingWorkQueueChannel, this); 
}
//-----------------------------------------------------------------------------
Cinema* AbstractCinemaTrack::GetParent()
{
	return _parent;
}
//-----------------------------------------------------------------------------
size_t AbstractCinemaTrack::GetKeyFrameCount() const
{
	return _keyframes.size();
}
//-----------------------------------------------------------------------------
CinemaKeyFrame* AbstractCinemaTrack::GetKeyFrameByIndex( int idx )
{
	assert( idx < (int)_keyframes.size() );
	if(idx >= (int)_keyframes.size())
	{
		OGRE_EXCEPT(Exception::ERR_RT_ASSERTION_FAILED,
			"Keyframe index overflow:" + StringConverter::toString(idx)
			+ " Max:" + StringConverter::toString(idx), __FUNCTION__);
	}

	return _keyframes[idx];

}
//-----------------------------------------------------------------------------
float AbstractCinemaTrack::GetKeyFramesAtTime(
	const Ogre::TimeIndex& timeIndex, CinemaKeyFrame** keyFrame1,
	CinemaKeyFrame** keyFrame2, unsigned short* firstKeyIndex ) const
{
	// Parametric time
	// t1 = time of previous keyframe
	// t2 = time of next keyframe
	Real t1, t2;

	Real timePos = timeIndex.getTimePos();

	// Find first keyframe after or on current time
	CinemaKeyFrameList::const_iterator i;
	if (timeIndex.hasKeyIndex())
	{
		// Global keyframe index available, map to local keyframe index directly.
		assert(timeIndex.getKeyIndex() < _keyFrameIndexMap.size());
		i = _keyframes.begin() + _keyFrameIndexMap[timeIndex.getKeyIndex()];
#ifdef _DEBUG
		CinemaKeyFrame timeKey(0, timePos);
		if (i != std::lower_bound(_keyframes.begin(), _keyframes.end(), &timeKey, KeyFrameTimeLess()))
		{
			OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
				"Optimised key frame search failed",
				"AbstractCinemaTrack::GetKeyFramesAtTime");
		}
#endif
	}
	else
	{
		// Wrap time
		Real totalAnimationLength = _parent->GetLength();
		assert(totalAnimationLength > 0.0f && "Invalid animation length!");

		while (timePos > totalAnimationLength && totalAnimationLength > 0.0f)
		{
			timePos -= totalAnimationLength;
		}

		// No global keyframe index, need to search with local keyframes.
		CinemaKeyFrame timeKey(0, timePos);
		i = std::lower_bound(_keyframes.begin(), _keyframes.end(), &timeKey, KeyFrameTimeLess());
	}

	if (i == _keyframes.end())
	{
		// There is no keyframe after this time, wrap back to first
		*keyFrame2 = _keyframes.front();
		t2 = _parent->GetLength() + (*keyFrame2)->GetTime();

		// Use last keyframe as previous keyframe
		--i;
	}
	else
	{
		*keyFrame2 = *i;
		t2 = (*keyFrame2)->GetTime();

		// Find last keyframe before or on current time
		if (i != _keyframes.begin() && timePos < (*i)->GetTime())
		{
			--i;
		}
	}

	// Fill index of the first key
	if (firstKeyIndex)
	{
		*firstKeyIndex = static_cast<unsigned short>(std::distance(_keyframes.begin(), i));
	}

	*keyFrame1 = *i;

	t1 = (*keyFrame1)->GetTime();

	if (t1 == t2)
	{
		// Same KeyFrame (only one)
		return 0.0;
	}
	else
	{
		return (timePos - t1) / (t2 - t1);
	}
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::_CollectKeyFrameTimes(vector<Real>::type& keyFrameTimes)
{
	for (CinemaKeyFrameList::const_iterator i = _keyframes.begin(); i != _keyframes.end(); ++i)
	{
		Real timePos = (*i)->GetTime();

		vector<Real>::type::iterator it =
			std::lower_bound(keyFrameTimes.begin(), keyFrameTimes.end(), timePos);
		if ((it == keyFrameTimes.end()) || (*it != timePos))
		{
			keyFrameTimes.insert(it, timePos);
		}
	}
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::_BuildKeyFrameIndexMap(const Ogre::vector<Real>::type& keyFrameTimes)
{
	// Pre-allocate memory
	_keyFrameIndexMap.resize(keyFrameTimes.size() + 1);

	size_t i = 0, j = 0;
	while (j <= keyFrameTimes.size())
	{
		_keyFrameIndexMap[j] = static_cast<ushort>(i);
		while ((i < _keyframes.size()) && (_keyframes[i]->GetTime() <= keyFrameTimes[j]))
		{
			++i;
		}

		++j;
	}
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::Load( bool forceSynchronous /*= false*/ )
{
	if(_hasLoad || _derivedDataLoadingInProgress)
	{
		return;
	}

	_derivedDataLoadingInProgress = true;

	LoadTrackRequest req;
	req.track = this;
	_loadingID = Root::getSingleton().getWorkQueue()->addRequest(
		Cinema::LoadingWorkQueueChannel, WORKQUEUE_TRACK_LOADING_REQUEST, 
		Any(req), 0, forceSynchronous); 
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::CancelLoad()
{
	if(_loadingID != -1)
	{
		Root::getSingleton().getWorkQueue()->abortRequest(_loadingID);
		_loadingID = -1;
	}
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::_OnKeyFrameDataChanged()
{

}
//-----------------------------------------------------------------------------
CinemaKeyFrame* AbstractCinemaTrack::CreateKeyFrame(float timePos)
{
	CinemaKeyFrame* kf = CreateKeyFrameImpl(timePos);

	// Insert just before upper bound
	CinemaKeyFrameList::iterator i =
		std::upper_bound(_keyframes.begin(), _keyframes.end(), kf, KeyFrameTimeLess());
	_keyframes.insert(i, kf);

	_OnKeyFrameDataChanged();
	_parent->_OnKeyFrameListChanged();

	if(_needLoad)
	{
		_hasLoad = false;
	}

	return kf;
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::DestroyKeyFrame( CinemaKeyFrame* kf )
{
	CinemaKeyFrameList::iterator i = std::find(_keyframes.begin(), _keyframes.end(), kf);
	if(i != _keyframes.end())
	{
		OGRE_DELETE *i;

		_keyframes.erase(i);

		_OnKeyFrameDataChanged();
		_parent->_OnKeyFrameListChanged();
	}
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::DestroyKeyFrame( size_t idx )
{
	// If you hit this assert, then the keyframe index is out of bounds
	assert( idx < (ushort)_keyframes.size() );

	CinemaKeyFrameList::iterator i = _keyframes.begin();

	i += idx;

	OGRE_DELETE *i;

	_keyframes.erase(i);

	_OnKeyFrameDataChanged();
	_parent->_OnKeyFrameListChanged();
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::DestroyAllKeyFrames()
{
	CinemaKeyFrameList::iterator i = _keyframes.begin();

	for (; i != _keyframes.end(); ++i)
	{
		OGRE_DELETE *i;
	}

	_OnKeyFrameDataChanged();
	_parent->_OnKeyFrameListChanged();

	_keyframes.clear();
}
//-----------------------------------------------------------------------------
bool AbstractCinemaTrack::canHandleRequest( const WorkQueue::Request* req, const WorkQueue* srcQ )
{
	LoadTrackRequest ddr = any_cast<LoadTrackRequest>(req->getData());
	// only deal with own requests
	// we do this because if we delete a terrain we want any pending tasks to be discarded
	if (ddr.track != this)
		return false;
	else
		return RequestHandler::canHandleRequest(req, srcQ); 
}
//-----------------------------------------------------------------------------
WorkQueue::Response* AbstractCinemaTrack::handleRequest( 
	const WorkQueue::Request* req, const WorkQueue* srcQ )
{
	LoadTrackRequest ddr = any_cast<LoadTrackRequest>(req->getData());

	ddr.track->LoadImpl();

	LoadTrackResponse ddres; 
	WorkQueue::Response* response = OGRE_NEW WorkQueue::Response(req, true, Any(ddres));
	return response;
}
//-----------------------------------------------------------------------------
bool AbstractCinemaTrack::canHandleResponse( const WorkQueue::Response* res, const WorkQueue* srcQ )
{
	LoadTrackRequest ddreq = any_cast<LoadTrackRequest>(res->getRequest()->getData());
	// only deal with own requests
	// we do this because if we delete a terrain we want any pending tasks to be discarded
	if (ddreq.track != this)
		return false;
	else
		return true;
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::handleResponse( const WorkQueue::Response* res, const WorkQueue* srcQ )
{
	// Main thread
	_derivedDataLoadingInProgress = false;

	LoadTrackRequest ddreq = any_cast<LoadTrackRequest>(res->getRequest()->getData());

	ddreq.track->OnLoadCompleted();
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::WaitForDerivedProcesses()
{
	while (_derivedDataLoadingInProgress)
	{
		// we need to wait for this to finish
		OGRE_THREAD_SLEEP(50);
		Root::getSingleton().getWorkQueue()->processResponses();
	}
} 
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::LoadImpl()
{

}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::OnLoadCompleted()
{
	if(_needTarget)
	{
		_target = GameApp::getSingleton().GetGame()->GetGameObject(_targetObjectID);
	}

	_enable = true;
	_parent->OnTrackLoadCompleted(this);
}
//-----------------------------------------------------------------------------
bool AbstractCinemaTrack::NeedLoad() const
{
	return _needLoad;
}
//-----------------------------------------------------------------------------
bool AbstractCinemaTrack::HasLoad() const
{
	return _hasLoad;
}
//-----------------------------------------------------------------------------
IGameObject* AbstractCinemaTrack::GetTargetObjct()
{
	return _target;
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::SetTargetObjct( IGameObject* object )
{
	_target = object;
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::AddTip( const SKeyFrameTip& tip )
{
	_tipList.push_back(tip);
}
//-----------------------------------------------------------------------------
size_t AbstractCinemaTrack::GetTipCount() const
{
	return _tipList.size();
}
//-----------------------------------------------------------------------------
const SKeyFrameTip& AbstractCinemaTrack::GetTip( size_t idx )
{
	if(idx < _tipList.size())
	{
		return _tipList[idx];
	}

	OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
		"CinemaTrack tip index overflow", __FUNCTION__);
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::RemoveTip( size_t idx )
{
	if(idx < _tipList.size())
	{
		_tipList.erase(_tipList.begin() + idx);
	}
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::RemoveAllTip()
{
	_tipList.clear();
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::BuildPropertyMap()
{
	ICinemaTrack::BuildPropertyMap();

	AddProp(Property_String, Comment, AbstractCinemaTrack, "Comment", "Track");
	if(_needTarget)
	{
		//AddProp(Property_GameObject, TargetObjct, AbstractCinemaTrack, "TargetObjct", "Track");
		AddProp(Property_XGUID, TargetObjectID, AbstractCinemaTrack, "TargetObjctID", "Track");
	}
	AddProp(Property_Bool, Enable, AbstractCinemaTrack, "Enable", "Track");	
}
//-----------------------------------------------------------------------------
//CinemaKeyFrame* AbstractCinemaTrack::AutoSnapshot( float timePos )
//{
//	CinemaKeyFrame* kf = CreateKeyFrame(timePos);
//	AutoUpdateKeyFrame(kf);
//	return kf;
//}
//-----------------------------------------------------------------------------
String AbstractCinemaTrack::ToString() const
{
	IGameObject* obj = _target;
	if(_needTarget && _target)
	{
		String cap = formatString("%s [%s] [%s]", GetTypeName().c_str(),
			obj->GetTypeName().c_str(), _comment.c_str());
		return cap;
	}
	else
	{
		String cap = formatString("%s [%s]", GetTypeName().c_str(), _comment.c_str());
		return cap;
	}
}
//-----------------------------------------------------------------------------
const ICinemaTrack::KeyFrameTipList& AbstractCinemaTrack::GetTips() const
{
	return _tipList;
}
//-----------------------------------------------------------------------------
void AbstractCinemaTrack::SetTips( const ICinemaTrack::KeyFrameTipList& tips )
{
	_tipList = tips;
}
//-----------------------------------------------------------------------------
// CinemaTrackFactoryManager
//-----------------------------------------------------------------------------
template<> CinemaManager* Singleton<CinemaManager>::ms_Singleton = 0;

CinemaManager::CinemaManager()
: _camera(0)
{
	GameApp::getSingleton().AddUpdateable(this);
}
//-----------------------------------------------------------------------------
CinemaManager::~CinemaManager()
{

}
//-----------------------------------------------------------------------------
void CinemaManager::AddTrackFactory( const String& typeName,
										   ICinemaTrackFactory* factory,
										   bool overrideExist /*= true*/ )
{
	FactoryMap::iterator i = _factoryMap.find(typeName);
	if((i != _factoryMap.end()) && !overrideExist)
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
			"CinemaTrackFactory already exist:" + typeName, __FUNCTION__);
	}

	_factoryMap[typeName] = factory;
}
//-----------------------------------------------------------------------------
ICinemaTrackFactory* CinemaManager::GetTrackFactory( const String& typeName ) const
{
	FactoryMap::const_iterator i = _factoryMap.find(typeName);
	if(i == _factoryMap.end())
	{
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
			"CinemaTrackFactory not found:" + typeName, __FUNCTION__);
	}

	return i->second;
}
//-----------------------------------------------------------------------------
Cinema* CinemaManager::CreateCinema( const String& name, float timeLength )
{
	CinemaMap::iterator i = _cinemaMap.find(name);
	if(i != _cinemaMap.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
			"Cinema already exist:" + name, __FUNCTION__);
	}
	
	Cinema* c = OGRE_NEW Cinema(name, timeLength);
	_cinemaMap.insert(std::make_pair(name, c));
	return c;
}
//-----------------------------------------------------------------------------
Cinema* CinemaManager::GetCinema( const String& name )
{
	CinemaMap::iterator i = _cinemaMap.find(name);
	if(i != _cinemaMap.end())
	{
		return i->second;
	}

	return 0;
}
//-----------------------------------------------------------------------------
const CinemaManager::CinemaMap& CinemaManager::GetAllCinema() const
{
	return _cinemaMap;
}
//-----------------------------------------------------------------------------
void CinemaManager::DestroyCinema( const String& name )
{
	CinemaMap::iterator i = _cinemaMap.find(name);
	if(i != _cinemaMap.end())
	{
		OGRE_DELETE i->second;
		_cinemaMap.erase(i);
	}
}
//-----------------------------------------------------------------------------
void CinemaManager::DestroyCinema( Cinema* cinema )
{
	DestroyCinema(cinema->GetName());
}
//-----------------------------------------------------------------------------
void CinemaManager::DestroyAllCinema()
{
	CinemaMap::iterator i = _cinemaMap.begin();
	for (; i != _cinemaMap.end(); ++i)
	{
		OGRE_DELETE i->second;
	}

	_cinemaMap.clear();
}
//-----------------------------------------------------------------------------
void CinemaManager::LoadCinemas( const String& cinemaFile )
{
	XMLDoc doc;
	if(doc.loadFromFile(cinemaFile))
	{
		LoadCinemasFromXML(doc);
	}
}
//-----------------------------------------------------------------------------
void CinemaManager::LoadCinemasFromResourceSystem( const String& cinemaFile,
												  const String& resGrp)
{
	if(!ResourceGroupManager::getSingleton().resourceExists(resGrp, cinemaFile))
	{
		LogErrorIDf("Engine", "Cinema file not exist:[%s]%s",
			resGrp.c_str(), cinemaFile.c_str());
		return;
	}

	DataStreamPtr ds = ResourceGroupManager::getSingleton()
		.openResource(cinemaFile, resGrp);

	String str = ds->getAsString();
	LoadCinemasFromXMLString(str);
}
//-----------------------------------------------------------------------------
void CinemaManager::LoadCinemasFromXMLString( const String& cinemaFile )
{
	XMLDoc doc;
	doc.loadFromXML(cinemaFile);
	LoadCinemasFromXML(doc);
}
//-----------------------------------------------------------------------------
void CinemaManager::LoadCinemasFromXML( XMLDoc& doc )
{
	XMLElement cinemaNode = doc.getRoot().firstChild();
	while (!cinemaNode.isNull()) // Cinema
	{
		String cinemaName = cinemaNode.getAttribute("Name");
		float len = getFloatAttribuate("Length", cinemaNode);
		Cinema* cinema = CreateCinema(cinemaName, len);

		XMLElement trackNode = cinemaNode.firstChild();
		while (!trackNode.isNull()) // track
		{
			ICinemaTrack* track = cinema->CreateTrack(
				trackNode.getAttribute("TypeName"), 0);

			XMLElement trackChildNode = trackNode.firstChild();
			while(!trackChildNode.isNull())
			{
				if(trackChildNode.name() == "Property")
				{
					XMLElement propNode = trackChildNode.firstChild();
					while (!propNode.isNull())
					{
						String propName = propNode.name();
						String propVal = propNode.getAttribute("Value");
						track->SetProperty(propName, propVal);

						propNode = propNode.nextSibling();
					}
				}
				else if(trackChildNode.name() == "KeyFrames")
				{
					XMLElement kfNode = trackChildNode.firstChild();
					while (!kfNode.isNull())
					{
						float kfTime = getFloatAttribuate("Time", kfNode);
						String kfVal = kfNode.getAttribute("Value");
						CinemaKeyFrame* kf = track->CreateKeyFrame(kfTime);
						kf->FromString(kfVal);

						kfNode = kfNode.nextSibling();
					}
				}
				else if(trackChildNode.name() == "Tips")
				{
					XMLElement tipNode = trackChildNode.firstChild();
					while (!tipNode.isNull())
					{
						SKeyFrameTip tip;
						tip.Time = getFloatAttribuate("Time", tipNode);
						tip.TipText = tipNode.getAttribute("Text");
						track->AddTip(tip);

						tipNode = tipNode.nextSibling();
					}
				}

				trackChildNode = trackChildNode.nextSibling();
			}

			trackNode = trackNode.nextSibling();
		}

		cinema->EnableAll(false);

		cinemaNode = cinemaNode.nextSibling();
	}
}
//-----------------------------------------------------------------------------
void CinemaManager::SaveCinemas( const String& cinemaFile )
{
	XMLDoc doc;
	doc.createRootNode("Cinemas");
	XMLElement root = doc.getRoot();
	CinemaMap::const_iterator i = _cinemaMap.begin();
	for (; i != _cinemaMap.end(); ++i)
	{
		const Cinema* cinema = i->second;
		XMLElement cinemasNode = root.addChild("Cinema");
		cinemasNode.setAttribute("Name", i->first);
		cinemasNode.setAttribute("Length", StringConverter::toString(cinema->GetLength()));

		size_t cnt = cinema->GetTrackCount();
		for (size_t j = 0; j < cnt; ++j)
		{
			ICinemaTrack* t = cinema->GetTrack(j);
			String trackName = formatString("%s", t->GetTypeName().c_str());
			XMLElement trackNode = cinemasNode.addChild(trackName);
			trackNode.setAttribute("TypeName", t->GetTypeName());

			XMLElement propsNode = trackNode.addChild("Property");
			const PropertyMap& props = t->GetProperties();
			PropertyMap::const_iterator k = props.begin();
			for (; k != props.end(); ++k)
			{
				XMLElement propNode = propsNode.addChild(k->first);
				propNode.setAttribute("Value", k->second->ToString());
			}

			XMLElement kfsNode = trackNode.addChild("KeyFrames");
			for (size_t kfIdx = 0; kfIdx < t->GetKeyFrameCount(); ++kfIdx)
			{
				CinemaKeyFrame* keyFrame = t->GetKeyFrameByIndex(kfIdx);
				XMLElement kfNode = kfsNode.addChild("KeyFrame");
				kfNode.setAttribute("Time", StringConverter::toString(keyFrame->GetTime()));
				kfNode.setAttribute("Value", keyFrame->ToString());
			}

			XMLElement tipsNode = trackNode.addChild("Tips");
			size_t tipcnt = t->GetTipCount();
			for (size_t l = 0; l < tipcnt; ++l)
			{
				XMLElement tipNode = tipsNode.addChild("Tip");
				const SKeyFrameTip& tip = t->GetTip(l);
				tipNode.setAttribute("Time", StringConverter::toString(tip.Time));
				tipNode.setAttribute("Text", tip.TipText);
			}
		}
	}

	doc.save(cinemaFile);
}
//-----------------------------------------------------------------------------
bool CinemaManager::HasCinema() const
{
	return !_cinemaMap.empty();
}
//-----------------------------------------------------------------------------
const CinemaManager::FactoryMap& CinemaManager::GetAllTrackFactory() const
{
	return _factoryMap;
}
//-----------------------------------------------------------------------------
void CinemaManager::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(eventName == "SetMainCamera")
	{
		Ogre::Camera* cam = any_cast<Ogre::Camera*>(eventContex);
		_camera = cam;
	}
}
//-----------------------------------------------------------------------------
void CinemaManager::Update( float deltaTime )
{
	CinemaMap::iterator i = _cinemaMap.begin();
	for (; i != _cinemaMap.end(); ++i)
	{
		Cinema* c = i->second;
		if(c->IsPlaying())
		{
			c->AddTime(deltaTime);
			if(GameApp::CurrentRunType == ERT_Editor)
			{
				EventsManager::getSingleton().CreateEvent(
					"CinemaTimeChange", Any(c));
			}
		}
	}
}
//-----------------------------------------------------------------------------
// CinemaKeyFrame
//-----------------------------------------------------------------------------
CinemaKeyFrame::CinemaKeyFrame( ICinemaTrack* parent, float time )
: _time(time)
, _parent(parent)
{

}
//-----------------------------------------------------------------------------
CinemaKeyFrame::~CinemaKeyFrame()
{

}
//-----------------------------------------------------------------------------
String CinemaKeyFrame::ToString() const
{
	return "";	
}
//-----------------------------------------------------------------------------
void CinemaKeyFrame::FromString( const String& val )
{

}
//-----------------------------------------------------------------------------
