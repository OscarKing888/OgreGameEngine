// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-12-03
// *************************************************************************************

#include "StdAfx.h"
#include "SkyBox.h"
#include "GameApp.h"

template<> SkyBox* Singleton<SkyBox>::ms_Singleton = 0;
template<> SkyType* Singleton<SkyType>::ms_Singleton = 0;

class SkyBoxFactory : public TGameObjectFactory<SkyBox>
{
public:

	virtual void Destroy(IGameObject* obj)
	{
		// 不要删除singleton
		//SDestroy<SkyBox> d(obj);
	}

	virtual IGameObject* CreateImp(const XGUID& objID, const NameValuePairList* para = 0)
	{
		// 返回已经创建的singleton
		if(SkyBox::getSingletonPtr())
		{
			return SkyBox::getSingletonPtr();
		}

		IGameObject* obj = OGRE_NEW SkyBox();
		obj->SetGUID(String("0xFF88FF88FFFFFFF2"));
		return obj;
	}

	virtual bool IsShownInEditorClassList() const
	{
		return true;
	}
};



M_REG_FACTORY(SkyBox, SkyBoxFactory, Scene);

static SkyType _SkyTypeInstance;

SkyType::SkyType()
{		
	M_AddEnumToDescriptorEx(EST_None, None);
	M_AddEnumToDescriptorEx(EST_Box, Box);
	M_AddEnumToDescriptorEx(EST_Dome, Dome);
	M_AddEnumToDescriptorEx(EST_Plane, Plane);

	AbstractEnumDescriptor::Init();
}

//------------------------------------------------------------------------

SkyBox::SkyBox(void)
: _skyType(EST_None)
, _skyBoxMaterial("DefaultSkyBox")
, _skyDomeMaterial("DefaultSkyDome")
, _skyPlaneMaterial("DefaultSkyPlane")
, _enableSkyBox(false)
, _enableSkyDome(false)
, _enableSkyPlane(false)
, _skyDistance(5000.0f)
, _skyPlaneScale(1000)
, _skyPlaneTiling(10)
, _skyPlaneBow(0)
, _skyPlaneXSegments(1)
, _skyPlaneYSegments(1)
, _skyDomeCurvature(10)
, _skyDomeTiling(8)
, _skyDomeXSegments(16)
, _skyDomeYSegments(16)
,_skyDomeYSegmentsKeep(-1)
{
	SceneManager* sm = GetSceneManager();
	if(sm->isSkyBoxEnabled())
	{
		_skyType = EST_Box;
	}
	else if(sm->isSkyDomeEnabled())
	{
		_skyType = EST_Dome;
	}
	else if(sm->isSkyPlaneEnabled())
	{
		_skyType = EST_Plane;
	}
}

SkyBox::~SkyBox(void)
{
}

void SkyBox::BuildPropertyMap()
{
	//AbstractGameObject::BuildPropertyMap();
	_propMap.clear();
	AddEnumProp(SkyType, ESkyType, SkyType, SkyBox, "MapHeight", "Sky");
	AddProp(Property_Material, SkyBoxMaterial, SkyBox, "SkyBoxMaterial", "SkyBox");
	//AddProp(Property_Bool, EnableSkyBox, SkyBox, "MapHeight", "SceneInfo");
	//AddProp(Property_Bool, EnableSkyDome, SkyBox, "MapHeight", "SceneInfo");
	//AddProp(Property_Bool, EnableSkyPlane, SkyBox, "MapHeight", "SceneInfo");

	AddProp(Property_Material, SkyPlaneMaterial, SkyBox, "SkyPlaneMaterial", "SkyPlane");
	AddProp(Property_Float, SkyDistance			, SkyBox, "SkyDistance", "SkyPlane");
	AddProp(Property_Float, SkyPlaneScale		, SkyBox, "SkyPlaneScale", "SkyPlane");
	AddProp(Property_Float, SkyPlaneTiling		, SkyBox, "SkyPlaneTiling", "SkyPlane");
	AddProp(Property_Float, SkyPlaneBow			, SkyBox, "SkyPlaneBow", "SkyPlane");
	AddProp(Property_Int,	SkyPlaneXSegments	, SkyBox, "SkyPlaneXSegments", "SkyPlane");
	AddProp(Property_Int,	SkyPlaneYSegments	, SkyBox, "SkyPlaneYSegments", "SkyPlane");

	AddProp(Property_Material, SkyDomeMaterial, SkyBox, "SkyDomeMaterial", "SkyDome");
	AddProp(Property_Float, SkyDomeCurvature	, SkyBox, "SkyDomeCurvature", "SkyDome");
	AddProp(Property_Float, SkyDomeTiling		, SkyBox, "SkyDomeTiling", "SkyDome");
	AddProp(Property_Int,	SkyDomeXSegments	, SkyBox, "SkyDomeXSegments", "SkyDome");
	AddProp(Property_Int,	SkyDomeYSegments	, SkyBox, "SkyDomeYSegments", "SkyDome");
	AddProp(Property_Int,	SkyDomeYSegmentsKeep, SkyBox, "SkyDomeYSegmentsKeep", "SkyDome");
}

