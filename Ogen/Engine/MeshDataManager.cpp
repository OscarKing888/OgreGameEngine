#include "stdafx.h"
#include "MeshDataManager.h"

template<> MeshDataManager* Singleton<MeshDataManager>::ms_Singleton = 0;



namespace MeshDataNode
{
	static String FrameEventNode = "FrameEvent";

	static String AttachPointNode = "AttachPoints";
	static String AttachPointNodeBone = "bone";
	static String AttachPointNodePosOffset = "positionOffset";
	static String AttachPointNodeScaleOffset = "scaleOffset";
	static String AttachPointNodeOrientationOffset = "orientationOffset";

	static String BoneGroupNode = "BoneGroup";
	static String BoneGroupNodeBoneList = "BoneList";

	static String EffectsGroupNode = "EffectsGroup";
	static String EffectsTemplateNameAttribute = "effectsTemplateName";
	static String EffectsAttachPointNameAttribute = "attachPointName";

	static String LinkedSkeletonList = "LinkedSkeletonList";
	static String LinkedSkeletonFile = "SkeletonFile";

	static String AnimationMoveDistance = "AnimationMoveDistance";
}

MeshData::MeshData()
: _enableSort(false)
{

}

MeshData::MeshData(ResourceManager* creator, const String& name, ResourceHandle handle,
				   const String& group, bool isManual, ManualResourceLoader* loader)
				   : Resource(creator, name, handle, group, isManual, loader)
				   , _enableSort(false)
				   , _is4Legs(false)
{

}

MeshData::~MeshData()
{

}

void MeshData::loadImpl(void)
{
	if(ResourceGroupManager::getSingleton().resourceExists(mGroup, mName))
	{
		DataStreamPtr ds = ResourceGroupManager::getSingleton().openResource(mName, mGroup);
		LoadFromDataStream(ds);
	}
}

void MeshData::unloadImpl(void)
{
	_animationFrameEventMap.clear();
	_attachPointList.clear();
	_boneGroupMap.clear();
	_linkedSkeletonList.clear();
	_enableSort = true;
	_moveDistMap.clear();
	_is4Legs = false;

	_leftFootBone.clear();
	_rightFootBone.clear();
	_leftBackFootBone.clear();
	_rightBackFootBone.clear();
}

size_t MeshData::calculateSize(void) const
{
	return 0;
}

