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
#include "SceneEnvironment.h"
#include "GameObjectFactoryManager.h"
#include "GameApp.h"

template<> SceneEnvironment* Singleton<SceneEnvironment>::ms_Singleton = 0;

class SceneEnvironmentFactory : public TGameObjectFactory<SceneEnvironment>
{
public:

	virtual void Destroy(IGameObject* obj)
	{
		// 不要删除singleton
		//SDestroy<SceneEnvironment> d(obj);
	}

	virtual IGameObject* CreateImp(const XGUID& objID, const NameValuePairList* para = 0)
	{
		// 返回已经创建的singleton
		if(SceneEnvironment::getSingletonPtr())
		{
			return SceneEnvironment::getSingletonPtr();
		}

		IGameObject* obj = OGRE_NEW SceneEnvironment();
		obj->SetGUID(String("0xFF88FF88FFFFFFF0"));
		return obj;
	}

	virtual bool IsShownInEditorClassList() const
	{
		return true;
	}
};

M_REG_FACTORY(SceneEnvironment, SceneEnvironmentFactory, Scene);

static FogModeType _FogModeTypeInstance;

FogModeType::FogModeType()
{		
	M_AddEnumToDescriptorEx(FOG_NONE, None);
	M_AddEnumToDescriptorEx(FOG_EXP, EXP);
	M_AddEnumToDescriptorEx(FOG_EXP2, EXP2);
	M_AddEnumToDescriptorEx(FOG_LINEAR, Linear);

	AbstractEnumDescriptor::Init();
}
//------------------------------------------------------------------------

SceneEnvironment::SceneEnvironment(void)
	: _showBoundingBoxes(false)
	, _shadowUseInfiniteFarPlane(true)
	, _shadowTextureSize(512)
	, _shadowTextureFadeStart(0.7f)
	, _shadowTextureFadeEnd(0.9f)
	, _fogStart(0.0f)
	, _fogEnd(1.0f)
	, _fogDensity(0.001f)
{
}

SceneEnvironment::~SceneEnvironment(void)
{
}

void SceneEnvironment::BuildPropertyMap()
{
	//AbstractGameObject::BuildPropertyMap();
	_propMap.clear();

	AddProp(Property_ColourValue, AmbientLight, SceneEnvironment, "AmbientLight", "SceneEnvironment");

	AddProp(Property_Bool, DisplaySceneNodes, SceneEnvironment, "DisplaySceneNodes", "Debug");
	AddProp(Property_Bool, FindVisibleObjects, SceneEnvironment, "FindVisibleObjects", "Debug");
	AddProp(Property_Bool, FlipCullingOnNegativeScale, SceneEnvironment, "FlipCullingOnNegativeScale", "Debug");
	AddProp(Property_Bool, ShowBoundingBoxes, SceneEnvironment, "ShowBoundingBoxes", "Debug");

	AddEnumProp(FogModeType, FogMode, FogType, SceneEnvironment, "FogType", "Fog");
	AddProp(Property_ColourValue, FogColour, SceneEnvironment, "FogColour", "Fog");		
	AddProp(Property_Float, FogDensity, SceneEnvironment, "FogDensity", "Fog");
	AddProp(Property_Float, FogStart, SceneEnvironment, "FogStart", "Fog");
	AddProp(Property_Float, FogEnd, SceneEnvironment, "FogEnd", "Fog");

	AddEnumProp(ShadowTechniqueType, ShadowTechnique, ShadowTechnique, SceneEnvironment, "ShadowTechniqueType", "Shadow");
	AddProp(Property_ColourValue, ShadowColour, SceneEnvironment, "ShadowColour", "Shadow");
	AddProp(Property_Bool, ShadowCasterRenderBackFaces, SceneEnvironment, "ShadowCasterRenderBackFaces", "Shadow");
	AddProp(Property_Float, ShadowDirectionalLightExtrusionDistance, SceneEnvironment, "ShadowDirectionalLightExtrusionDistance", "Shadow");
	AddProp(Property_Float, ShadowDirLightTextureOffset, SceneEnvironment, "ShadowDirLightTextureOffset", "Shadow");
	AddProp(Property_Float, ShadowFarDistance, SceneEnvironment, "ShadowFarDistance", "Shadow");
	AddProp(Property_Int, ShadowIndexBufferSize, SceneEnvironment, "ShadowIndexBufferSize", "Shadow");
	AddProp(Property_Material, ShadowTextureCasterMaterial, SceneEnvironment, "ShadowTextureCasterMaterial", "Shadow");
	AddProp(Property_Material, ShadowTextureReceiverMaterial, SceneEnvironment, "ShadowTextureReceiverMaterial", "Shadow");
	AddProp(Property_Int, ShadowTextureCount, SceneEnvironment, "ShadowTextureCount", "Shadow");
	AddProp(Property_Float, ShadowTextureFadeEnd, SceneEnvironment, "ShadowTextureCount", "Shadow");
	AddProp(Property_Float, ShadowTextureFadeStart, SceneEnvironment, "ShadowTextureFadeStart", "Shadow");
	AddProp(Property_Bool, ShadowTextureSelfShadow, SceneEnvironment, "ShadowTextureSelfShadow", "Shadow");
	AddProp(Property_Int, ShadowTextureSize, SceneEnvironment, "AmbientLight", "ShadowTextureSize");
	AddProp(Property_Bool, ShadowUseInfiniteFarPlane, SceneEnvironment, "ShadowUseInfiniteFarPlane", "Shadow");
	AddProp(Property_Bool, ShadowUseLightClipPlanes, SceneEnvironment, "ShadowUseLightClipPlanes", "Shadow");
	AddProp(Property_Bool, ShowDebugShadows, SceneEnvironment, "ShowDebugShadows", "Shadow");

}

