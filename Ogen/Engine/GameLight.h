// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-12-03
// *************************************************************************************

#pragma once
#include "VisualObject.h"


class _EngineExport LightType
	: public AbstractEnumDescriptor
	, public Singleton<LightType>
{
public:

	LightType();
	virtual ~LightType(){}	
};

///
/// Light object to place in scene
///
class _EngineExport GameLight : public VisualObject
{
public:

	GameLight(void);
	virtual ~GameLight(void);

	virtual const String& GetTypeName() const;

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

	void SetOn(bool visible);
	bool GetOn() const;

protected:

	virtual void BuildPropertyMap();
	virtual void PostCreateNode();
	virtual void PreDestroyNode();


	Entity*	_entity;
	Light*	_light;
	BillboardSet* _billboard;
	MaterialPtr _lightMat;

	void CreateLight();
	void DestroyLight();

	virtual void OnEditorSelect();
	virtual void OnEditorDeselect();

};

///
/// Main Light for cast shadows
///

class _EngineExport MainLight : public GameLight, public Singleton<MainLight>
{
public:

	MainLight();
	virtual ~MainLight();
};