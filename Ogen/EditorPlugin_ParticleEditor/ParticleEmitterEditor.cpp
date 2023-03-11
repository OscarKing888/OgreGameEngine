#include "stdafx.h"
#include "ParticleEmitterEditor.h"

AbstractEmitterEditor::AbstractEmitterEditor()
: _emitter(0)
{

}

AbstractEmitterEditor::~AbstractEmitterEditor()
{

}

const String& AbstractEmitterEditor::GetTypeName() const
{
	static String typeName = "AbstractEmitterEditor";
	return typeName;
}

void AbstractEmitterEditor::SetEmitter(ParticleEmitter* emitter)
{
	_emitter = emitter;
}

ParticleEmitter* AbstractEmitterEditor::GetEmitter() const
{
	return _emitter;
}


void AbstractEmitterEditor::BuildPropertyMap()
{
	AddProp(Property_Vector3, Position, AbstractEmitterEditor, "Position", "Emitter");
	AddProp(Property_Vector3, Direction, AbstractEmitterEditor, "Direction", "Emitter");
	AddProp(Property_Float, Angle, AbstractEmitterEditor, "Angle", "Emitter");
	
	AddProp(Property_Float, ParticleVelocity, AbstractEmitterEditor, "ParticleVelocity", "Emitter");
	AddProp(Property_Float, MinParticleVelocity, AbstractEmitterEditor, "MinParticleVelocity", "Emitter");
	AddProp(Property_Float, MaxParticleVelocity, AbstractEmitterEditor, "MaxParticleVelocity", "Emitter");
	
	AddProp(Property_Float, EmissionRate, AbstractEmitterEditor, "EmissionRate", "Emitter");
	
	AddProp(Property_Float, TimeToLive, AbstractEmitterEditor, "TimeToLive", "Emitter");
	AddProp(Property_Float, MinTimeToLive, AbstractEmitterEditor, "MinTimeToLive", "Emitter");
	AddProp(Property_Float, MaxTimeToLive, AbstractEmitterEditor, "MaxTimeToLive", "Emitter");

	AddProp(Property_ColourValue, Colour, AbstractEmitterEditor, "Colour", "Emitter");
	AddProp(Property_ColourValue, ColourRangeStart, AbstractEmitterEditor, "ColourRangeStart", "Emitter");
	AddProp(Property_ColourValue, ColourRangeEnd, AbstractEmitterEditor, "ColourRangeEnd", "Emitter");

	AddProp(Property_Bool, Enabled, AbstractEmitterEditor, "Enabled", "Emitter");

	AddProp(Property_Float, StartTime, AbstractEmitterEditor, "StartTime", "Emitter");

	AddProp(Property_Float, Duration, AbstractEmitterEditor, "Duration", "Emitter");
	AddProp(Property_Float, MinDuration, AbstractEmitterEditor, "MinDuration", "Emitter");
	AddProp(Property_Float, MaxDuration, AbstractEmitterEditor, "MaxDuration", "Emitter");

	AddProp(Property_Float, RepeatDelay, AbstractEmitterEditor, "RepeatDelay", "Emitter");
	AddProp(Property_Float, MinRepeatDelay, AbstractEmitterEditor, "MinRepeatDelay", "Emitter");
	AddProp(Property_Float, MaxRepeatDelay, AbstractEmitterEditor, "MaxRepeatDelay", "Emitter");

	AddProp(Property_String, Name, AbstractEmitterEditor, "Name", "Emitter");
	AddProp(Property_String, EmittedEmitter, AbstractEmitterEditor, "EmittedEmitter", "Emitter");	
}

void AbstractEmitterEditor::SetPosition(const Vector3& pos)
{
	_emitter->setPosition(pos);
}

const Vector3& AbstractEmitterEditor::GetPosition(void) const
{
	return _emitter->getPosition();
}

void AbstractEmitterEditor::SetDirection(const Vector3& direction)
{
	_emitter->setDirection(direction);
}

const Vector3& AbstractEmitterEditor::GetDirection(void) const
{
	return _emitter->getDirection();
}