void MeshData::LoadFromDataStream(DataStreamPtr ds)
{
	_enableSort = false;

	XMLDoc doc;
	String str = ds->getAsString();
	doc.loadFromXML(str);

	XMLElement root = doc.getRoot();
	XMLElement c = root.firstChild();
	while(!c.isNull())
	{
		String nodeName = c.name();
		if(nodeName == MeshDataNode::FrameEventNode) // FrameEventNode
		{
			XMLElement animationNode = c.firstChild();

			while(!animationNode.isNull())
			{
				XMLElement evtNode = animationNode.firstChild();
				while(!evtNode.isNull())
				{
					SFrameEvent evt;
					evt.EventName = evtNode.name();
					evt.AnimationTime = StringConverter::parseReal(evtNode.getAttribute("Time"));
					AddFrameEvent(animationNode.name(), evt);
					evtNode = evtNode.nextSibling();
				}

				animationNode = animationNode.nextSibling();
			}
		}
		else if(nodeName == MeshDataNode::AttachPointNode) // AttachPointNode
		{
			XMLElement e = c.firstChild();

			while(!e.isNull())
			{					
				SAttachPoint atp;
				atp.AttachPointName = e.getAttribute("name");
				atp.BoneName = e.getAttribute(MeshDataNode::AttachPointNodeBone);
				String str = e.getAttribute(MeshDataNode::AttachPointNodeOrientationOffset);
				atp.OrientationOffset = StringConverter::parseVector3(str);

				str = e.getAttribute(MeshDataNode::AttachPointNodePosOffset);
				atp.PositionOffset = StringConverter::parseVector3(str);

				str = e.getAttribute(MeshDataNode::AttachPointNodeScaleOffset);
				atp.ScaleOffset = StringConverter::parseVector3(str);

				_attachPointList.push_back(atp);

				e = e.nextSibling();
			}
		}
		else if(nodeName == MeshDataNode::BoneGroupNode) // BoneGroupNode
		{
			XMLElement e = c.firstChild();
			while(!e.isNull())
			{					
				String groupName = e.name();
				String val = e.getAttribute(MeshDataNode::BoneGroupNodeBoneList);
				StringVector l = StringUtil::split(val, "|");
				BoneHandleList blst;
				for (StringVector::iterator it = l.begin(); it != l.end(); ++it)
				{
					const String& s = *it;
					int boneHandle = StringConverter::parseInt(s);
					blst.push_back(boneHandle);
				}

				_boneGroupMap.insert(std::make_pair(groupName, blst));

				e = e.nextSibling();
			}
		}
		else if(nodeName == MeshDataNode::EffectsGroupNode) // EffectsGroupNode
		{
			XMLElement e = c.firstChild();
			while(!e.isNull())
			{					
				String groupName = e.name();
				EffectAttachPointPair effectPair;
				effectPair.AttachPointName = e.getAttribute(MeshDataNode::EffectsAttachPointNameAttribute);
				effectPair.EffectTemplateName =  e.getAttribute(MeshDataNode::EffectsTemplateNameAttribute);
				AddAttachEffect(groupName, effectPair);
				e = e.nextSibling();
			}
		}
		else if(nodeName == MeshDataNode::LinkedSkeletonList)
		{
			MeshPtr m = MeshPtr(MeshManager::getSingleton().createOrRetrieve(
				GetMeshFile(), ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).first);

			m->load();

			// import external skeleton animation export by oFusion
			OSMAnimSerializer sr;
			sr.importExternalAnimations(m.get(), GetMeshFile());


			XMLElement e = c.firstChild();
			while(!e.isNull())
			{					
				String skeletonFile = e.getAttribute("Value");
				_linkedSkeletonList.push_back(skeletonFile);

				SkeletonPtr sk = m->getSkeleton();
				sk->addLinkedSkeletonAnimationSource(skeletonFile);


				e = e.nextSibling();
			}
		}
		else if(nodeName == MeshDataNode::AnimationMoveDistance)
		{
			_is4Legs = getBoolAttribuate("is4Legs", c);
			_leftFootBone = c.getAttribute("leftFootBone");
			_rightFootBone = c.getAttribute("rightFootBone");
			_leftBackFootBone = c.getAttribute("leftBackFootBone");
			_rightBackFootBone = c.getAttribute("rightBackFootBone");

			XMLElement e = c.firstChild();
			while(!e.isNull())
			{
				String ani = e.getAttribute("Animation");
				float dist = getFloatAttribuate("Dist", e);
				_moveDistMap.insert(std::make_pair(ani, dist));

				e = e.nextSibling();
			}
		}

		c = c.nextSibling();
	}

	_enableSort = true;

	AnimationFrameEventMap::iterator it = _animationFrameEventMap.begin();
	for (; it != _animationFrameEventMap.end(); ++it)
	{
		Sort(it->second);
	}
}

