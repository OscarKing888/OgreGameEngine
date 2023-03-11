#include "stdafx.h"
#include "Tile.h"
#include "AbstractGame.h"

const uint16 Tile::WORKQUEUE_TILE_DATA_REQUEST = 1;

Tile::Tile(SceneManager* sm, IGame* game)
	: _visualScene(sm)
	, _x(0)
	, _z(0)
	, _tileSize(0)
	, _node(0)
	, _dataLoadInProgress(false)
	, _game(game)
	, _isLoaded(false)
	, _modified(false)
{
	WorkQueue* wq = Root::getSingleton().getWorkQueue();
	_workQueueChannel = wq->getChannel("Engine/Tile");
	wq->addRequestHandler(_workQueueChannel, this);
	wq->addResponseHandler(_workQueueChannel, this);
}
//--------------------------------------------------------------------------------
Tile::~Tile()
{
	WaitForDerivedProcesses();
	WorkQueue* wq = Root::getSingleton().getWorkQueue();
	wq->removeRequestHandler(_workQueueChannel, this);
	wq->removeResponseHandler(_workQueueChannel, this);	
}
//--------------------------------------------------------------------------------
void Tile::SetCoord( long x, long z )
{
	_x = x;
	_z = z;

	CreateTileRootNode();
}
//--------------------------------------------------------------------------------
void Tile::AddGameObject(IGameObject* obj)
{
	String typeName = getClassName(obj);
	if(HasGameObject(obj->GetGUID()))
	{
		String msg = formatString("Object already exist [%s], type [%s]", obj->GetGUID().toString().c_str(), typeName.c_str());

		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, msg, "Tile::GetGameObjectsByType");
	}

	TypedGameObjectMap::iterator i = _objects.find(typeName);
	if(i == _objects.end())
	{
		i = _objects.insert(std::make_pair(typeName, GameObjectMap())).first;
	}

	i->second.insert(std::make_pair(obj->GetGUID(), obj));
	obj->OnAddToScene(_node);
}
//--------------------------------------------------------------------------------
void Tile::RemoveGameObject(IGameObject* obj)
{
	TypedGameObjectMap::iterator i = _objects.begin();
	for (; i != _objects.end(); ++i)
	{
		GameObjectMap& m = i->second;
		GameObjectMap::iterator oi = m.find(obj->GetGUID());
		if(oi != m.end())
		{
			m.erase(oi);
			obj->OnRemoveFromScene(_node);
		}
	}
}
//--------------------------------------------------------------------------------
const TypedGameObjectMap& Tile::GetAllGameObjects() const
{
	return _objects;
}
//--------------------------------------------------------------------------------
const GameObjectMap& Tile::GetGameObjectsByType(const String& typeName) const
{
	TypedGameObjectMap::const_iterator i = _objects.find(typeName);
	if(i != _objects.end())
	{
		return i->second;
	}

	static GameObjectMap EmptyMap;
	return EmptyMap;
#if 0
	OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
		"Object type not found [" + typeName + "]", 
		"Tile::GetGameObjectsByType");