void AbstractEmitterEditor::SetAngle(float degree)
{
	_emitter->setAngle(Radian(Degree(degree)));
}

float AbstractEmitterEditor::GetAngle(void) const
{
	return _emitter->getAngle().valueDegrees();
}

void AbstractEmitterEditor::SetParticleVelocity(Real speed)
{
	_emitter->setParticleVelocity(speed);
}

void AbstractEmitterEditor::SetMinParticleVelocity(Real min)
{
	_emitter->setMinParticleVelocity(min);
}

void AbstractEmitterEditor::SetMaxParticleVelocity(Real max)
{
	_emitter->setMaxParticleVelocity(max);
}

Real AbstractEmitterEditor::GetParticleVelocity(void) const
{
	return _emitter->getParticleVelocity();
}

Real AbstractEmitterEditor::GetMinParticleVelocity(void) const
{
	return _emitter->getMinParticleVelocity();
}

Real AbstractEmitterEditor::GetMaxParticleVelocity(void) const
{
	return _emitter->getMaxParticleVelocity();
}

void AbstractEmitterEditor::SetEmissionRate(Real particlesPerSecond)
{
	_emitter->setEmissionRate(particlesPerSecond);
}

Real AbstractEmitterEditor::GetEmissionRate(void) const
{
	return _emitter->getEmissionRate();

}

void AbstractEmitterEditor::SetTimeToLive(Real ttl)
{
	_emitter->setTimeToLive(ttl);
}

void AbstractEmitterEditor::SetMinTimeToLive(Real min)
{
	_emitter->setMinTimeToLive(min);
}

void AbstractEmitterEditor::SetMaxTimeToLive(Real max)
{
	_emitter->setMaxTimeToLive(max);
}

Real AbstractEmitterEditor::GetTimeToLive(void) const
{
	return _emitter->getTimeToLive();
}

Real AbstractEmitterEditor::GetMinTimeToLive(void) const
{
	return _emitter->getMinTimeToLive();
}

Real AbstractEmitterEditor::GetMaxTimeToLive(void) const
{
	return _emitter->getMaxTimeToLive();
}

void AbstractEmitterEditor::SetColour(const ColourValue& colour)
{
	_emitter->setColour(colour);
}

void AbstractEmitterEditor::SetColourRangeStart(const ColourValue& colour)
{
	_emitter->setColourRangeStart(colour);
}

void AbstractEmitterEditor::SetColourRangeEnd(const ColourValue& colour)
{
	_emitter->setColourRangeEnd(colour);
}

const ColourValue& AbstractEmitterEditor::GetColour(void) const
{
	return _emitter->getColour();
}

const ColourValue& AbstractEmitterEditor::GetColourRangeStart(void) const
{
	return _emitter->getColourRangeStart();
}

const ColourValue& AbstractEmitterEditor::GetColourRangeEnd(void) const
{
	return _emitter->getColourRangeEnd();
}

void AbstractEmitterEditor::SetEnabled(bool enabled)
{
	_emitter->setEnabled(enabled);
}

bool AbstractEmitterEditor::GetEnabled(void) const
{
	return _emitter->getEnabled();
}

void AbstractEmitterEditor::SetStartTime(Real startTime)
{
	_emitter->setStartTime(startTime);
}

Real AbstractEmitterEditor::GetStartTime(void) const
{
	return _emitter->getStartTime();
}

void AbstractEmitterEditor::SetDuration(Real duration)
{
	_emitter->setDuration(duration);
}

Real AbstractEmitterEditor::GetDuration(void) const
{
	return _emitter->getDuration();
}

void AbstractEmitterEditor::SetMinDuration(Real min)
{
	_emitter->setMinDuration(min);
}

void AbstractEmitterEditor::SetMaxDuration(Real max)
{
	_emitter->setMaxDuration(max);
}

Real AbstractEmitterEditor::GetMinDuration(void) const
{
	return _emitter->getMinDuration();
}

Real AbstractEmitterEditor::GetMaxDuration(void) const
{
	return _emitter->getMaxDuration();
}

