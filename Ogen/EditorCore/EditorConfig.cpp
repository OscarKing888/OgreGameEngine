#include "stdafx.h"
#include "EditorConfig.h"
#include <OgreCodec.h>

static ECameraSpeedConverter _ECameraSpeedConverterInstance;
static ESnapSizeConverter _ESnapSizeConverterInstance;

ECameraSpeedConverter::ECameraSpeedConverter()
{		
	M_AddEnumToDescriptorEx(eCS_Slow, Slow);
	M_AddEnumToDescriptorEx(eCS_Normal, Normal);
	M_AddEnumToDescriptorEx(eCS_Fast, Fast);

	AbstractEnumDescriptor::Init();
}

ECameraSpeedConverter::~ECameraSpeedConverter()
{
}

//--------------------------------------------------------------------------------

ESnapSizeConverter::ESnapSizeConverter()
{		
	M_AddEnumToDescriptorEx(eSS_256, 256);
	M_AddEnumToDescriptorEx(eSS_512, 512);
	M_AddEnumToDescriptorEx(eSS_1024, 1024);
	M_AddEnumToDescriptorEx(eSS_2048, 2048);
	M_AddEnumToDescriptorEx(eSS_4096, 4096);
	M_AddEnumToDescriptorEx(eSS_8192, 8192);

	AbstractEnumDescriptor::Init();
}

ESnapSizeConverter::~ESnapSizeConverter()
{
}

//--------------------------------------------------------------------------------
// EditorConfig
//--------------------------------------------------------------------------------
EditorConfig::EditorConfig()
: _viewportColour(0.49f, 0.49f, 0.49f)
, _autoClampTerrainOnMove(true)
, _autoAlignTerrainNormal(false)
, _randomOrientationOnCreate(true)
, _randomScaleOnCreate(false)
, _randomScaleRange(0.8f, 1.2f)
, _translateUIToLocal(true)
, _cameraMoveSpeed(eCS_Normal)
, PickFilter(0xFFFFFFFF)
, _snapshotAngle(Degree(45.0f))
, _snapshotTexSize(eSS_2048)
, _snapshotFov(Degree(45.0f))
, _imageType("bmp")
, _camOffsetScale(1.0f, 0.7f, 1.4f)
, _cameraHeight(180.0f)
, _clampCameraToTerrain(false)
, _showSnapGrid(false)
, _snapGridSize(500, 500)
{
	BuildPropertyMap();
}
//--------------------------------------------------------------------------------
void EditorConfig::LoadConfig()
{
	ConfigFile cfg;
	cfg.loadFromResourceSystem("Editor.cfg", "Editor");
	_translateUIToLocal = StringConverter::parseBool(cfg.getSetting("TranslateUIToLocal"));
}
//------------------------------------------------------------------------
void EditorConfig::BuildPropertyMap()
{	
	AddProp(Property_Bool, TranslateUIToLocal, EditorConfig, "TranslateUIToLocal", "UI");
	
	AddProp(Property_ColourValue, ViewportColour, EditorConfig, "ViewportColour", "View");

	AddEnumProp(ECameraSpeedConverter, ECameraSpeed, CameraMoveSpeed, EditorConfig, "CameraMoveSpeed", "Camera");
	AddProp(Property_Bool, ClampCameraToTerrain, EditorConfig, "ClampCameraToTerrain", "Camera");
	AddProp(Property_Float, CameraHeight, EditorConfig, "CameraHeight", "Camera");


	AddProp(Property_Bool, AutoClampTerrainOnMove, EditorConfig, "AutoClampTerrainOnMove", "Move");
	AddProp(Property_Bool, AutoAlignTerrainNormal, EditorConfig, "AutoAlignTerrainNormal", "Move");
	AddProp(Property_Bool, SnapToGrid, EditorConfig, "SnapToGrid", "Move");
	AddProp(Property_Bool, ShowSnapGrid, EditorConfig, "ShowSnapGrid", "Move");
	AddProp(Property_Vector2, SnapGridSize, EditorConfig, "SnapGridSize", "Move");

	AddProp(Property_Bool, RandomOrientationOnCreate, EditorConfig, "RandomOrientationOnCreate", "Create");
	AddProp(Property_Bool, RandomScaleOnCreate, EditorConfig, "RandomScaleOnCreate", "Create");
	AddProp(Property_Vector2, RandomScaleRange, EditorConfig, "RandomScaleRange", "Create");


	AddProp(Property_Radian, SnapshotAngle, EditorConfig, "SnapshotAngle", "Snapshot");
	AddProp(Property_Radian, SnapshotFov, EditorConfig, "SnapshotFov", "Snapshot");
	AddEnumProp(ESnapSizeConverter, ESnapSize, SnapshotSize, EditorConfig, "SnapshotSize", "Snapshot");
	IPropertyPtr prop = AddProp(Property_StringVector, ImageTypeList, EditorConfig, "ImageTypeList", "Snapshot");
	prop->SetFlag(0);
	AddStringPropWithSourceList(ImageType, ImageTypeList, EditorConfig, "ImageType", "Snapshot");
	AddProp(Property_Vector3, CameraOffsetScale, EditorConfig, "CameraOffsetScale", "Snapshot");
	
}
//--------------------------------------------------------------------------------
const Radian EditorConfig::GetSnapshotAngle() const
{
	return _snapshotAngle;
}
//--------------------------------------------------------------------------------
void EditorConfig::SetSnapshotAngle(const Radian& ang)
{
	_snapshotAngle = ang;
}
//--------------------------------------------------------------------------------
ESnapSize EditorConfig::GetSnapshotSize() const
{
	return _snapshotTexSize;
}
//--------------------------------------------------------------------------------
void EditorConfig::SetSnapshotSize(ESnapSize sz)
{
	_snapshotTexSize = sz;
}
//--------------------------------------------------------------------------------
const Radian EditorConfig::GetSnapshotFov() const
{
	return _snapshotFov;
}
//--------------------------------------------------------------------------------
void EditorConfig::SetSnapshotFov(const Radian& ang)
{
	_snapshotFov = ang;
}
//--------------------------------------------------------------------------------
String EditorConfig::GetImageType() const
{
	return _imageType;
}
//--------------------------------------------------------------------------------
void EditorConfig::SetImageType(const String& t)
{
	_imageType = t;
}
//--------------------------------------------------------------------------------
StringVector EditorConfig::GetImageTypeList() const
{
	return Codec::getExtensions();
}
//--------------------------------------------------------------------------------
void EditorConfig::SetImageTypeList(const StringVector& lst)
{

}
//--------------------------------------------------------------------------------
Vector3 EditorConfig::GetCameraOffsetScale() const
{
	return _camOffsetScale;
}
//--------------------------------------------------------------------------------
void EditorConfig::SetCameraOffsetScale(const Vector3& o)
{
	_camOffsetScale = o;
}
//--------------------------------------------------------------------------------
