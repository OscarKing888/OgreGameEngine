#pragma once
#include "ParticleEditorProxy.h"

class AbstractAffectorEditor : public IParticleAffectorEditor
{
public:

	AbstractAffectorEditor();
	virtual ~AbstractAffectorEditor();

	virtual void SetAffector(ParticleAffector* emitter);
	virtual ParticleAffector* GetAffector() const;

	virtual const String& GetTypeName() const;

	virtual void ShowEditorProxyHelper(bool on){}

	virtual CWnd* GetEditorWnd() const;

	virtual void OnAffectorParameterChanged();

protected:

	virtual void BuildPropertyMap();

	ParticleAffector*	_affector;
};

//------------------------------------------------------------------------
// ColourFaderAffectorEditor
//------------------------------------------------------------------------
class ColourFaderAffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetRedAdjust(float red);
	float GetRedAdjust(void) const;
	void SetGreenAdjust(float green);
	float GetGreenAdjust(void) const;
	void SetBlueAdjust(float blue);
	float GetBlueAdjust(void) const;
	void SetAlphaAdjust(float alpha);
	float GetAlphaAdjust(void) const;

protected:

	virtual void BuildPropertyMap();
};

class ColourFaderAffectorEditorFactory : public TAffectorEditorFactory<ColourFaderAffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "ColourFader";
	}
};

//------------------------------------------------------------------------
// ColourFader2AffectorEditor
//------------------------------------------------------------------------
class ColourFader2AffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetRedAdjust1(float red);
	void SetRedAdjust2(float red);
	float GetRedAdjust1(void) const;
	float GetRedAdjust2(void) const;
	void SetGreenAdjust1(float green);
	void SetGreenAdjust2(float green);
	float GetGreenAdjust1(void) const;
	float GetGreenAdjust2(void) const;
	void SetBlueAdjust1(float blue);
	void SetBlueAdjust2(float blue);
	float GetBlueAdjust1(void) const;
	float GetBlueAdjust2(void) const;
	void SetAlphaAdjust1(float alpha);
	void SetAlphaAdjust2(float alpha);
	float GetAlphaAdjust1(void) const;
	float GetAlphaAdjust2(void) const;
	void SetStateChange(Real NewValue );
	Real GetStateChange(void) const;

protected:

	virtual void BuildPropertyMap();
};

class ColourFader2AffectorEditorFactory : public TAffectorEditorFactory<ColourFader2AffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "ColourFader2";
	}
};

//------------------------------------------------------------------------
// ColourImageAffectorEditor
//------------------------------------------------------------------------
class ColourImageAffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetImageAdjust(String name);
	String GetImageAdjust(void) const;

protected:

	virtual void BuildPropertyMap();
};

class ColourImageAffectorEditorFactory : public TAffectorEditorFactory<ColourImageAffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "ColourImage";
	}
};

//------------------------------------------------------------------------
// ColourInterpolatorAffectorEditor
//------------------------------------------------------------------------
class ColourInterpolatorAffector_TimeLineEditor;

class ColourInterpolatorAffectorEditor : public AbstractAffectorEditor
{
public:

	ColourInterpolatorAffectorEditor();
	virtual ~ColourInterpolatorAffectorEditor();

	virtual const String& GetTypeName() const;

	void SetColourAdjust0(ColourValue colour);
	ColourValue GetColourAdjust0() const;

	void SetColourAdjust1(ColourValue colour);
	ColourValue GetColourAdjust1() const;

	void SetColourAdjust2(ColourValue colour);
	ColourValue GetColourAdjust2() const;

	void SetColourAdjust3(ColourValue colour);
	ColourValue GetColourAdjust3() const;

	void SetColourAdjust4(ColourValue colour);
	ColourValue GetColourAdjust4() const;

	void SetColourAdjust5(ColourValue colour);
	ColourValue GetColourAdjust5() const;

	void SetTimeAdjust0(Real time);
	Real GetTimeAdjust0() const;

	void SetTimeAdjust1(Real time);
	Real GetTimeAdjust1() const;

	void SetTimeAdjust2(Real time);
	Real GetTimeAdjust2() const;

	void SetTimeAdjust3(Real time);
	Real GetTimeAdjust3() const;

	void SetTimeAdjust4(Real time);
	Real GetTimeAdjust4() const;

	void SetTimeAdjust5(Real time);
	Real GetTimeAdjust5() const;

	void SetTimeAdjust(int idx, Real time);
	Real GetTimeAdjust(int idx) const;

	void SetColourAdjust(int idx, ColourValue colour);
	ColourValue GetColourAdjust(int idx) const;


	virtual void SetAffector(ParticleAffector* affector);

protected:

	virtual void BuildPropertyMap();

	virtual CWnd* GetEditorWnd() const;

	virtual void OnAffectorParameterChanged();

	ColourInterpolatorAffector_TimeLineEditor* _editor;
};

class ColourInterpolatorAffectorEditorFactory : public TAffectorEditorFactory<ColourInterpolatorAffectorEditor>
{
public:

