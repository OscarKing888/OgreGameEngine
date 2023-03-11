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
#include "ParticleSys.h"

class ParticleSysFactory : public TGameObjectFactory<ParticleSys>
{
};


M_REG_FACTORY(ParticleSys, ParticleSysFactory, VisualObject);

ParticleSys::ParticleSys(void)
	: _particleSys(0)
{
}

ParticleSys::~ParticleSys(void)
{
}

void ParticleSys::BuildPropertyMap()
{
	VisualObject::BuildPropertyMap();

	// 	AddPropEx1(Property_Vector3, Position, VisualObject, const Vector3&);
	// 	AddPropEx1(Property_Vector3, Scale, VisualObject, const Vector3&);
	// 	AddPropEx1(Property_Quaternion, Orientation, VisualObject, const Quaternion&);
	AddProp(Property_String, ParticleTemplateName, ParticleSys, "ParticleTemplateName", "ParticleSystem");
}

const String& ParticleSys::GetTypeName() const
{
	static String sName("ParticleSys");
	return sName;
}

void ParticleSys::PostCreateNode()
{
	VisualObject::PostCreateNode();
	CreateParticleSystem();
}

void ParticleSys::PreDestroyNode()
{
	VisualObject::PreDestroyNode();
	DestroyParticleSystem();
}

void ParticleSys::CreateParticleSystem()
{
	SceneManager* sm = GetSceneManager();
	if(!_particleTemplateName.empty())
	{
		_particleSys = sm->createParticleSystem(GetGUID().toString(), _particleTemplateName);
		_particleSys->setUserAny(Any(AnyGameObject(this)));
		_node->attachObject(_particleSys);
		_particleSys->setQueryFlags(GetQueryFlag());
	}
}

void ParticleSys::DestroyParticleSystem()
{
	if(_particleSys == 0)
	{
		return;
	}

	SceneManager* sm = GetSceneManager();
	sm->destroyParticleSystem(_particleSys);
	_particleSys = 0;
}

void ParticleSys::OnEditorSelect()
{

}

void ParticleSys::OnEditorDeselect()
{

}

void ParticleSys::SetParticleTemplateName(const String& tempName)
{
	_particleTemplateName = tempName;
	if(_node)
	{
		DestroyParticleSystem();
		CreateParticleSystem();
	}
}

