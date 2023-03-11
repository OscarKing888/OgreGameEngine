#pragma once
#include "AbstractEffectElement.h"

class EffectElement_RibbonTrail : public AbstractEffectElement
{
public:

	EffectElement_RibbonTrail(const String& name, ISpecialEffects* parent);
	virtual ~EffectElement_RibbonTrail(void);

	virtual void SetName(const String& name);

	virtual const String& GetTypeName() const;
	virtual bool IsAttachable() const{ return true; }

	virtual void SetMaterialName(const String& matName);
	virtual const String& GetMaterialName() const { return _materialName; }

	virtual void SetTargetNodeName(const StringVector& nodeName){ _targetNodeName = nodeName; }
	virtual const StringVector& GetTargetNodeName() const { return _targetNodeName; }

	void SetTrailLength(float len);
	float GetTrailLength(void) const { return _trailLength; }

	void SetMaxChainElements(size_t maxElements);
	size_t GetMaxChainElements() const { return _maxElements; }

	void SetNumberOfChains(size_t numChains);
	size_t GetNumberOfChains() const { return _numChains; }

	void SetInitialWidth(size_t chainIndex, float width);
	float GetInitialWidth(size_t chainIndex) const;

	void SetWidthChange(size_t chainIndex, float widthDeltaPerSecond);
	float GetWidthChange(size_t chainIndex) const;

	void SetColourChange(size_t chainIndex, const ColourValue& clr);
	const ColourValue& GetColourChange(size_t chainIndex) const;


	void SetInitialWidths(const StringMap& idxAndVal);
	StringMap GetInitialWidths() const;

	void SetWidthChanges(const StringMap& idxAndVal);
	StringMap GetWidthChanges() const;

	void SetColourChanges(const StringMap& idxAndVal);
	StringMap GetColourChanges() const;

	void SetUseLocalPosition(bool on);
	bool GetUseLocalPosition() const { return _useLocalPosition; }

	void SetFaceToCamera(bool on);
	bool GetFaceToCamera() const { return _faceToCamera; }

	void SetElementPositionOffsetOnWidthExtent(float val);
	void SetElementOrientationOffset(const Vector3& val);
	void SetElementScaleOffset(const Vector3& val);

	float GetElementPositionOffsetOnWidthExtent() const{ return _elementPositionOffsetOnWidthExtent; }
	const Vector3& GetElementOrientationOffset() const{ return _elementOrientationOffset; }
	const Vector3& GetElementScaleOffset() const{ return _elementScaleOffset; }

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


	RibbonTrail* _trail;
	String	_materialName;
	StringVector	_targetNodeName;
	float			_trailLength;
	size_t			_maxElements;
	size_t			_numChains;
	bool			_useLocalPosition;
	bool			_faceToCamera;

	float _elementPositionOffsetOnWidthExtent;
	Vector3 _elementOrientationOffset;
	Vector3 _elementScaleOffset;

	typedef map<size_t, float>::type WidthMap;
	WidthMap	_initWidthMap;
	WidthMap	_widthChangeMap;

	typedef map<size_t, ColourValue>::type ColourMap;
	ColourMap	_colourChangeMap;

};

