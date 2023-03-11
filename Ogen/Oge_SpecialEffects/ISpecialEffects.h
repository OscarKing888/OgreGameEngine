#pragma once
#include "OgeSpecialEffectsPrerequisites.h"

class ISpecialEffects;
class IEffectsElement;

typedef vector<IEffectsElement*>::type EffectElementList;

//--------------------------------------------------------------------------------
class _OgeSFXExport IEffectsElement
	: public PropertyObject
	, public IUpdateAble
	, public OgeAlloc
{
public:

	virtual const String& GetTypeName() const = 0;

	virtual const String& GetName() const = 0;
	virtual void SetName(const String& name) = 0;

	virtual void Reset() = 0;
	virtual bool IsEnd() const = 0;

	virtual const Vector3& GetPositionOffset() const = 0;
	virtual void SetPositionOffset(const Vector3& pos) = 0;

	virtual const Vector3& GetScaleOffset() const = 0;
	virtual void SetScaleOffset(const Vector3& s) = 0;

	virtual const Quaternion& GetOrientationOffset() const = 0;
	virtual void SetOrientationOffset(const Quaternion& pos) = 0;

	virtual const Vector3& GetPosition() const = 0;
	virtual void SetPosition(const Vector3& pos) = 0;
	virtual const Vector3& GetWorldPosition() const = 0;

	virtual const Vector3& GetScale() const = 0;
	virtual void SetScale(const Vector3& scale) = 0;
	virtual const Vector3& GetWorldScale() const = 0;

	virtual const Quaternion& GetOrientation() const = 0;
	virtual void SetOrientation(const Quaternion& orient) = 0;
	virtual const Quaternion& GetWorldOrientation() const = 0;

	virtual ISpecialEffects* GetParent() const = 0;
	virtual bool IsAttachable() const = 0;
	virtual bool GetAttach() const = 0;
	virtual void SetAttach(bool on) = 0;

	virtual bool IsManualLifeTime() const = 0;
	virtual void SetManualLifeTime(bool on) = 0;

	virtual void Cancel() = 0;
	virtual void OnDestroy() = 0;

	virtual AnimableValuePtr CreateAnimableValue(const String& valueName) = 0;
	virtual bool HasAnimableValue(const String& valueName) const = 0;
	virtual StringVector GetAnimableValueList() const = 0;

	virtual float GetLifeTime() const = 0;
	virtual void SetLifeTime(float val) = 0;

	virtual float GetStartTime() const = 0;
	virtual void SetStartTime(float val) = 0;
};
//--------------------------------------------------------------------------------
class _OgeSFXExport ISpecialEffects
	: public IUpdateAble
	, public PropertyObject
	, public OgeAlloc
{
public:

	virtual const String& GetName() const = 0;
	virtual void SetName(const String& name) = 0;

	virtual ISpecialEffects* Clone(const String& newName) = 0;
	virtual size_t GetElementsCount() const = 0;
	virtual IEffectsElement* GetElementsByIndex(size_t idx) const = 0;
	virtual IEffectsElement* GetElementsByName(const String& name) const = 0;
	virtual EffectElementList GetElementsByType(const String& name) const = 0;
	virtual int AddElement(IEffectsElement* element) = 0;
	virtual void RemoveElement(const String& name) = 0;
	virtual void RemoveElement(size_t idx) = 0;

	virtual bool IsEnd() const = 0;
	virtual bool IsAllElementEnd() const = 0;

	virtual void Cancel() = 0;
	virtual void Reset() = 0;

	virtual const Vector3& GetPosition() const = 0;
	virtual void SetPosition(const Vector3& pos) = 0;
	virtual void SetPosition(float x, float y, float z) = 0;

	virtual const Vector3& GetScale() const = 0;
	virtual void SetScale(const Vector3& scale) = 0;
	virtual void SetScale(float x, float y, float z) = 0;

	virtual const Quaternion& GetOrientation() const = 0;
	virtual void SetOrientation(const Quaternion& orient) = 0;
	virtual void SetOrientation(float yawDegree, float pitchDegree, float rollDegree) = 0;
	virtual void SetOrientation(float w, float x, float y, float z) = 0;

	virtual void SetDirection(const Vector3& dir) = 0;

	virtual SceneNode* GetNode() = 0;

	virtual void SetVisible(bool on) = 0;
	virtual bool GetVisible() const = 0;

	virtual float GetMinStartTime() const = 0;
	virtual float GetMaxLifeTime() const = 0;

	virtual void SetAutoLifeTimeManage(bool on)  = 0;
	virtual bool GetAutoLifeTimeManage() const = 0;
};
//--------------------------------------------------------------------------------
class _OgeSFXExport IEffectsElementFactory
{
public:

	virtual IEffectsElement* Create(const String& name, ISpecialEffects* parent) = 0;
	virtual void Destroy(IEffectsElement* effects) = 0;
};
//--------------------------------------------------------------------------------