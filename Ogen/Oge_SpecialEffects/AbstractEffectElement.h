#pragma once
#include "ISpecialEffects.h"

class _OgeSFXExport AbstractEffectElement : public IEffectsElement
{
public:

	AbstractEffectElement(const String& name, ISpecialEffects* parent);
	virtual ~AbstractEffectElement(void);

	virtual bool IsEnd() const;
	virtual void Reset();

	const String& GetAttachPointName() const { return _attachPointName; }
	void SetAttachPointName(const String& name) { _attachPointName = name; }

	virtual const Vector3& GetPositionOffset() const;
	virtual void SetPositionOffset(const Vector3& pos);

	virtual const Vector3& GetScaleOffset() const;
	virtual void SetScaleOffset(const Vector3& s);

	virtual const Quaternion& GetOrientationOffset() const;
	virtual void SetOrientationOffset(const Quaternion& orient);

	virtual const Vector3& GetPosition() const;
	virtual void SetPosition(const Vector3& pos);
	virtual const Vector3& GetWorldPosition() const;

	virtual const Vector3& GetScale() const;
	virtual void SetScale(const Vector3& scale);
	virtual const Vector3& GetWorldScale() const;

	virtual const Quaternion& GetOrientation() const;
	virtual void SetOrientation(const Quaternion& orient);
	virtual const Quaternion& GetWorldOrientation() const;

	virtual ISpecialEffects* GetParent() const;
	virtual void Update(float deltaTime);

	virtual bool IsAttachable() const{ return true; }
	virtual bool GetAttach() const { return _attach; }
	virtual void SetAttach(bool on);

	virtual bool IsManualLifeTime() const;
	virtual bool GetManualLifeTime() const;
	virtual void SetManualLifeTime(bool on);

	virtual float GetLifeTime() const;
	virtual void SetLifeTime(float val);

	virtual float GetStartTime() const;
	virtual void SetStartTime(float val);

	virtual void Cancel();
	virtual void OnDestroy();

	virtual AnimableValuePtr CreateAnimableValue(const String& valueName);
	virtual bool HasAnimableValue(const String& valueName) const;
	virtual StringVector GetAnimableValueList() const;

	virtual const String& GetName() const { return _name; }
	virtual void SetName(const String& name) { _name = name; }

protected:

	virtual void OnStart();
	virtual void OnEnd();
	virtual void OnUpdateController(float deltaTime);
	virtual void BuildPropertyMap();

	SceneManager* GetSceneManager();
	virtual Node* GetNode() const;

	String		_attachPointName;

	ISpecialEffects*	_parent;
	Vector3		_positionOffset;
	Vector3		_scaleOffset;
	Quaternion	_orientationOffset;

	float				_timeElapsed;
	float				_startTime;
	float				_lifeTime;

	bool				_isStarted;
	bool				_isEnd;
	bool				_isManualLifeTime;

	bool				_attach;

	String				_name;
	SceneNode*			_node;
};

