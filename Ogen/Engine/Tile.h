#pragma once
#include "EnginePrerequisites.h"

class _EngineExport Tile
	: public ITile
	, public WorkQueue::RequestHandler
	, public WorkQueue::ResponseHandler
	, public OgeAlloc
{
public:

	Tile(SceneManager* sm, IGame* game);
	virtual ~Tile();

	virtual long GetX() const { return _x; }
	virtual long GetZ() const { return _z; }
	virtual void SetCoord(long x, long z);
	virtual void SetSize(uint16 sz) { _tileSize = sz; }
	virtual uint16 GetSize() const { return _tileSize; }

	virtual SceneNode* GetRootSceneNode() const { return _node; }
	virtual void AddGameObject(IGameObject* o);
	virtual void RemoveGameObject(IGameObject* obj);

	virtual void DestroyGameObject(const XGUID& guid);
	virtual void DestroyGameObject(IGameObject* obj);
	virtual void DestoryAllGameObjectByType(const String& classTypeName);
	virtual void DestroyAllGameObject();

	virtual const GameObjectList& LoadedAllObjects() const;
	virtual const TypedGameObjectMap& GetAllGameObjects() const;
	virtual const GameObjectMap& GetGameObjectsByType(const String& typeName) const;
	virtual IGameObject* GetGameObject(const XGUID& guid);
	virtual bool HasGameObject(const XGUID& guid);

	virtual void Load(bool synchronous = false);
	virtual void Save(const String& sceneDir, ISerializationMask* query, bool onlyIfModified);
	virtual void Unload();
	virtual void Destroy();

	virtual bool IsLoaded() const { return _isLoaded; }
	virtual bool IsModified() const { return _modified; }
	virtual void SetModified(bool on) { _modified = on; }

	template<class T>
	T* GetGameObject(const XGUID& guid)
	{
		T* obj = dynamic_cast<T*>(GetGameObject(guid));
		return obj;
	}

	void AddGameObjects(const GameObjectList& lst);

protected:

	TypedGameObjectMap	_objects;
	GameObjectList _loadObjects;
	IGame*	_game;

	SceneManager*	_visualScene;
	long _x;
	long _z;
	uint16 _tileSize;
	bool _modified;
	bool _isLoaded;

	SceneNode* _node;

	typedef map<String, InstancedGeometry*>::type InstancedMap;
	InstancedMap		_instancedMap;

	struct LoadTileRequest
	{
		Tile* tile;
		_EngineExport friend std::ostream& operator<<(std::ostream& o, const LoadTileRequest& r)
		{ return o; }		
	};

	struct LoadTileResponse
	{
		GameObjectList LoadObjects;
		_EngineExport friend std::ostream& operator<<(std::ostream& o, const LoadTileResponse& r)
		{ return o; }		
	};


	virtual void CreateTileRootNode();
	virtual void DestroyTileRootNode();

	virtual void BuildInstanceBatch();
	virtual void DestroyBatch();

	virtual void SetupInstancedMaterialToEntity(Entity* ent);
	virtual String BuildInstancedMaterial(const String& originalMaterialName);

	InstancedGeometry* GetInstanceBatch(const String& meshName);

	/// WorkQueue::RequestHandler override
	virtual bool canHandleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
	/// WorkQueue::RequestHandler override
	virtual WorkQueue::Response* handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
	/// WorkQueue::ResponseHandler override
	virtual bool canHandleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);
	/// WorkQueue::ResponseHandler override
	virtual void handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);

	static const uint16 WORKQUEUE_TILE_DATA_REQUEST;
	uint16 _workQueueChannel;
	bool _dataLoadInProgress;
	
	void WaitForDerivedProcesses();
};