const String& SceneEnvironment::GetTypeName() const
{
	static String sName("SceneEnvironment");
	return sName;
}

SceneManager* SceneEnvironment::GetSceneManager() const
{
	IGame* game = GameApp::getSingleton().GetGame();
	SceneManager* sm = const_cast<SceneManager*>(game->GetVisualScene());
	return sm;
}

void SceneEnvironment::SetAmbientLight(const ColourValue& clr)
{
	SceneManager* sm = GetSceneManager();
	sm->setAmbientLight(clr);
}

void SceneEnvironment::SetShadowTechnique(ShadowTechnique st)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowTechnique(st);
}

const ColourValue& SceneEnvironment::GetAmbientLight() const
{
	SceneManager* sm = GetSceneManager();
	return sm->getAmbientLight();
}

ShadowTechnique SceneEnvironment::GetShadowTechnique() const
{
	SceneManager* sm = GetSceneManager();
	return sm->getShadowTechnique();
}

void SceneEnvironment::SetShadowCasterRenderBackFaces(bool bf)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowCasterRenderBackFaces(bf);
}

bool SceneEnvironment::GetShadowCasterRenderBackFaces()
{
	SceneManager* sm = GetSceneManager();
	return sm->getShadowCasterRenderBackFaces();
}

void SceneEnvironment::SetDisplaySceneNodes(bool display)
{
	SceneManager* sm = GetSceneManager();
	sm->setDisplaySceneNodes(display);
}

bool SceneEnvironment::GetDisplaySceneNodes()
{
	SceneManager* sm = GetSceneManager();
	return sm->getDisplaySceneNodes();
}

void SceneEnvironment::SetFindVisibleObjects(bool find)
{
	SceneManager* sm = GetSceneManager();
	sm->setFindVisibleObjects(find);
}

bool SceneEnvironment::GetFindVisibleObjects()
{
	SceneManager* sm = GetSceneManager();
	return sm->getFindVisibleObjects();
}

void SceneEnvironment::SetFlipCullingOnNegativeScale(bool on)
{
	SceneManager* sm = GetSceneManager();
	sm->setFlipCullingOnNegativeScale(on);
}

bool SceneEnvironment::GetFlipCullingOnNegativeScale()
{
	SceneManager* sm = GetSceneManager();
	return sm->getFlipCullingOnNegativeScale();
}

FogMode SceneEnvironment::GetFogType() const
{
	SceneManager* sm = GetSceneManager();
	return sm->getFogMode();
}

void SceneEnvironment::SetFogType(FogMode st)
{
	SceneManager* sm = GetSceneManager();
	sm->setFog(st, GetFogColour(), GetFogDensity(), GetFogStart(), GetFogEnd());
}

const ColourValue& SceneEnvironment::GetFogColour() const
{
	SceneManager* sm = GetSceneManager();
	return sm->getFogColour();
}

void SceneEnvironment::SetFogColour(const ColourValue& clr)
{
	SceneManager* sm = GetSceneManager();
	sm->setFog(GetFogType(), clr, GetFogDensity(), GetFogStart(), GetFogEnd());
}

void SceneEnvironment::SetFogDensity(float val)
{
	_fogDensity = val;
	SceneManager* sm = GetSceneManager();
	sm->setFog(GetFogType(), GetFogColour(), val, GetFogStart(), GetFogEnd());
}

float SceneEnvironment::GetFogDensity()
{
	return _fogDensity;
}

void SceneEnvironment::SetFogStart(float val)
{
	_fogStart = val;
	SceneManager* sm = GetSceneManager();
	sm->setFog(GetFogType(), GetFogColour(), GetFogDensity(), val, GetFogEnd());
}

float SceneEnvironment::GetFogStart()
{
	return _fogStart;
}

void SceneEnvironment::SetFogEnd(float val)
{
	_fogEnd = val;
	SceneManager* sm = GetSceneManager();
	sm->setFog(GetFogType(), GetFogColour(), GetFogDensity(), GetFogStart(), val);
}

