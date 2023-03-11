//  -------------------------------------------------------------
// 	Copyright (c) 2007 XGames
// 	By   :	Oscar.Chen
// 	Date :	2008-08-24
//  Copyright (C) 2008 - All Rights Reserved
//  -------------------------------------------------------------

#include "StdAfx.h"
#include "TypeUIEditorFactory.h"

TypeUIEditorFactoryManager::TypeUIEditorFactoryManager(void)
{
}

TypeUIEditorFactoryManager::~TypeUIEditorFactoryManager(void)
{
}

PropertyEditorBase* TypeUIEditorFactoryManager::createTypeUIEditor(IPropertyPtr prop)
{
	ITypeUIEditorFactory* f = getFactory(prop->GetTypeName());
	if(!f)
	{
		f = getFactory("_Base_");
		/*OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Factory not fount for:" + prop->getTypeName(),
			"TypeUIEditorFactoryManager::createTypeUIEditor(IPropertyPtr)");*/
	}
	return f->createTypeUIEditor(prop);
}

void TypeUIEditorFactoryManager::registerFactory(const String& classTypeName,
												 ITypeUIEditorFactory* factory, bool overrideExisting)
{
	FactoryMap::iterator i = _factoryMap.find(classTypeName);

	if(overrideExisting)
	{
		if(i != _factoryMap.end())
		{
			LogWarningIDf("Editor", "TypeUIEditorFactoryManager factory[%s] class type[%s] is replaced by class type[%s]",
				classTypeName.c_str(), getClassName(i->second),
				getClassName(factory));
		}
	}
	else if(i != _factoryMap.end())
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
			"Object factory already exist [" + classTypeName + "]", 
			"TypeUIEditorFactoryManager::registerFactory");
	}

	if(i != _factoryMap.end())
	{
		_factoryMap.erase(i);
	}

	_factoryMap.insert(std::make_pair(classTypeName, factory));
}

ITypeUIEditorFactory* TypeUIEditorFactoryManager::getFactory(const String& classTypeName)
{
	FactoryMap::iterator i = _factoryMap.find(classTypeName);

	if(i != _factoryMap.end())
	{
		return i->second;
	}

	return 0;
}

void TypeUIEditorFactoryManager::printRegisteredFactory()
{
#ifndef _PRODUCT_
	printf("---------Registered TypeUIEditor Factory---------\n");
	FactoryMap::iterator i = _factoryMap.begin();
	for(; i != _factoryMap.end(); ++i)
	{
		printf("[%s] -> [%s]\n", i->first.c_str(), getClassName(i->second).c_str());
	}
	printf("------------------------------------------------\n");
#endif
}