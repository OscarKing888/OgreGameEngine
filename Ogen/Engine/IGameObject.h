// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"

class ITile;
class IStateManager;

class _EngineExport IGameObject : virtual public IPropertyObject
{
public:

	virtual const String& GetTypeName() const = 0;

	virtual const XGUID& GetGUID() const = 0;
	virtual void SetGUID(const XGUID& guid) = 0;

	//virtual IGameObject* Clone(XGUID newGUID = XGUID::createGUID()) const = 0;

	virtual void Destroy() = 0;

	virtual void SetSerializeable(bool on) = 0;
	virtual bool GetSerializeable() const = 0;

	virtual uint32 GetTriggerMask() const = 0;
	virtual void SetTriggerMask(uint32 mask) = 0;

	virtual void OnAddToScene(SceneNode* rootNode) = 0;
	virtual void OnRemoveFromScene(SceneNode* rootNode) = 0;

	virtual IStateManager* GetStateManager() = 0;
	virtual void ActiveState(const Ogre::String& stateID) = 0;
	virtual const Ogre::String& GetActiveStateID() const = 0;
	virtual const Ogre::String& GetPreviousStateID() const = 0;
	virtual void PushState(const Ogre::String& stateID) = 0;
	virtual void PopState() = 0;
	virtual Ogre::StringVector GetStateNameList() const = 0;
	virtual Ogre::StringVector GetStateStack() const = 0;

	//virtual ITile* GetTile() const = 0;

	virtual void PreSave() = 0;
	virtual void PostSave() = 0;
	virtual void PreLoad() = 0;
	virtual void PostLoad() = 0;
	virtual void PostSceneLoad() = 0;
};

typedef vector<IGameObject*>::type GameObjectList;
typedef map<XGUID, IGameObject*>::type GameObjectMap;
typedef map<String, GameObjectMap>::type TypedGameObjectMap;

//----------------------------------------------------------------------------------------------

class ISerializationMask;

class _EngineExport ITile
{
public:

	virtual long GetX() const = 0;
	virtual long GetZ() const = 0;
	virtual void SetCoord(long x, long z) = 0;

	virtual void SetSize(uint16 sz) = 0;
	virtual uint16 GetSize() const = 0;

	virtual void Destroy() = 0;

	virtual void DestroyGameObject(const XGUID& guid) = 0;
	virtual void DestroyGameObject(IGameObject* obj) = 0;
	virtual void DestoryAllGameObjectByType(const String& classTypeName) = 0;
	virtual void DestroyAllGameObject() = 0;

	virtual void AddGameObject(IGameObject* obj) = 0;
	virtual void RemoveGameObject(IGameObject* obj) = 0;

	virtual const GameObjectList& LoadedAllObjects() const = 0;
	virtual const TypedGameObjectMap& GetAllGameObjects() const = 0;
	virtual const GameObjectMap& GetGameObjectsByType(const String& typeName) const = 0;
	virtual IGameObject* GetGameObject(const XGUID& guid) = 0;
	virtual bool HasGameObject(const XGUID& guid) = 0;

	virtual SceneNode* GetRootSceneNode() const = 0;

	virtual void Load(bool synchronous = false) = 0;
	virtual void Save(const String& sceneDir, ISerializationMask* query, bool onlyIfModified) = 0;
	virtual void Unload() = 0;

	virtual bool IsLoaded() const = 0;
	virtual bool IsModified() const = 0;
	virtual void SetModified(bool on) = 0;

	static String ConvertToTileFileName(long x, long z);
	static bool ConvertFromTileFileName(const String& fileName, long& x, long& z);
};

typedef vector<ITile*>::type TilesList;

//----------------------------------------------------------------------------------------------
// Clone macros
//----------------------------------------------------------------------------------------------
#define DeepClone(className)														\
public:																				\
	virtual IGameObject* Clone(XGUID newGUID = XGUID::createGUID()) const			\
	{																				\
		className* obj = OGRE_NEW className(*this, newGUID);						\
		obj->BuildPropertyMap();													\
		return obj;																	\
	}																				\
	void Assign(const className &src, XGUID newGUID);								\
	className(const className &src, XGUID newGUID)									\
	{ className::Assign(src, newGUID); }