void AbstractEmitterEditor::SetRepeatDelay(Real duration)
{
	_emitter->setRepeatDelay(duration);
}

Real AbstractEmitterEditor::GetRepeatDelay(void) const
{
	return _emitter->getRepeatDelay();
}

void AbstractEmitterEditor::SetMinRepeatDelay(Real min)
{
	_emitter->setMinRepeatDelay(min);
}

void AbstractEmitterEditor::SetMaxRepeatDelay(Real max)
{
	_emitter->setMaxRepeatDelay(max);
}

Real AbstractEmitterEditor::GetMinRepeatDelay(void) const
{
	return _emitter->getMinRepeatDelay();
}

Real AbstractEmitterEditor::GetMaxRepeatDelay(void) const
{
	return _emitter->getMaxRepeatDelay();
}

const String &AbstractEmitterEditor::GetName(void) const
{
	return _emitter->getName();
}

void AbstractEmitterEditor::SetName(const String& newName)
{
	_emitter->setName(newName);
}

const String &AbstractEmitterEditor::GetEmittedEmitter(void) const
{
	return _emitter->getEmittedEmitter();
}

void AbstractEmitterEditor::SetEmittedEmitter(const String& emittedEmitter)
{
	_emitter->setEmittedEmitter(emittedEmitter);
}

//------------------------------------------------------------------------
// PointEmitterEditor
//------------------------------------------------------------------------

const String& PointEmitterEditor::GetTypeName() const
{
	static String typeName = "Point";
	return typeName;
}

M_REG_EMITTER_EDITOR_FACTORY(PointEmitterEditor, PointEmitterEditorFactory);

//------------------------------------------------------------------------
// AreaEmitterEditor
//------------------------------------------------------------------------

const String& AreaEmitterEditor::GetTypeName() const
{
	static String typeName = "AreaEmitterEditor";
	return typeName;
}

void AreaEmitterEditor::BuildPropertyMap()
{
	AbstractEmitterEditor::BuildPropertyMap();

	AddProp(Property_Float, Width, AreaEmitterEditor, "Width", "AreaEmitter");
	AddProp(Property_Float, Height, AreaEmitterEditor, "Height", "AreaEmitter");
	AddProp(Property_Float, Depth, AreaEmitterEditor, "Depth", "AreaEmitter");
}

void AreaEmitterEditor::SetWidth(Real width)
{
	_emitter->setParameter("width", StringConverter::toString(width));
}

Real AreaEmitterEditor::GetWidth(void) const
{
	return StringConverter::parseReal(_emitter->getParameter("width"));
}

void AreaEmitterEditor::SetHeight(Real height)
{
	_emitter->setParameter("height", StringConverter::toString(height));
}

Real AreaEmitterEditor::GetHeight(void) const
{
	return StringConverter::parseReal(_emitter->getParameter("height"));
}

void AreaEmitterEditor::SetDepth(Real depth)
{
	_emitter->setParameter("depth", StringConverter::toString(depth));
}

Real AreaEmitterEditor::GetDepth(void) const
{
	return StringConverter::parseReal(_emitter->getParameter("depth"));
}

//------------------------------------------------------------------------
// BoxEmitterEditor
//------------------------------------------------------------------------

const String& BoxEmitterEditor::GetTypeName() const
{
	static String typeName = "Box";
	return typeName;
}

Ogre::NameValuePairList BoxEmitterEditorFactory::GetInitDefaultParameters()
{
	NameValuePairList parms = 
		TEmitterEditorFactory<BoxEmitterEditor>::GetInitDefaultParameters();

	parms["width"] = StringConverter::toString(100.0f);
	parms["height"] = StringConverter::toString(100.0f);
	parms["depth"] = StringConverter::toString(100.0f);

	return parms;
}

M_REG_EMITTER_EDITOR_FACTORY(BoxEmitterEditor, BoxEmitterEditorFactory);

//------------------------------------------------------------------------
// CylinderEmitterEditor
//------------------------------------------------------------------------

