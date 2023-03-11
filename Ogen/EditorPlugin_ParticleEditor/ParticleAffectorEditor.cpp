#include "stdafx.h"
#include "ParticleAffectorEditor.h"
#include "ColourInterpolatorAffector_TimeLineEditor.h"
#include "TimeLinePanel.h"

AbstractAffectorEditor::AbstractAffectorEditor()
: _affector(0)
{

}

AbstractAffectorEditor::~AbstractAffectorEditor()
{

}

const String& AbstractAffectorEditor::GetTypeName() const
{
	static String typeName = "AbstractAffectorEditor";
	return typeName;
}

void AbstractAffectorEditor::SetAffector(ParticleAffector* affector)
{
	_affector = affector;
}

ParticleAffector* AbstractAffectorEditor::GetAffector() const
{
	return _affector;
}


void AbstractAffectorEditor::BuildPropertyMap()
{
}

CWnd* AbstractAffectorEditor::GetEditorWnd() const
{
	return 0;
}

void AbstractAffectorEditor::OnAffectorParameterChanged()
{

}

//------------------------------------------------------------------------
// ColourFaderAffectorEditor
//------------------------------------------------------------------------
void ColourFaderAffectorEditor::SetRedAdjust(float red)
{
	_affector->setParameter("red", StringConverter::toString(red));
}

float ColourFaderAffectorEditor::GetRedAdjust(void) const
{
	return StringConverter::parseReal(_affector->getParameter("red"));
}

void ColourFaderAffectorEditor::SetGreenAdjust(float green)
{
	_affector->setParameter("green", StringConverter::toString(green));
}

float ColourFaderAffectorEditor::GetGreenAdjust(void) const
{
	return StringConverter::parseReal(_affector->getParameter("green"));
}

void ColourFaderAffectorEditor::SetBlueAdjust(float blue)
{
	_affector->setParameter("blue", StringConverter::toString(blue));
}

float ColourFaderAffectorEditor::GetBlueAdjust(void) const
{
	return StringConverter::parseReal(_affector->getParameter("blue"));
}

void ColourFaderAffectorEditor::SetAlphaAdjust(float alpha)
{
	_affector->setParameter("alpha", StringConverter::toString(alpha));
}

float ColourFaderAffectorEditor::GetAlphaAdjust(void) const
{
	return StringConverter::parseReal(_affector->getParameter("alpha"));
}

void ColourFaderAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_Float, RedAdjust, ColourFaderAffectorEditor, "RedAdjust", "ColourFaderAffector");
	AddProp(Property_Float, GreenAdjust, ColourFaderAffectorEditor, "GreenAdjust", "ColourFaderAffector");
	AddProp(Property_Float, BlueAdjust, ColourFaderAffectorEditor, "BlueAdjust", "ColourFaderAffector");
	AddProp(Property_Float, AlphaAdjust, ColourFaderAffectorEditor, "AlphaAdjust", "ColourFaderAffector");
}

const String& ColourFaderAffectorEditor::GetTypeName() const
{
	static String typeName = "ColourFader";
	return typeName;
}

M_REG_AFFECTOR_EDITOR_FACTORY(ColourFaderAffectorEditor, ColourFaderAffectorEditorFactory);

//------------------------------------------------------------------------
// ColourFader2AffectorEditor
//------------------------------------------------------------------------
void ColourFader2AffectorEditor::SetRedAdjust1(float red)
{
	_affector->setParameter("red1", StringConverter::toString(red));
}

float ColourFader2AffectorEditor::GetRedAdjust1(void) const
{
	return StringConverter::parseReal(_affector->getParameter("red1"));
}

void ColourFader2AffectorEditor::SetGreenAdjust1(float green)
{
	_affector->setParameter("green1", StringConverter::toString(green));
}

float ColourFader2AffectorEditor::GetGreenAdjust1(void) const
{
	return StringConverter::parseReal(_affector->getParameter("green1"));
}

void ColourFader2AffectorEditor::SetBlueAdjust1(float blue)
{
	_affector->setParameter("blue1", StringConverter::toString(blue));
}

float ColourFader2AffectorEditor::GetBlueAdjust1(void) const
{
	return StringConverter::parseReal(_affector->getParameter("blue1"));
}

void ColourFader2AffectorEditor::SetAlphaAdjust1(float alpha)
{
	_affector->setParameter("alpha1", StringConverter::toString(alpha));
}

float ColourFader2AffectorEditor::GetAlphaAdjust1(void) const
{
	return StringConverter::parseReal(_affector->getParameter("alpha1"));
}

void ColourFader2AffectorEditor::SetRedAdjust2(float red)
{
	_affector->setParameter("red2", StringConverter::toString(red));
}

float ColourFader2AffectorEditor::GetRedAdjust2(void) const
{
	return StringConverter::parseReal(_affector->getParameter("red2"));
}

void ColourFader2AffectorEditor::SetGreenAdjust2(float green)
{
	_affector->setParameter("green2", StringConverter::toString(green));
}

float ColourFader2AffectorEditor::GetGreenAdjust2(void) const
{
	return StringConverter::parseReal(_affector->getParameter("green2"));
}

void ColourFader2AffectorEditor::SetBlueAdjust2(float blue)
{
	_affector->setParameter("blue2", StringConverter::toString(blue));
}

float ColourFader2AffectorEditor::GetBlueAdjust2(void) const
{
	return StringConverter::parseReal(_affector->getParameter("blue2"));
}

void ColourFader2AffectorEditor::SetAlphaAdjust2(float alpha)
{
	_affector->setParameter("alpha2", StringConverter::toString(alpha));
}

float ColourFader2AffectorEditor::GetAlphaAdjust2(void) const
{
	return StringConverter::parseReal(_affector->getParameter("alpha2"));
}

void ColourFader2AffectorEditor::SetStateChange(Real newValue )
{
	_affector->setParameter("state_change", StringConverter::toString(newValue));
}

Real ColourFader2AffectorEditor::GetStateChange(void) const
{
	return StringConverter::parseReal(_affector->getParameter("state_change"));
}

