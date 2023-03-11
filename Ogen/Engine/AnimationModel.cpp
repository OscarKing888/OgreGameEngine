// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************
#include "stdafx.h"
#include "AnimationModel.h"
#include "Property_Bool.h"
#include "GameApp.h"
#include "GameObjectFactoryManager.h"
#include "Property_StringVector.h"
#include <OgreTagPoint.h>
#include "MeshDataManager.h"

float AnimationModel::AnimationFadeSpeed = 7.5f;

class AnimationModelFactory : public TGameObjectFactory<AnimationModel>
{
};

M_REG_FACTORY(AnimationModel, AnimationModelFactory, VisualObject)
//------------------------------------------------------------------------------------
AnimationModel::AnimationModel(void)
: _enableUpdate(true)
, _animUpdateSpeed(1.0f)
, _meshData(0)
{
	GameApp::getSingleton().AddUpdateable(this);
}
//----------------------------------------------------------------------------------------------
AnimationModel::~AnimationModel(void)
{
}
//----------------------------------------------------------------------------------------------
void AnimationModel::BuildPropertyMap()
{
	StaticModel::BuildPropertyMap();
	AddProp(Property_Bool, UpdateEnabled, AnimationModel, "UpdateEnabled", "Animation");
	AddReadonlyProp(Property_StringVector, AllAnimations, AnimationModel, "AllAnimations", "Animation");
	AddProp(Property_StringVector, EnabledAnimations, AnimationModel, "EnabledAnimations", "Animation");
	AddStringPropWithSourceList(EnabledAnimation, AllAnimations, AnimationModel, "EnabledAnimation", "Animation");
	AddProp(Property_Float, AnimationUpdateSpeed, AnimationModel, "AnimationUpdateSpeed", "Animation");
}
//----------------------------------------------------------------------------------------------
void AnimationModel::SetEnabledAnimation(const String& aniName)
{
	StringVector v;
	v.push_back(aniName);
	SetEnabledAnimations(v);
}
//----------------------------------------------------------------------------------------------
const String& AnimationModel::GetEnabledTargetAnimation() const
{
	return _targetAnimation;
}
//----------------------------------------------------------------------------------------------
String AnimationModel::GetEnabledAnimation() const
{
	return _targetAnimation;
}
//----------------------------------------------------------------------------------------------
float AnimationModel::GetEnabledAnimationLength()
{
	String ani = GetEnabledAnimation();

	if(!_entity || ani.empty())
	{
		return 0.0f;
	}

	return _entity->getAnimationState(ani)->getLength();
}
//----------------------------------------------------------------------------------------------
void AnimationModel::Update(float deltaTime)
{
	if (_enableUpdate && _entity && (_entity->getAllAnimationStates() != 0))
	{
		FadeAnimations(deltaTime);

		ConstEnabledAnimationStateIterator asi =
			_entity->getAllAnimationStates()->getEnabledAnimationStateIterator();

		while (asi.hasMoreElements())
		{
			AnimationState* as = asi.peekNext();
			as->addTime(deltaTime * _animUpdateSpeed);
			OnAnimationAddTime(as->getAnimationName(), as->getTimePosition());

			if(as->hasEnded())
			{
				StopAnimation(as->getAnimationName());
			}

			asi.moveNext();
		}
	}
}
//----------------------------------------------------------------------------------------------
StringVector AnimationModel::GetEnabledAnimations() const
{
#if 1
	StringVector lst;
	if(_entity)
	{
		if(_entity->getAllAnimationStates())
		{
			ConstEnabledAnimationStateIterator asi =
				_entity->getAllAnimationStates()->getEnabledAnimationStateIterator();
			while (asi.hasMoreElements())
			{
				AnimationState* as = asi.peekNext();
				lst.push_back(as->getAnimationName());
				asi.moveNext();
			}
		}
	}

	return lst;
#else
	return _enabledAnimations;
#endif
}
//----------------------------------------------------------------------------------------------
void AnimationModel::Destroy()
{
	DeatchAllObject();

	StaticModel::Destroy();
	GameApp::getSingleton().RemoveUpdateable(this);
}
//----------------------------------------------------------------------------------------------
AnimationState* AnimationModel::GetAnimationState(const String& aniName)
{
	if(!_entity)
	{
		return 0;
	}

	return _entity->getAnimationState(aniName);
}
//----------------------------------------------------------------------------------------------
void AnimationModel::PlayAnimation(const String& aniName, float duration)
{
	_targetAnimation = aniName;

	if(!_entity)
	{
		_enabledAnimations.push_back(aniName);
		return;
	}

	if(!_entity->getAllAnimationStates())
	{
		LogErrorIDf("Engine", "[AnimationModel::PlayAnimation] [%s] has no animations!", GetGUID().toString().c_str());
		return;
	}

	if(HasAnimation(aniName))
	{
		StringVector anis = GetEnabledAnimations();
		
		AnimationState* a = _entity->getAnimationState(aniName);
		a->setEnabled(true);
		a->setLoop(true);
		_enabledAnimations.push_back(aniName);
		_fadeInList.push_back(a);
		OnAnimationBegin(aniName);
	}
	else
	{
		LogErrorIDf("Engine", "[AnimationModel::PlayAnimation] Animation [%s] not found for[%s]", aniName.c_str(), GetGUID().toString().c_str());
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::StopAnimation(const String& aniName)
{
	if(HasAnimation(aniName))
	{
		AnimationState* a = _entity->getAnimationState(aniName);
		//a->setEnabled(false);
		_fadeOutList.push_back(a);
		OnAnimationEnd(aniName);
	}
	else
	{
		LogErrorIDf("Engine", "[AnimationModel::StopAnimation] Animation [%s] not found for[%s]", aniName.c_str(), GetGUID().toString().c_str());
	}

	_targetAnimation.clear();
	_enabledAnimations.clear();
}
//----------------------------------------------------------------------------------------------
bool AnimationModel::HasAnimation(const String& aniName) const
{
	if(!_entity)
	{
		return false;
	}

	if(!_entity->getAllAnimationStates())
	{
		return false;
	}

	return _entity->getAllAnimationStates()->hasAnimationState(aniName);
}
//----------------------------------------------------------------------------------------------
void AnimationModel::StopAllAnimation()
{
	for (size_t i = 0; i < _enabledAnimations.size(); ++i)
	{
		StopAnimation(_enabledAnimations[i]);
	}	
	/*ConstEnabledAnimationStateIterator asi =
		_entity->getAllAnimationStates()->getEnabledAnimationStateIterator();
	while (asi.hasMoreElements())
	{
		asi.peekNext()->setEnabled(false);
		asi.moveNext();
	}*/
	_targetAnimation.clear();
	_enabledAnimations.clear();
}
//----------------------------------------------------------------------------------------------
void AnimationModel::ResetAnimation(const String& aniName)
{
	GetAnimationState(aniName)->setTimePosition(0.0f);
}
//----------------------------------------------------------------------------------------------
void AnimationModel::ResetAllAnimation()
{
	AnimationStateIterator asi = _entity->getAllAnimationStates()->getAnimationStateIterator();

	while (asi.hasMoreElements())
	{
		asi.peekNextValue()->setTimePosition(0.0f);
		asi.moveNext();
	}
}
//----------------------------------------------------------------------------------------------
SkeletonInstance* AnimationModel::GetSkeletonInstance()
{
	return _entity->getSkeleton();
}
//----------------------------------------------------------------------------------------------
void AnimationModel::PreDestroyEntity()
{
	DestroyAttachEffects();
	_allAnimations.clear();
	StaticModel::PreDestroyEntity();
}
//----------------------------------------------------------------------------------------------
void AnimationModel::PostCreateEntity()
{
	StaticModel::PostCreateEntity();

	_allAnimations = GetAnimations(_entity);	

	if(!_enabledAnimations.empty())
	{
		SetEnabledAnimations(_enabledAnimations);
	}

	if(_entity)
	{
		_entity->setQueryFlags(GetQueryFlag());

		if(!_meshFile.empty())
		{
			CreateAttachEffects();
		}
	}

#if defined(_DEBUG) && (!defined(_PRODUCT_) && 0)
	SceneNode* pN = GetNode(); 
	Entity* pE = pN->getCreator()->createEntity(name + (".DebugPos"), (M_DEBUG_MESH_FILE));
	pN->attachObject(pE);
#endif
	if(HasSkeleton())
	{
#if 0
		String skName = GetSkeletonName();
		String blendFileName = skName + ".blend";
		if(ResourceGroupManager::getSingleton()
			.resourceExists(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, blendFileName))
		{
			Animations::SkeletonBlendingConfigManager::getSingleton().LoadFromResourceSystem(blendFileName);
		}
#endif
	}

	CacheBoneBlendingInfo();
}
//----------------------------------------------------------------------------------------------
StringVector AnimationModel::GetAnimations(Entity* e)
{
	StringVector anis;
	if(e->getAllAnimationStates())
	{
		AnimationStateIterator asi = e->getAllAnimationStates()->getAnimationStateIterator();

		while (asi.hasMoreElements())
		{
			anis.push_back(asi.peekNextKey());
			asi.moveNext();
		}
	}

	return anis;
}
//----------------------------------------------------------------------------------------------
void AnimationModel::SetEnabledAnimations(const StringVector& aniList)
{
	StopAllAnimation();

	if(_entity)
	{
		for (size_t i = 0; i < aniList.size(); ++i)
		{
			PlayAnimation(aniList[i]);

			if(i == 0)
			{
				_targetAnimation = aniList[i];
			}
		}
	}

	_enabledAnimations = aniList;	
}
//----------------------------------------------------------------------------------------------
const String& AnimationModel::GetSkeletonName()
{
	return _entity->getSkeleton()->getName();
}
//----------------------------------------------------------------------------------------------
bool AnimationModel::HasSkeleton() const
{
	if(_entity)
	{
		return _entity->hasSkeleton();
	}

	return false;
}
//----------------------------------------------------------------------------------------------
const String& AnimationModel::GetTypeName() const
{
	static String sName("AnimationModel");
	return sName;
}
//----------------------------------------------------------------------------------------------
const StringVector& AnimationModel::GetAllAnimations() const
{
	return _allAnimations;
}
//----------------------------------------------------------------------------------------------
void AnimationModel::FadeAnimations( float deltaTime )
{
	///
	/// Fade in animations
	///
	EnabledAnimationStateList::iterator i = _fadeInList.begin();
	while (i != _fadeInList.end())
	{
		AnimationState* as = *i;
		float newWeight = as->getWeight() + deltaTime * AnimationFadeSpeed;
		as->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
		if (newWeight >= 1)
		{
			i = _fadeInList.erase(i);
		}
		else
		{
			++i;
		}
	}

	///
	/// Fade out animations
	///
	i = _fadeOutList.begin();
	while (i != _fadeOutList.end())
	{
		AnimationState* as = *i;
		float newWeight = as->getWeight() - deltaTime * AnimationFadeSpeed;
		as->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
		if (newWeight <= 0)
		{
			i = _fadeOutList.erase(i);
			as->setEnabled(false);
		}
		else
		{
			++i;
		}
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::PreCreateEntity()
{
	StaticModel::PreCreateEntity();

	if(!_meshFile.empty())
	{
		String baseName, extName;
		StringUtil::splitBaseFilename(_meshFile, baseName, extName);			
		_meshData = MeshDataManager::getSingleton().createOrRetrieve(baseName + MESHDATA_EXT).get();
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::DeatchAllObject()
{
	AttatchMap m = _attatchMap;
	AttatchMap::iterator i = m.begin();
	for (; i != m.end(); ++i)
	{
		DetachObject(i->first);
	}
	m.clear();
}
//----------------------------------------------------------------------------------------------
void AnimationModel::CacheBoneBlendingInfo()
{
	if(HasSkeleton())
	{
		//SkeletonBlendingConfigManager::getSingleton().CacheToBoneIndex(GetSkeletonInstance());
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::AddAnimationStateObserver(IAnimationStateObserver* pO)
{
	AnimationStateObserverList::iterator i =
		std::find(_animationStateObservers.begin(),
		_animationStateObservers.end(), pO);

	if(i == _animationStateObservers.end())
	{
		_animationStateObservers.push_back(pO);
	}
	else
	{
		assert(false);
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::RemoveAnimationStateObserver(IAnimationStateObserver* pO)
{
	AnimationStateObserverList::iterator i =
		std::find(_animationStateObservers.begin(),
		_animationStateObservers.end(), pO);

	if(i != _animationStateObservers.end())
	{
		_animationStateObservers.erase(i);
	}
	else
	{
		//assert(false);
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::NotifyAnimationEnded(const String& animationName)
{
	for(size_t i = 0; i < _animationStateObservers.size(); ++i)
	{
		_animationStateObservers[i]->OnAnimationEnded(this, animationName);
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::NotifyAnimationBegin(const String& animationName)
{
	for(size_t i = 0; i < _animationStateObservers.size(); ++i)
	{
		_animationStateObservers[i]->OnAnimationBegin(this, animationName);
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::NotifyAnimationEvent(const String& animationName, const String& eventName)
{
	LogDebugIDf("Editor", "%s -> %s", animationName.c_str(), eventName.c_str());
	for(size_t i = 0; i < _animationStateObservers.size(); ++i)
	{
		_animationStateObservers[i]->OnAnimationEvents(this, animationName, eventName);
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::AddSkeletonAnimation(const String& fileName)
{
	OSMAnimSerializer sr;
	sr.addAnimation(this->GetSkeletonInstance(), fileName);
}
//----------------------------------------------------------------------------------------------
StringVector AnimationModel::GetAllExternalSkeletonAnimationList(const String& meshFileName)
{
	OSMAnimSerializer osm;
	return osm.getAllExternalSkeletonAnimationList(meshFileName);
}
//----------------------------------------------------------------------------------------------
void AnimationModel::LoadAnimationCategoryConfig(const String& meshFileName)
{
	String cfgFile = meshFileName + ".alst";
	if (ResourceGroupManager::getSingleton().resourceExists(
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, cfgFile))
	{
		ConfigFile cfg;
		cfg.loadFromResourceSystem(cfgFile,
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "\t=;");

		ConfigFile::SectionIterator sit = cfg.getSectionIterator();
		while  (sit.hasMoreElements())
		{
			String secName = sit.peekNextKey();
			if(secName.empty())
			{
				sit.moveNext();
				continue;
			}

			LogDebugIDf("GameLogic", "Actor [%s] [%s]", GetGUID().toString().c_str(), secName.c_str());

			ConfigFile::SettingsMultiMap* pMap = sit.peekNextValue();
			ConfigFile::SettingsMultiMap::iterator it = pMap->begin();
			StringVector lst;
			while (it != pMap->end())
			{
				lst.push_back(it->first);
				LogDebugIDf("GameLogic", "\t\t[%s]", it->first.c_str());
				++it;
			}

			AnimationIndexListPair p;
			p.first = 0;
			p.second = lst;
			_animationCategoryMap.insert(std::make_pair(secName, p));

			sit.moveNext();
		}
	}
}
//----------------------------------------------------------------------------------------------
bool AnimationModel::HasAttatchPoint(const String& pointName) const
{
	if(_meshData)
	{
		return _meshData->HasAttachPoint(pointName);
	}

	return false;
}
//----------------------------------------------------------------------------------------------
StringVector AnimationModel::GetAttatchPoints() const
{
	StringVector lst;
	if(_meshData)
	{
		const MeshData::AttachPointList& atpList = _meshData->GetAttachPointList();

		MeshData::AttachPointList::const_iterator i = atpList.begin();
		for (; i != atpList.end(); ++i)
		{
			lst.push_back(i->AttachPointName);
		}
	}

	return lst;
}
//----------------------------------------------------------------------------------------------
bool AnimationModel::AttatchObject(MovableObject* object, const String& attatchPointName,
						  const Vector3& offsetPos,
						  const Quaternion& offsetOrientation)
{
	if(!HasAttatchPoint(attatchPointName))
	{
		LogErrorIDf("GameLogic", "Attatch point [%s] not found!", attatchPointName.c_str());
		return false;
	}

	if(!HasSkeleton())
	{
		LogErrorIDf("GameLogic", 
			"Actor[%s] doesn't have skeleton, mesh file[%s].", _guid.toString().c_str(), _meshFile.c_str());
		return false;
	}

	Entity* pE = this->GetEntity();
	//SkeletonInstance* pSI = pE->getSkeleton();

	Bone* pBone = GetAttatchPointBone(attatchPointName);

	TagPointAndParent tPAndPar;
	tPAndPar.tagPoint = 0;
	tPAndPar.parent = object->getParentSceneNode();
	if (tPAndPar.parent != NULL)
	{
		tPAndPar.parent->detachObject(object);
	}

	tPAndPar.tagPoint = pE->attachObjectToBone(pBone->getName(), object);
	_attatchMap.insert(std::make_pair(object, tPAndPar));

	tPAndPar.tagPoint->setOrientation(offsetOrientation);
	//printf("%s", StringConverter::toString(offsetOrientation).c_str());
	tPAndPar.tagPoint->setPosition(offsetPos);

	return true;
}
//----------------------------------------------------------------------------------------------
void AnimationModel::DetachObject(MovableObject* object)
{
	Entity* pE = this->GetEntity();
	pE->detachObjectFromBone(object);
	AttatchMap::iterator i = _attatchMap.find(object);
	if(i != _attatchMap.end())
	{
		_attatchMap.erase(i);
	}
	else
	{
		//assert(false);
	}
}
//----------------------------------------------------------------------------------------------
bool AnimationModel::GetIfObjectAttached(MovableObject* object)
{
	AttatchMap::iterator i = _attatchMap.find(object);
	if(i != _attatchMap.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}
//----------------------------------------------------------------------------------------------
Bone* AnimationModel::GetAttatchPointBone(const String& attatchPointName)
{
	String boneName;
	if(_meshData)
	{
		boneName = _meshData->GetAttachPoint(attatchPointName).BoneName;
	}

	Bone* b = 0;

	if(!boneName.empty())
	{
		b = GetSkeletonInstance()->getBone(boneName);
	}

	if(!b)
	{
		b = GetSkeletonInstance()->getBone(attatchPointName);
	}

	assert((b != 0) && "[AnimationModel::GetAttatchPointBone] bone name not found!");

	return b;
}
//----------------------------------------------------------------------------------------------
void AnimationModel::AttachEffectObject(MovableObject* obj,
							   const String& attatchPointName,
							   const Vector3& offsetPos,
							   const Quaternion& offsetOrientation)
{
	AttatchObject(obj, attatchPointName, offsetPos, offsetOrientation);
}
//----------------------------------------------------------------------------------------------
void AnimationModel::DetachEffectObject(MovableObject* object)
{
	DetachObject(object);
}
//----------------------------------------------------------------------------------------------
void AnimationModel::OnAnimationBegin(const String& animation)
{
	_frameEventMap[animation] = FrameEventStatus();

	NotifyAnimationBegin(animation);
}
//----------------------------------------------------------------------------------------------
void AnimationModel::OnAnimationEnd(const String& animation)
{
	ResetFrameEventPos(animation);
	NotifyAnimationEnded(animation);
}
//----------------------------------------------------------------------------------------------
void AnimationModel::OnAnimationAddTime(const String& animation, float timePosition)
{
	if(_meshData)
	{		
		UpdateFrameEventPos(animation, timePosition);
		// 		const MeshData::FrameEventList& evtLst =
		// 			_meshData->GetAnimationFrameEventList(animation);
		// 
		// 		if(!evtLst.empty())
		// 		{
		// 			MeshData::FrameEventList::const_iterator i = evtLst.begin();
		// 			for (; i != evtLst.end(); ++i)
		// 			{
		// 				const SFrameEvent& fevt = *i;
		// 				NotifyAnimationEvent(animation, fevt.EventName);
		// 			}
		// 		}
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::ResetFrameEventPos(const String& animation)
{
	FrameEventMap::iterator it = _frameEventMap.find(animation);
	if(it != _frameEventMap.end())
	{
		it->second.CurrentFrameEventIndex = 0;
		it->second.Notified = false;
		it->second.LastTimePosition = 0.0f;
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::UpdateFrameEventPos(const String& animation, float timePosition)
{
	if(!_meshData)
	{
		return;
	}

	FrameEventMap::iterator it = _frameEventMap.find(animation);
	if(it != _frameEventMap.end())
	{
		int& idx = it->second.CurrentFrameEventIndex;

		MeshData::FrameEventList& frmLst = _meshData->GetAnimationFrameEventList(animation);

		if((idx < (int)frmLst.size()) && !it->second.Notified)
		{
			SFrameEvent* evt = &frmLst[idx];
			if(evt != 0)
			{
				if(timePosition >= evt->AnimationTime)
				{
					NotifyAnimationEvent(animation, evt->EventName);
					it->second.Notified = true;
					it->second.LastTimePosition = timePosition;

					// move to next frame event
					if(idx + 1 < (int)frmLst.size())
					{
						++idx;
						it->second.Notified = false;
					}
				}
			}
		}
		else if(timePosition < it->second.LastTimePosition)
		{
			ResetFrameEventPos(animation);
		}
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::ClearAllFrameEventPos()
{
	_frameEventMap.clear();
}
//----------------------------------------------------------------------------------------------
void AnimationModel::CreateAttachEffects()
{
	if(!_meshData)
	{
		return;
	}

	const MeshData::EffectsAttachPointMap& effectsMap = _meshData->GetAttachEffectsMap();
	MeshData::EffectsAttachPointMap::const_iterator it = effectsMap.begin();
	for (; it != effectsMap.end(); ++it)
	{
		if(ParticleSystemManager::getSingleton().getTemplate(it->second.EffectTemplateName))
		{
			SceneManager* sm = _node->getCreator();
			ParticleSystem* effects = sm->createParticleSystem(
				GetGUID().toString() + it->first, it->second.EffectTemplateName);
			effects->setVisibilityFlags(GetVisibilityFlag());
			AnimationModel::AttachEffectObject(effects, it->second.AttachPointName);
			_effectMap.insert(std::make_pair(it->first, effects));
		}
		else
		{
			LogErrorIDf("Engine", "[%s] particle template[%s] not exist!",
				__FUNCTION__, it->second.EffectTemplateName.c_str());
		}
	}
}
//----------------------------------------------------------------------------------------------
void AnimationModel::DestroyAttachEffects()
{
	ParticleEffectsMap::iterator i = _effectMap.begin();
	for(; i != _effectMap.end(); ++i)
	{
		DetachObject(i->second);
		_node->getCreator()->destroyParticleSystem(i->second);
	}

	_effectMap.clear();
}
//----------------------------------------------------------------------------------------------
ParticleSystem* AnimationModel::GetAttachEffects(const String& effectGroupName) const
{
	ParticleEffectsMap::const_iterator it = _effectMap.find(effectGroupName);
	if(it != _effectMap.end())
	{
		return it->second;
	}

	return 0;
}

