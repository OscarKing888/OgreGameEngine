#pragma once
#include "ParticleEditorProxy.h"

class AbstractEmitterEditor : public IParticleEmitterEditor
{
public:

	AbstractEmitterEditor();
	virtual ~AbstractEmitterEditor();

	virtual void SetEmitter(ParticleEmitter* emitter);
	virtual ParticleEmitter* GetEmitter() const;

	virtual const String& GetTypeName() const;

	void SetPosition(const Vector3& pos);
	const Vector3& GetPosition(void) const;
	void SetDirection(const Vector3& direction);
	const Vector3& GetDirection(void) const;
	void SetAngle(float degree);
	float GetAngle(void) const;
	void SetParticleVelocity(Real speed);
	void SetMinParticleVelocity(Real min);
	void SetMaxParticleVelocity(Real max);
	Real GetParticleVelocity(void) const;
	Real GetMinParticleVelocity(void) const;
	Real GetMaxParticleVelocity(void) const;
	void SetEmissionRate(Real particlesPerSecond);
	Real GetEmissionRate(void) const;
	void SetTimeToLive(Real ttl);
	void SetMinTimeToLive(Real min);
	void SetMaxTimeToLive(Real max);
	Real GetTimeToLive(void) const;
	Real GetMinTimeToLive(void) const;
	Real GetMaxTimeToLive(void) const;
	void SetColour(const ColourValue& colour);
	void SetColourRangeStart(const ColourValue& colour);
	void SetColourRangeEnd(const ColourValue& colour);
	const ColourValue& GetColour(void) const;
	const ColourValue& GetColourRangeStart(void) const;
	const ColourValue& GetColourRangeEnd(void) const;
	void SetEnabled(bool enabled);
	bool GetEnabled(void) const;
	void SetStartTime(Real startTime);
	Real GetStartTime(void) const;
	void SetDuration(Real duration);
	Real GetDuration(void) const;
	void SetMinDuration(Real min);
	void SetMaxDuration(Real max);
	Real GetMinDuration(void) const;
	Real GetMaxDuration(void) const;
	void SetRepeatDelay(Real duration);
	Real GetRepeatDelay(void) const;
	void SetMinRepeatDelay(Real min);
	void SetMaxRepeatDelay(Real max);
	Real GetMinRepeatDelay(void) const;
	Real GetMaxRepeatDelay(void) const;
	const String &GetName(void) const;
	void SetName(const String& newName);
	const String &GetEmittedEmitter(void) const;
	void SetEmittedEmitter(const String& emittedEmitter);

	virtual void ShowEditorProxyHelper(bool on){}

protected:

	virtual void BuildPropertyMap();

	ParticleEmitter*	_emitter;
};

//------------------------------------------------------------------------

class PointEmitterEditor : public AbstractEmitterEditor
{
public:

	virtual const String& GetTypeName() const;
};

class PointEmitterEditorFactory : public TEmitterEditorFactory<PointEmitterEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "Point";
	}
};

//------------------------------------------------------------------------

class AreaEmitterEditor : public AbstractEmitterEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetWidth(Real width);
	Real GetWidth(void) const;
	void SetHeight(Real height);
	Real GetHeight(void) const;
	void SetDepth(Real depth);
	Real GetDepth(void) const;

protected:

	virtual void BuildPropertyMap();
};

//------------------------------------------------------------------------

class BoxEmitterEditor : public AreaEmitterEditor
{
public:

	virtual const String& GetTypeName() const;
};

class BoxEmitterEditorFactory : public TEmitterEditorFactory<BoxEmitterEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "Box";
	}

	virtual NameValuePairList GetInitDefaultParameters();
};

//------------------------------------------------------------------------

class CylinderEmitterEditor : public AreaEmitterEditor
{
public:

	virtual const String& GetTypeName() const;
};

class CylinderEmitterEditorFactory : public TEmitterEditorFactory<CylinderEmitterEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "Cylinder";
	}

	virtual NameValuePairList GetInitDefaultParameters();
};

//------------------------------------------------------------------------

class EllipsoidEmitterEditor : public AreaEmitterEditor
{
public:

	virtual const String& GetTypeName() const;
};

class EllipsoidEmitterEditorFactory : public TEmitterEditorFactory<EllipsoidEmitterEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "Ellipsoid";
	}

	virtual NameValuePairList GetInitDefaultParameters();
};

//------------------------------------------------------------------------

class HollowEllipsoidEmitterEditor : public EllipsoidEmitterEditor
{
public:

	virtual const String& GetTypeName() const;

	float GetInnerWidth() const;
	void SetInnerWidth(float sz);
	
	float GetInnerHeight() const;
	void SetInnerHeight(float sz);

	float GetInnerDepth() const;
	void SetInnerDepth(float sz);

protected:

	virtual void BuildPropertyMap();
};

class HollowEllipsoidEmitterEditorFactory : public TEmitterEditorFactory<HollowEllipsoidEmitterEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "HollowEllipsoid";
	}

	virtual NameValuePairList GetInitDefaultParameters();
};

//------------------------------------------------------------------------

class RingEmitterEditor : public AreaEmitterEditor
{
public:

	virtual const String& GetTypeName() const;

	float GetInnerWidth() const;
	void SetInnerWidth(float sz);

	float GetInnerHeight() const;
	void SetInnerHeight(float sz);


protected:

	virtual void BuildPropertyMap();
};

class RingEmitterEditorFactory : public TEmitterEditorFactory<RingEmitterEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "Ring";
	}

	virtual NameValuePairList GetInitDefaultParameters();
};

//------------------------------------------------------------------------

class PolarEmitterEditor : public AreaEmitterEditor
{
public:

	virtual const String& GetTypeName() const;

	float GetRadiusStart() const;
	void SetRadiusStart(float val);

	float GetRadiusStep() const;
	void SetRadiusStep(float val);

	float GetRadiusEnd() const;
	void SetRadiusEnd(float val);

	float GetThetaStart() const;
	void SetThetaStart(float val);

	float GetThetaStep() const;
	void SetThetaStep(float val);

	float GetThetaEnd() const;
	void SetThetaEnd(float val);

	float GetPhiStart() const;
	void SetPhiStart(float val);

	float GetPhiStep() const;
	void SetPhiStep(float val);

	float GetPhiEnd() const;
	void SetPhiEnd(float val);

	bool GetUsePolarStep() const;
	void SetUsePolarStep(bool val);

	bool GetFlipYZAxis() const;
	void SetFlipYZAxis(bool val);

	int GetResetRadiusCount() const;
	void SetResetRadiusCount(int val);

	bool GetResetRadius() const;
	void SetResetRadius(bool val);

protected:

	virtual void BuildPropertyMap();
};

class PolarEmitterEditorFactory : public TEmitterEditorFactory<PolarEmitterEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "PolarEmitter";
	}

	virtual NameValuePairList GetInitDefaultParameters();
};
