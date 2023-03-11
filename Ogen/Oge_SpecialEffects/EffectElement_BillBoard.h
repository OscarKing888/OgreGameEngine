#pragma once
#include "AbstractEffectElement.h"

class EffectElement_Billboard : public AbstractEffectElement
{
public:

	EffectElement_Billboard(const String& name, ISpecialEffects* parent);
	virtual ~EffectElement_Billboard(void);

	virtual void SetName(const String& name);

	virtual const String& GetTypeName() const;
	virtual bool IsAttachable() const{ return true; }

	void SetMaterialName(const String& matName);
	const String& GetMaterialName() const { return _materialName; }

	void SetColour(const ColourValue& val);
	const ColourValue& GetColour() const { return _color; }

	void SetWidth(float val);
	float GetWidth() const { return _width; }

	void SetHeight(float val);
	float GetHeight() const { return _height; }

	void SetPoolSize(int val);
	float GetPoolSize() const { return _poolSize; }

	virtual void OnDestroy();

protected:

	virtual void OnStart();
	virtual void OnEnd();
	virtual void BuildPropertyMap();

	virtual void OnCreateBillboard();
	virtual void OnDestroyBillboard();
	void UpdateMaterial();
	virtual Node* GetNode() const;

	BillboardSet*		_billboard;
	uint				_poolSize;
	String			_materialName;
	ColourValue		_color;
	float					_width;
	float					_height;
};

