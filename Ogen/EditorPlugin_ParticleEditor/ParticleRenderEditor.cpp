#include "stdafx.h"
#include "ParticleRenderEditor.h"
#include <OgreParticleSystemRenderer.h>

AbstractParticleRenderEditor::AbstractParticleRenderEditor()
: _render(0)
{

}

AbstractParticleRenderEditor::~AbstractParticleRenderEditor()
{

}

//------------------------------------------------------------------------

M_REG_RENDER_EDITOR_FACTORY(ParticleRenderEditor_Billboard, ParticleRenderEditor_BillboardFactory);

static BillboardTypeConverter _BillboardTypeConverterInstance;

BillboardTypeConverter::BillboardTypeConverter()
{		
	M_AddEnumToDescriptorEx(BBT_POINT, point);
	M_AddEnumToDescriptorEx(BBT_ORIENTED_COMMON, oriented_common);
	M_AddEnumToDescriptorEx(BBT_ORIENTED_SELF, oriented_self);
	M_AddEnumToDescriptorEx(BBT_PERPENDICULAR_COMMON, perpendicular_common);
	M_AddEnumToDescriptorEx(BBT_PERPENDICULAR_SELF, perpendicular_self);

	AbstractEnumDescriptor::Init();
}

static BillboardOriginConverter _BillboardOriginConverterInstance;

BillboardOriginConverter::BillboardOriginConverter()
{		
	M_AddEnumToDescriptorEx(BBO_TOP_LEFT, top_left);
	M_AddEnumToDescriptorEx(BBO_TOP_CENTER, top_center);
	M_AddEnumToDescriptorEx(BBO_TOP_RIGHT, top_right);
	M_AddEnumToDescriptorEx(BBO_CENTER_LEFT, center_left);
	M_AddEnumToDescriptorEx(BBO_CENTER, center);
	M_AddEnumToDescriptorEx(BBO_CENTER_RIGHT, center_right);
	M_AddEnumToDescriptorEx(BBO_BOTTOM_LEFT, bottom_left);
	M_AddEnumToDescriptorEx(BBO_BOTTOM_CENTER, bottom_center);
	M_AddEnumToDescriptorEx(BBO_BOTTOM_RIGHT, bottom_right);

	AbstractEnumDescriptor::Init();
}

static BillboardRotationTypeConverter _BillboardRotationTypeConverterInstance;

BillboardRotationTypeConverter::BillboardRotationTypeConverter()
{		
	M_AddEnumToDescriptorEx(BBR_VERTEX, vertex);
	M_AddEnumToDescriptorEx(BBR_TEXCOORD, texcoord);

	AbstractEnumDescriptor::Init();
}


const String& ParticleRenderEditor_Billboard::GetTypeName() const
{
	static String typeName = "billboard";
	return typeName;
}

void ParticleRenderEditor_Billboard::BuildPropertyMap()
{
	AddEnumProp(BillboardTypeConverter, BillboardType, BillboardType, ParticleRenderEditor_Billboard, "BillboardType", "BillboardRender");
	AddEnumProp(BillboardOriginConverter, BillboardOrigin, BillboardOrigin, ParticleRenderEditor_Billboard, "BillboardOrigin", "BillboardRender");
	AddEnumProp(BillboardRotationTypeConverter, BillboardRotationType, BillboardRotationType, ParticleRenderEditor_Billboard, "BillboardRotationType", "BillboardRender");
	
	AddProp(Property_Bool, UseAccurateFacing, ParticleRenderEditor_Billboard, "UseAccurateFacing", "BillboardRender");
	AddProp(Property_Vector3, CommonDirection, ParticleRenderEditor_Billboard, "CommonDirection", "BillboardRender");
	AddProp(Property_Vector3, CommonUpVector, ParticleRenderEditor_Billboard, "CommonUpVector", "BillboardRender");
	AddProp(Property_Bool, PointRenderingEnabled, ParticleRenderEditor_Billboard, "PointRenderingEnabled", "BillboardRender");
}

void ParticleRenderEditor_Billboard::SetBillboardType(BillboardType bbt)
{
	_render->setParameter("billboard_type", _BillboardTypeConverterInstance.ToString(bbt));
}

BillboardType ParticleRenderEditor_Billboard::GetBillboardType(void) const
{
	return (BillboardType)_BillboardTypeConverterInstance.FromString(
		_render->getParameter("billboard_type"));
}

void ParticleRenderEditor_Billboard::SetUseAccurateFacing(bool acc)
{
	_render->setParameter("accurate_facing", StringConverter::toString(acc));
}

