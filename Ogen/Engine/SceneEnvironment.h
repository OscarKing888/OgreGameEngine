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
#include "LogicObject.h"

class _EngineExport FogModeType
	: public AbstractEnumDescriptor
{
public:

	FogModeType();
	virtual ~FogModeType(){}	
};

class _EngineExport SceneEnvironment
	: public LogicObject
	, public Singleton<SceneEnvironment>
{
public:

	SceneEnvironment(void);
	virtual ~SceneEnvironment(void);

	const ColourValue& GetAmbientLight() const;
	void SetAmbientLight(const ColourValue& clr);

	ShadowTechnique GetShadowTechnique() const;
	void SetShadowTechnique(ShadowTechnique st);

	void SetShadowCasterRenderBackFaces(bool bf);
	bool GetShadowCasterRenderBackFaces();

	void SetDisplaySceneNodes(bool display);
	bool GetDisplaySceneNodes();

	void SetFindVisibleObjects(bool find);
	bool GetFindVisibleObjects();

	void SetFlipCullingOnNegativeScale(bool on);
	bool GetFlipCullingOnNegativeScale();

	FogMode GetFogType() const;
	void SetFogType(FogMode st);

	const ColourValue& GetFogColour() const;
	void SetFogColour(const ColourValue& clr);

	void SetFogDensity(float val);
	float GetFogDensity();

	void SetFogStart(float val);
	float GetFogStart();

	void SetFogEnd(float val);
	float GetFogEnd();

	const ColourValue& GetShadowColour() const;
	void SetShadowColour(const ColourValue& clr);

	void SetShadowDirectionalLightExtrusionDistance(float val);
	float GetShadowDirectionalLightExtrusionDistance();

	void SetShadowDirLightTextureOffset(float val);
	float GetShadowDirLightTextureOffset();

	void SetShadowFarDistance(float val);
	float GetShadowFarDistance();

	void SetShadowIndexBufferSize(size_t val);
	size_t GetShadowIndexBufferSize();

	void SetShadowTextureCasterMaterial(const String& name);
	const String& GetShadowTextureCasterMaterial();

	void SetShadowTextureReceiverMaterial(const String& name);
	const String& GetShadowTextureReceiverMaterial();

	void SetShadowTextureCount(size_t val);
	size_t GetShadowTextureCount();

	void SetShadowTextureFadeEnd(float val);
	float GetShadowTextureFadeEnd();

	void SetShadowTextureFadeStart(float val);
	float GetShadowTextureFadeStart();

	void SetShadowTextureSelfShadow(bool on);
	bool GetShadowTextureSelfShadow();

	void SetShadowTextureSize(int val);
	int GetShadowTextureSize();

	void SetShadowUseInfiniteFarPlane(bool on);
	bool GetShadowUseInfiniteFarPlane();

	void SetShadowUseLightClipPlanes(bool on);
	bool GetShadowUseLightClipPlanes();

	void SetShowDebugShadows(bool on);
	bool GetShowDebugShadows();

	void SetShowBoundingBoxes(bool on);
	bool GetShowBoundingBoxes();

	virtual const String& GetTypeName() const;

protected:

	virtual void BuildPropertyMap();

	SceneManager* GetSceneManager() const;

	bool _showBoundingBoxes;
	bool _shadowUseInfiniteFarPlane;
	int	 _shadowTextureSize;
	float _shadowTextureFadeStart;
	float _shadowTextureFadeEnd;
	String _shadowTextureReceiverMaterial;
	String _shadowTextureCasterMaterial;

	float _fogStart;
	float _fogEnd;
	float _fogDensity;
};