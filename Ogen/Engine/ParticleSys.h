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
#include "IGameObjectFactory.h"

class _EngineExport ParticleSys : public VisualObject
{
public:

	ParticleSys(void);
	virtual ~ParticleSys(void);

	void SetParticleTemplateName(const String& tempName);
	const String& GetParticleTemplateName() const { return _particleTemplateName; }

	virtual const String& GetTypeName() const;

protected:

	virtual void BuildPropertyMap();
	virtual void PostCreateNode();
	virtual void PreDestroyNode();

	ParticleSystem*	_particleSys;
	String			_particleTemplateName;

	void CreateParticleSystem();
	void DestroyParticleSystem();

	virtual void OnEditorSelect();
	virtual void OnEditorDeselect();
};