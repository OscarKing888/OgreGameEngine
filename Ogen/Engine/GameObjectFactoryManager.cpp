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
#include "GameObjectFactoryManager.h"

template<> GameObjectFactoryManager* Singleton<GameObjectFactoryManager>::ms_Singleton = 0;


 
	void GameObjectFactoryManager::RegisterFactory(const String& classTypeName, IGameObjectFactory* factory, bool overrideExisting)
	{
		FactoryMap::iterator i = _factoryMap.find(classTypeName);

		if(overrideExisting)
		{
			if(i != _factoryMap.end())
			{
				LogWarningIDf("Engine", "Game object factory[%s] class type[%s] is replaced by class type[%s]",
					classTypeName.c_str(), getClassName(i->second), getClassName(factory));
			}
		}
		else if(i != _factoryMap.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"Object factory already exist [" + classTypeName + "]", 
				"GameObjectFactoryManager::RegisterFactory");
		}

		if(i != _factoryMap.end())
		{
			_factoryMap.erase(i);
		}

		_factoryMap.insert(std::make_pair(classTypeName, factory));
	}

	IGameObjectFactory* GameObjectFactoryManager::GetFactory(const String& classTypeName)
	{
		FactoryMap::iterator i = _factoryMap.find(classTypeName);

		if(i != _factoryMap.end())
		{
			return i->second;
		}

		LogErrorIDf("Engine", "[GameObjectFactoryManager::GetFactory] Cannot find game object factory with name[%s]!", classTypeName.c_str());
		return 0;

// 		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
// 			"Cannot find game object factory with name [" + classTypeName + "]", 
// 			"GameObjectFactoryManager::GetFactory");
	}

	void GameObjectFactoryManager::PrintRegisteredFactory()
	{
#ifndef _PRODUCT_
		printf("---------Registered Game Object Factory---------\n");
		FactoryMap::iterator i = _factoryMap.begin();
		for(; i != _factoryMap.end(); ++i)
		{
			printf("[%s] -> [%s]\n", i->first.c_str(), getClassName(i->second).c_str());
		}
		printf("------------------------------------------------\n");
#endif
	}

	void GameObjectFactoryManager::SetQueryFlag(const String& classTypeName, uint32 flag)
	{
		_queryFlagMap[classTypeName] = flag;
	}

	uint32 GameObjectFactoryManager::GetQueryFlag(const String& classTypeName) const
	{
		QueryFlagMap::const_iterator i = _queryFlagMap.find(classTypeName);
		if(i != _queryFlagMap.end())
		{
			return i->second;
		}

		return 0;
	}

