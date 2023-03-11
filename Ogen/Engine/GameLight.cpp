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
#include "GameLight.h"
#include "GameApp.h"
#include "GameObjectFactoryManager.h"
#include "IGame.h"
#include "Property_String.h"
#include "GameLightStates.h"

template<> LightType* Singleton<LightType>::ms_Singleton = 0;
template<> MainLight* Singleton<MainLight>::ms_Singleton = 0;

class GameLightFactory : public TGameObjectFactory<GameLight>
{
};

M_REG_FACTORY(GameLight, GameLightFactory, VisualObject);

static LightType _LightTypeInstance;

LightType::LightType()
{		
	M_AddEnumToDescriptorEx(Light::LT_POINT, Point);
	M_AddEnumToDescriptorEx(Light::LT_DIRECTIONAL, Directional);
	M_AddEnumToDescriptorEx(Light::LT_SPOTLIGHT, Spot);

	AbstractEnumDescriptor::Init();
}


GameLight::GameLight(void)
: _entity(0)
, _light(0)
, _billboard(0)
{
	_isEditorDislpayOnly = true;

	_stateManager->AddState(OGRE_NEW GameLightState_On(this));
	_stateManager->AddState(OGRE_NEW GameLightState_Off(this));
}

GameLight::~GameLight(void)
{
}

void GameLight::BuildPropertyMap()
{
	VisualObject::BuildPropertyMap();

	AddEnumProp(LightType, Light::LightTypes, LightType, GameLight, "LightType", "Light");
	AddProp(Property_ColourValue, DiffuseColour, GameLight, "DiffuseColour", "Light");
	AddProp(Property_ColourValue, SpecularColour, GameLight, "SpecularColour", "Light");
	AddProp(Property_Float, AttenuationRange, GameLight, "AttenuationRange", "Light");
	AddProp(Property_Float, AttenuationConstant, GameLight, "AttenuationConstant", "Light");
	AddProp(Property_Float, AttenuationLinear, GameLight, "AttenuationLinear", "Light");
	AddProp(Property_Float, AttenuationQuadric, GameLight, "AttenuationQuadric", "Light");
	AddProp(Property_Vector3, Direction, GameLight, "Direction", "Light");
	AddProp(Property_Float, SpotlightInnerAngle, GameLight, "SpotlightInnerAngle", "Light");
	AddProp(Property_Float, SpotlightOuterAngle, GameLight, "SpotlightOuterAngle", "Light");
	AddProp(Property_Float, SpotlightFalloff, GameLight, "SpotlightFalloff", "Light");
	AddProp(Property_Float, PowerScale, GameLight, "PowerScale", "Light");
	AddProp(Property_Bool, On, GameLight, "On", "Light");
}

const String& GameLight::GetTypeName() const
{
	static String sName("Light");
	return sName;
}

void GameLight::PostCreateNode()
{
	VisualObject::PostCreateNode();
	CreateLight();
}

void GameLight::PreDestroyNode()
{
	VisualObject::PreDestroyNode();
	DestroyLight();
}

void GameLight::CreateLight()
{
	IGame* game = GameApp::getSingleton().GetGame();
	SceneManager* sm = game->GetVisualScene();
	_light = sm->createLight(GetGUID().toString());
	_light->setUserAny(Any(AnyGameObject(this)));
	_light->setQueryFlags(GetQueryFlag());
	SetAttenuationRange(2000.0f); // 默认20米远影响
	_node->attachObject(_light);

	if((GameApp::CurrentRunType == ERT_Editor) || _displayEditorObjectInGame)
	{
		_billboard = sm->createBillboardSet(GetGUID().toString() + ".Helper", 1);
		_billboard->setDefaultDimensions(50, 50);
		Billboard* bb = _billboard->createBillboard(Vector3::ZERO);
		(void*)(bb);
		//bs->setBillboardType(BillboardType::BBT_ORIENTED_SELF);
		if(_lightMat.isNull())
		{
			MaterialPtr srcMat = MaterialPtr(MaterialManager::getSingleton().getByName("__EditorLight__"));
			_lightMat = srcMat->clone(GetGUID().toString() + ".HelperMaterial");
			_lightMat->setSpecular(ColourValue::ZERO);
			SetLightType(_light->getType());
			SetDiffuseColour(_light->getDiffuseColour());
			//_lightMat->getTechnique(0)->getPass(0)->setVertexColourTracking(TVC_DIFFUSE);
		}

		_billboard->setMaterialName(_lightMat->getName());
		//_light->setQueryFlags(MovableObject::getDefaultQueryFlags());
		_billboard->setQueryFlags(MovableObject::getDefaultQueryFlags());
		//_billboard->setQueryFlags(GetQueryFlag());
		_billboard->MovableObject::setUserAny(Any(AnyGameObject(this)));
		_node->attachObject(_billboard);
		//_entity = sm->createEntity(GetGUID().toString() + ".Helper", SceneManager::PT_SPHERE); // "EditorCone.mesh"
		//_entity->setUserAny(Any(this));
		//_entity->setQueryFlags(MovableObject::getDefaultQueryFlags()); // GameObjectFactoryManager::getSingleton().GetQueryFlag<Light>()
		//_node->attachObject(_entity);
	}
}