#endif
}
//--------------------------------------------------------------------------------
IGameObject* Tile::GetGameObject(const XGUID& guid)
{
	TypedGameObjectMap::iterator i = _objects.begin();
	for (; i != _objects.end(); ++i)
	{
		const GameObjectMap& m = i->second;
		GameObjectMap::const_iterator oi = m.find(guid);
		if(oi != m.end())
		{
			return oi->second;
		}			
	}

	return 0;
}
//--------------------------------------------------------------------------------
bool Tile::HasGameObject(const XGUID& guid)
{
	return GetGameObject(guid) != 0;
}
//--------------------------------------------------------------------------------
void Tile::DestroyGameObject(const XGUID& guid)
{
	TypedGameObjectMap::iterator i = _objects.begin();
	for (; i != _objects.end(); ++i)
	{
		GameObjectMap& m = i->second;
		GameObjectMap::iterator oi = m.find(guid);
		if(oi != m.end())
		{
			IGameObject* o = oi->second;
			m.erase(oi);
			o->OnRemoveFromScene(_node);

			String typeName = getClassName(o);
			IGameObjectFactory* f = GameObjectFactoryManager::getSingleton().GetFactory(typeName);
			f->Destroy(o);
			return;
		}		
	}

	LogWarningIDf("GameLogic", "[Tile::DestroyGameObject] object [%s] not found!", guid.toString().c_str());
}
//--------------------------------------------------------------------------------
void Tile::DestroyGameObject(IGameObject* obj)
{
	DestroyGameObject(obj->GetGUID());
}
//--------------------------------------------------------------------------------
void Tile::DestoryAllGameObjectByType(const String& classTypeName)
{
	TypedGameObjectMap::iterator i = _objects.find(classTypeName);
	if(i != _objects.end())
	{
		GameObjectMap::iterator j = i->second.begin();
		for (; j != i->second.end(); ++j)
		{
			IGameObject* o = j->second;
			o->OnRemoveFromScene(_node);

			String typeName = getClassName(o);
			IGameObjectFactory* f = GameObjectFactoryManager::getSingleton().GetFactory(typeName);
			f->Destroy(o);
		}
		_objects.erase(i);
		return;
	}

	LogWarningIDf("GameLogic", "[Tile::DestoryAllGameObjectByType] type [%s] not found!", classTypeName.c_str());
}
//--------------------------------------------------------------------------------
void Tile::DestroyAllGameObject()
{
	TypedGameObjectMap::iterator i = _objects.begin();
	for (; i != _objects.end(); ++i)
	{
		GameObjectMap::iterator j = i->second.begin();
		for (; j != i->second.end(); ++j)
		{
			IGameObject* o = j->second;
			o->OnRemoveFromScene(_node);
			String typeName = getClassName(o);
			IGameObjectFactory* f = GameObjectFactoryManager::getSingleton().GetFactory(typeName);
			f->Destroy(o);
		}
	}

	_objects.clear();
}
//--------------------------------------------------------------------------------
void Tile::Save(const String& sceneDir, ISerializationMask* query, bool onlyIfModified)
{
	WaitForDerivedProcesses();

	if(onlyIfModified)
	{
		if(!_modified)
		{
			return;
		}
	}

	GameObjectList lst;

	for (TypedGameObjectMap::const_iterator i = _objects.begin(); i != _objects.end(); ++i)
	{
		const GameObjectMap& objs = i->second;
		for(GameObjectMap::const_iterator j = objs.begin(); j != objs.end(); ++j)
		{
			IGameObject* obj = j->second;
			if(obj->GetSerializeable())
			{
				lst.push_back(obj);
			}
		}
	}

	String fileName = ITile::ConvertToTileFileName(_x, _z);
	fileName = sceneDir + fileName;

	ClassXMLSerializer ser;
	ser.Save(lst, fileName, query);
}
//--------------------------------------------------------------------------------
void Tile::Load(bool synchronous)
{
	Unload();

	_dataLoadInProgress = true;

	LoadTileRequest req;
	req.tile = this;

	Root::getSingleton().getWorkQueue()->addRequest(
		_workQueueChannel, WORKQUEUE_TILE_DATA_REQUEST, 
		Any(req), 0, synchronous);

	_isLoaded = true;
}
//--------------------------------------------------------------------------------
void Tile::Unload()
{
	WaitForDerivedProcesses();

	_game->RemoveGameObject(_loadObjects);

	DestroyAllGameObject();

	_isLoaded = false;
	//DestroyTileRootNode();

	//if(GameApp::CurrentRunType == ERT_Game)
	//{
	//	DestroyBatch();
	//}
}
//--------------------------------------------------------------------------------
void Tile::Destroy()
{
	Unload();
	DestroyTileRootNode();
	OGRE_DELETE this;
}
//--------------------------------------------------------------------------------
void Tile::AddGameObjects(const GameObjectList& lst)
{
	for (size_t i = 0; i < lst.size(); ++i)
	{
		AddGameObject(lst[i]);
	}
}
//--------------------------------------------------------------------------------
void Tile::CreateTileRootNode()
{
	DestroyTileRootNode();

	String nodeName = formatString("Tile[%d, %d]", _x, _z);
	_node = _visualScene->getRootSceneNode()->createChildSceneNode(nodeName);

	IGame* game = GameApp::getSingleton().GetGame();
	TerrainGroup* tg = game->GetTerrainGroup();

	Vector3 pos;
	tg->convertTerrainSlotToWorldPosition(_x, _z, &pos);
	_node->setPosition(pos);
}
//--------------------------------------------------------------------------------
void Tile::DestroyTileRootNode()
{
	if(_node)
	{
		_node->removeAllChildren();
		_visualScene->destroySceneNode(_node);
		_node = 0;
	}
}
//--------------------------------------------------------------------------------
InstancedGeometry* Tile::GetInstanceBatch(const String& meshName)
{
	InstancedMap::iterator i = _instancedMap.find(meshName);
	if(i != _instancedMap.end())
	{
		return i->second;
	}

	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	InstancedGeometry* ig = sm->createInstancedGeometry(meshName + ".__Instance__");
	_instancedMap.insert(std::make_pair(meshName, ig));
	ig->setCastShadows(false);
	ig->setBatchInstanceDimensions(Vector3(_tileSize, 1000000, _tileSize));
	return ig;
}
//--------------------------------------------------------------------------------
void Tile::BuildInstanceBatch()
{
#if 0
	SceneManager* pMgr = GameApp::getSingleton().GetGame()->GetVisualScene();

	ShadowTechnique oldTech = pMgr->getShadowTechnique();

	typedef std::map<String, GameObjectList> MeshObjectListMap;
	MeshObjectListMap meshObjectMap;

	const GameObjectMap& staticModels = GetGameObjectsByType("StaticModel");
	for (GameObjectMap::const_iterator j = staticModels.begin(); j != staticModels.end(); ++j)
	{
		StaticModel* sm = dynamic_cast<StaticModel*>(j->second);
		if(sm->GetBatchInstance())
		{
			MeshObjectListMap::iterator i = meshObjectMap.find(sm->GetMeshFile());
			if(i == meshObjectMap.end())
			{
				i = meshObjectMap.insert(std::make_pair(sm->GetMeshFile(), GameObjectList())).first;
			}

			i->second.push_back(sm);
		}

		//ig->addEntity(sm->GetEntity(), sm->GetPosition(), sm->GetOrientation(), sm->GetScale());
		//sm->SetVisible(false);
		//sm->DestroyEntity();
	}

	// ensure no shadow vertex buffer was created when create entity for instance geometry
	pMgr->setShadowTechnique(SHADOWTYPE_NONE);
	MeshManager::getSingleton().setPrepareAllMeshesForShadowVolumes(false);

	for (MeshObjectListMap::iterator im = meshObjectMap.begin(); im != meshObjectMap.end(); ++im)
	{
		const size_t max_batch_size = 80;
		size_t objCnt = im->second.size();

		if( objCnt<= 1 )		// no need to create instance for one object
			continue;

		InstancedGeometry* ig = GetInstanceBatch(im->first);

		size_t batchSize = objCnt >  max_batch_size ? max_batch_size : objCnt;

		StaticModel* sm = dynamic_cast<StaticModel*>(im->second.front());

		String mesh = sm->GetMeshFile();
		MeshManager::getSingleton().remove(mesh);		// ensure the problem mesh is removed
		// to prevent retrieve operation

		for( int i=0; i<(int)objCnt; i++ )
		{
			StaticModel* m = dynamic_cast<StaticModel*>(im->second[i]);
			m->DestroyEntity();
		}

		uint32 queryFlag = GameObjectFactoryManager::getSingleton().GetQueryFlag<StaticModel>();

		Entity* pEnt = pMgr->createEntity( _T("_TempEntity_"), mesh );
		SetupInstancedMaterialToEntity(pEnt);
		pEnt->setQueryFlags(queryFlag);

		// add entity
		for (size_t ie = 0; ie < batchSize; ++ie)
		{
			ig->addEntity( pEnt, sm->GetPosition(), sm->GetOrientation(), sm->GetScale() );
		}

		pMgr->destroyEntity( pEnt );

		ig->setOrigin(Vector3::ZERO);
		ig->build();

		// add batch instance

		size_t numRender = 1;
		while(objCnt > max_batch_size)
		{
			numRender++;
			objCnt -= max_batch_size;
		}

		for (size_t k = 0; k < numRender - 1; ++k)
		{
			ig->addBatchInstance();
		}


		// update position
		size_t i = 0;
		InstancedGeometry::BatchInstanceIterator regIt = ig->getBatchInstanceIterator();
		int j = 0;
		while (regIt.hasMoreElements ())
		{
			InstancedGeometry::BatchInstance *r = regIt.getNext();
			r->setQueryFlags(queryFlag);
			InstancedGeometry::BatchInstance::InstancedObjectIterator bit = r->getObjectIterator();
			while(bit.hasMoreElements() && (j < (int)im->second.size()))
			{
				InstancedGeometry::InstancedObject* obj = bit.getNext();

				StaticModel* tsm = dynamic_cast<StaticModel*>(im->second[j]);
				obj->setPosition(tsm->GetPosition());
				obj->setOrientation(tsm->GetOrientation());
				obj->setScale(tsm->GetScale());
				++j;
			}
		}

		ig->setVisible(true);
	}

	//for (InstancedMap::iterator i = _instancedMap.begin(); i != _instancedMap.end(); ++i)
	//{
	//	InstancedGeometry* ig = i->second;
	//	ig->setOrigin(Vector3::ZERO);
	//	ig->build();

	//	size_t cnt = ig->getObjectCount();
	//	size_t batchCnt = cnt / max_batch_size;

	//	ig->addBatchInstance();
	//	ig->setVisible(true);
	//}

	pMgr->setShadowTechnique(oldTech);
#endif
}
//--------------------------------------------------------------------------------
void Tile::DestroyBatch()
{
	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	for (InstancedMap::iterator i = _instancedMap.begin(); i != _instancedMap.end(); ++i)
	{
		InstancedGeometry* ig = i->second;
		sm->destroyInstancedGeometry(ig);
	}

	_instancedMap.clear();
}