	virtual NameValuePairList GetInitDefaultParameters();

	virtual String GetTypeName() const
	{
		return "ColourInterpolator";
	}
};

class ColourFadingAffectorEditorFactory : public ColourInterpolatorAffectorEditorFactory
{
public:

	virtual String GetTypeName() const
	{
		return "ColourFading";
	}
};

//------------------------------------------------------------------------
// DeflectorPlaneAffectorEditor
//------------------------------------------------------------------------
class DeflectorPlaneAffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetPlanePoint(const Vector3& pos);
	Vector3 GetPlanePoint(void) const;
	void SetPlaneNormal(const Vector3& normal);
	Vector3 GetPlaneNormal(void) const;
	void SetBounce(Real bounce);
	Real GetBounce(void) const;

protected:

	virtual void BuildPropertyMap();
};

class DeflectorPlaneAffectorEditorFactory : public TAffectorEditorFactory<DeflectorPlaneAffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "DeflectorPlane";
	}
};

//------------------------------------------------------------------------
// DirectionRandomiserAffectorEditor
//------------------------------------------------------------------------
class DirectionRandomiserAffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetRandomness(Real force);
	void SetScope(Real force);
	void SetKeepVelocity(bool keepVelocity);
	Real GetRandomness(void) const;
	Real GetScope(void) const;
	bool GetKeepVelocity(void) const;

protected:

	virtual void BuildPropertyMap();
};

class DirectionRandomiserAffectorEditorFactory : public TAffectorEditorFactory<DirectionRandomiserAffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "DirectionRandomiser";
	}
};

//------------------------------------------------------------------------
// LinearForceAffectorEditor
//------------------------------------------------------------------------
enum ForceApplication
{
	/// Take the average of the force vector and the particle momentum
	FA_AVERAGE,
	/// Add the force vector to the particle momentum
	FA_ADD
};

class ForceApplicationType
	: public AbstractEnumDescriptor
{
public:

	ForceApplicationType();
	virtual ~ForceApplicationType(){}	
};

class LinearForceAffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetForceVector(const Vector3& force);
	Vector3 GetForceVector(void) const;
	void SetForceApplication(ForceApplication fa);
	ForceApplication GetForceApplication(void) const;

protected:

	virtual void BuildPropertyMap();
};

class LinearForceAffectorEditorFactory : public TAffectorEditorFactory<LinearForceAffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "LinearForce";
	}
};

//------------------------------------------------------------------------
// MovementAffectorEditor
//------------------------------------------------------------------------
class MovementAffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetUseStartVelocity(bool val);
	bool GetUseStartVelocity() const;

	void SetStartVelocityMin(const Vector3& val);
	Vector3 GetStartVelocityMin() const;

	void SetStartVelocityMax(const Vector3& val);
	Vector3 GetStartVelocityMax() const;

	void SetAcceleration(const Vector3& val);
	Vector3 GetAcceleration() const;

	void SetVelocityLossMin(const Vector3& val);
	Vector3 GetVelocityLossMin() const;

	void SetVelocityLossMax(const Vector3& val);
	Vector3 GetVelocityLossMax() const;

	void SetRandomnessMin(const Vector3& val);
	Vector3 GetRandomnessMin() const;

	void SetRandomnessMax(const Vector3& val);
	Vector3 GetRandomnessMax() const;

protected:

	virtual void BuildPropertyMap();
};

class MovementAffectorEditorFactory : public TAffectorEditorFactory<MovementAffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "Movement";
	}
};

//------------------------------------------------------------------------
// RotationAffectorEditor
//------------------------------------------------------------------------
class RotationAffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetRotationSpeedRangeStart(float angle);
	void SetRotationSpeedRangeEnd(float angle);
	float GetRotationSpeedRangeStart(void) const;
	float GetRotationSpeedRangeEnd(void) const;
	void SetRotationRangeStart(float angle);
	void SetRotationRangeEnd(float angle);
	float GetRotationRangeStart(void) const;
	float GetRotationRangeEnd(void) const;

protected:

	virtual void BuildPropertyMap();
};

class RotationAffectorEditorFactory : public TAffectorEditorFactory<RotationAffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "Rotator";
	}
};

//------------------------------------------------------------------------
// ScaleAffectorEditor
//------------------------------------------------------------------------
class ScaleAffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetAdjust( Real rate );
	Real GetAdjust(void) const;

protected:

	virtual void BuildPropertyMap();
};

class ScaleAffectorEditorFactory : public TAffectorEditorFactory<ScaleAffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "Scaler";
	}
};

//------------------------------------------------------------------------
// ScaleInterpolatorAffectorEditor
//------------------------------------------------------------------------
class ScaleInterpolatorAffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetTime0(float newValue);
	float GetTime0() const;
	void SetTime1(float newValue);
	float GetTime1() const;
	void SetTime2(float newValue);
	float GetTime2() const;
	void SetTime3(float newValue);
	float GetTime3() const;
	void SetTime4(float newValue);
	float GetTime4() const;
	void SetTime5(float newValue);
	float GetTime5() const;

	void SetScale0(const Vector3& newValue);
	Vector3 GetScale0() const;
	void SetScale1(const Vector3& newValue);
	Vector3 GetScale1() const;
	void SetScale2(const Vector3& newValue);
	Vector3 GetScale2() const;
	void SetScale3(const Vector3& newValue);
	Vector3 GetScale3() const;
	void SetScale4(const Vector3& newValue);
	Vector3 GetScale4() const;
	void SetScale5(const Vector3& newValue);
	Vector3 GetScale5() const;

