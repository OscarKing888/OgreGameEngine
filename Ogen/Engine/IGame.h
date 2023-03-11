// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-27
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"
#include "IState.h"
#include "IGameObject.h"
#include "IGameObjectFactory.h"
#include "IUpdateAble.h"

class _EngineExport IGame : public IUpdateAble
{
public:

	virtual void Init() = 0;
	virtual void ShutDown() = 0;
	virtual IStateManager& GetStateManager() = 0;
	//virtual IEffectManager& GetEffectManager() = 0;
	virtual SceneManager* GetVisualScene() = 0;
	virtual void OnGameWindowLostFocus() = 0;
	virtual void OnGameWindowSetFocus() = 0;

	virtual void RegisterGameObjectFactory(const String& classTypeName, IGameObjectFactory* factory, bool overrideExisting = false) = 0;

	virtual IGameObject* CreateGameObject(const String& classTypeName, const XGUID& guid,
		bool addToGame = true, const NameValuePairList* para = 0) = 0;

	virtual void DestroyGameObject(const XGUID& guid) = 0;
	virtual void DestroyGameObject(IGameObject* obj) = 0;
	virtual void DestoryAllGameObjectByType(const String& classTypeName) = 0;
	virtual void DestroyAllGameObject() = 0;

	virtual void AddGameObject(IGameObject* obj) = 0;
	virtual void RemoveGameObject(IGameObject* obj) = 0;
	virtual void AddGameObjects(const GameObjectList& lst) = 0;
	virtual void RemoveGameObject(const GameObjectList& lst) = 0;

	virtual const TypedGameObjectMap& GetAllGameObjects() const = 0;
	virtual const GameObjectMap& GetGameObjectsByType(const String& typeName) const = 0;
	virtual IGameObject* GetGameObject(const XGUID& guid) = 0;
	virtual bool HasGameObject(const XGUID& guid) = 0;

	virtual void CreateSceneManager(const String& sceneType) = 0;
	virtual Camera* GetActiveCamera() = 0;
	virtual Viewport* GetActiveViewport() = 0;

	virtual ITile* CreateTile(long x, long z) = 0;
	virtual void AddTile(ITile* tile) = 0;
	virtual void RemoveTile(long x, long z) = 0;
	virtual void DestroyTile(long x, long z) = 0;
	virtual void DestroyAllTile() = 0;

	virtual TilesList GetTileList() const = 0;
	virtual bool HasTile(long x, long z) const = 0;
	virtual ITile* GetTile(long x, long z) const = 0;

	virtual void LoadTile(long x, long z, bool synchronous = false) = 0;
	virtual void UnloadTile(long x, long z) = 0;
	virtual void UnloadAllTiles() = 0;
	virtual ITile* GetTileFromPosition(const Vector3& worldPos) = 0;

	virtual TerrainGroup* GetTerrainGroup() = 0;
	virtual float GetTerrainHeight(float x, float z) = 0;
	virtual float GetTerrainHeight(const Vector3& pos) = 0;
	virtual TerrainGroup::RayResult RayIntersects(const Ray& ray, Real distanceLimit = 0) = 0;

	virtual void LoadTileAndTerrain(long x, long z, bool synchronous = false) = 0;
	virtual void UnloadTileAndTerrain(long x, long z) = 0;

	virtual long GetTerrainXCount() const = 0;
	virtual long GetTerrainZCount() const = 0;
	virtual uint16 GetTerrainSize() const = 0;
	virtual float GetTerrainWorldSize() const = 0;
	
	virtual const String& GetLoadMapName() const = 0;
	virtual const String& GetSceneDir() const = 0;

	virtual TerrainGlobalOptions* GetTerrainGlobalOptions() = 0;
};
