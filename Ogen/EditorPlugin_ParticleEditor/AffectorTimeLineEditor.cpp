#include "stdafx.h"
#include "AffectorTimeLineEditor.h"


AffectorTimeLineEditorFactoryManager::AffectorTimeLineEditorFactoryManager(void)
{
}

AffectorTimeLineEditorFactoryManager::~AffectorTimeLineEditorFactoryManager(void)
{
}

IAffectorTimeLineEditor* AffectorTimeLineEditorFactoryManager::createAffectorTimeLineEditor(
	const String& classTypeName, CWnd* parentPanel)
{
	IAffectorTimeLineEditorFactory* f = getFactory(classTypeName);
	if(!f)
	{
		/*OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Factory not fount for:" + prop->getTypeName(),
			"AffectorTimeLineEditorFactoryManager::createTypeUIEditor(IPropertyPtr)");*/

		return 0;
	}

	return f->CreateEditor(parentPanel);
}

void AffectorTimeLineEditorFactoryManager::registerFactory(
	const String& classTypeName,
	IAffectorTimeLineEditorFactory* factory, bool overrideExisting)
{
	FactoryMap::iterator i = _factoryMap.find(classTypeName);

	if(overrideExisting)
	{
		if(i != _factoryMap.end())
		{
			LogWarningIDf("Editor", "AffectorTimeLineEditorFactoryManager factory[%s] class type[%s] is replaced by class type[%s]",
				classTypeName.c_str(), getClassName(i->second),
				getClassName(factory));
		}
	}
	else if(i != _factoryMap.end())
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
			"Object factory already exist [" + classTypeName + "]", 
			"AffectorTimeLineEditorFactoryManager::registerFactory");
	}

	if(i != _factoryMap.end())
	{
		_factoryMap.erase(i);
	}

	_factoryMap.insert(std::make_pair(classTypeName, factory));
}

IAffectorTimeLineEditorFactory* AffectorTimeLineEditorFactoryManager::getFactory(const String& classTypeName)
{
	FactoryMap::iterator i = _factoryMap.find(classTypeName);

	if(i != _factoryMap.end())
	{
		return i->second;
	}

	return 0;
}