#pragma once
#include "Trigger.h"
#include "IUpdateAble.h"

///
/// Teleport object in scene
///
class _EngineExport Teleport : public Trigger, public IUpdateAble
{
public:

	Teleport();
	virtual ~Teleport();

	const String& GetTargetSceneName() const { return _targetSceneName; }
	void SetTargetSceneName(const String& val) { _targetSceneName = val; }

	const Vector3& GetTargetPosition() const { return _targetPosition; }
	void SetTargetPosition(const Vector3& val) { _targetPosition = val; }

	virtual void SetMeshFile(const String& meshFile);
	virtual const String& GetMeshFile() const { return _meshFile; }

	virtual StringVector GetAllAnimations() const;
	// read only
	void SetAllAnimations(const StringVector&) { }

	virtual StringVector GetEnabledAnimations() const;
	virtual void SetEnabledAnimations(const StringVector& aniList);

	virtual const String& GetTypeName() const;

	virtual void Update(float deltaTime);

	virtual void SetVisibilityFlag(uint32 flag);
	
protected:

	String	_targetSceneName;
	Vector3	_targetPosition;
	Entity*	_entity;
	String	_meshFile;
	StringVector _enabledAnimations;

	virtual void BuildPropertyMap();

	virtual void PostCreateEntity();
	virtual void PreDestroyEntity();

	virtual String GetModelName();

	virtual void DestroyDisplayModel();
	virtual void CreateDisplayModel();

	virtual void Destroy();
};