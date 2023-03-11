#pragma once
#include "ISpecialEffects.h"

class _OgeSFXExport SpecialEffects
	: public ISpecialEffects
{
public:

	SpecialEffects(const String& name);
	virtual ~SpecialEffects();

	virtual void SetName(const String& name);
	virtual const String& GetName() const { return _name; }

	virtual const String& GetTypeName() const;
	virtual void Update(float deltaTime);

	virtual ISpecialEffects* Clone(const String& newName);
	virtual size_t GetElementsCount() const;
	virtual IEffectsElement* GetElementsByIndex(size_t idx) const;
	virtual IEffectsElement* GetElementsByName(const String& name) const;
	virtual EffectElementList GetElementsByType(const String& name) const;

	virtual int AddElement(IEffectsElement* element);
	virtual void RemoveElement(const String& name);
	virtual void RemoveElement(size_t idx);

	virtual bool IsEnd() const;
	virtual bool IsAllElementEnd() const;
	virtual void Cancel();
	virtual void Reset();

	virtual const Vector3& GetPosition() const;
	virtual void SetPosition(const Vector3& pos);
	virtual void SetPosition(float x, float y, float z) { SetPosition(Vector3(x, y, z)); }

	virtual const Vector3& GetScale() const;
	virtual void SetScale(const Vector3& scale);
	virtual void SetScale(float x, float y, float z) { SetScale(Vector3(x, y, z)); }

	virtual const Quaternion& GetTargetWorldOrientation() const;
	virtual const Quaternion& GetOrientation() const;
	virtual void SetOrientation(const Quaternion& orient);
	virtual void SetOrientation(float yawDegree, float pitchDegree, float rollDegree);
	virtual void SetOrientation(float w, float x, float y, float z){ SetOrientation(Quaternion(w, x, y, z)); }

	virtual void SetDirection(const Vector3& dir);

	virtual SceneNode* GetNode() { return _node; }
	virtual void OnEffectsDestroy(ISpecialEffects* effects);

	virtual void SetVisible(bool on);
	virtual bool GetVisible() const;

	virtual void SetAutoLifeTimeManage(bool on) { _autoLifeTimeManage = on; }
	virtual bool GetAutoLifeTimeManage() const{ return _autoLifeTimeManage; }

	virtual float GetMinStartTime() const;
	virtual float GetMaxLifeTime() const;

protected:

	EffectElementList	_elements;

	typedef map<MovableObject*, SceneNode*>::type AttachMap;
	AttachMap			_attachMap;
	SceneNode*	_node;

	bool				_visible;
	bool				_autoLifeTimeManage;
	Vector3		_position;
	Vector3		_scale;
	Quaternion	_orientation;
	String				_name;

	void OnCreateNode();
	void OnDestroyNode();

	virtual void BuildPropertyMap();

	virtual const Vector3&  GetTargetWorldPosition() const;
	virtual void PlaySkeletonAnimation(const String& aniName, bool loopAnimation);
	virtual void StopSkeletonAnimation(const String& aniName);
	virtual void AttachEffectObject(MovableObject* obj, const String& attatchPointName,
		const Vector3& offsetPos = Vector3::ZERO, const Quaternion& offsetOrientation = Quaternion::IDENTITY);
	virtual void DetachEffectObject(MovableObject* object);
	virtual String GetEffectMeshFile() const;
	virtual void ShowWeapons(bool show);
	virtual void HittedProcess();
	virtual Node* GetEffectNode(const String& nodeName);
};
