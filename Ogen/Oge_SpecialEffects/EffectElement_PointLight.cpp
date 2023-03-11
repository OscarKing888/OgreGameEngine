#include "StdAfx.h"
#include "EffectElement_PointLight.h"
#include "SpecialEffectsManager.h"


EffectElement_PointLight::EffectElement_PointLight(const String& name, ISpecialEffects* parent)
: AbstractEffectElement(name, parent)
, _light(0)
, _lightType(Light::LT_POINT)
{
	_diffuseColour = ColourValue::White;
	_speculaColour = ColourValue::White;
	_range = 2000.0f;
	_attenuationConst = 1.0f;
	_attenuationLinear = 0.0f;
	_attenuationQuadric = 0.0f;
	_spotInnerAngle = 0.523599f;
	_spotOuterAngle = 0.698132f;
	_spotFalloff = 1.0f;
	_powerScale = 1.0f;
	_dir = Vector3::NEGATIVE_UNIT_Y;
}

EffectElement_PointLight::~EffectElement_PointLight(void)
{
}

const String& EffectElement_PointLight::GetTypeName() const
{
	static const String C_TypeName = "PointLight";
	return C_TypeName;
}

void EffectElement_PointLight::SetName(const String& name)
{
	OnDestroyLight();
	_name = name;
	OnCreateLight();
}

void EffectElement_PointLight::OnStart()
{
	if(_light)
	{
		_light->setVisible(true);
	}
}

void EffectElement_PointLight::OnEnd()
{
	if(_light)
	{
		_light->setVisible(false);
	}
}

void EffectElement_PointLight::BuildPropertyMap()
{
	AbstractEffectElement::BuildPropertyMap();
	AddEnumProp(LightType, Light::LightTypes, LightType, EffectElement_PointLight, "LightType", "PointLight");
	AddProp(Property_ColourValue, DiffuseColour, EffectElement_PointLight, "DiffuseColour", "PointLight");
	AddProp(Property_ColourValue, SpecularColour, EffectElement_PointLight, "SpecularColour", "PointLight");
	AddProp(Property_Float, AttenuationRange, EffectElement_PointLight, "AttenuationRange", "PointLight");
	AddProp(Property_Float, AttenuationConstant, EffectElement_PointLight, "AttenuationConstant", "PointLight");
	AddProp(Property_Float, AttenuationLinear, EffectElement_PointLight, "AttenuationLinear", "PointLight");
	AddProp(Property_Float, AttenuationQuadric, EffectElement_PointLight, "AttenuationQuadric", "PointLight");
	AddProp(Property_Vector3, Direction, EffectElement_PointLight, "Direction", "PointLight");
	AddProp(Property_Float, SpotlightInnerAngle, EffectElement_PointLight, "SpotlightInnerAngle", "PointLight");
	AddProp(Property_Float, SpotlightOuterAngle, EffectElement_PointLight, "SpotlightOuterAngle", "PointLight");
	AddProp(Property_Float, SpotlightFalloff, EffectElement_PointLight, "SpotlightFalloff", "PointLight");
	AddProp(Property_Float, PowerScale, EffectElement_PointLight, "PowerScale", "PointLight");
}

void EffectElement_PointLight::OnCreateLight()
{
	SceneManager* sm = GetSceneManager();
	if(_name.empty())
	{
		return;
	}

	_light = sm->createLight(_parent->GetName() + "/" + GetName());
	_light->setVisible(false);
	_light->setQueryFlags(0);
	_light->setType(_lightType);
	_light->setDiffuseColour(_diffuseColour);
	_light->setSpecularColour(_speculaColour);
	_light->setDirection(_dir);
	_light->setSpotlightFalloff(_spotFalloff);
	_light->setSpotlightInnerAngle(Radian(_spotInnerAngle));
	_light->setSpotlightOuterAngle(Radian(_spotOuterAngle));
	_light->setAttenuation(_range, _attenuationConst, _attenuationLinear, _attenuationQuadric);
	_light->setPowerScale(_powerScale);
	_light->setDirection(_dir);
	_light->setCastShadows(false);


	if(!_node)
	{
		String nodeName = _name;
		_node = _parent->GetNode()->createChildSceneNode(nodeName);
	}

	if(_light)
	{
		_node->attachObject(_light);
	}
}

