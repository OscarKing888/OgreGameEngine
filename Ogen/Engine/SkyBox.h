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

enum _EngineExport ESkyType
{
	EST_None,
	EST_Box,
	EST_Dome,
	EST_Plane
};

class _EngineExport SkyType
	: public AbstractEnumDescriptor
	, public Singleton<SkyType>
{
public:

	SkyType();
	virtual ~SkyType(){}	
};

//------------------------------------------------------------------------

class _EngineExport SkyBox
	: public LogicObject
	, public Singleton<SkyBox>
{
public:

	SkyBox(void);
	virtual ~SkyBox(void);

	virtual const String& GetTypeName() const;

	ESkyType GetSkyType() const { return _skyType; }
	void SetSkyType(ESkyType skt);

	const String& GetSkyBoxMaterial() const { return _skyBoxMaterial; }
	void SetSkyBoxMaterial(const String& mat);

	const String& GetSkyDomeMaterial() const { return _skyDomeMaterial; }
	void SetSkyDomeMaterial(const String& mat);

	const String& GetSkyPlaneMaterial() const { return _skyPlaneMaterial; }
	void SetSkyPlaneMaterial(const String& mat);

	bool GetEnableSkyBox() const { return _enableSkyBox; }
	void SetEnableSkyBox(bool on);

	bool GetEnableSkyDome() const { return _enableSkyDome; }
	void SetEnableSkyDome(bool on);

	bool GetEnableSkyPlane() const { return _enableSkyPlane; }
	void SetEnableSkyPlane(bool on);

	float GetSkyDistance() const { return _skyDistance; }
	void SetSkyDistance(float val);

	// Sky plane
	float GetSkyPlaneScale() const { return _skyPlaneScale; }
	void SetSkyPlaneScale(float val);

	float GetSkyPlaneTiling() const { return _skyPlaneTiling; }
	void SetSkyPlaneTiling(float val);

	float GetSkyPlaneBow() const { return _skyPlaneBow; }
	void SetSkyPlaneBow(float val);

	int GetSkyPlaneXSegments() const { return _skyPlaneXSegments; }
	void SetSkyPlaneXSegments(int val);

	int GetSkyPlaneYSegments() const { return _skyPlaneYSegments; }
	void SetSkyPlaneYSegments(int val);


	// Sky dome
	float GetSkyDomeCurvature() const { return _skyDomeCurvature; }
	void SetSkyDomeCurvature(float val);

	float GetSkyDomeTiling() const { return _skyDomeTiling; }
	void SetSkyDomeTiling(float val);

	int GetSkyDomeXSegments() const { return _skyDomeXSegments; }
	void SetSkyDomeXSegments(int val);

	int GetSkyDomeYSegments() const { return _skyDomeYSegments; }
	void SetSkyDomeYSegments(int val);

	int GetSkyDomeYSegmentsKeep() const { return _skyDomeYSegmentsKeep; }
	void SetSkyDomeYSegmentsKeep(int val);

protected:

	ESkyType	_skyType;
	bool		_enableSkyBox;
	bool		_enableSkyDome;
	bool		_enableSkyPlane;

	float		_skyDistance;

	float		_skyPlaneScale;
	float		_skyPlaneTiling;
	float		_skyPlaneBow;
	int			_skyPlaneXSegments;
	int			_skyPlaneYSegments;

	float		_skyDomeCurvature;
	float		_skyDomeTiling;
	int			_skyDomeXSegments;
	int			_skyDomeYSegments;
	int			_skyDomeYSegmentsKeep;

	String _skyBoxMaterial;
	String _skyDomeMaterial;
	String _skyPlaneMaterial;

	virtual void BuildPropertyMap();
	SceneManager* GetSceneManager();
};