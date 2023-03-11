#pragma once
#include "ParticleEditorProxy.h"

class AbstractParticleRenderEditor : public IParticleRenderEditor
{
public:

	AbstractParticleRenderEditor();
	virtual ~AbstractParticleRenderEditor();

	virtual void SetRender(ParticleSystemRenderer* render)
	{
		_render = render;
	}

	virtual ParticleSystemRenderer* GetRender() const
	{
		return _render;
	}

	virtual void ShowEditorProxyHelper(bool on){}

protected:
	
	ParticleSystemRenderer* _render;
};

//------------------------------------------------------------------------

class ParticleRenderEditor_Billboard : public AbstractParticleRenderEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetBillboardType(BillboardType bbt);
	BillboardType GetBillboardType(void) const;
	void SetUseAccurateFacing(bool acc);
	bool GetUseAccurateFacing(void) const;
	void SetBillboardOrigin(BillboardOrigin origin);
	BillboardOrigin GetBillboardOrigin(void) const;
	void SetBillboardRotationType(BillboardRotationType rotationType);
	BillboardRotationType GetBillboardRotationType(void) const;
	void SetCommonDirection(const Vector3& vec);
	Vector3 GetCommonDirection(void) const;
	void SetCommonUpVector(const Vector3& vec);
	Vector3 GetCommonUpVector(void) const;
	void SetPointRenderingEnabled(bool enabled);
	bool GetPointRenderingEnabled(void) const;

protected:

	virtual void BuildPropertyMap();

};

class ParticleRenderEditor_BillboardFactory : public TParticleRenderEditorFactory<ParticleRenderEditor_Billboard>
{
public:

	virtual String GetTypeName() const
	{
		return "billboard";
	}
};

//------------------------------------------------------------------------
class BillboardTypeConverter
	: public AbstractEnumDescriptor
{
public:

	BillboardTypeConverter();
	virtual ~BillboardTypeConverter(){}	
};

class BillboardOriginConverter
	: public AbstractEnumDescriptor
{
public:

	BillboardOriginConverter();
	virtual ~BillboardOriginConverter(){}	
};

class BillboardRotationTypeConverter
	: public AbstractEnumDescriptor
{
public:

	BillboardRotationTypeConverter();
	virtual ~BillboardRotationTypeConverter(){}	
};

//--------------------------------------------------------------------------------

enum MeshOrientationType
{
	/// origin mesh orientation
	MOT_ORIENTED_ORIGIN,
	/// oriented speed direction
	MOT_ORIENTED_SELF,
};

class ParticleRenderEditor_Mesh : public AbstractParticleRenderEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetMeshName(const String& meshName);
	String GetMeshName(void) const;

	void SetMeshOrientationType(MeshOrientationType mot);
	MeshOrientationType GetMeshOrientationType(void) const;

protected:

	virtual void BuildPropertyMap();

};

class MeshOrientationTypeConverter
	: public AbstractEnumDescriptor
{
public:

	MeshOrientationTypeConverter();
	virtual ~MeshOrientationTypeConverter(){}	
};

class ParticleRenderEditor_MeshFactory : public TParticleRenderEditorFactory<ParticleRenderEditor_Mesh>
{
public:

	virtual String GetTypeName() const
	{
		return "mesh";
	}

	virtual NameValuePairList GetInitDefaultParameters();
};

//--------------------------------------------------------------------------------

class ParticleRenderEditor_TexCoordBillboard : public ParticleRenderEditor_Billboard
{
public:

	virtual const String& GetTypeName() const;

	void SetStacks(int val);
	int GetStacks(void) const;

	void SetSlices(int val);
	int GetSlices(void) const;

	void SetRepeatTimes(float val);
	float GetRepeatTimes(void) const;

protected:

	virtual void BuildPropertyMap();

};

class ParticleRenderEditor_TexCoordBillboardFactory : public TParticleRenderEditorFactory<ParticleRenderEditor_TexCoordBillboard>
{
public:

	virtual String GetTypeName() const
	{
		return "texcoord_billboard";
	}
};

//--------------------------------------------------------------------------------

class ParticleRenderEditor_Ribbon : public AbstractParticleRenderEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetRibbonHeadAlpha(Real alpha);
	Real GetRibbonHeadAlpha(void) const;

	void SetRibbonTailAlpha(Real alpha);
	Real GetRibbonTailAlpha(void) const;

	void SetRibbonHeadWidthScale(Real width);
	Real GetRibbonHeadWidthScale(void) const;

	void SetRibbonTailWidthScale(Real width);
	Real GetRibbonTailWidthScale(void) const;

	void SetRibbonElementCount(int count);
	int GetRibbonElementCount(void) const;

protected:

	virtual void BuildPropertyMap();

};

class ParticleRenderEditor_RibbonFactory : public TParticleRenderEditorFactory<ParticleRenderEditor_Ribbon>
{
public:

	virtual String GetTypeName() const
	{
		return "ribbon";
	}
};
