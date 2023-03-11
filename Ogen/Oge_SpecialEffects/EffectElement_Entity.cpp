#include "StdAfx.h"
#include "EffectElement_Entity.h"
#include "SpecialEffectsManager.h"


EffectElement_Entity::EffectElement_Entity(const String& name, ISpecialEffects* parent)
: AbstractEffectElement(name, parent)
, _entity(0)
, _startPlayAnimation(0.0f)
, _playAnimationTime(0.0f)
, _animationPlayed(false)
, _loopAnimation(false)
, _meshFile(M_DEBUG_MESH_FILE)
{
}

EffectElement_Entity::~EffectElement_Entity(void)
{
}

const String& EffectElement_Entity::GetTypeName() const
{
	static const String C_TypeName = "Entity";
	return C_TypeName;
}

void EffectElement_Entity::SetName(const String& name)
{
	OnDestroyEntity();
	_name = name;
	OnCreateEntity();
}

void EffectElement_Entity::OnStart()
{
	if(_entity)
	{
		_entity->setVisible(true);
	}
}

void EffectElement_Entity::OnEnd()
{
	if(_entity)
	{
		_entity->setVisible(false);
	}
}

void EffectElement_Entity::BuildPropertyMap()
{
	AbstractEffectElement::BuildPropertyMap();
	AddProp(Property_MeshFile, MeshFile, EffectElement_Entity, "MeshFile", "Entity");
	AddProp(Property_StringMap, SubMaterials, EffectElement_Entity, "SubMaterials", "Entity");

	AddProp(Property_String, PlayAnimationName, EffectElement_Entity, "PlayAnimationName", "Entity");
	AddProp(Property_Bool, LoopAnimation, EffectElement_Entity, "LoopAnimation", "Entity");
	AddProp(Property_Float, StartPlayAnimationTime, EffectElement_Entity, "StartPlayAnimationTime", "Entity");
}

void EffectElement_Entity::OnCreateEntity()
{
	SceneManager* sm = GetSceneManager();
	if(_name.empty())
	{
		return;
	}

	if(!_meshFile.empty())
	{
		String name = _parent->GetName() + "/" + GetName();
		if(!_meshFile.empty())
		{
			_entity = sm->createEntity(name, _meshFile);
			_entity->setQueryFlags(0);
			_entity->setVisible(false);
		}
		else
		{
			LogErrorIDf("Effects", "[EffectElement_Entity::OnCreateEntity][%s] mesh file is empty!", name.c_str());
		}
	}

	if(!_node)
	{
		String nodeName = _name;
		_node = _parent->GetNode()->createChildSceneNode(nodeName);
	}

	if(_entity)
	{
		_node->attachObject(_entity);
	}
}

void EffectElement_Entity::OnDestroyEntity()
{
	if(_entity)
	{	
		SceneManager* sm = GetSceneManager();
		sm->destroyEntity(_entity);
		_entity = 0;
	}
}


void EffectElement_Entity::SetMeshFile(const String& meshFile)
{
	OnDestroyEntity();
	_meshFile = meshFile;
	OnCreateEntity();
}


void EffectElement_Entity::SetSubEntityMaterial(size_t subEntityIdx, const String& matName)
{
	_materials[subEntityIdx] = matName;
	if(_entity)
	{
		assert(subEntityIdx < _entity->getNumSubEntities());
		SubEntity* sub = _entity->getSubEntity(subEntityIdx);
		sub->setMaterialName(matName);
	}
}

const String& EffectElement_Entity::GetSubEntityMaterial(size_t subEntityIdx)
{
	if(_materials.find(subEntityIdx) != _materials.end())
	{
		return _materials[subEntityIdx];
	}

	if(_entity)
	{
		assert(subEntityIdx < _entity->getNumSubEntities());
		SubEntity* sub = _entity->getSubEntity(subEntityIdx);
		_materials[subEntityIdx] = sub->getMaterialName();
	}

	return _materials[subEntityIdx];
}

void EffectElement_Entity::SetSubMaterials(const StringMap& idxAndMatName)
{
	for (StringMap::const_iterator i = idxAndMatName.begin(); i != idxAndMatName.end(); ++i)
	{
		size_t idx = StringConverter::parseInt(i->first);
		SetSubEntityMaterial(idx, i->second);
	}
}

StringMap EffectElement_Entity::GetSubMaterials() const
{
	StringMap mats;
	if(_entity)
	{
		size_t c = _entity->getNumSubEntities();
		for (size_t i = 0; i < c; ++i)
		{
			SubEntity* sub = _entity->getSubEntity(i);
			mats.insert(std::make_pair(StringConverter::toString(i), sub->getMaterialName()));
		}
	}

	return mats;
}

void EffectElement_Entity::OnDestroy()
{
	OnDestroyEntity();
}

void EffectElement_Entity::Update(float deltaTime)
{
	AbstractEffectElement::Update(deltaTime);
	_playAnimationTime += deltaTime;

	if(!_animationPlayed && _entity && (!_playAnimationName.empty())
		&& (_playAnimationTime >= _startPlayAnimation))
	{
		_animationPlayed = true;

		AnimationStateSet* ass = _entity->getAllAnimationStates();
		if(ass)
		{
			if(ass->hasAnimationState(_playAnimationName))
			{
				AnimationState* as = ass->getAnimationState(_playAnimationName);
				as->setEnabled(true);
				as->setLoop(_loopAnimation);
				as->setTimePosition(0.0f);
			}
			else
			{
				LogErrorIDf("Effects", "[EffectElement_Entity::Update] mesh[%s] animation[%s] not found!",
					_meshFile.c_str(), _playAnimationName.c_str());
			}
		}
		else
		{
			LogErrorIDf("Effects", "[EffectElement_Entity::Update] mesh[%s] doesn't have skeleton!", _meshFile.c_str());
		}
	}

	if(_entity && _animationPlayed && _entity->getAllAnimationStates())
	{
		ConstEnabledAnimationStateIterator asi =
			_entity->getAllAnimationStates()->getEnabledAnimationStateIterator();
		while (asi.hasMoreElements())
		{
			asi.peekNext()->addTime(deltaTime);
			asi.moveNext();
		}
	}
}

void EffectElement_Entity::Reset()
{
	AbstractEffectElement::Reset();
	_playAnimationTime = 0.0f;
	_animationPlayed = false;
}

Node* EffectElement_Entity::GetNode() const
{
	if(_entity)
	{
		return _entity->getParentNode();
	}
	return 0;
}