/**
* <root>
* <FrameEvent>
*	<animationName>
*		<eventName>time</eventname>
* <AttachPoint>
*	<name bone= offset=/>
* <BoneGroup>
*	<groupName boneList/>
* <EffectsGroup>
*	<groupName effectTemplateName= attachPointName=/>
* <LinkedSkeletonList>
* <LinkedSkeletonFile Value="xxx.skeleton"/>
* </root>
*/
void MeshData::SaveToFile(const String& fileName) const
{

	XMLDoc doc;
	doc.createRootNode("root");

	XMLElement root = doc.getRoot();
	//----------------------------------------------------------------------------------------------
	// FrameEvent
	//----------------------------------------------------------------------------------------------

	XMLElement frameEvent = root.addChild(MeshDataNode::FrameEventNode);
	AnimationFrameEventMap::const_iterator it = _animationFrameEventMap.begin();
	for (; it != _animationFrameEventMap.end(); ++it)
	{
		if(!it->second.empty())
		{
			XMLElement animationNode = frameEvent.addChild(it->first);
			for (FrameEventList::const_iterator j = it->second.begin(); j != it->second.end(); ++j)
			{
				XMLElement e = animationNode.addChild(j->EventName);
				e.setAttribute("Time", StringConverter::toString(j->AnimationTime));
				//e.SetValue(StringConverter::toString(j->AnimationTime));
			}
		}
	}

	//----------------------------------------------------------------------------------------------
	// AttachPoint
	//----------------------------------------------------------------------------------------------

	XMLElement attachPoint = root.addChild(MeshDataNode::AttachPointNode);
	AttachPointList::const_iterator it2 = _attachPointList.begin();
	for (; it2 != _attachPointList.end(); ++it2)
	{
		const SAttachPoint& atp = *it2;
		XMLElement e = attachPoint.addChild("AttachPoint");
		e.setAttribute("name", atp.AttachPointName);
		e.setAttribute(MeshDataNode::AttachPointNodeBone, atp.BoneName);
		e.setAttribute(MeshDataNode::AttachPointNodePosOffset, StringConverter::toString(atp.PositionOffset));
		e.setAttribute(MeshDataNode::AttachPointNodeScaleOffset, StringConverter::toString(atp.ScaleOffset));
		e.setAttribute(MeshDataNode::AttachPointNodeOrientationOffset, StringConverter::toString(atp.OrientationOffset));
	}

	//----------------------------------------------------------------------------------------------
	// BoneGroup
	//----------------------------------------------------------------------------------------------

	XMLElement boneGroup = root.addChild(MeshDataNode::BoneGroupNode);
	BoneGroupMap::const_iterator it3 = _boneGroupMap.begin();
	for (; it3 != _boneGroupMap.end(); ++it3)
	{			
		XMLElement e = boneGroup.addChild(it3->first);
		String lst;

		for (BoneHandleList::const_iterator ci = it3->second.begin(); ci != it3->second.end(); ++ci)
		{
			lst += StringConverter::toString(*ci) + "|";
		}

		e.setAttribute(MeshDataNode::BoneGroupNodeBoneList, lst);
	}

	//----------------------------------------------------------------------------------------------
	// EffectsGroup
	//----------------------------------------------------------------------------------------------

	XMLElement effectsGroup = root.addChild(MeshDataNode::EffectsGroupNode);
	EffectsAttachPointMap::const_iterator it4 = _effectsMap.begin();
	for(; it4 != _effectsMap.end(); ++it4)
	{
		XMLElement e = effectsGroup.addChild(it4->first);
		e.setAttribute(MeshDataNode::EffectsAttachPointNameAttribute, it4->second.AttachPointName);
		e.setAttribute(MeshDataNode::EffectsTemplateNameAttribute, it4->second.EffectTemplateName);
	}

	//----------------------------------------------------------------------------------------------
	// LinkedSkeleton List
	//----------------------------------------------------------------------------------------------
	XMLElement linkedSkeletonList = root.addChild(MeshDataNode::LinkedSkeletonList);
	StringVector::const_iterator it5 = _linkedSkeletonList.begin();
	for(; it5 != _linkedSkeletonList.end(); ++it5)
	{
		XMLElement e = linkedSkeletonList.addChild(MeshDataNode::LinkedSkeletonFile);
		e.setAttribute("Value", *it5);
	}

	//----------------------------------------------------------------------------------------------
	// Animation move distance map
	//----------------------------------------------------------------------------------------------
	XMLElement animationMoveDist = root.addChild(MeshDataNode::AnimationMoveDistance);
	animationMoveDist.setAttribute("is4Legs", StringConverter::toString(_is4Legs));
	animationMoveDist.setAttribute("leftFootBone", _leftFootBone);
	animationMoveDist.setAttribute("rightFootBone", _rightFootBone);
	animationMoveDist.setAttribute("leftBackFootBone", _leftBackFootBone);
	animationMoveDist.setAttribute("rightBackFootBone", _rightBackFootBone);

	MoveDistMap::const_iterator it6 = _moveDistMap.begin(); 
	for (; it6 != _moveDistMap.end(); ++it6)
	{
		XMLElement e = animationMoveDist.addChild("Animation");
		e.setAttribute("Animation", it6->first);
		e.setAttribute("Dist", StringConverter::toString(it6->second));
	}

	doc.save(fileName);
}
//--------------------------------------------------------------------------------
struct FFindFrameEvent
{
	String EventName;
	bool operator()(const SFrameEvent& val)
	{
		return EventName == val.EventName;
	}
};
//--------------------------------------------------------------------------------
bool MeshData::HasFrameEvent(const String& animationName, const String& eventName) const
{
	if(animationName.empty())
	{
		return false;
	}

	AnimationFrameEventMap::const_iterator it =
		_animationFrameEventMap.find(animationName);
	if(it != _animationFrameEventMap.end())
	{
		FFindFrameEvent evt;
		evt.EventName = eventName;
		FrameEventList::const_iterator it2 =
			std::find_if(it->second.begin(), it->second.end(), evt);

		if(it2 != it->second.end())
		{
			return true;
		}
	}

	return false;
}
//--------------------------------------------------------------------------------
bool MeshData::HasFrameEvent(const String& animationName) const
{
	if(animationName.empty())
	{
		return false;
	}

	AnimationFrameEventMap::const_iterator it =
		_animationFrameEventMap.find(animationName);
	if(it != _animationFrameEventMap.end())
	{
		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------
MeshData::FrameEventList& MeshData::GetAnimationFrameEventList(const String& animationName)
{
	AnimationFrameEventMap::iterator it = _animationFrameEventMap.find(animationName);
	if(it != _animationFrameEventMap.end())
	{
		return it->second;
	}

	it = _animationFrameEventMap.insert(std::make_pair(animationName, FrameEventList())).first;
	return it->second;
}
//--------------------------------------------------------------------------------
void MeshData::AddFrameEvent(const String& animationName, const SFrameEvent& evt)
{
	bool findEvent = false;
	FrameEventList& lst = GetAnimationFrameEventList(animationName);
	for (FrameEventList::iterator it = lst.begin(); it != lst.end(); ++it)
	{
		SFrameEvent& frmEvt = *it;
		if(frmEvt.EventName == evt.EventName)
		{
			frmEvt.AnimationTime = evt.AnimationTime;
			findEvent = true;
			break;
		}
	}

	if(!findEvent)
	{
		lst.push_back(evt);
	}

	Sort(lst);		
}
//--------------------------------------------------------------------------------
void MeshData::RemoveFrameEvent(const String& animationName,
								const String& eventName)
{
	FrameEventList& lst = GetAnimationFrameEventList(animationName);
	for (FrameEventList::iterator it = lst.begin(); it != lst.end(); ++it)
	{
		SFrameEvent& frmEvt = *it;
		if(frmEvt.EventName == eventName)
		{
			lst.erase(it);
			return;
		}
	}
}
//--------------------------------------------------------------------------------
SFrameEvent* MeshData::GetFrameEvent(const String& animationName,
									 const String& eventName)
{
	AnimationFrameEventMap::iterator it =
		_animationFrameEventMap.find(animationName);
	if(it != _animationFrameEventMap.end())
	{
		FFindFrameEvent evt;
		evt.EventName = eventName;
		FrameEventList::iterator it2 =
			std::find_if(it->second.begin(), it->second.end(), evt);

		if(it2 != it->second.end())
		{
			return &(*it2);
		}
	}

	return 0;
}
//--------------------------------------------------------------------------------
struct FSortFrameEvent
{
	bool operator()(const SFrameEvent& lhs, const SFrameEvent& rhs) const
	{
		return lhs.AnimationTime < rhs.AnimationTime;
	}
};
//--------------------------------------------------------------------------------
void MeshData::Sort(FrameEventList& lst)
{
	if(_enableSort)
	{
		std::sort(lst.begin(), lst.end(), FSortFrameEvent());
	}
}
//--------------------------------------------------------------------------------
struct FFindAttachPoint
{
	String TargetName;

	FFindAttachPoint(const String& targetName)
		: TargetName(targetName)
	{
	}

	bool operator()(const SAttachPoint& cmp) const
	{
		return cmp.AttachPointName == TargetName;
	}
};
//--------------------------------------------------------------------------------
bool MeshData::HasAttachPoint(const String& attachPointName)
{
	FFindAttachPoint fun(attachPointName);
	AttachPointList::iterator i = std::find_if(_attachPointList.begin(), _attachPointList.end(), fun);
	return i != _attachPointList.end();
}
//--------------------------------------------------------------------------------
const SAttachPoint& MeshData::GetAttachPoint(const String& attachPointName)
{
	FFindAttachPoint fun(attachPointName);
	AttachPointList::iterator i = std::find_if(_attachPointList.begin(), _attachPointList.end(), fun);
	if(i != _attachPointList.end())
	{
		return *i;
	}

	LogErrorIDf("Engine", "Attach point not found:[%s]", attachPointName.c_str());
	static SAttachPoint atp;
	return atp;
}
//--------------------------------------------------------------------------------
void MeshData::AddAttchPoint(const SAttachPoint& atp)
{
	FFindAttachPoint fun(atp.AttachPointName);
	AttachPointList::iterator i = std::find_if(_attachPointList.begin(), _attachPointList.end(), fun);
	if(i != _attachPointList.end())
	{
		*i = atp;
	}
	else
	{
		_attachPointList.push_back(atp);
	}
}
//--------------------------------------------------------------------------------
void MeshData::RemoveAttachPoint(const String& attachPointName)
{
	FFindAttachPoint fun(attachPointName);
	AttachPointList::iterator i = std::find_if(_attachPointList.begin(), _attachPointList.end(), fun);
	if(i != _attachPointList.end())
	{
		_attachPointList.erase(i);
	}
}
//--------------------------------------------------------------------------------
const MeshData::BoneHandleList& MeshData::GetBoneGroup(const String& groupName) const
{
	BoneGroupMap::const_iterator it = _boneGroupMap.find(groupName);
	if(it != _boneGroupMap.end())
	{
		return it->second;
	}

	OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
		"Bone group not found:" + groupName, "MeshData::GetBoneGroup");
}
//--------------------------------------------------------------------------------
MeshData::BoneHandleList& MeshData::GetBoneGroup(const String& groupName)
{
	BoneGroupMap::iterator it = _boneGroupMap.find(groupName);
	if(it != _boneGroupMap.end())
	{
		return it->second;
	}

	OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
		"Bone group not found:" + groupName, "MeshData::GetBoneGroup by Reference");
}
//--------------------------------------------------------------------------------
bool MeshData::HasBoneGroup(const String& groupName) const
{
	BoneGroupMap::const_iterator it = _boneGroupMap.find(groupName);
	return it != _boneGroupMap.end();
}
//--------------------------------------------------------------------------------
void MeshData::AddBoneGroup(const String& groupName, const BoneHandleList& handleList)
{
	if(HasBoneGroup(groupName))
	{
		RemoveBoneGroup(groupName);
	}

	_boneGroupMap.insert(std::make_pair(groupName, handleList));
}
//--------------------------------------------------------------------------------
void MeshData::RemoveBoneGroup(const String& groupName)
{
	BoneGroupMap::iterator it = _boneGroupMap.find(groupName);
	if(it != _boneGroupMap.end())
	{
		_boneGroupMap.erase(it);
	}
}
//--------------------------------------------------------------------------------
void MeshData::AddAttachEffect(const String& groupName, const EffectAttachPointPair& effectPair)
{
	EffectsAttachPointMap::iterator it = _effectsMap.find(groupName);
	if(it == _effectsMap.end())
	{
		_effectsMap.insert(std::make_pair(groupName, effectPair));
	}
	else
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
			"Effect group already exist:" + groupName, "MeshData::AddAttachEffect");
	}
}
//--------------------------------------------------------------------------------
void MeshData::RemoveAttachEffect(const String& groupName)
{
	EffectsAttachPointMap::iterator it = _effectsMap.find(groupName);
	if(it != _effectsMap.end())
	{
		_effectsMap.erase(groupName);
	}
}
//--------------------------------------------------------------------------------
bool MeshData::HasAttachEffect(const String& groupName) const
{
	EffectsAttachPointMap::const_iterator it = _effectsMap.find(groupName);
	return it != _effectsMap.end();
}
//--------------------------------------------------------------------------------
const String& MeshData::GetAttachEffectAttachPoint(const String& groupName) const
{
	EffectsAttachPointMap::const_iterator it = _effectsMap.find(groupName);
	if(it != _effectsMap.end())
	{
		return it->second.AttachPointName;
	}
	else
	{
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
			"Effect group not found:" + groupName, "MeshData::GetAttachEffectAttachPoint");
	}
}
//--------------------------------------------------------------------------------
const String& MeshData::GetAttachEffectTemplate(const String& groupName) const
{
	EffectsAttachPointMap::const_iterator it = _effectsMap.find(groupName);
	if(it != _effectsMap.end())
	{
		return it->second.EffectTemplateName;
	}
	else
	{
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
			"Effect group not found:" + groupName, "MeshData::GetAttachEffectTemplate");
	}
}
//--------------------------------------------------------------------------------
void MeshData::AddLinkedSkeleton(const String& skeletonName)
{
	_linkedSkeletonList.push_back(skeletonName);

	//if(!SkeletonManager::getSingleton().resourceExists(skeletonName))
	//{
	//	SkeletonManager::getSingleton().load(skeletonName,
	//		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	//}

	MeshPtr m = MeshPtr(MeshManager::getSingleton().createOrRetrieve(
		GetMeshFile(), ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).first);
	SkeletonPtr sk = SkeletonPtr(SkeletonManager::getSingleton().createOrRetrieve(
		m->getSkeletonName(), ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).first); //m->getSkeleton();
	sk->addLinkedSkeletonAnimationSource(skeletonName);
}
//--------------------------------------------------------------------------------
void MeshData::RemoveLinkedSkeleton(size_t idx)
{
	if(idx < _linkedSkeletonList.size())
	{
		_linkedSkeletonList.erase(_linkedSkeletonList.begin() + idx);
	}
}
//--------------------------------------------------------------------------------
void MeshData::RemoveLinkedSkeleton(const String& skeletonName)
{
	StringVector::iterator it = std::find(_linkedSkeletonList.begin(),
		_linkedSkeletonList.end(), skeletonName);
	if(it != _linkedSkeletonList.end())
	{
		_linkedSkeletonList.erase(it);
	}
}
//--------------------------------------------------------------------------------
String MeshData::GetMeshFile() const
{
	String name = this->mName;
	name = name.substr(0, name.size() - MESHDATA_EXT.size()); // ".MeshData"
	return name + ".mesh";
}
//--------------------------------------------------------------------------------
float MeshData::GetAnimationMoveDistance(const String& aniName)
{
	MoveDistMap::iterator it = _moveDistMap.find(aniName);
	if(it != _moveDistMap.end())
	{
		return it->second;
	}

	return 0.0f;
}
//--------------------------------------------------------------------------------
void MeshData::SetAnimationMoveDistance(const String& aniName, float dist)
{
	_moveDistMap[aniName] = dist;
}
//--------------------------------------------------------------------------------
// MeshDataManager
//--------------------------------------------------------------------------------
MeshDataManager::MeshDataManager()
{
	// Resource type
	mResourceType = "MeshData";

	// Register with resource group manager
	ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}

