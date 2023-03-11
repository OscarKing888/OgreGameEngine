//  -------------------------------------------------------------
// 	Copyright (c) 2007 XGames
// 	By   :	Oscar.Chen
// 	Date :	2008-08-24
//  Copyright (C) 2008 - All Rights Reserved
//  -------------------------------------------------------------

#pragma once
#include "EditorCorePrerequisites.h"

class PropertyEditorBase;

class _EditorCoreExport ITypeUIEditorFactory
{
public:

	virtual PropertyEditorBase* createTypeUIEditor(IPropertyPtr prop) = 0;		
};

//------------------------------------------------------------------------------

template <class T>
class TTypeUIEditorFactor : public ITypeUIEditorFactory
{
	virtual PropertyEditorBase* createTypeUIEditor(IPropertyPtr prop)
	{
		return new T(prop);
	}
};

//------------------------------------------------------------------------------

class _EditorCoreExport TypeUIEditorFactoryManager
	: public SingletonMyers<TypeUIEditorFactoryManager>
{
public:

	typedef map<String, ITypeUIEditorFactory*>::type FactoryMap;

	TypeUIEditorFactoryManager(void);
	virtual ~TypeUIEditorFactoryManager(void);

	void registerFactory(const String& classTypeName, ITypeUIEditorFactory* factory, bool overrideExisting = false);
	ITypeUIEditorFactory* getFactory(const String& classTypeName);

	const FactoryMap& getFactoryMap() const { return _factoryMap; }

	void printRegisteredFactory();

	virtual PropertyEditorBase* createTypeUIEditor(IPropertyPtr prop);

protected:

	FactoryMap	_factoryMap;		
};

//------------------------------------------------------------------------------

struct _EditorCoreExport TypeUIEditorFactoryRegHelper
{
	explicit TypeUIEditorFactoryRegHelper(const String& propTypeName,
		ITypeUIEditorFactory* factory, bool overrideExisting = false)
	{
		TypeUIEditorFactoryManager::getSingleton()
			.registerFactory(propTypeName, factory, overrideExisting);
	}
};

#define M_REG_TYPEUIEDITOR_FACTORY_EX(propTypeName, factoryClass, overrideExisting) static factoryClass _##factoryClass##Instance; \
	static TypeUIEditorFactoryRegHelper __##factoryClass##RegHelper(#propTypeName, &_##factoryClass##Instance, overrideExisting);

#define M_REG_TYPEUIEDITOR_FACTORY(propTypeName, factoryClass) M_REG_TYPEUIEDITOR_FACTORY_EX(propTypeName, factoryClass, false)
