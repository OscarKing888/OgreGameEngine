#pragma once
#include "AbstractEffectElement.h"

class EffectElement_Entity : public AbstractEffectElement
{
public:

	EffectElement_Entity(const String& name, ISpecialEffects* parent);
	virtual ~EffectElement_Entity(void);

	virtual void SetName(const String& name);

	virtual const String& GetTypeName() const;
	virtual bool IsAttachable() const{ return true; }

	virtual void SetMeshFile(const String& meshFile);
	virtual const String& GetMeshFile() const { return _meshFile; }

	virtual void SetPlayAnimationName(const String& aniName) { _playAnimationName = aniName; }
	virtual const String& GetPlayAnimationName() const { return _playAnimationName; }

	virtual void SetLoopAnimation(bool loop) { _loopAnimation = loop; }
	virtual bool GetLoopAnimation() const { return _loopAnimation; }

	virtual void SetStartPlayAnimationTime(float t) { _startPlayAnimation = t; }
	virtual float GetStartPlayAnimationTime() const { return _startPlayAnimation; }

	void SetSubEntityMaterial(size_t subEntityIdx, const String& matName);
	const String& GetSubEntityMaterial(size_t subEntityIdx);

	// composited string value  idx:materialName;idx:materialName
	void SetSubMaterials(const StringMap& idxAndMatName);
	StringMap GetSubMaterials() const;

	virtual void OnDestroy();

	virtual void Update(float deltaTime);

	virtual void Reset();

protected:

	virtual void OnStart();
	virtual void OnEnd();
	virtual void BuildPropertyMap();

	virtual void OnCreateEntity();
	virtual void OnDestroyEntity();
	virtual Node* GetNode() const;

	Entity*	_entity;
	String	_meshFile;

	typedef map<size_t, String>::type SubMaterialMap;
	SubMaterialMap	_materials;
	String	_playAnimationName;
	float			_startPlayAnimation;
	float			_playAnimationTime;
	bool			_animationPlayed;
	bool			_loopAnimation;
};

