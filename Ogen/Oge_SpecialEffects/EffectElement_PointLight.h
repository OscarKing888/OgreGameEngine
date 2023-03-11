#pragma once
#include "AbstractEffectElement.h"

class EffectElement_PointLight : public AbstractEffectElement
{
public:

	EffectElement_PointLight(const String& name, ISpecialEffects* parent);
	virtual ~EffectElement_PointLight(void);

	virtual void SetName(const String& name);

	virtual const String& GetTypeName() const;
	virtual bool IsAttachable() const{ return true; }

	Light::LightTypes GetLightType() const;
	void SetLightType(Light::LightTypes lt);

	void SetDiffuseColour(const ColourValue& clr);
	const ColourValue& GetDiffuseColour() const;

	void SetSpecularColour(const ColourValue& clr);
	const ColourValue& GetSpecularColour() const;

	float GetAttenuationRange(void) const;
	float GetAttenuationConstant(void) const;
	float GetAttenuationLinear(void) const;
	float GetAttenuationQuadric(void) const;

	void SetAttenuationRange(float val);
	void SetAttenuationConstant(float val);
	void SetAttenuationLinear(float val);
	void SetAttenuationQuadric(float val);

	void SetDirection(const Vector3& vec);
	const Vector3& GetDirection(void) const;

	float GetSpotlightInnerAngle(void) const;
	float GetSpotlightOuterAngle(void) const;
	float GetSpotlightFalloff(void) const;

	void SetSpotlightInnerAngle(float val);
	void SetSpotlightOuterAngle(float val);
	void SetSpotlightFalloff(float val);

	void SetPowerScale(float power);
	float GetPowerScale(void) const;

	virtual void OnDestroy();

protected:

	virtual void OnStart();
	virtual void OnEnd();
	virtual void BuildPropertyMap();

	virtual void OnCreateLight();
	virtual void OnDestroyLight();
	virtual Node* GetNode() const;

	Light*	_light;
	Light::LightTypes	_lightType;
	Vector3		_dir;
	//ColourValue	_ambientColour;
	ColourValue	_diffuseColour;
	ColourValue	_speculaColour;
	//ColourValue	_emissiveColour;
	float				_range;
	float				_attenuationConst;
	float				_attenuationLinear;
	float				_attenuationQuadric;
	float				_spotInnerAngle;
	float				_spotOuterAngle;
	float				_spotFalloff;
	float				_powerScale;
};