void ColourFader2AffectorEditor::BuildPropertyMap()
{
	AddProp(Property_Float, RedAdjust1, ColourFader2AffectorEditor, "RedAdjust1", "ColourFader2Affector");
	AddProp(Property_Float, GreenAdjust1, ColourFader2AffectorEditor, "GreenAdjust1", "ColourFader2Affector");
	AddProp(Property_Float, BlueAdjust1, ColourFader2AffectorEditor, "BlueAdjust1", "ColourFader2Affector");
	AddProp(Property_Float, AlphaAdjust1, ColourFader2AffectorEditor, "AlphaAdjust1", "ColourFader2Affector");

	AddProp(Property_Float, RedAdjust2, ColourFader2AffectorEditor, "RedAdjust2", "ColourFader2Affector");
	AddProp(Property_Float, GreenAdjust2, ColourFader2AffectorEditor, "GreenAdjust2", "ColourFader2Affector");
	AddProp(Property_Float, BlueAdjust2, ColourFader2AffectorEditor, "BlueAdjust2", "ColourFader2Affector");
	AddProp(Property_Float, AlphaAdjust2, ColourFader2AffectorEditor, "AlphaAdjust2", "ColourFader2Affector");

	AddProp(Property_Float, StateChange, ColourFader2AffectorEditor, "StateChange", "ColourFader2Affector");

}

const String& ColourFader2AffectorEditor::GetTypeName() const
{
	static String typeName = "ColourFader2";
	return typeName;
}

M_REG_AFFECTOR_EDITOR_FACTORY(ColourFader2AffectorEditor, ColourFader2AffectorEditorFactory);

//------------------------------------------------------------------------
// ColourImageAffectorEditor
//------------------------------------------------------------------------
void ColourImageAffectorEditor::SetImageAdjust(String name)
{
	_affector->setParameter("image", name);
}

String ColourImageAffectorEditor::GetImageAdjust(void) const
{
	return _affector->getParameter("image");
}

void ColourImageAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_String, ImageAdjust, ColourImageAffectorEditor, "ImageAdjust", "ColourImageAffector");
}

const String& ColourImageAffectorEditor::GetTypeName() const
{
	static String typeName = "ColourImage";
	return typeName;
}

M_REG_AFFECTOR_EDITOR_FACTORY(ColourImageAffectorEditor, ColourImageAffectorEditorFactory);

//------------------------------------------------------------------------
// ColourInterpolatorAffectorEditor
//------------------------------------------------------------------------

void ColourInterpolatorAffectorEditor::SetColourAdjust0(ColourValue colour)
{
	_affector->setParameter("colour0", StringConverter::toString(colour));
}

ColourValue ColourInterpolatorAffectorEditor::GetColourAdjust0() const
{
	return StringConverter::parseColourValue(_affector->getParameter("colour0"));
}

void ColourInterpolatorAffectorEditor::SetColourAdjust1(ColourValue colour)
{
	_affector->setParameter("colour1", StringConverter::toString(colour));
}

ColourValue ColourInterpolatorAffectorEditor::GetColourAdjust1() const
{
	return StringConverter::parseColourValue(_affector->getParameter("colour1"));
}


void ColourInterpolatorAffectorEditor::SetColourAdjust2(ColourValue colour)
{
	_affector->setParameter("colour2", StringConverter::toString(colour));
}

ColourValue ColourInterpolatorAffectorEditor::GetColourAdjust2() const
{
	return StringConverter::parseColourValue(_affector->getParameter("colour2"));
}


void ColourInterpolatorAffectorEditor::SetColourAdjust3(ColourValue colour)
{
	_affector->setParameter("colour3", StringConverter::toString(colour));
}

ColourValue ColourInterpolatorAffectorEditor::GetColourAdjust3() const
{
	return StringConverter::parseColourValue(_affector->getParameter("colour3"));
}

void ColourInterpolatorAffectorEditor::SetColourAdjust4(ColourValue colour)
{
	_affector->setParameter("colour4", StringConverter::toString(colour));
}

ColourValue ColourInterpolatorAffectorEditor::GetColourAdjust4() const
{
	return StringConverter::parseColourValue(_affector->getParameter("colour4"));
}


void ColourInterpolatorAffectorEditor::SetColourAdjust5(ColourValue colour)
{
	_affector->setParameter("colour5", StringConverter::toString(colour));
}

ColourValue ColourInterpolatorAffectorEditor::GetColourAdjust5() const
{
	return StringConverter::parseColourValue(_affector->getParameter("colour5"));
}

void ColourInterpolatorAffectorEditor::SetTimeAdjust0(Real time)
{
	_affector->setParameter("time0", StringConverter::toString(time));
}

Real ColourInterpolatorAffectorEditor::GetTimeAdjust0() const
{
	return StringConverter::parseReal(_affector->getParameter("time0"));
}

void ColourInterpolatorAffectorEditor::SetTimeAdjust1(Real time)
{
	_affector->setParameter("time1", StringConverter::toString(time));
}

Real ColourInterpolatorAffectorEditor::GetTimeAdjust1() const
{
	return StringConverter::parseReal(_affector->getParameter("time1"));
}

void ColourInterpolatorAffectorEditor::SetTimeAdjust2(Real time)
{
	_affector->setParameter("time2", StringConverter::toString(time));
}

Real ColourInterpolatorAffectorEditor::GetTimeAdjust2() const
{
	return StringConverter::parseReal(_affector->getParameter("time2"));
}

void ColourInterpolatorAffectorEditor::SetTimeAdjust3(Real time)
{
	_affector->setParameter("time3", StringConverter::toString(time));
}

Real ColourInterpolatorAffectorEditor::GetTimeAdjust3() const
{
	return StringConverter::parseReal(_affector->getParameter("time3"));
}

void ColourInterpolatorAffectorEditor::SetTimeAdjust4(Real time)
{
	_affector->setParameter("time4", StringConverter::toString(time));
}

Real ColourInterpolatorAffectorEditor::GetTimeAdjust4() const
{
	return StringConverter::parseReal(_affector->getParameter("time4"));
}

void ColourInterpolatorAffectorEditor::SetTimeAdjust5(Real time)
{
	_affector->setParameter("time5", StringConverter::toString(time));
}

Real ColourInterpolatorAffectorEditor::GetTimeAdjust5() const
{
	return StringConverter::parseReal(_affector->getParameter("time5"));
}

void ColourInterpolatorAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_ColourValue, ColourAdjust0, ColourInterpolatorAffectorEditor, "ColourAdjust0", "ColourInterpolatorAffector");
	AddProp(Property_ColourValue, ColourAdjust1, ColourInterpolatorAffectorEditor, "ColourAdjust1", "ColourInterpolatorAffector");
	AddProp(Property_ColourValue, ColourAdjust2, ColourInterpolatorAffectorEditor, "ColourAdjust2", "ColourInterpolatorAffector");
	AddProp(Property_ColourValue, ColourAdjust3, ColourInterpolatorAffectorEditor, "ColourAdjust3", "ColourInterpolatorAffector");
	AddProp(Property_ColourValue, ColourAdjust4, ColourInterpolatorAffectorEditor, "ColourAdjust4", "ColourInterpolatorAffector");
	AddProp(Property_ColourValue, ColourAdjust5, ColourInterpolatorAffectorEditor, "ColourAdjust5", "ColourInterpolatorAffector");

	AddProp(Property_Float, TimeAdjust0, ColourInterpolatorAffectorEditor, "TimeAdjust0", "ColourInterpolatorAffector");
	AddProp(Property_Float, TimeAdjust1, ColourInterpolatorAffectorEditor, "TimeAdjust1", "ColourInterpolatorAffector");
	AddProp(Property_Float, TimeAdjust2, ColourInterpolatorAffectorEditor, "TimeAdjust2", "ColourInterpolatorAffector");
	AddProp(Property_Float, TimeAdjust3, ColourInterpolatorAffectorEditor, "TimeAdjust3", "ColourInterpolatorAffector");
	AddProp(Property_Float, TimeAdjust4, ColourInterpolatorAffectorEditor, "TimeAdjust4", "ColourInterpolatorAffector");
	AddProp(Property_Float, TimeAdjust5, ColourInterpolatorAffectorEditor, "TimeAdjust5", "ColourInterpolatorAffector");

}

const String& ColourInterpolatorAffectorEditor::GetTypeName() const
{
	static String typeName = "ColourInterpolator";
	return typeName;
}

void ColourInterpolatorAffectorEditor::SetTimeAdjust( int idx, Real time )
{
	_affector->setParameter(
		formatString("time%d", idx), StringConverter::toString(time));
}

Ogre::Real ColourInterpolatorAffectorEditor::GetTimeAdjust( int idx ) const
{
	return StringConverter::parseReal(_affector->getParameter(
		formatString("time%d", idx)));
}

void ColourInterpolatorAffectorEditor::SetColourAdjust( int idx, ColourValue colour )
{
	_affector->setParameter(formatString("colour%d", idx),
		StringConverter::toString(colour));
}

Ogre::ColourValue ColourInterpolatorAffectorEditor::GetColourAdjust( int idx ) const
{
	return StringConverter::parseColourValue(
		_affector->getParameter(formatString("colour%d", idx)));
}

ColourInterpolatorAffectorEditor::ColourInterpolatorAffectorEditor()
: _editor(0)
{
	_editor = new ColourInterpolatorAffector_TimeLineEditor();
	VERIFY(_editor->Create(NULL, this->GetTypeName().c_str(), WS_VISIBLE | WS_CHILD,
		CRect(0, 0, 100, 100), TimeLinePanel::getSingletonPtr(), 1234));
	_editor->SetEditor(this);
	TimeLinePanel::getSingleton().SetEditorWnd(_editor);
	TimeLinePanel::getSingleton().RelayoutEditor();
}

ColourInterpolatorAffectorEditor::~ColourInterpolatorAffectorEditor()
{
	TimeLinePanel::getSingleton().SetEditorWnd(0);
	_editor->DestroyWindow();
	delete _editor;
}

CWnd* ColourInterpolatorAffectorEditor::GetEditorWnd() const
{
	return _editor;
}

void ColourInterpolatorAffectorEditor::OnAffectorParameterChanged()
{
	if(_affector)
	{
		TimeColorList lst;
		for (int i = 0; i < 6; ++i)
		{
			Real t = GetTimeAdjust(i);
			ColourValue clr = GetColourAdjust(i);
			lst.push_back(std::make_pair(t, clr.getAsABGR()));
		}
		_editor->SetColourList(lst);
	}
}

void ColourInterpolatorAffectorEditor::SetAffector( ParticleAffector* affector )
{
	AbstractAffectorEditor::SetAffector(affector);

	OnAffectorParameterChanged();
}

Ogre::NameValuePairList ColourInterpolatorAffectorEditorFactory::GetInitDefaultParameters()
{
	NameValuePairList parms;

	parms["time0"] = "0.0f";

	for (int i = 1; i <= 5; ++i)
	{
		String key = formatString("time%d", i);
		parms[key] = StringConverter::toString(float(i) / 5.0f);
	}

	for (int i = 0; i < 6; ++i)
	{
		String key2 = formatString("colour%d", i);
		parms[key2] = StringConverter::toString(ColourValue(float(i) / 6.0f, 0.0f, 0.0f));
	}

	return parms;
}

M_REG_AFFECTOR_EDITOR_FACTORY(ColourInterpolatorAffectorEditor, ColourInterpolatorAffectorEditorFactory);
M_REG_AFFECTOR_EDITOR_FACTORY(ColourFading, ColourFadingAffectorEditorFactory);

//------------------------------------------------------------------------
// DeflectorPlaneAffectorEditor
//------------------------------------------------------------------------

void DeflectorPlaneAffectorEditor::SetPlanePoint(const Vector3& pos)
{
	_affector->setParameter("plane_point", StringConverter::toString(pos));
}

Vector3 DeflectorPlaneAffectorEditor::GetPlanePoint(void) const
{
	return StringConverter::parseVector3(_affector->getParameter("plane_point"));
}

void DeflectorPlaneAffectorEditor::SetPlaneNormal(const Vector3& normal)
{
	_affector->setParameter("plane_normal", StringConverter::toString(normal));
}

Vector3 DeflectorPlaneAffectorEditor::GetPlaneNormal(void) const
{
	return StringConverter::parseVector3(_affector->getParameter("plane_normal"));
}

void DeflectorPlaneAffectorEditor::SetBounce(Real bounce)
{
	_affector->setParameter("bounce", StringConverter::toString(bounce));
}

Real DeflectorPlaneAffectorEditor::GetBounce(void) const
{
	return StringConverter::parseReal(_affector->getParameter("bounce"));
}

void DeflectorPlaneAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_Vector3, PlanePoint, DeflectorPlaneAffectorEditor, "PlanePoint", "DeflectorPlaneAffector");
	AddProp(Property_Vector3, PlaneNormal, DeflectorPlaneAffectorEditor, "PlaneNormal", "DeflectorPlaneAffector");
	AddProp(Property_Float, Bounce, DeflectorPlaneAffectorEditor, "Bounce", "DeflectorPlaneAffector");

}

const String& DeflectorPlaneAffectorEditor::GetTypeName() const
{
	static String typeName = "DeflectorPlane";
	return typeName;
}

M_REG_AFFECTOR_EDITOR_FACTORY(DeflectorPlaneAffectorEditor, DeflectorPlaneAffectorEditorFactory);

//------------------------------------------------------------------------
// DirectionRandomiserAffectorEditor
//------------------------------------------------------------------------
void DirectionRandomiserAffectorEditor::SetRandomness(Real force)
{
	_affector->setParameter("randomness", StringConverter::toString(force));
}

void DirectionRandomiserAffectorEditor::SetScope(Real force)
{
	_affector->setParameter("scope", StringConverter::toString(force));
}

void DirectionRandomiserAffectorEditor::SetKeepVelocity(bool keepVelocity)
{
	_affector->setParameter("keep_velocity", StringConverter::toString(keepVelocity));
}

Real DirectionRandomiserAffectorEditor::GetRandomness(void) const
{
	return StringConverter::parseReal(_affector->getParameter("randomness"));
}

Real DirectionRandomiserAffectorEditor::GetScope(void) const
{
	return StringConverter::parseReal(_affector->getParameter("scope"));
}

bool DirectionRandomiserAffectorEditor::GetKeepVelocity(void) const
{
	return StringConverter::parseBool(_affector->getParameter("keep_velocity"));
}

void DirectionRandomiserAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_Float, Randomness, DirectionRandomiserAffectorEditor, "Randomness", "DirectionRandomiserAffector");
	AddProp(Property_Float, Scope, DirectionRandomiserAffectorEditor, "Scope", "DirectionRandomiserAffector");
	AddProp(Property_Bool, KeepVelocity, DirectionRandomiserAffectorEditor, "KeepVelocity", "DirectionRandomiserAffector");

}

const String& DirectionRandomiserAffectorEditor::GetTypeName() const
{
	static String typeName = "DirectionRandomiser";
	return typeName;
}

M_REG_AFFECTOR_EDITOR_FACTORY(DirectionRandomiserAffectorEditor, DirectionRandomiserAffectorEditorFactory);

//------------------------------------------------------------------------
// LinearForceAffectorEditor
//------------------------------------------------------------------------
static ForceApplicationType _ForceApplicationTypeInstance;

ForceApplicationType::ForceApplicationType()
{		
	M_AddEnumToDescriptorEx(FA_AVERAGE, average);
	M_AddEnumToDescriptorEx(FA_ADD, add);

	AbstractEnumDescriptor::Init();
}

void LinearForceAffectorEditor::SetForceVector(const Vector3& force)
{
	_affector->setParameter("force_vector", StringConverter::toString(force));
}

Vector3 LinearForceAffectorEditor::GetForceVector(void) const
{
	return StringConverter::parseVector3(_affector->getParameter("force_vector"));
}

void LinearForceAffectorEditor::SetForceApplication(ForceApplication fa)
{
	_affector->setParameter("force_application", _ForceApplicationTypeInstance.ToString(fa));
}

ForceApplication LinearForceAffectorEditor::GetForceApplication(void) const
{
	return (ForceApplication)_ForceApplicationTypeInstance.FromString(_affector->getParameter("force_application"));
}

void LinearForceAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_Vector3, ForceVector, LinearForceAffectorEditor, "ForceVector", "LinearForceAffector");
	AddEnumProp(ForceApplicationType, ForceApplication, ForceApplication, LinearForceAffectorEditor, "ForceApplicationType", "LinearForceAffector");
}

const String& LinearForceAffectorEditor::GetTypeName() const
{
	static String typeName = "LinearForce";
	return typeName;
}

M_REG_AFFECTOR_EDITOR_FACTORY(LinearForceAffectorEditor, LinearForceAffectorEditorFactory);

//------------------------------------------------------------------------
// MovementAffectorEditor
//------------------------------------------------------------------------
void MovementAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_Bool, UseStartVelocity, MovementAffectorEditor, "UseStartVelocity", "MovementAffector");

	AddProp(Property_Vector3, StartVelocityMin, MovementAffectorEditor, "StartVelocityMin", "MovementAffector");
	AddProp(Property_Vector3, StartVelocityMax, MovementAffectorEditor, "StartVelocityMax", "MovementAffector");

	AddProp(Property_Vector3, Acceleration, MovementAffectorEditor, "Acceleration", "MovementAffector");

	AddProp(Property_Vector3, VelocityLossMin, MovementAffectorEditor, "VelocityLossMin", "MovementAffector");
	AddProp(Property_Vector3, VelocityLossMax, MovementAffectorEditor, "VelocityLossMax", "MovementAffector");

	AddProp(Property_Vector3, RandomnessMin, MovementAffectorEditor, "RandomnessMin", "MovementAffector");
	AddProp(Property_Vector3, RandomnessMax, MovementAffectorEditor, "RandomnessMax", "MovementAffector");
}

const String& MovementAffectorEditor::GetTypeName() const
{
	static String typeName = "Movement";
	return typeName;
}

void MovementAffectorEditor::SetUseStartVelocity( bool val )
{
	_affector->setParameter("use_start_velocity",
		StringConverter::toString(val));
}

bool MovementAffectorEditor::GetUseStartVelocity() const
{
	return StringConverter::parseBool(
		_affector->getParameter("use_start_velocity"));
}

void MovementAffectorEditor::SetStartVelocityMin( const Vector3& val )
{
	_affector->setParameter("start_velocity_min",
		StringConverter::toString(val));
}

Vector3 MovementAffectorEditor::GetStartVelocityMin() const
{
	return StringConverter::parseVector3(
		_affector->getParameter("start_velocity_min"));
}

void MovementAffectorEditor::SetStartVelocityMax( const Vector3& val )
{
	_affector->setParameter("start_velocity_max",
		StringConverter::toString(val));
}

