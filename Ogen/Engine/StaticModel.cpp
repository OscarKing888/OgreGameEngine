// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************
#include "stdafx.h"
#include "StaticModel.h"

class StaticModelFactory : public TGameObjectFactory<StaticModel>
{
public:

	StaticModelFactory()
	{
		MovableObject::setDefaultVisibilityFlags(VisualObject::DefaultVisibilityFlag);
	}
};


M_REG_FACTORY(StaticModel, StaticModelFactory, VisualObject)

StaticModel::StaticModel(void)
: _entity(0)
, _castShadow(true)
, _meshFile(M_DEBUG_MESH_FILE)
, _meshLoadTicket((BackgroundProcessTicket)~0)
, _loadInBackground(false)
, _isBatchInstance(false)
{
}

StaticModel::~StaticModel(void)
{
}

void StaticModel::SetMeshFile(const String& meshFile)
{
	SetMeshFileEx(meshFile, true);
}

void StaticModel::SetMeshFileEx(const String& meshFile, bool loadInBackground)
{
	_loadInBackground = loadInBackground;
	if(_meshFile != meshFile)
	{
		_meshFile = meshFile;
		if(!ResourceGroupManager::getSingleton().resourceExists(
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, _meshFile))
		{
			_meshFile = M_DEBUG_MESH_FILE;
		}

		OnMeshFileChanged();
	}
}

void StaticModel::OnMeshFileChanged()
{
	DestroyEntity();
	CreateEntity();
}

String StaticModel::GetEntityName()
{
	return GetGUID().toString();// + ".Entity";
}

void StaticModel::BuildPropertyMap()
{
	VisualObject::BuildPropertyMap();

	AddProp(Property_MeshFile, MeshFile, StaticModel, "MeshFile", "Model");
	AddProp(Property_Bool, CastShadow, StaticModel, "CastShadow", "Model");
	AddProp(Property_Bool, BatchInstance, StaticModel, "BatchInstance", "Model");
	AddProp(Property_StringMap, SubMaterials, StaticModel, "SubMaterials", "Material");
}

void StaticModel::DestroyEntity()
{
	if(_entity == 0)
	{
		return;
	}

	PreDestroyEntity();
	DestroyEntityImp();
	PostDestroyEntity();
}

void StaticModel::CreateEntity()
{
	if(_meshFile.empty())
	{
		return;
	}

	PreCreateEntity();

	if(_loadInBackground)
	{

	_meshLoadTicket = ResourceBackgroundQueue::getSingleton().load(
		MeshManager::getSingleton().getResourceType(),
		_meshFile, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		false, 0, 0, this);
	}
	else
	{
		CreateEntityImp();
	 	PostCreateEntity();
	}
}

void StaticModel::DestroyEntityImp()
{
	IGame* game = GameApp::getSingleton().GetGame();
	SceneManager* sm = game->GetVisualScene();
	if(_entity->sharesSkeletonInstance())
	{
		_entity->stopSharingSkeletonInstance();
	}
	sm->destroyEntity(_entity);
	_entity = 0;
}

void StaticModel::CreateEntityImp()
{
	IGame* game = GameApp::getSingleton().GetGame();
	SceneManager* sm = game->GetVisualScene();
	_entity = sm->createEntity(GetEntityName(), _meshFile);
	_entity->setUserAny(Any(AnyGameObject(this)));
	_node->attachObject(_entity);
	_entity->setCastShadows(_castShadow);
	_entity->setQueryFlags(GetQueryFlag());
	_entity->setVisibilityFlags(GetVisibilityFlag());

	size_t c = _entity->getNumSubEntities();
	for (size_t i = 0; i < c; ++i)
	{
		SubEntity* sub = _entity->getSubEntity(i);
		_materials[i] = sub->getMaterialName();
	}
}

void StaticModel::PreCreateEntity()
{
}

void StaticModel::PostCreateEntity()
{

}

void StaticModel::PreDestroyEntity()
{	
}

void StaticModel::PostDestroyEntity()
{

}

void StaticModel::PostCreateNode()
{
	VisualObject::PostCreateNode();
	CreateEntity();
}

void StaticModel::PreDestroyNode()
{
	VisualObject::PreDestroyNode();
	DestroyEntity();
}

Mesh* StaticModel::GetMesh()
{
	return _entity->getMesh().getPointer();
}

const String& StaticModel::GetTypeName() const
{
	static String sName("StaticModel");
	return sName;
}

void StaticModel::SetCastShadow(bool on)
{
	_castShadow = on;
	if(_entity)
	{
		_entity->setCastShadows(_castShadow);
	}
}

void StaticModel::SetSubEntityMaterial(size_t subEntityIdx, const String& matName)
{
	_materials[subEntityIdx] = matName;
	if(_entity)
	{
		if(subEntityIdx < _entity->getNumSubEntities())		
		{
			SubEntity* sub = _entity->getSubEntity(subEntityIdx);
			sub->setMaterialName(matName);
		}
	}
}

const String& StaticModel::GetSubEntityMaterial(size_t subEntityIdx)
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

void StaticModel::SetSubMaterials(const StringMap& idxAndMatName)
{
	for (StringMap::const_iterator i = idxAndMatName.begin(); i != idxAndMatName.end(); ++i)
	{
		size_t idx = StringConverter::parseInt(i->first);
		SetSubEntityMaterial(idx, i->second);
	}
}

StringMap StaticModel::GetSubMaterials() const
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

StringVector StaticModel::GetSubEntities() const
{
	assert(_entity);
	StringVector mats;

	size_t c = _entity->getNumSubEntities();
	for (size_t i = 0; i < c; ++i)
	{
		SubEntity* sub = _entity->getSubEntity(i);
		mats.push_back(sub->getMaterialName());	
	}

	return mats;
}

void StaticModel::SetVisibilityFlag(uint32 flag)
{
	VisualObject::SetVisibilityFlag(flag);
	if(_entity)
	{
		_entity->setVisibilityFlags(flag);
	}
}

void StaticModel::operationCompleted(BackgroundProcessTicket ticket, const BackgroundProcessResult& result)
{
	if(!result.error)
	{
		if(ticket == _meshLoadTicket)
		{
			CreateEntityImp();
			PostCreateEntity();
		}
	}
	else
	{
		LogErrorIDf("Engine", "[%s]Load resource error:%s", __FUNCTION__, result.message.c_str());
	}
}

void StaticModel::SetBatchInstance( bool on )
{
	_isBatchInstance = on;
}
//--------------------------------------------------------------------------------