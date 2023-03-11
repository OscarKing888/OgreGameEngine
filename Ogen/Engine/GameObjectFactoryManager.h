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
#include "IGameObjectFactory.h"


class _EngineExport GameObjectFactoryManager : public SingletonMyers<GameObjectFactoryManager>
{
public:

	typedef std::map<String, IGameObjectFactory*> FactoryMap;
	typedef std::map<String, uint32> QueryFlagMap;

	void RegisterFactory(const String& classTypeName, IGameObjectFactory* factory, bool overrideExisting = false);
	IGameObjectFactory* GetFactory(const String& classTypeName);

	const FactoryMap& GetFactoryMap() const { return _factoryMap; }

	void PrintRegisteredFactory();

	template<class T>
	IGameObjectFactory* GetFactory()
	{
		String classTypeName = getClassName<T>();
		return GetFactory(classTypeName);
	}

	void SetQueryFlag(const String& classTypeName, uint32 flag);
	uint32 GetQueryFlag(const String& classTypeName) const;

	template<class T>
	void SetQueryFlag(uint32 flag)
	{
		String classTypeName = getClassName<T>();
		SetQueryFlag(classTypeName, flag);
	}

	template<class T>
	uint32 GetQueryFlag()
	{
		String classTypeName = getClassName<T>();
		return GetQueryFlag(classTypeName);
	}

protected:

	FactoryMap		_factoryMap;
	QueryFlagMap	_queryFlagMap;
};

struct GameObjectFactoryRegHelper
{
	explicit GameObjectFactoryRegHelper(const String& classTypeName, const String& classCategory, IGameObjectFactory* factory, bool overrideExisting = false)
	{
		factory->InitClassCategory(classCategory);
		GameObjectFactoryManager::getSingleton().RegisterFactory(classTypeName, factory, overrideExisting);
	}
};

#define M_REG_FACTORY_EX(classTypeName, factoryClass, classCategory, overrideExisting) static factoryClass _##factoryClass##Instance; \
	static GameObjectFactoryRegHelper __##classTypeName##Factory(#classTypeName, #classCategory, &_##factoryClass##Instance, overrideExisting);

#define M_REG_FACTORY(classTypeName, factoryClass, classCategory) M_REG_FACTORY_EX(classTypeName, factoryClass, classCategory, false)