MeshDataManager::~MeshDataManager()
{
	ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

Resource* MeshDataManager::createImpl(const String& name, ResourceHandle handle, 
									  const String& group, bool isManual, ManualResourceLoader* loader, 
									  const NameValuePairList* createParams)
{
	Resource* r = OGRE_NEW MeshData(this, name, handle, group, isManual, loader);
	r->touch();
	return r;
}

MeshDataPtr MeshDataManager::createOrRetrieve(const String& name)
{
	ResourceCreateOrRetrieveResult r = ResourceManager::createOrRetrieve(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	return r.first;
}

//-------------------------------------------------------------------------------------------------

OSMAnimSerializer::OSMAnimSerializer()
{
	mVersion = "[oFusion_Serializer_v1.0]";
}

void OSMAnimSerializer::addAnimation(Skeleton* skel, const String& fileName)
{
	SkeletonSerializer serializer;
	StringUtil::StrStreamType msg;
	msg << "Skeletal Animation: Loading " << fileName;
	LogDebugID("Engine", msg.str().c_str());

	DataStreamPtr stream = ResourceGroupManager::getSingleton().openResource(fileName);

	// Check header
	readFileHeader(stream);

	uint16 numBones;

	// Read number of bones needed for this animation
	readShorts(stream, &numBones, 1);

	if(numBones != skel->getNumBones())
	{

		OGRE_EXCEPT(Exception::ERR_RT_ASSERTION_FAILED,
			formatString("OSMAnimSerializer: Animation[%s][%d] not valid for skeleton[%s][%d], bones count not equal!",
			fileName.c_str(), numBones, skel->getName().c_str(), skel->getNumBones()),
			"OSMAnimSerializer::addAnimation");
	}

	// record the reading position

	size_t curPos = stream->tell();
	//readFileHeader(stream);
	unsigned short headerID = 0;
	readShorts(stream, &headerID, 1);
	String ver = readString(stream);

	unsigned short streamID = 0;
	streamID = readChunk(stream);
	String name;
	name = readString(stream);

	if(skel->hasAnimation(name))
	{
		LogInfoIDf("Engine", "[%s] already has the animation[%s], auto replaced!", skel->getName().c_str(), name.c_str());
		skel->removeAnimation(name);
	}

	// restore the reading position
	stream->seek(curPos);

	size_t size = (stream->size() - stream->tell());

	unsigned char* buffer = OGE_ALLOC_T(unsigned char, size);
	//unsigned char* buffer = OGRE_NEW unsigned char[size];

	stream->read(buffer, size);

	DataStreamPtr bufferStream(OGRE_NEW MemoryDataStream(buffer, size, true));

	serializer.importSkeleton(bufferStream, skel);
}	

void OSMAnimSerializer::importExternalAnimations(Mesh* pMesh, const String& meshFileName)
{
	StringVector lst = getAllExternalSkeletonAnimationList(meshFileName);
	for (size_t i = 0; i < lst.size(); ++i)
	{
		// 			try
		// 			{
		addAnimation(pMesh->getSkeleton().get(), lst[i]);
		// 			}
		// 			catch (const Exception& /*e*/)
		// 			{
		// 				//LogError(e.getDescription().c_str());
		// 			}
	}
}

StringVector OSMAnimSerializer::getAllExternalSkeletonAnimationList(const String& meshFileName)
{
	return getFileNameList(meshFileName, ".skani");	
}

ListFileReader::ListFileReader()
{

}

ListFileReader::~ListFileReader()
{

}

String ListFileReader::FormatTargetFileName(const String& meshFileName, const String& fileExt)
{
	String aniName = meshFileName + fileExt;
	return aniName;
}

StringVector ListFileReader::getFileNameList(const String& meshFileName, const String& fileExt)
{
	StringVector lst;
	String skeletonAnimationConfigFile = FormatTargetFileName(meshFileName, fileExt);
	if(	ResourceGroupManager::getSingleton().resourceExists(
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, skeletonAnimationConfigFile))
	{
		ConfigFile cfg;
		cfg.loadFromResourceSystem(skeletonAnimationConfigFile, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "\t=;");
		ConfigFile::SettingsIterator it = cfg.getSettingsIterator();
		// add the animation files
		while (it.hasMoreElements())
		{
			lst.push_back(it.peekNextKey());
			it.moveNext();
		}
	}
	else
	{
		LogErrorIDf("Engine", "Skeleton animation config file not exist:[%s]", skeletonAnimationConfigFile.c_str());
	}

	return lst;
}