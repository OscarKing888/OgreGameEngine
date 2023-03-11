#pragma once
#include "ISpecialEffects.h"

typedef map<String, IEffectsElementFactory*>::type EffectsElementFactoryMap;

class _OgeSFXExport SpecialEffectsManager
	: public Singleton<SpecialEffectsManager>
	, public IUpdateAble
	, public OgeAlloc
{
public:

	SpecialEffectsManager(void);
	virtual ~SpecialEffectsManager(void);

	void ShutDown();

	ISpecialEffects* Create(const String& templateName, const String& name);
	ISpecialEffects* Create(const String& templateName);
	void Destroy(ISpecialEffects* effects);
	void DestroyAllEffects();

	// EffectsElement factory
	IEffectsElement* CreateEffectsElement(const String& className, const String& name, ISpecialEffects* parent);
	void DestroyEffectsElement(IEffectsElement* effectsElement);


	void RegisterEffectsElementFactory(const String& elementName, IEffectsElementFactory* factory, bool overrideExisting = false);
	IEffectsElementFactory* GetEffectsElementFactory(const String& elementName);
	const EffectsElementFactoryMap& GetEffectsElementFactoryMap() const { return _factoryMap; }

	template<class T>
	IEffectsElementFactory* GetEffectsElementFactory()
	{
		String classTypeName = getClassName<T>();
		return GetEffectsElementFactory(classTypeName);
	}

	template<class T>
	IEffectsElementFactory* GetElementControllerFactory()
	{
		String classTypeName = getClassName<T>();
		return GetElementControllerFactory(classTypeName);
	}


	void AddTemplate(ISpecialEffects* effects);
	void RemoveTemplate(const String& templateName);
	bool IsTemplateExists(const String& templateName);
	void ReplaceTemplate(ISpecialEffects* eff, const String& templateName);

	void LoadTemplateFromFile(const String& fileName);
	void LoadTemplateFromResourceSystem(const String& fileName);
	void LoadTemplateFromXMLRootNode(XMLElement& rootNode);
	void SaveTemplateToFile(const String& fileName);

	void PrintRegisteredFactory();

	virtual void Update(float deltaTime);

protected:

	EffectsElementFactoryMap		_factoryMap;

	typedef map<String, ISpecialEffects*>::type SpecialEffectsMap;
	SpecialEffectsMap		_templateMap;
	SpecialEffectsMap		_instanceMap;
};

struct _OgeSFXExport EffectsElementObjectFactoryRegHelper
{
	explicit EffectsElementObjectFactoryRegHelper(const String& classTypeName, IEffectsElementFactory* factory, bool overrideExisting = false)
	{
		SpecialEffectsManager::getSingleton().RegisterEffectsElementFactory(classTypeName, factory, overrideExisting);
	}
};

_OgeSFXExport ISpecialEffects* CreateEffects(const String& templateName, const String& name);
_OgeSFXExport ISpecialEffects* CreateEffects(const String& templateName);
_OgeSFXExport void DestroyEffects(ISpecialEffects* effects);


#define M_REG_EFFECTS_ELEMENT_FACTORY_EX(classTypeName, factoryClass, overrideExisting) static factoryClass _##factoryClass##Instance; \
	static EffectsElementObjectFactoryRegHelper __##classTypeName##Factory(#classTypeName, &_##factoryClass##Instance, overrideExisting);

#define M_REG_EFFECTS_ELEMENT_FACTORY(classTypeName, factoryClass) M_REG_EFFECTS_ELEMENT_FACTORY_EX(classTypeName, factoryClass, false)


template<class IT, class T, class F, class P>
class TFactory : public F
{
public:

	template<class IT2, class T2>
	struct SDestroy
	{
		SDestroy(IT2* obj)
		{
			OGRE_DELETE dynamic_cast<T2*>(obj);
		}
	};


	virtual IT* Create(const String& name, P* parent)
	{
		IT* obj = OGRE_NEW T(name, parent);
		obj->BuildPropertyMap();
		obj->SetName(name);
		return obj;
	}

	virtual void Destroy(IT* effects)
	{
		SDestroy<IT, T> d(effects);
	}
};


template<class T>
class TEffectElementFactory : public TFactory<IEffectsElement, T, IEffectsElementFactory, ISpecialEffects>
{
public:	
};