void Tile::SetupInstancedMaterialToEntity(Entity* ent)
{
	for (uint i = 0; i < ent->getNumSubEntities(); ++i)
	{
		SubEntity* se = ent->getSubEntity(i);
		String materialName= se->getMaterialName();
		se->setMaterialName(BuildInstancedMaterial(materialName));
	}
}
//--------------------------------------------------------------------------------
String Tile::BuildInstancedMaterial(const String& originalMaterialName)
{
	// already instanced ?
	if (StringUtil::endsWith (originalMaterialName, "/instanced"))
		return originalMaterialName;

	MaterialPtr originalMaterial = MaterialManager::getSingleton ().getByName (originalMaterialName);

	// if originalMat doesn't exists use "Instancing" material name
	const String instancedMaterialName (originalMaterial.isNull() ? "Instancing" : originalMaterialName + "/Instanced");
	MaterialPtr  instancedMaterial = MaterialManager::getSingleton ().getByName (instancedMaterialName);

	// already exists ?
	if (instancedMaterial.isNull())
	{
		instancedMaterial = originalMaterial->clone(instancedMaterialName);
		instancedMaterial->load();
		Technique::PassIterator pIt = instancedMaterial->getBestTechnique ()->getPassIterator();
		while (pIt.hasMoreElements())
		{

			Pass * const p = pIt.getNext();
			p->setVertexProgram("Instancing", false);
			p->setShadowCasterVertexProgram("InstancingShadowCaster");
		}
	}
	instancedMaterial->load();
	return instancedMaterialName;
}
//--------------------------------------------------------------------------------
bool Tile::canHandleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ)
{
	LoadTileRequest ltr = any_cast<LoadTileRequest>(req->getData());
	// only deal with own requests
	// we do this because if we delete a terrain we want any pending tasks to be discarded
	if (ltr.tile != this)
		return false;
	else
		return RequestHandler::canHandleRequest(req, srcQ);
}
//--------------------------------------------------------------------------------
bool Tile::canHandleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ)
{
	LoadTileRequest ltr = any_cast<LoadTileRequest>(res->getRequest()->getData());

	if (ltr.tile != this)
		return false;
	else
		return true;

}
//--------------------------------------------------------------------------------
WorkQueue::Response* Tile::handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ)
{
	// Background thread (maybe)

	LoadTileRequest ltr = any_cast<LoadTileRequest>(req->getData());
	// only deal with own requests; we shouldn't ever get here though
	if (ltr.tile != this)
		return 0;

	LoadTileResponse rp;

	ClassXMLSerializer ser;
	String fileName = ITile::ConvertToTileFileName(ltr.tile->_x, ltr.tile->_z);
	
	// only load exist file!
	if(ResourceGroupManager::getSingleton()
		.resourceExists(AbstractGame::SceneResourceGroupName, fileName))
	{
		rp.LoadObjects = ser.LoadFromResourceSystem(fileName,
			AbstractGame::SceneResourceGroupName);
	}

	WorkQueue::Response* response = OGRE_NEW WorkQueue::Response(req, true, Any(rp));
	return response;
}
//--------------------------------------------------------------------------------
void Tile::handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ)
{
	// Main thread

	LoadTileResponse resp = any_cast<LoadTileResponse>(res->getData());
	LoadTileRequest req = any_cast<LoadTileRequest>(res->getRequest()->getData());

	if(!req.tile->_node)
	{
		req.tile->CreateTileRootNode();
	}
	req.tile->_loadObjects = resp.LoadObjects;
	req.tile->AddGameObjects(_loadObjects);
	_game->AddGameObjects(_loadObjects);
	_dataLoadInProgress = false;
}
//--------------------------------------------------------------------------------
void Tile::WaitForDerivedProcesses()
{
	while (_dataLoadInProgress)
	{
		// we need to wait for this to finish
		OGRE_THREAD_SLEEP(50);
		Root::getSingleton().getWorkQueue()->processResponses();
	}
}
//--------------------------------------------------------------------------------
const GameObjectList& Tile::LoadedAllObjects() const
{
	return _loadObjects;
}