protected:

	virtual void BuildPropertyMap();
};

class ScaleInterpolatorAffectorEditorFactory : public TAffectorEditorFactory<ScaleInterpolatorAffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "ScaleInterpolator";
	}
};

//------------------------------------------------------------------------
// ScaleAffectorEditor
//------------------------------------------------------------------------
class MeshAnimationAffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetAnimationName(const String& val);
	String GetAnimationName(void) const;

	void SetAnimationLoop(bool val);
	bool GetAnimationLoop(void) const;

	void SetAnimationSpeedFactor(float val);
	float GetAnimationSpeedFactor(void) const;

protected:

	virtual void BuildPropertyMap();
};

class MeshAnimationAffectorEditorFactory : public TAffectorEditorFactory<MeshAnimationAffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "MeshAnimationAffector";
	}
};

//------------------------------------------------------------------------
// RevolutionAffectorEditor
//------------------------------------------------------------------------
class RevolutionAffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	void SetRotationSpeed(float val);
	float GetRotationSpeed(void) const;

	void SetAxis(const Vector3& val);
	Vector3 GetAxis(void) const;

	void SetRadiusIncrement(float val);
	float GetRadiusIncrement(void) const;

	void SetCenterOffsetMin(const Vector3& val);
	Vector3 GetCenterOffsetMin(void) const;

	void SetCenterOffsetMax(const Vector3& val);
	Vector3 GetCenterOffsetMax(void) const;

	void SetTime(int idx, Real time);
	Real GetTime(int idx) const;

	void SetRadiusIncrementScale(int idx, Real val);
	Real GetRadiusIncrementScale(int idx) const;

	void SetTime0(float newValue);
	float GetTime0() const;
	void SetTime1(float newValue);
	float GetTime1() const;
	void SetTime2(float newValue);
	float GetTime2() const;
	void SetTime3(float newValue);
	float GetTime3() const;
	void SetTime4(float newValue);
	float GetTime4() const;
	void SetTime5(float newValue);
	float GetTime5() const;

	void SetRadiusIncrementScale0(float newValue);
	float GetRadiusIncrementScale0() const;
	void SetRadiusIncrementScale1(float newValue);
	float GetRadiusIncrementScale1() const;
	void SetRadiusIncrementScale2(float newValue);
	float GetRadiusIncrementScale2() const;
	void SetRadiusIncrementScale3(float newValue);
	float GetRadiusIncrementScale3() const;
	void SetRadiusIncrementScale4(float newValue);
	float GetRadiusIncrementScale4() const;
	void SetRadiusIncrementScale5(float newValue);
	float GetRadiusIncrementScale5() const;


protected:

	virtual void BuildPropertyMap();
};

class RevolutionAffectorEditorFactory : public TAffectorEditorFactory<RevolutionAffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "Revolution";
	}
};


//------------------------------------------------------------------------
// MeshRotatorAffectorEditor
//------------------------------------------------------------------------

class MeshRotatorAffectorEditor : public AbstractAffectorEditor
{
public:

	virtual const String& GetTypeName() const;

	DECL_AFFECTOR_EDITOR_FUN(yaw_rotation_speed_range_start, Real);
	DECL_AFFECTOR_EDITOR_FUN(yaw_rotation_speed_range_end, Real);

	DECL_AFFECTOR_EDITOR_FUN(yaw_rotation_range_start, Real);
	DECL_AFFECTOR_EDITOR_FUN(yaw_rotation_range_end, Real);

	DECL_AFFECTOR_EDITOR_FUN(pitch_rotation_speed_range_start, Real);
	DECL_AFFECTOR_EDITOR_FUN(pitch_rotation_speed_range_end, Real);

	DECL_AFFECTOR_EDITOR_FUN(pitch_rotation_range_start, Real);
	DECL_AFFECTOR_EDITOR_FUN(pitch_rotation_range_end, Real);

	DECL_AFFECTOR_EDITOR_FUN(roll_rotation_speed_range_start, Real);
	DECL_AFFECTOR_EDITOR_FUN(roll_rotation_speed_range_end, Real);

	DECL_AFFECTOR_EDITOR_FUN(roll_rotation_range_start, Real);
	DECL_AFFECTOR_EDITOR_FUN(roll_rotation_range_end, Real);
	
protected:

	virtual void BuildPropertyMap();
};

class MeshRotatorAffectorEditorFactory : public TAffectorEditorFactory<MeshRotatorAffectorEditor>
{
public:

	virtual String GetTypeName() const
	{
		return "MeshRotator";
	}
};