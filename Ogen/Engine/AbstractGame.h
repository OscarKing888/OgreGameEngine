#pragma once
#include "EnginePrerequisites.h"
#include "IGame.h"
#include "IState.h"
#include "IEffect.h"
#include "EnumDescriptorManager.h"
#include "AbstractGameObject.h"
#include "ClassUtil.h"
#include "Trigger.h"

class _EngineExport AbstractGame
	: public IGame
	, public ITriggerEventHandler
	, public OgeAlloc
	, public ResourceBackgroundQueue::Listener
{
public:

	static String Event_PostSceneLoad;
	static String SceneResourceGroupName;

	AbstractGame(void);
	virtual ~AbstractGame(void);

	virtual void Init();
	virtual void ShutDown();
	virtual IStateManager& GetStateManager();
	virtual IEffectManager& GetEffectManager();
	virtual SceneManager* GetVisualScene();
	virtual void OnGameWindowLostFocus();
	virtual void OnGameWindowSetFocus();

	virtual void RegisterGameObjectFactory(const String& classTypeName,
		IGameObjectFactory* factory, bool overrideExisting = false);

	virtual IGameObject* CreateGameObject(const String& classTypeName,
		const XGUID& guid, bool addToGame = true,
		const NameValuePairList* para = 0);

	virtual void DestroyGameObject(const XGUID& guid);
	virtual void DestroyGameObject(IGameObject* obj);
	virtual void DestoryAllGameObjectByType(const String& classTypeName);
	virtual void DestroyAllGameObject();

	virtual const TypedGameObjectMap& GetAllGameObjects() const;
	virtual const GameObjectMap& GetGameObjectsByType(const String& typeName) const;
	virtual IGameObject* GetGameObject(const XGUID& guid);
	virtual bool HasGameObject(const XGUID& guid);

	virtual void SetShadowTechnique(ShadowTechnique t, bool soft = false);

	virtual void ShowDebugOverlay(bool on);
	virtual void SetDebugText(const String& debugTxt);

	virtual void SetPolygonMode(PolygonMode mode);

	template<class T>
	T* GetGameObject(const XGUID& guid)
	{
		T* obj = dynamic_cast<T*>(GetGameObject(guid));
		return obj;
	}

	template<class T>
	T* CreateGameObject(const XGUID& guid, bool addToGame = true,
		const NameValuePairList* para = 0)
	{
		T* obj = dynamic_cast<T*>(CreateGameObject(
			getClassName<T>(), guid, addToGame, para));

		return obj;
	}

	/**
	* 在屏幕上显示文字，坐标以像素为单位，由于使用Ogre overlay实现
	* 需要为每个DrawString指定一个name作为唯一标识
	*/
	static void DrawString(
		const String& name, const String& text, int x, int y);

	virtual void CreateSceneManager(const String& sceneType);

	virtual void SaveMap(const String& mapName, ISerializationMask* query = 0);
	virtual void LoadMap(const String mapName);

	virtual void UnloadMap();
	
	void UnloadUnreferencedReceoruces(const String& resGroupName);

	/// 加入自创建物体进行管理，退出游戏时会自动删除相应物体
	virtual void AddGameObject(IGameObject* o);
	virtual void RemoveGameObject(IGameObject* obj);
	virtual void AddGameObjects(const GameObjectList& lst);
	virtual void RemoveGameObject(const GameObjectList& lst);

	virtual ITile* CreateTile(long x, long z);
	virtual void AddTile(ITile* tile);
	virtual void RemoveTile(long x, long z);
	virtual void DestroyTile(long x, long z);
	virtual void DestroyAllTile();

	virtual TilesList GetTileList() const;
	virtual bool HasTile(long x, long z) const;
	virtual ITile* GetTile(long x, long z) const;

	virtual void LoadTile(long x, long z, bool synchronous = false);
	virtual void UnloadTile(long x, long z);
	virtual void UnloadAllTiles();
	virtual ITile* GetTileFromPosition(const Vector3& worldPos);

	virtual TerrainGroup* GetTerrainGroup();
	virtual float GetTerrainHeight(const Vector3& pos);
	virtual float GetTerrainHeight(float x, float z);
	virtual TerrainGroup::RayResult RayIntersects(const Ray& ray,
		Real distanceLimit = 0);

	/// Return the scene directory
	virtual const String& GetSceneDir() const;

	virtual long GetTerrainXCount() const;
	virtual long GetTerrainZCount() const;
	virtual uint16 GetTerrainSize() const;
	virtual float GetTerrainWorldSize() const;

	virtual const String& GetLoadMapName() const;


	virtual void LoadTileAndTerrain(long x, long z, bool synchronous = false);
	virtual void UnloadTileAndTerrain(long x, long z);

	virtual TerrainGlobalOptions* GetTerrainGlobalOptions();

protected:

	TypedGameObjectMap	_objects;
	SceneManager*		_visualScene;
	IStateManager*		_stateManager;
	IEffectManager*		_effectManager;

	int					_shadowTechIdx;
	bool				_softShadow;
	Overlay*			_debugOverlay;
	GameObjectList		_loadedGameObjectList;

	TerrainGlobalOptions* _terrainGlobalOpt;
	TerrainGroup* _terrainGroup;
	String				_mapName;

	BackgroundProcessTicket _initMapResGrpTicket;

	typedef map<int, ITile*>::type TileZMap;
	typedef map<int, TileZMap>::type TileMap;
	TileMap				_tileMap;
	long				_terrainXCnt;
	long				_terrainZCnt;
	uint16				_terrainSize;
	float				_terrainWorldSize;

	virtual void PreInit();
	virtual void InitImp();
	virtual void PostInit();

	virtual void InitTerrain();

	virtual void CreateVisualScene();
	virtual SceneManager* _CreateVisualScene();

	virtual void Update(float deltaTime);

	void SelectNextShadowTechnique();

	void UpdateStats(float deltaTime);

	/** 根据配置文件中的数值初始化相机 */
	void InitCamera(Camera* cam);

	void DestroySceneManager();

	virtual void OnTouch(Trigger* trigger, IGameObject* object);
	virtual void OnStay(Trigger* trigger, IGameObject* object);
	virtual void OnLeave(Trigger* trigger, IGameObject* object);

	virtual void CheckTriggers();

	virtual void operationCompleted(BackgroundProcessTicket ticket,
		const BackgroundProcessResult& result);

	virtual void OnMapResourceGroupReady();
};