const String& CylinderEmitterEditor::GetTypeName() const
{
	static String typeName = "Cylinder";
	return typeName;
}

Ogre::NameValuePairList CylinderEmitterEditorFactory::GetInitDefaultParameters()
{
	NameValuePairList parms = 
		TEmitterEditorFactory<CylinderEmitterEditor>::GetInitDefaultParameters();

	parms["width"] = StringConverter::toString(100.0f);
	parms["height"] = StringConverter::toString(100.0f);
	parms["depth"] = StringConverter::toString(100.0f);

	return parms;
}

M_REG_EMITTER_EDITOR_FACTORY(CylinderEmitterEditor, CylinderEmitterEditorFactory);

//------------------------------------------------------------------------
// EllipsoidEmitterEditor
//------------------------------------------------------------------------

const String& EllipsoidEmitterEditor::GetTypeName() const
{
	static String typeName = "Ellipsoid";
	return typeName;
}

Ogre::NameValuePairList EllipsoidEmitterEditorFactory::GetInitDefaultParameters()
{
	NameValuePairList parms = 
		TEmitterEditorFactory<EllipsoidEmitterEditor>::GetInitDefaultParameters();

	parms["width"] = StringConverter::toString(100.0f);
	parms["height"] = StringConverter::toString(100.0f);
	parms["depth"] = StringConverter::toString(100.0f);

	return parms;
}

M_REG_EMITTER_EDITOR_FACTORY(EllipsoidEmitterEditor, EllipsoidEmitterEditorFactory);

//------------------------------------------------------------------------
// Ellipsoid EmitterEditor
//------------------------------------------------------------------------

void HollowEllipsoidEmitterEditor::SetInnerWidth(float sz)
{
	_emitter->setParameter("inner_width", StringConverter::toString(sz));
}

float HollowEllipsoidEmitterEditor::GetInnerWidth() const
{
	return StringConverter::parseReal(_emitter->getParameter("inner_width"));
}

float HollowEllipsoidEmitterEditor::GetInnerHeight() const
{
	return StringConverter::parseReal(_emitter->getParameter("inner_height"));
}

void HollowEllipsoidEmitterEditor::SetInnerHeight(float sz)
{
	_emitter->setParameter("inner_height", StringConverter::toString(sz));
}

float HollowEllipsoidEmitterEditor::GetInnerDepth() const
{
	return StringConverter::parseReal(_emitter->getParameter("inner_depth"));
}

void HollowEllipsoidEmitterEditor::SetInnerDepth(float sz)
{
	_emitter->setParameter("inner_depth", StringConverter::toString(sz));
}

void HollowEllipsoidEmitterEditor::BuildPropertyMap()
{
	EllipsoidEmitterEditor::BuildPropertyMap();

	AddProp(Property_Float, InnerWidth, HollowEllipsoidEmitterEditor, "InnerWidth", "HollowEllipsoidEmitter");
	AddProp(Property_Float, InnerHeight, HollowEllipsoidEmitterEditor, "InnerHeight", "HollowEllipsoidEmitter");
	AddProp(Property_Float, InnerDepth, HollowEllipsoidEmitterEditor, "InnerDepth", "HollowEllipsoidEmitter");
}

const String& HollowEllipsoidEmitterEditor::GetTypeName() const
{
	static String typeName = "HollowEllipsoid";
	return typeName;
}

Ogre::NameValuePairList HollowEllipsoidEmitterEditorFactory::GetInitDefaultParameters()
{
	NameValuePairList parms = 
		TEmitterEditorFactory<HollowEllipsoidEmitterEditor>::GetInitDefaultParameters();

	parms["width"] = StringConverter::toString(100.0f);
	parms["height"] = StringConverter::toString(100.0f);
	parms["depth"] = StringConverter::toString(100.0f);

	return parms;
}

M_REG_EMITTER_EDITOR_FACTORY(HollowEllipsoidEmitterEditor, HollowEllipsoidEmitterEditorFactory);


//------------------------------------------------------------------------
// RingEmitterEditor
//------------------------------------------------------------------------