Vector3 MovementAffectorEditor::GetStartVelocityMax() const
{
	return StringConverter::parseVector3(
		_affector->getParameter("start_velocity_max"));
}

void MovementAffectorEditor::SetAcceleration( const Vector3& val )
{
	_affector->setParameter("acceleration",
		StringConverter::toString(val));
}

Vector3 MovementAffectorEditor::GetAcceleration() const
{
	return StringConverter::parseVector3(
		_affector->getParameter("acceleration"));
}

void MovementAffectorEditor::SetVelocityLossMin( const Vector3& val )
{
	_affector->setParameter("velocity_loss_min",
		StringConverter::toString(val));
}

Vector3 MovementAffectorEditor::GetVelocityLossMin() const
{
	return StringConverter::parseVector3(
		_affector->getParameter("velocity_loss_min"));
}

void MovementAffectorEditor::SetVelocityLossMax( const Vector3& val )
{
	_affector->setParameter("velocity_loss_max",
		StringConverter::toString(val));
}

Vector3 MovementAffectorEditor::GetVelocityLossMax() const
{
	return StringConverter::parseVector3(
		_affector->getParameter("velocity_loss_max"));
}

void MovementAffectorEditor::SetRandomnessMin( const Vector3& val )
{
	_affector->setParameter("randomness_min",
		StringConverter::toString(val));
}

Vector3 MovementAffectorEditor::GetRandomnessMin() const
{
	return StringConverter::parseVector3(
		_affector->getParameter("randomness_min"));
}

void MovementAffectorEditor::SetRandomnessMax( const Vector3& val )
{
	_affector->setParameter("randomness_max",
		StringConverter::toString(val));
}

Vector3 MovementAffectorEditor::GetRandomnessMax() const
{
	return StringConverter::parseVector3(
		_affector->getParameter("randomness_max"));
}



M_REG_AFFECTOR_EDITOR_FACTORY(MovementAffectorEditor, MovementAffectorEditorFactory);

//------------------------------------------------------------------------
// RotationAffectorEditor
//------------------------------------------------------------------------
void RotationAffectorEditor::SetRotationSpeedRangeStart(float angle)
{
	_affector->setParameter("rotation_speed_range_start", StringConverter::toString(Radian(Degree(angle))));
}

void RotationAffectorEditor::SetRotationSpeedRangeEnd(float angle)
{
	_affector->setParameter("rotation_speed_range_end", StringConverter::toString(Radian(Degree(angle))));
}

float RotationAffectorEditor::GetRotationSpeedRangeStart(void) const
{
	return StringConverter::parseAngle(_affector->getParameter("rotation_speed_range_start")).valueDegrees();
}

float RotationAffectorEditor::GetRotationSpeedRangeEnd(void) const
{
	return StringConverter::parseAngle(_affector->getParameter("rotation_speed_range_end")).valueDegrees();
}

void RotationAffectorEditor::SetRotationRangeStart(float angle)
{
	_affector->setParameter("rotation_range_start", StringConverter::toString(Radian(Degree(angle))));
}

void RotationAffectorEditor::SetRotationRangeEnd(float angle)
{
	_affector->setParameter("rotation_range_end", StringConverter::toString(Radian(Degree(angle))));
}

float RotationAffectorEditor::GetRotationRangeStart(void) const
{
	return StringConverter::parseAngle(_affector->getParameter("rotation_range_start")).valueDegrees();
}

float RotationAffectorEditor::GetRotationRangeEnd(void) const
{
	return StringConverter::parseAngle(_affector->getParameter("rotation_range_end")).valueDegrees();
}

void RotationAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_Float, RotationSpeedRangeStart, RotationAffectorEditor, "RotationSpeedRangeStart", "RotationAffector");
	AddProp(Property_Float, RotationSpeedRangeEnd, RotationAffectorEditor, "RotationSpeedRangeEnd", "RotationAffector");

	AddProp(Property_Float, RotationRangeStart, RotationAffectorEditor, "RotationRangeStart", "RotationAffector");
	AddProp(Property_Float, RotationRangeEnd, RotationAffectorEditor, "RotationRangeEnd", "RotationAffector");
}

const String& RotationAffectorEditor::GetTypeName() const
{
	static String typeName = "Rotator";
	return typeName;
}

M_REG_AFFECTOR_EDITOR_FACTORY(RotationAffectorEditor, RotationAffectorEditorFactory);

//------------------------------------------------------------------------
// ScaleAffectorEditor
//------------------------------------------------------------------------
void ScaleAffectorEditor::SetAdjust( Real rate )
{
	_affector->setParameter("rate", StringConverter::toString(rate));
}

Real ScaleAffectorEditor::GetAdjust(void) const
{
	return StringConverter::parseReal(_affector->getParameter("rate"));
}

void ScaleAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_Float, Adjust, ScaleAffectorEditor, "Adjust", "ScaleAffector");
}

const String& ScaleAffectorEditor::GetTypeName() const
{
	static String typeName = "Scaler";
	return typeName;
}

M_REG_AFFECTOR_EDITOR_FACTORY(ScaleAffectorEditor, ScaleAffectorEditorFactory);

//------------------------------------------------------------------------
// ScaleInterpolatorAffectorEditor
//------------------------------------------------------------------------

void ScaleInterpolatorAffectorEditor::SetTime0(float newValue)
{
	_affector->setParameter("time0", StringConverter::toString(newValue));
}

float ScaleInterpolatorAffectorEditor::GetTime0() const
{
	return StringConverter::parseReal(_affector->getParameter("time0"));
}

void ScaleInterpolatorAffectorEditor::SetTime1(float newValue)
{
	_affector->setParameter("time1", StringConverter::toString(newValue));
}

float ScaleInterpolatorAffectorEditor::GetTime1() const
{
	return StringConverter::parseReal(_affector->getParameter("time1"));
}

void ScaleInterpolatorAffectorEditor::SetTime2(float newValue)
{
	_affector->setParameter("time2", StringConverter::toString(newValue));
}

float ScaleInterpolatorAffectorEditor::GetTime2() const
{
	return StringConverter::parseReal(_affector->getParameter("time2"));
}

void ScaleInterpolatorAffectorEditor::SetTime3(float newValue)
{
	_affector->setParameter("time3", StringConverter::toString(newValue));
}

float ScaleInterpolatorAffectorEditor::GetTime3() const
{
	return StringConverter::parseReal(_affector->getParameter("time3"));
}