const String& SkyBox::GetTypeName() const
{
	static String sName("SkyBox");
	return sName;
}

SceneManager* SkyBox::GetSceneManager()
{
	IGame* game = GameApp::getSingleton().GetGame();
	SceneManager* sm = game->GetVisualScene();
	return sm;
}

void SkyBox::SetSkyType(ESkyType skt)
{
	_skyType = skt;

	// disable sky box

	SceneManager* sm = GetSceneManager();
	if(_skyType != EST_Box)
	{
		sm->setSkyBox(false, _skyBoxMaterial);
	}

	if(_skyType != EST_Dome)
	{
		sm->setSkyDome(false, _skyDomeMaterial);
	}

	if(_skyType != EST_Plane)
	{
		Plane plane(Vector3::NEGATIVE_UNIT_Y, Vector3::ZERO);
		sm->setSkyPlane(false, plane, _skyPlaneMaterial);
	}

	// enable sky box
	if(_skyType == EST_Box)
	{
		sm->setSkyBox(true, _skyBoxMaterial, _skyDistance);
	}
	else if(_skyType == EST_Dome)
	{
		sm->setSkyDome(true, _skyDomeMaterial, _skyDistance);
	}
	else if(_skyType == EST_Plane)
	{
		Plane plane;
		plane.d = _skyDistance;
		plane.normal = -Vector3::UNIT_Y;

		sm->setSkyPlane(true, plane, _skyPlaneMaterial);
	}
}

void SkyBox::SetSkyBoxMaterial(const String& mat)
{
	_skyBoxMaterial = mat;
	SetSkyType(_skyType);
}

void SkyBox::SetSkyDomeMaterial(const String& mat)
{
	_skyDomeMaterial = mat;
	SetSkyType(_skyType);
}

void SkyBox::SetSkyPlaneMaterial(const String& mat)
{
	_skyPlaneMaterial = mat;
	SetSkyType(_skyType);
}

void SkyBox::SetEnableSkyBox(bool on)
{
	SceneManager* sm = GetSceneManager();
	sm->setSkyBox(on, _skyBoxMaterial);
}

void SkyBox::SetEnableSkyDome(bool on)
{
	SceneManager* sm = GetSceneManager();
	sm->setSkyDome(on, _skyDomeMaterial);
}

void SkyBox::SetEnableSkyPlane(bool on)
{
	SceneManager* sm = GetSceneManager();
	Plane plane;
	plane.d = 5000;
	plane.normal = -Vector3::UNIT_Y;

	sm->setSkyPlane(on, plane, _skyPlaneMaterial);
}

void SkyBox::SetSkyDistance(float val)
{
	_skyDistance = val;
	SetSkyType(_skyType);
}

void SkyBox::SetSkyPlaneScale(float val)
{
	_skyPlaneScale = val;
	SetSkyType(_skyType);
}

void SkyBox::SetSkyPlaneTiling(float val)
{
	_skyPlaneTiling = val;
	SetSkyType(_skyType);
}

void SkyBox::SetSkyPlaneBow(float val)
{
	_skyPlaneBow = val;
	SetSkyType(_skyType);
}

void SkyBox::SetSkyPlaneXSegments(int val)
{
	_skyPlaneXSegments = val;
	SetSkyType(_skyType);
}

void SkyBox::SetSkyPlaneYSegments(int val)
{
	_skyPlaneYSegments = val;
	SetSkyType(_skyType);
}

void SkyBox::SetSkyDomeCurvature(float val)
{
	_skyDomeCurvature = val;
	SetSkyType(_skyType);
}

void SkyBox::SetSkyDomeTiling(float val)
{
	_skyDomeTiling = val;
	SetSkyType(_skyType);
}

void SkyBox::SetSkyDomeXSegments(int val)
{
	_skyDomeXSegments = val;
	SetSkyType(_skyType);
}

void SkyBox::SetSkyDomeYSegments(int val)
{
	_skyDomeYSegments = val;
	SetSkyType(_skyType);
}

void SkyBox::SetSkyDomeYSegmentsKeep(int val)
{
	_skyDomeYSegmentsKeep = val;
	SetSkyType(_skyType);
}
