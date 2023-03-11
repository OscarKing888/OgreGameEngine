// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************

#pragma once
#include "StaticModel.h"
#include "AnimationModel.h"
#include "IUpdateAble.h"

class MeshData;
class AnimationModel;

/**
*	动画状态监听接口
*/
class IAnimationStateObserver
{
public:

	/**	一个动画开始 */
	virtual void OnAnimationBegin(AnimationModel* actor, const String& animationName){}

	/**	动画结束 */
	virtual void OnAnimationEnded(AnimationModel* actor, const String& animationName){}

	/** Animation events notify */
	virtual void OnAnimationEvents(AnimationModel* actor, const String& animationName, const String& eventName){}
};


class _EngineExport AnimationModel : public StaticModel, public IUpdateAble
{
public:

	AnimationModel(void);
	virtual ~AnimationModel(void);

	virtual const String& GetTypeName() const;

	virtual const StringVector& GetAllAnimations() const;
	// read only
	void SetAllAnimations(const StringVector&) { }

	virtual StringVector GetEnabledAnimations() const;
	virtual void SetEnabledAnimations(const StringVector& aniList);

	virtual void SetEnabledAnimation(const String& aniName);
	virtual String GetEnabledAnimation() const;
	float GetEnabledAnimationLength();

	virtual AnimationState* GetAnimationState(const String& aniName);
	virtual void PlayAnimation(const String& aniName, float duration = 0.2f);
	virtual void StopAnimation(const String& aniName);
	virtual bool HasAnimation(const String& aniName) const;
	virtual void StopAllAnimation();
	virtual void ResetAnimation(const String& aniName);
	virtual void ResetAllAnimation();
	virtual SkeletonInstance* GetSkeletonInstance();
	const String& GetEnabledTargetAnimation() const;

	virtual const String& GetSkeletonName();
	virtual bool HasSkeleton() const;

	virtual bool GetUpdateEnabled() const { return _enableUpdate; }
	virtual void SetUpdateEnabled(bool on) { _enableUpdate = on; }

	float GetAnimationUpdateSpeed() const { return _animUpdateSpeed; }
	void SetAnimationUpdateSpeed(float r) { _animUpdateSpeed = r; }

	static StringVector GetAnimations(Entity* e);


	/// Global fade in/out speed
	static float AnimationFadeSpeed;

	/**	取得外部分组动画名列表,传入模型文件名,通过.skani配置的文件列表 */
	static StringVector GetAllExternalSkeletonAnimationList(const String& meshFileName);

	/**	取得动画分类配置,通过.alst配置的文件列表 */
	void LoadAnimationCategoryConfig(const String& meshFileName);

	/** 是否有某个骨头绑定点 */
	virtual bool HasAttatchPoint(const String& pointName) const;

	virtual StringVector GetAttatchPoints() const;

	void SetAttatchPoints(const StringVector& ){}

	/** 将一个对象挂定到相应绑定点上，绑定成功返回true*/
	virtual bool AttatchObject(MovableObject* object, const String& attatchPointName,
		const Vector3& offsetPos = Vector3::ZERO, const Quaternion& offsetOrientation = Quaternion::IDENTITY);

	virtual void DetachObject(MovableObject* object);

	virtual void AddAnimationStateObserver(IAnimationStateObserver* pO);
	virtual void RemoveAnimationStateObserver(IAnimationStateObserver* pO);

	void AddSkeletonAnimation(const String& fileName);
	bool GetIfObjectAttached(MovableObject* object);

	struct TagPointAndParent
	{
		TagPoint* tagPoint;
		SceneNode* parent;
	};

	void AttachEffectObject(MovableObject* obj, const String& attatchPointName,
		const Vector3& offsetPos = Vector3::ZERO, const Quaternion& offsetOrientation = Quaternion::IDENTITY);
	void DetachEffectObject(MovableObject* object);

	Bone* GetAttatchPointBone(const String& attatchPointName);

	virtual void Update(float deltaTime);

	void DeatchAllObject();

	ParticleSystem* GetAttachEffects(const String& effectGroupName) const;
	virtual void CreateAttachEffects();
	virtual void DestroyAttachEffects();

	MeshData* GetMeshData() { return _meshData; }
	virtual void BuildPropertyMap();

protected:

	virtual void FadeAnimations(float deltaTime);
	virtual void Destroy();

	virtual void PreDestroyEntity();
	virtual void PreCreateEntity();
	virtual void PostCreateEntity();

	bool _enableUpdate;
	StringVector	_allAnimations;
	StringVector	_enabledAnimations;
	float _animUpdateSpeed;
	String _targetAnimation;
	EnabledAnimationStateList _fadeInList;
	EnabledAnimationStateList _fadeOutList;

	typedef Ogre::map<MovableObject*, TagPointAndParent>::type AttatchMap;
	AttatchMap							_attatchMap;

	typedef Ogre::vector<IAnimationStateObserver*>::type	AnimationStateObserverList;
	AnimationStateObserverList			_animationStateObservers;

	typedef std::pair<int, StringVector> AnimationIndexListPair;
	typedef std::map<String, AnimationIndexListPair> AnimationCategoryMap;
	AnimationCategoryMap	_animationCategoryMap;

	MeshData* _meshData;

	struct FrameEventStatus
	{
		int CurrentFrameEventIndex;
		bool Notified;
		float LastTimePosition;

		FrameEventStatus()
			: CurrentFrameEventIndex(0)
			, Notified(false)
			, LastTimePosition(0.0f)
		{
		}
	};

	typedef Ogre::map<String, FrameEventStatus>::type FrameEventMap;
	FrameEventMap _frameEventMap;

	void ResetFrameEventPos(const String& animation);
	void ClearAllFrameEventPos();
	void UpdateFrameEventPos(const String& animation, float timePosition);

	virtual void OnAnimationBegin(const String& animation);
	virtual void OnAnimationEnd(const String& animation);
	virtual void OnAnimationAddTime(const String& animation, float timePosition);

	void NotifyAnimationBegin(const String& animationName);
	void NotifyAnimationEnded(const String& animationName);
	void NotifyAnimationEvent(const String& animationName, const String& eventName);

	virtual void CacheBoneBlendingInfo();

	typedef std::map<String, ParticleSystem*> ParticleEffectsMap;
	ParticleEffectsMap _effectMap;
};