void ScaleInterpolatorAffectorEditor::SetTime4(float newValue)
{
	_affector->setParameter("time4", StringConverter::toString(newValue));
}

float ScaleInterpolatorAffectorEditor::GetTime4() const
{
	return StringConverter::parseReal(_affector->getParameter("time4"));
}

void ScaleInterpolatorAffectorEditor::SetTime5(float newValue)
{
	_affector->setParameter("time5", StringConverter::toString(newValue));
}

float ScaleInterpolatorAffectorEditor::GetTime5() const
{
	return StringConverter::parseReal(_affector->getParameter("time5"));
}

void ScaleInterpolatorAffectorEditor::SetScale0(const Vector3& newValue)
{
	_affector->setParameter("scale0", StringConverter::toString(newValue));
}

Vector3 ScaleInterpolatorAffectorEditor::GetScale0() const
{
	Vector3 val = StringConverter::parseVector3(_affector->getParameter("scale0"));
	
// 	EnableLogID("Editor", true);
// 	LogErrorIDf("Editor", "GetScale0: %f", val);

	return val;
}

void ScaleInterpolatorAffectorEditor::SetScale1(const Vector3& newValue)
{
	_affector->setParameter("scale1", StringConverter::toString(newValue));
}

Vector3 ScaleInterpolatorAffectorEditor::GetScale1() const
{
	return StringConverter::parseVector3(_affector->getParameter("scale1"));
}

void ScaleInterpolatorAffectorEditor::SetScale2(const Vector3& newValue)
{
	_affector->setParameter("scale2", StringConverter::toString(newValue));
}

Vector3 ScaleInterpolatorAffectorEditor::GetScale2() const
{
	return StringConverter::parseVector3(_affector->getParameter("scale2"));
}

void ScaleInterpolatorAffectorEditor::SetScale3(const Vector3& newValue)
{
	_affector->setParameter("scale3", StringConverter::toString(newValue));
}

Vector3 ScaleInterpolatorAffectorEditor::GetScale3() const
{
	return StringConverter::parseVector3(_affector->getParameter("scale3"));
}

void ScaleInterpolatorAffectorEditor::SetScale4(const Vector3& newValue)
{
	_affector->setParameter("scale4", StringConverter::toString(newValue));
}

Vector3 ScaleInterpolatorAffectorEditor::GetScale4() const
{
	return StringConverter::parseVector3(_affector->getParameter("scale4"));
}

void ScaleInterpolatorAffectorEditor::SetScale5(const Vector3& newValue)
{
	_affector->setParameter("scale5", StringConverter::toString(newValue));
}

Vector3 ScaleInterpolatorAffectorEditor::GetScale5() const
{
	return StringConverter::parseVector3(_affector->getParameter("scale5"));
}

void ScaleInterpolatorAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_Vector3, Scale0, ScaleInterpolatorAffectorEditor, "Scale0", "ScaleInterpolatorAffector");
	AddProp(Property_Vector3, Scale1, ScaleInterpolatorAffectorEditor, "Scale1", "ScaleInterpolatorAffector");
	AddProp(Property_Vector3, Scale2, ScaleInterpolatorAffectorEditor, "Scale2", "ScaleInterpolatorAffector");
	AddProp(Property_Vector3, Scale3, ScaleInterpolatorAffectorEditor, "Scale3", "ScaleInterpolatorAffector");
	AddProp(Property_Vector3, Scale4, ScaleInterpolatorAffectorEditor, "Scale4", "ScaleInterpolatorAffector");
	AddProp(Property_Vector3, Scale5, ScaleInterpolatorAffectorEditor, "Scale5", "ScaleInterpolatorAffector");

	AddProp(Property_Float, Time0, ScaleInterpolatorAffectorEditor, "Time0", "ScaleInterpolatorAffector");
	AddProp(Property_Float, Time1, ScaleInterpolatorAffectorEditor, "Time1", "ScaleInterpolatorAffector");
	AddProp(Property_Float, Time2, ScaleInterpolatorAffectorEditor, "Time2", "ScaleInterpolatorAffector");
	AddProp(Property_Float, Time3, ScaleInterpolatorAffectorEditor, "Time3", "ScaleInterpolatorAffector");
	AddProp(Property_Float, Time4, ScaleInterpolatorAffectorEditor, "Time4", "ScaleInterpolatorAffector");
	AddProp(Property_Float, Time5, ScaleInterpolatorAffectorEditor, "Time5", "ScaleInterpolatorAffector");
}

const String& ScaleInterpolatorAffectorEditor::GetTypeName() const
{
	static String typeName = "ScaleInterpolator";
	return typeName;
}

M_REG_AFFECTOR_EDITOR_FACTORY(ScaleInterpolatorAffectorEditor, ScaleInterpolatorAffectorEditorFactory);

//------------------------------------------------------------------------
// MeshAnimationAffectorEditor
//------------------------------------------------------------------------
M_REG_AFFECTOR_EDITOR_FACTORY(MeshAnimationAffectorEditor, MeshAnimationAffectorEditorFactory);

const String& MeshAnimationAffectorEditor::GetTypeName() const
{
	static String typeName = "MeshAnimationAffector";
	return typeName;
}

void MeshAnimationAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_String, AnimationName, MeshAnimationAffectorEditor, "AnimationName", "MeshAnimationAffectorEditor");
	AddProp(Property_Bool, AnimationLoop, MeshAnimationAffectorEditor, "AnimationLoop", "MeshAnimationAffectorEditor");
	AddProp(Property_Float, AnimationSpeedFactor, MeshAnimationAffectorEditor, "AnimationSpeedFactor", "MeshAnimationAffectorEditor");
}

void MeshAnimationAffectorEditor::SetAnimationName( const String& val )
{
	_affector->setParameter("animation_name", val);
}

String MeshAnimationAffectorEditor::GetAnimationName( void ) const
{
	return _affector->getParameter("animation_name");
}

void MeshAnimationAffectorEditor::SetAnimationLoop( bool val )
{
	_affector->setParameter("animation_loop", StringConverter::toString(val));
}

bool MeshAnimationAffectorEditor::GetAnimationLoop( void ) const
{
	return StringConverter::parseBool(
		_affector->getParameter("animation_loop"));
}