void RingEmitterEditor::SetInnerWidth(float sz)
{
	_emitter->setParameter("inner_width", StringConverter::toString(sz));
}

float RingEmitterEditor::GetInnerWidth() const
{
	return StringConverter::parseReal(_emitter->getParameter("inner_width"));
}

float RingEmitterEditor::GetInnerHeight() const
{
	return StringConverter::parseReal(_emitter->getParameter("inner_height"));
}

void RingEmitterEditor::SetInnerHeight(float sz)
{
	_emitter->setParameter("inner_height", StringConverter::toString(sz));
}

void RingEmitterEditor::BuildPropertyMap()
{
	AreaEmitterEditor::BuildPropertyMap();

	AddProp(Property_Float, InnerWidth, RingEmitterEditor, "InnerWidth", "RingEmitterEditor");
	AddProp(Property_Float, InnerHeight, RingEmitterEditor, "InnerHeight", "RingEmitterEditor");
}

const String& RingEmitterEditor::GetTypeName() const
{
	static String typeName = "Ring";
	return typeName;
}

Ogre::NameValuePairList RingEmitterEditorFactory::GetInitDefaultParameters()
{
	NameValuePairList parms = 
		TEmitterEditorFactory<RingEmitterEditor>::GetInitDefaultParameters();

	parms["width"] = StringConverter::toString(100.0f);
	parms["height"] = StringConverter::toString(100.0f);
	parms["depth"] = StringConverter::toString(100.0f);

	return parms;
}

M_REG_EMITTER_EDITOR_FACTORY(RingEmitterEditor, RingEmitterEditorFactory);


//------------------------------------------------------------------------
// PolarEmitterEditor
//------------------------------------------------------------------------
Ogre::NameValuePairList PolarEmitterEditorFactory::GetInitDefaultParameters()
{
	NameValuePairList parms = 
		TEmitterEditorFactory<PolarEmitterEditor>::GetInitDefaultParameters();

	parms["width"] = StringConverter::toString(100.0f);
	parms["height"] = StringConverter::toString(100.0f);
	parms["depth"] = StringConverter::toString(100.0f);

	return parms;
}

M_REG_EMITTER_EDITOR_FACTORY(PolarEmitterEditor, PolarEmitterEditorFactory);

void PolarEmitterEditor::BuildPropertyMap()
{
	AreaEmitterEditor::BuildPropertyMap();

	AddProp(Property_Float, RadiusStart, PolarEmitterEditor, "RadiusStart", "PolarEmitterEditor");
	AddProp(Property_Float, RadiusStep, PolarEmitterEditor, "RadiusStep", "PolarEmitterEditor");
	AddProp(Property_Float, RadiusEnd, PolarEmitterEditor, "RadiusEnd", "PolarEmitterEditor");
	AddProp(Property_Float, ThetaStart, PolarEmitterEditor, "ThetaStart", "PolarEmitterEditor");
	AddProp(Property_Float, ThetaStep, PolarEmitterEditor, "ThetaStep", "PolarEmitterEditor");
	AddProp(Property_Float, ThetaEnd, PolarEmitterEditor, "ThetaEnd", "PolarEmitterEditor");
	AddProp(Property_Float, PhiStart, PolarEmitterEditor, "PhiStart", "PolarEmitterEditor");
	AddProp(Property_Float, PhiStep, PolarEmitterEditor, "PhiStep", "PolarEmitterEditor");
	AddProp(Property_Float, PhiEnd, PolarEmitterEditor, "PhiEnd", "PolarEmitterEditor");
	AddProp(Property_Bool, UsePolarStep, PolarEmitterEditor, "UsePolarStep", "PolarEmitterEditor");
	AddProp(Property_Bool, FlipYZAxis, PolarEmitterEditor, "FlipYZAxis", "PolarEmitterEditor");
	AddProp(Property_Int, ResetRadiusCount, PolarEmitterEditor, "ResetRadiusCount", "PolarEmitterEditor");
	AddProp(Property_Bool, ResetRadius, PolarEmitterEditor, "ResetRadius", "PolarEmitterEditor");
}