float SceneEnvironment::GetFogEnd()
{
	return _fogEnd;
}

const ColourValue& SceneEnvironment::GetShadowColour() const
{
	SceneManager* sm = GetSceneManager();
	return sm->getShadowColour();
}

void SceneEnvironment::SetShadowColour(const ColourValue& clr)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowColour(clr);
}

void SceneEnvironment::SetShadowDirectionalLightExtrusionDistance(float val)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowDirectionalLightExtrusionDistance(val);
}

float SceneEnvironment::GetShadowDirectionalLightExtrusionDistance()
{
	SceneManager* sm = GetSceneManager();
	return sm->getShadowDirectionalLightExtrusionDistance();
}

void SceneEnvironment::SetShadowDirLightTextureOffset(float val)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowDirLightTextureOffset(val);
}

float SceneEnvironment::GetShadowDirLightTextureOffset()
{
	SceneManager* sm = GetSceneManager();
	return sm->getShadowDirLightTextureOffset();
}

void SceneEnvironment::SetShadowFarDistance(float val)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowFarDistance(val);
}

float SceneEnvironment::GetShadowFarDistance()
{
	SceneManager* sm = GetSceneManager();
	return sm->getShadowFarDistance();
}

void SceneEnvironment::SetShadowIndexBufferSize(size_t val)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowIndexBufferSize(val);
}

size_t SceneEnvironment::GetShadowIndexBufferSize()
{
	SceneManager* sm = GetSceneManager();
	return sm->getShadowIndexBufferSize();
}

void SceneEnvironment::SetShadowTextureCasterMaterial(const String& name)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowTextureCasterMaterial(name);
	_shadowTextureCasterMaterial = name;
}

const String& SceneEnvironment::GetShadowTextureCasterMaterial()
{
	return _shadowTextureCasterMaterial;
}

void SceneEnvironment::SetShadowTextureReceiverMaterial(const String& name)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowTextureReceiverMaterial(name);
	_shadowTextureReceiverMaterial = name;
}

const String& SceneEnvironment::GetShadowTextureReceiverMaterial()
{
	return _shadowTextureReceiverMaterial;
}

void SceneEnvironment::SetShadowTextureCount(size_t val)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowTextureCount(val);
}

size_t SceneEnvironment::GetShadowTextureCount()
{
	SceneManager* sm = GetSceneManager();
	return sm->getShadowTextureCount();
}

void SceneEnvironment::SetShadowTextureFadeEnd(float val)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowTextureFadeEnd(val);
	_shadowTextureFadeEnd = val;
}

float SceneEnvironment::GetShadowTextureFadeEnd()
{
	return _shadowTextureFadeEnd;
}

void SceneEnvironment::SetShadowTextureFadeStart(float val)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowTextureFadeStart(val);
	_shadowTextureFadeStart = val;
}

float SceneEnvironment::GetShadowTextureFadeStart()
{
	return _shadowTextureFadeStart;
}

void SceneEnvironment::SetShadowTextureSelfShadow(bool on)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowTextureSelfShadow(on);
}

bool SceneEnvironment::GetShadowTextureSelfShadow()
{
	SceneManager* sm = GetSceneManager();
	return sm->getShadowTextureSelfShadow();
}

void SceneEnvironment::SetShadowTextureSize(int val)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowTextureSize(val);
	_shadowTextureSize = val;
}

int SceneEnvironment::GetShadowTextureSize()
{
	return _shadowTextureSize;
}

void SceneEnvironment::SetShadowUseInfiniteFarPlane(bool on)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowUseInfiniteFarPlane(on);
	_shadowUseInfiniteFarPlane = on;
}

bool SceneEnvironment::GetShadowUseInfiniteFarPlane()
{
	return _shadowUseInfiniteFarPlane;
}

void SceneEnvironment::SetShadowUseLightClipPlanes(bool on)
{
	SceneManager* sm = GetSceneManager();
	sm->setShadowUseLightClipPlanes(on);
}

bool SceneEnvironment::GetShadowUseLightClipPlanes()
{
	SceneManager* sm = GetSceneManager();
	return sm->getShadowUseLightClipPlanes();
}

void SceneEnvironment::SetShowDebugShadows(bool on)
{
	SceneManager* sm = GetSceneManager();
	sm->setShowDebugShadows(on);
}

bool SceneEnvironment::GetShowDebugShadows()
{
	SceneManager* sm = GetSceneManager();
	return sm->getShowDebugShadows();
}

void SceneEnvironment::SetShowBoundingBoxes(bool on)
{
	SceneManager* sm = GetSceneManager();
	sm->showBoundingBoxes(on);
	_showBoundingBoxes = on;
}

bool SceneEnvironment::GetShowBoundingBoxes()
{
	return _showBoundingBoxes;
}