bool ParticleRenderEditor_Billboard::GetUseAccurateFacing(void) const
{
	return StringConverter::parseBool(_render->getParameter("accurate_facing"));
}

void ParticleRenderEditor_Billboard::SetBillboardOrigin(BillboardOrigin origin)
{
	_render->setParameter("billboard_origin",
		_BillboardOriginConverterInstance.ToString(origin));
}

BillboardOrigin ParticleRenderEditor_Billboard::GetBillboardOrigin(void) const
{
	return (BillboardOrigin)_BillboardOriginConverterInstance.FromString(
		_render->getParameter("billboard_origin"));
}

void ParticleRenderEditor_Billboard::SetBillboardRotationType(BillboardRotationType rotationType)
{
	_render->setParameter("billboard_rotation_type",
		_BillboardRotationTypeConverterInstance.ToString(rotationType));
}

BillboardRotationType ParticleRenderEditor_Billboard::GetBillboardRotationType(void) const
{
	return (BillboardRotationType)_BillboardRotationTypeConverterInstance.FromString(
		_render->getParameter("billboard_rotation_type"));
}

void ParticleRenderEditor_Billboard::SetCommonDirection(const Vector3& vec)
{
	_render->setParameter("common_direction", StringConverter::toString(vec));
}

Vector3 ParticleRenderEditor_Billboard::GetCommonDirection(void) const
{
	return StringConverter::parseVector3(_render->getParameter("common_direction"));
}

void ParticleRenderEditor_Billboard::SetCommonUpVector(const Vector3& vec)
{
	_render->setParameter("common_up_vector", StringConverter::toString(vec));
}

Vector3 ParticleRenderEditor_Billboard::GetCommonUpVector(void) const
{
	return StringConverter::parseVector3(_render->getParameter("common_up_vector"));
}

void ParticleRenderEditor_Billboard::SetPointRenderingEnabled(bool enabled)
{
	_render->setParameter("point_rendering", StringConverter::toString(enabled));
}

bool ParticleRenderEditor_Billboard::GetPointRenderingEnabled(void) const
{
	return StringConverter::parseBool(_render->getParameter("point_rendering"));
}

//------------------------------------------------------------------------
M_REG_RENDER_EDITOR_FACTORY(ParticleRenderEditor_Mesh, ParticleRenderEditor_MeshFactory);

Ogre::NameValuePairList ParticleRenderEditor_MeshFactory::GetInitDefaultParameters()
{
	NameValuePairList parms =
		TParticleRenderEditorFactory<ParticleRenderEditor_Mesh>::GetInitDefaultParameters();

	parms["mesh_name"] = "axes.mesh";

	return parms;
}

static MeshOrientationTypeConverter _MeshOrientationTypeConverter;

MeshOrientationTypeConverter::MeshOrientationTypeConverter()
{		
	M_AddEnumToDescriptorEx(MOT_ORIENTED_ORIGIN, oriented_origin);
	M_AddEnumToDescriptorEx(MOT_ORIENTED_SELF, oriented_self);

	AbstractEnumDescriptor::Init();
}

const String& ParticleRenderEditor_Mesh::GetTypeName() const
{
	static String typeName = "mesh";
	return typeName;
}

void ParticleRenderEditor_Mesh::BuildPropertyMap()
{
	AddProp(Property_MeshFile, MeshName, ParticleRenderEditor_Mesh, "MeshName", "MeshRender");
	AddEnumProp(MeshOrientationTypeConverter, MeshOrientationType, MeshOrientationType, ParticleRenderEditor_Mesh, "MeshOrientationType", "MeshRender");

}

void ParticleRenderEditor_Mesh::SetMeshName( const String& meshName )
{
	_render->setParameter("mesh_name", meshName);
}

String ParticleRenderEditor_Mesh::GetMeshName( void ) const
{
	return _render->getParameter("mesh_name");
}

void ParticleRenderEditor_Mesh::SetMeshOrientationType( MeshOrientationType mot )
{
	_render->setParameter("orientation_type",
		_MeshOrientationTypeConverter.ToString(mot));
}

MeshOrientationType ParticleRenderEditor_Mesh::GetMeshOrientationType( void ) const
{
	return (MeshOrientationType)_MeshOrientationTypeConverter.FromString(
		_render->getParameter("orientation_type"));
}
//--------------------------------------------------------------------------------

M_REG_RENDER_EDITOR_FACTORY(ParticleRenderEditor_TexCoordBillboard, ParticleRenderEditor_TexCoordBillboardFactory);