const String& PolarEmitterEditor::GetTypeName() const
{
	static String typeName = "PolarEmitter";
	return typeName;
}

float PolarEmitterEditor::GetRadiusStart() const
{
	return StringConverter::parseReal(_emitter->getParameter("radius_start"));
}

void PolarEmitterEditor::SetRadiusStart( float val )
{
	_emitter->setParameter("radius_start", StringConverter::toString(val));
}

float PolarEmitterEditor::GetRadiusStep() const
{
	return StringConverter::parseReal(_emitter->getParameter("radius_step"));
}

void PolarEmitterEditor::SetRadiusStep( float val )
{
	_emitter->setParameter("radius_step", StringConverter::toString(val));
}

float PolarEmitterEditor::GetRadiusEnd() const
{
	return StringConverter::parseReal(_emitter->getParameter("radius_end"));
}

void PolarEmitterEditor::SetRadiusEnd( float val )
{
	_emitter->setParameter("radius_end", StringConverter::toString(val));
}

float PolarEmitterEditor::GetThetaStart() const
{
	return StringConverter::parseReal(_emitter->getParameter("theta_start"));
}

void PolarEmitterEditor::SetThetaStart( float val )
{
	_emitter->setParameter("theta_start", StringConverter::toString(val));
}

float PolarEmitterEditor::GetThetaStep() const
{
	return StringConverter::parseReal(_emitter->getParameter("theta_step"));
}

void PolarEmitterEditor::SetThetaStep( float val )
{
	_emitter->setParameter("theta_step", StringConverter::toString(val));
}

float PolarEmitterEditor::GetThetaEnd() const
{
	return StringConverter::parseReal(_emitter->getParameter("theta_end"));
}

void PolarEmitterEditor::SetThetaEnd( float val )
{
	_emitter->setParameter("theta_end", StringConverter::toString(val));
}

float PolarEmitterEditor::GetPhiStart() const
{
	return StringConverter::parseReal(_emitter->getParameter("phi_start"));
}

void PolarEmitterEditor::SetPhiStart( float val )
{
	_emitter->setParameter("phi_start", StringConverter::toString(val));
}

float PolarEmitterEditor::GetPhiStep() const
{
	return StringConverter::parseReal(_emitter->getParameter("phi_step"));
}

void PolarEmitterEditor::SetPhiStep( float val )
{
	_emitter->setParameter("phi_step", StringConverter::toString(val));
}

float PolarEmitterEditor::GetPhiEnd() const
{
	return StringConverter::parseReal(_emitter->getParameter("phi_end"));
}

void PolarEmitterEditor::SetPhiEnd( float val )
{
	_emitter->setParameter("phi_end", StringConverter::toString(val));
}

bool PolarEmitterEditor::GetUsePolarStep() const
{
	return StringConverter::parseBool(_emitter->getParameter("use_polar_step"));
}

void PolarEmitterEditor::SetUsePolarStep( bool val )
{
	_emitter->setParameter("use_polar_step", StringConverter::toString(val));
}

bool PolarEmitterEditor::GetFlipYZAxis() const
{
	return StringConverter::parseBool(_emitter->getParameter("flip_yz_axis"));
}

void PolarEmitterEditor::SetFlipYZAxis( bool val )
{
	_emitter->setParameter("flip_yz_axis", StringConverter::toString(val));
}

int PolarEmitterEditor::GetResetRadiusCount() const
{
	return StringConverter::parseInt(_emitter->getParameter("reset_radius_count"));
}

void PolarEmitterEditor::SetResetRadiusCount( int val )
{
	_emitter->setParameter("reset_radius_count", StringConverter::toString(val));
}

bool PolarEmitterEditor::GetResetRadius() const
{
	return StringConverter::parseBool(_emitter->getParameter("reset_radius_count"));
}

void PolarEmitterEditor::SetResetRadius( bool val )
{
	_emitter->setParameter("reset_radius_count", StringConverter::toString(val));
}