void GameLight::DestroyLight()
{
	IGame* game = GameApp::getSingleton().GetGame();
	SceneManager* sm = game->GetVisualScene();
	sm->destroyLight(_light);
	_light = 0;

	if(_entity)
	{
		sm->destroyEntity(_entity);
		_entity = 0;
	}

	if(_billboard)
	{
		sm->destroyBillboardSet(_billboard);
		_billboard = 0;
	}
}

void GameLight::OnEditorSelect()
{

}

void GameLight::OnEditorDeselect()
{

}

Light::LightTypes GameLight::GetLightType() const
{
	return _light->getType();
}

void GameLight::SetLightType(Light::LightTypes lt)
{
	_light->setType(lt);

	if((GameApp::CurrentRunType == ERT_Editor) || _displayEditorObjectInGame)
	{
		if(!_lightMat.isNull())
		{
			String matTex[] = 
			{
				"Light_Point.tga",
				"Light_Directional.tga",
				"Light_Spot.tga"
			};

			_lightMat->getTechnique(0)->getPass(0)
				->getTextureUnitState(0)->setTextureName(
				matTex[lt]);
		}
	}
}

void GameLight::SetDiffuseColour(const ColourValue& clr)
{
	_light->setDiffuseColour(clr);

	if((GameApp::CurrentRunType == ERT_Editor) || _displayEditorObjectInGame)
	{
		if(!_lightMat.isNull())
		{
			_lightMat->setSelfIllumination(clr);
			_lightMat->setDiffuse(clr);
			_lightMat->setAmbient(clr);
		}
	}
}

const ColourValue& GameLight::GetDiffuseColour() const
{
	return _light->getDiffuseColour();
}

void GameLight::SetSpecularColour(const ColourValue& clr)
{
	_light->setSpecularColour(clr);
}

const ColourValue& GameLight::GetSpecularColour() const
{
	return _light->getSpecularColour();
}

float GameLight::GetAttenuationRange(void) const
{
	return _light->getAttenuationRange();
}

float GameLight::GetAttenuationConstant(void) const
{
	return _light->getAttenuationConstant();
}

float GameLight::GetAttenuationLinear(void) const
{
	return _light->getAttenuationLinear();
}

float GameLight::GetAttenuationQuadric(void) const
{
	return _light->getAttenuationQuadric();
}

void GameLight::SetAttenuationRange(float val)
{
	_light->setAttenuation(val, GetAttenuationConstant(), GetAttenuationLinear(), GetAttenuationQuadric());
}

void GameLight::SetAttenuationConstant(float val)
{
	_light->setAttenuation(GetAttenuationRange(), val, GetAttenuationLinear(), GetAttenuationQuadric());
}

void GameLight::SetAttenuationLinear(float val)
{
	_light->setAttenuation(GetAttenuationRange(), GetAttenuationConstant(), val, GetAttenuationQuadric());
}

void GameLight::SetAttenuationQuadric(float val)
{
	_light->setAttenuation(GetAttenuationRange(), GetAttenuationConstant(), GetAttenuationLinear(), val);
}

void GameLight::SetDirection(const Vector3& vec)
{
	_light->setDirection(vec);
}

const Vector3& GameLight::GetDirection(void) const
{
	return _light->getDirection();
}

float GameLight::GetSpotlightInnerAngle(void) const
{
	return _light->getSpotlightInnerAngle().valueRadians();
}

float GameLight::GetSpotlightOuterAngle(void) const
{
	return _light->getSpotlightOuterAngle().valueRadians();
}

float GameLight::GetSpotlightFalloff(void) const
{
	return _light->getSpotlightFalloff();
}

void GameLight::SetSpotlightInnerAngle(float val)
{
	_light->setSpotlightInnerAngle(Radian(val));
}

void GameLight::SetSpotlightOuterAngle(float val)
{
	_light->setSpotlightOuterAngle(Radian(val));
}

void GameLight::SetSpotlightFalloff(float val)
{
	_light->setSpotlightFalloff(val);
}

void GameLight::SetPowerScale(float power)
{
	_light->setPowerScale(power);
}

float GameLight::GetPowerScale(void) const
{
	return _light->getPowerScale();
}

void GameLight::SetOn(bool visible)
{
	_light->setVisible(visible);
}

bool GameLight::GetOn() const
{
	return _light->isVisible();
}

//--------------------------------------------------------------------------------
// MainLight
//--------------------------------------------------------------------------------
MainLight::MainLight()
{

}
//--------------------------------------------------------------------------------
MainLight::~MainLight()
{

}
//--------------------------------------------------------------------------------
class MainLightFactory : public TGameObjectFactory<MainLight>
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
		if(MainLight::getSingletonPtr())
		{
			return MainLight::getSingletonPtr();
		}

		IGameObject* obj = OGRE_NEW MainLight();
		obj->SetGUID(String("0xFF88FF88FFFFFFF3"));
		return obj;
	}

	virtual bool IsShownInEditorClassList() const
	{
		return true;
	}
};

M_REG_FACTORY(MainLight, MainLightFactory, VisualObject);