void MeshAnimationAffectorEditor::SetAnimationSpeedFactor( float val )
{
	_affector->setParameter("animation_speed", StringConverter::toString(val));
}

float MeshAnimationAffectorEditor::GetAnimationSpeedFactor( void ) const
{
	return StringConverter::parseReal(
		_affector->getParameter("animation_speed"));
}
//------------------------------------------------------------------------
// RevolutionAffectorEditor
//------------------------------------------------------------------------
M_REG_AFFECTOR_EDITOR_FACTORY(RevolutionAffectorEditor, RevolutionAffectorEditorFactory);

const String& RevolutionAffectorEditor::GetTypeName() const
{
	static String typeName = "Revolution";
	return typeName;
}

void RevolutionAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_Float, RotationSpeed, RevolutionAffectorEditor, "RotationSpeed", "RevolutionAffectorEditor");
	AddProp(Property_Float, RadiusIncrement, RevolutionAffectorEditor, "RadiusIncrement", "RevolutionAffectorEditor");

	AddProp(Property_Vector3, Axis, RevolutionAffectorEditor, "Axis", "RevolutionAffectorEditor");
	AddProp(Property_Vector3, CenterOffsetMin, RevolutionAffectorEditor, "CenterOffsetMin", "RevolutionAffectorEditor");
	AddProp(Property_Vector3, CenterOffsetMax, RevolutionAffectorEditor, "CenterOffsetMax", "RevolutionAffectorEditor");

	AddProp(Property_Float, Time0, RevolutionAffectorEditor, "Time0", "RevolutionAffectorEditor");
	AddProp(Property_Float, Time1, RevolutionAffectorEditor, "Time1", "RevolutionAffectorEditor");
	AddProp(Property_Float, Time2, RevolutionAffectorEditor, "Time2", "RevolutionAffectorEditor");
	AddProp(Property_Float, Time3, RevolutionAffectorEditor, "Time3", "RevolutionAffectorEditor");
	AddProp(Property_Float, Time4, RevolutionAffectorEditor, "Time4", "RevolutionAffectorEditor");
	AddProp(Property_Float, Time5, RevolutionAffectorEditor, "Time5", "RevolutionAffectorEditor");

	AddProp(Property_Float, RadiusIncrementScale0, RevolutionAffectorEditor, "RadiusIncrementScale0", "RevolutionAffectorEditor");
	AddProp(Property_Float, RadiusIncrementScale1, RevolutionAffectorEditor, "RadiusIncrementScale1", "RevolutionAffectorEditor");
	AddProp(Property_Float, RadiusIncrementScale2, RevolutionAffectorEditor, "RadiusIncrementScale2", "RevolutionAffectorEditor");
	AddProp(Property_Float, RadiusIncrementScale3, RevolutionAffectorEditor, "RadiusIncrementScale3", "RevolutionAffectorEditor");
	AddProp(Property_Float, RadiusIncrementScale4, RevolutionAffectorEditor, "RadiusIncrementScale4", "RevolutionAffectorEditor");
	AddProp(Property_Float, RadiusIncrementScale5, RevolutionAffectorEditor, "RadiusIncrementScale5", "RevolutionAffectorEditor");
}

void RevolutionAffectorEditor::SetTime( int idx, Real time )
{
	_affector->setParameter(
		formatString("time%d", idx), StringConverter::toString(time));
}

Ogre::Real RevolutionAffectorEditor::GetTime( int idx ) const
{
	return StringConverter::parseReal(_affector->getParameter(
		formatString("time%d", idx)));
}

void RevolutionAffectorEditor::SetRadiusIncrementScale( int idx, Real val )
{
	_affector->setParameter(
		formatString("radius_increment_scale%d", idx), StringConverter::toString(val));
}

Ogre::Real RevolutionAffectorEditor::GetRadiusIncrementScale( int idx ) const
{
	return StringConverter::parseReal(_affector->getParameter(
		formatString("radius_increment_scale%d", idx)));

}
void RevolutionAffectorEditor::SetTime0(float newValue)
{
	SetTime(0, newValue);
}

float RevolutionAffectorEditor::GetTime0() const
{
	return GetTime(0);
}

void RevolutionAffectorEditor::SetTime1(float newValue)
{
	SetTime(1, newValue);
}

float RevolutionAffectorEditor::GetTime1() const
{
	return GetTime(1);
}

void RevolutionAffectorEditor::SetTime2(float newValue)
{
	SetTime(2, newValue);
}

float RevolutionAffectorEditor::GetTime2() const
{
	return GetTime(2);
}

void RevolutionAffectorEditor::SetTime3(float newValue)
{
	SetTime(3, newValue);
}

float RevolutionAffectorEditor::GetTime3() const
{
	return GetTime(3);
}

void RevolutionAffectorEditor::SetTime4(float newValue)
{
	SetTime(4, newValue);
}

float RevolutionAffectorEditor::GetTime4() const
{
	return GetTime(4);
}

void RevolutionAffectorEditor::SetTime5(float newValue)
{
	SetTime(5, newValue);
}

float RevolutionAffectorEditor::GetTime5() const
{
	return GetTime(5);
}

void RevolutionAffectorEditor::SetRadiusIncrementScale0( float newValue )
{
	SetRadiusIncrementScale(0, newValue);
}

float RevolutionAffectorEditor::GetRadiusIncrementScale0() const
{
	return GetRadiusIncrementScale(0);
}

void RevolutionAffectorEditor::SetRadiusIncrementScale1( float newValue )
{
	SetRadiusIncrementScale(1, newValue);
}

float RevolutionAffectorEditor::GetRadiusIncrementScale1() const
{
	return GetRadiusIncrementScale(1);
}

void RevolutionAffectorEditor::SetRadiusIncrementScale2( float newValue )
{
	SetRadiusIncrementScale(2, newValue);
}

float RevolutionAffectorEditor::GetRadiusIncrementScale2() const
{
	return GetRadiusIncrementScale(2);
}

void RevolutionAffectorEditor::SetRadiusIncrementScale3( float newValue )
{
	SetRadiusIncrementScale(3, newValue);
}

float RevolutionAffectorEditor::GetRadiusIncrementScale3() const
{
	return GetRadiusIncrementScale(3);
}

void RevolutionAffectorEditor::SetRadiusIncrementScale4( float newValue )
{
	SetRadiusIncrementScale(4, newValue);
}