void EffectElement_PointLight::OnDestroyLight()
{
	if(_light)
	{
		SceneManager* sm = GetSceneManager();
		sm->destroyLight(_light);
		_light = 0;
	}
}

Light::LightTypes EffectElement_PointLight::GetLightType() const
{
	return _light->getType();
}

void EffectElement_PointLight::SetLightType(Light::LightTypes lt)
{
	_light->setType(lt);
	_lightType = lt;
}

void EffectElement_PointLight::SetDiffuseColour(const ColourValue& clr)
{
	_light->setDiffuseColour(clr);
	_diffuseColour = clr;
}

const ColourValue& EffectElement_PointLight::GetDiffuseColour() const
{
	return _light->getDiffuseColour();
}

void EffectElement_PointLight::SetSpecularColour(const ColourValue& clr)
{
	_light->setSpecularColour(clr);
	_speculaColour = clr;
}

const ColourValue& EffectElement_PointLight::GetSpecularColour() const
{
	return _light->getSpecularColour();
}

float EffectElement_PointLight::GetAttenuationRange(void) const
{
	return _light->getAttenuationRange();
}

float EffectElement_PointLight::GetAttenuationConstant(void) const
{
	return _light->getAttenuationConstant();
}

float EffectElement_PointLight::GetAttenuationLinear(void) const
{
	return _light->getAttenuationLinear();
}

float EffectElement_PointLight::GetAttenuationQuadric(void) const
{
	return _light->getAttenuationQuadric();
}

void EffectElement_PointLight::SetAttenuationRange(float val)
{
	_light->setAttenuation(val, GetAttenuationConstant(), GetAttenuationLinear(), GetAttenuationQuadric());
	_range = val;
}

void EffectElement_PointLight::SetAttenuationConstant(float val)
{
	_light->setAttenuation(GetAttenuationRange(), val, GetAttenuationLinear(), GetAttenuationQuadric());
	_attenuationConst = val;
}

void EffectElement_PointLight::SetAttenuationLinear(float val)
{
	_light->setAttenuation(GetAttenuationRange(), GetAttenuationConstant(), val, GetAttenuationQuadric());
	_attenuationLinear = val;
}

void EffectElement_PointLight::SetAttenuationQuadric(float val)
{
	_light->setAttenuation(GetAttenuationRange(), GetAttenuationConstant(), GetAttenuationLinear(), val);
	_attenuationQuadric = val;
}

void EffectElement_PointLight::SetDirection(const Vector3& vec)
{
	_light->setDirection(vec);
	_dir = vec;
}

const Vector3& EffectElement_PointLight::GetDirection(void) const
{
	return _light->getDirection();
}

float EffectElement_PointLight::GetSpotlightInnerAngle(void) const
{
	return _light->getSpotlightInnerAngle().valueRadians();
}

float EffectElement_PointLight::GetSpotlightOuterAngle(void) const
{
	return _light->getSpotlightOuterAngle().valueRadians();
}

float EffectElement_PointLight::GetSpotlightFalloff(void) const
{
	return _light->getSpotlightFalloff();
}

void EffectElement_PointLight::SetSpotlightInnerAngle(float val)
{
	_light->setSpotlightInnerAngle(Radian(val));
	_spotInnerAngle = val;
}

void EffectElement_PointLight::SetSpotlightOuterAngle(float val)
{
	_light->setSpotlightOuterAngle(Radian(val));
	_spotOuterAngle = val;
}

void EffectElement_PointLight::SetSpotlightFalloff(float val)
{
	_light->setSpotlightFalloff(val);
	_spotFalloff = val;
}

void EffectElement_PointLight::SetPowerScale(float power)
{
	_light->setPowerScale(power);
	_powerScale = power;
}

float EffectElement_PointLight::GetPowerScale(void) const
{
	return _light->getPowerScale();
}

void EffectElement_PointLight::OnDestroy()
{
	OnDestroyLight();
}

Node* EffectElement_PointLight::GetNode() const
{
	if(_light)
	{
		return _light->getParentNode();
	}
	return 0;
}