const String& ParticleRenderEditor_TexCoordBillboard::GetTypeName() const
{
	static String typeName = "texcoord_billboard";
	return typeName;
}

void ParticleRenderEditor_TexCoordBillboard::BuildPropertyMap()
{
	ParticleRenderEditor_Billboard::BuildPropertyMap();

	AddProp(Property_Int, Stacks, ParticleRenderEditor_TexCoordBillboard, "Stacks", "TexCoordBillboardRender");
	AddProp(Property_Int, Slices, ParticleRenderEditor_TexCoordBillboard, "Slices", "TexCoordBillboardRender");
	AddProp(Property_Float, RepeatTimes, ParticleRenderEditor_TexCoordBillboard, "RepeatTimes", "TexCoordBillboardRender");
}

void ParticleRenderEditor_TexCoordBillboard::SetStacks( int val )
{
	_render->setParameter("stacks", StringConverter::toString(val));
}

int ParticleRenderEditor_TexCoordBillboard::GetStacks( void ) const
{
	return StringConverter::parseInt(_render->getParameter("stacks"));
}

void ParticleRenderEditor_TexCoordBillboard::SetSlices( int val )
{
	_render->setParameter("slices", StringConverter::toString(val));
}

int ParticleRenderEditor_TexCoordBillboard::GetSlices( void ) const
{
	return StringConverter::parseInt(_render->getParameter("slices"));
}

void ParticleRenderEditor_TexCoordBillboard::SetRepeatTimes( float val )
{
	_render->setParameter("repeat_times", StringConverter::toString(val));
}

float ParticleRenderEditor_TexCoordBillboard::GetRepeatTimes( void ) const
{
	return StringConverter::parseReal(_render->getParameter("repeat_times"));
}
//--------------------------------------------------------------------------------

M_REG_RENDER_EDITOR_FACTORY(ParticleRenderEditor_Ribbon, ParticleRenderEditor_RibbonFactory);

const String& ParticleRenderEditor_Ribbon::GetTypeName() const
{
	static String typeName = "ribbon";
	return typeName;
}

void ParticleRenderEditor_Ribbon::BuildPropertyMap()
{
	AddProp(Property_Float, RibbonHeadAlpha, ParticleRenderEditor_Ribbon, "RibbonHeadAlpha", "RibbonRender");
	AddProp(Property_Float, RibbonTailAlpha, ParticleRenderEditor_Ribbon, "RibbonTailAlpha", "RibbonRender");
	AddProp(Property_Float, RibbonHeadWidthScale, ParticleRenderEditor_Ribbon, "RibbonHeadWidthScale", "RibbonRender");
	AddProp(Property_Float, RibbonTailWidthScale, ParticleRenderEditor_Ribbon, "RibbonTailWidthScale", "RibbonRender");

	AddProp(Property_Int, RibbonElementCount, ParticleRenderEditor_Ribbon, "RibbonElementCount", "RibbonRender");
}

void ParticleRenderEditor_Ribbon::SetRibbonHeadAlpha( float val )
{
	_render->setParameter("head_alpha", StringConverter::toString(val));
}

float ParticleRenderEditor_Ribbon::GetRibbonHeadAlpha( void ) const
{
	return StringConverter::parseReal(_render->getParameter("head_alpha"));
}

void ParticleRenderEditor_Ribbon::SetRibbonTailAlpha( float val )
{
	_render->setParameter("tail_alpha", StringConverter::toString(val));
}

float ParticleRenderEditor_Ribbon::GetRibbonTailAlpha( void ) const
{
	return StringConverter::parseReal(_render->getParameter("tail_alpha"));
}

void ParticleRenderEditor_Ribbon::SetRibbonHeadWidthScale( float val )
{
	_render->setParameter("head_width_scale", StringConverter::toString(val));
}

float ParticleRenderEditor_Ribbon::GetRibbonHeadWidthScale( void ) const
{
	return StringConverter::parseReal(_render->getParameter("head_width_scale"));
}

void ParticleRenderEditor_Ribbon::SetRibbonTailWidthScale( float val )
{
	_render->setParameter("tail_width_scale", StringConverter::toString(val));
}

float ParticleRenderEditor_Ribbon::GetRibbonTailWidthScale( void ) const
{
	return StringConverter::parseReal(_render->getParameter("tail_width_scale"));
}

void ParticleRenderEditor_Ribbon::SetRibbonElementCount( int val )
{
	_render->setParameter("element_count", StringConverter::toString(val));
}

int ParticleRenderEditor_Ribbon::GetRibbonElementCount( void ) const
{
	return StringConverter::parseInt(_render->getParameter("element_count"));
}