float RevolutionAffectorEditor::GetRadiusIncrementScale4() const
{
	return GetRadiusIncrementScale(4);
}

void RevolutionAffectorEditor::SetRadiusIncrementScale5( float newValue )
{
	SetRadiusIncrementScale(5, newValue);
}

float RevolutionAffectorEditor::GetRadiusIncrementScale5() const
{
	return GetRadiusIncrementScale(5);
}

void RevolutionAffectorEditor::SetRotationSpeed( float val )
{
	_affector->setParameter("rotation_speed", StringConverter::toString(val));
}

float RevolutionAffectorEditor::GetRotationSpeed( void ) const
{
	return StringConverter::parseReal(
		_affector->getParameter("rotation_speed"));
}

void RevolutionAffectorEditor::SetAxis( const Vector3& val )
{
	_affector->setParameter("rotation_axis", StringConverter::toString(val));
}

Vector3 RevolutionAffectorEditor::GetAxis( void ) const
{
	return StringConverter::parseVector3(
		_affector->getParameter("rotation_axis"));
}

void RevolutionAffectorEditor::SetRadiusIncrement( float val )
{
	_affector->setParameter("radius_increment", StringConverter::toString(val));
}

float RevolutionAffectorEditor::GetRadiusIncrement( void ) const
{
	return StringConverter::parseReal(
		_affector->getParameter("radius_increment"));
}

void RevolutionAffectorEditor::SetCenterOffsetMin( const Vector3& val )
{
	_affector->setParameter("center_offset_min", StringConverter::toString(val));
}

Vector3 RevolutionAffectorEditor::GetCenterOffsetMin( void ) const
{
	return StringConverter::parseVector3(
		_affector->getParameter("center_offset_min"));
}

void RevolutionAffectorEditor::SetCenterOffsetMax( const Vector3& val )
{
	_affector->setParameter("center_offset_max", StringConverter::toString(val));
}

Vector3 RevolutionAffectorEditor::GetCenterOffsetMax( void ) const
{
	return StringConverter::parseVector3(
		_affector->getParameter("center_offset_max"));
}

//------------------------------------------------------------------------
// MeshRotatorAffectorEditor
//------------------------------------------------------------------------
M_REG_AFFECTOR_EDITOR_FACTORY(MeshRotatorAffectorEditor, MeshRotatorAffectorEditorFactory);

const String& MeshRotatorAffectorEditor::GetTypeName() const
{
	static String typeName = "MeshRotator";
	return typeName;
}

void MeshRotatorAffectorEditor::BuildPropertyMap()
{
	AddProp(Property_Float, yaw_rotation_speed_range_start, MeshRotatorAffectorEditor, "yaw_rotation_speed_range_start", "MeshRotatorAffectorEditor");
	AddProp(Property_Float, yaw_rotation_speed_range_end, MeshRotatorAffectorEditor, "yaw_rotation_speed_range_end", "MeshRotatorAffectorEditor");

	AddProp(Property_Float, yaw_rotation_range_start, MeshRotatorAffectorEditor, "yaw_rotation_range_start", "MeshRotatorAffectorEditor");
	AddProp(Property_Float, yaw_rotation_range_end, MeshRotatorAffectorEditor, "yaw_rotation_range_end", "MeshRotatorAffectorEditor");

	AddProp(Property_Float, pitch_rotation_speed_range_start, MeshRotatorAffectorEditor, "pitch_rotation_speed_range_start", "MeshRotatorAffectorEditor");
	AddProp(Property_Float, pitch_rotation_speed_range_end, MeshRotatorAffectorEditor, "pitch_rotation_speed_range_end", "MeshRotatorAffectorEditor");

	AddProp(Property_Float, pitch_rotation_range_start, MeshRotatorAffectorEditor, "pitch_rotation_range_start", "MeshRotatorAffectorEditor");
	AddProp(Property_Float, pitch_rotation_speed_range_end, MeshRotatorAffectorEditor, "pitch_rotation_speed_range_end", "MeshRotatorAffectorEditor");

	AddProp(Property_Float, roll_rotation_speed_range_start, MeshRotatorAffectorEditor, "roll_rotation_speed_range_start", "MeshRotatorAffectorEditor");
	AddProp(Property_Float, roll_rotation_speed_range_end, MeshRotatorAffectorEditor, "roll_rotation_speed_range_end", "MeshRotatorAffectorEditor");

	AddProp(Property_Float, roll_rotation_range_start, MeshRotatorAffectorEditor, "roll_rotation_range_start", "MeshRotatorAffectorEditor");
	AddProp(Property_Float, roll_rotation_range_end, MeshRotatorAffectorEditor, "roll_rotation_range_end", "MeshRotatorAffectorEditor");
}

IMPL_AFFECTOR_EDITOR_FUN(MeshRotatorAffectorEditor, Real, yaw_rotation_speed_range_start);
IMPL_AFFECTOR_EDITOR_FUN(MeshRotatorAffectorEditor, Real, yaw_rotation_speed_range_end);

IMPL_AFFECTOR_EDITOR_FUN(MeshRotatorAffectorEditor, Real, yaw_rotation_range_start);
IMPL_AFFECTOR_EDITOR_FUN(MeshRotatorAffectorEditor, Real, yaw_rotation_range_end);

IMPL_AFFECTOR_EDITOR_FUN(MeshRotatorAffectorEditor, Real, pitch_rotation_speed_range_start);
IMPL_AFFECTOR_EDITOR_FUN(MeshRotatorAffectorEditor, Real, pitch_rotation_speed_range_end);

IMPL_AFFECTOR_EDITOR_FUN(MeshRotatorAffectorEditor, Real, pitch_rotation_range_start);
IMPL_AFFECTOR_EDITOR_FUN(MeshRotatorAffectorEditor, Real, pitch_rotation_range_end);

IMPL_AFFECTOR_EDITOR_FUN(MeshRotatorAffectorEditor, Real, roll_rotation_speed_range_start);
IMPL_AFFECTOR_EDITOR_FUN(MeshRotatorAffectorEditor, Real, roll_rotation_speed_range_end);

IMPL_AFFECTOR_EDITOR_FUN(MeshRotatorAffectorEditor, Real, roll_rotation_range_start);
IMPL_AFFECTOR_EDITOR_FUN(MeshRotatorAffectorEditor, Real, roll_rotation_range_end);
