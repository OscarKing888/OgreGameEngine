#include "StdAfx.h"
#include "SpecialEffectsManager.h"
#include "SpecialEffects.h"

const String C_EffectElement = "EffectElement";
const String SFXLogID = "SFX";

template<> SpecialEffectsManager* Singleton<SpecialEffectsManager>::ms_Singleton = 0;

ISpecialEffects* CreateEffects(const String& templateName, const String& name)
{
	return SpecialEffectsManager::getSingleton().Create(templateName, name);
}

ISpecialEffects* CreateEffects(const String& templateName)
{
	return SpecialEffectsManager::getSingleton().Create(templateName);
}

void DestroyEffects(ISpecialEffects* effects)
{
	SpecialEffectsManager::getSingleton().Destroy(effects);
}


SpecialEffectsManager::SpecialEffectsManager(void)
{
	GameApp::getSingleton().AddUpdateable(this);
}

SpecialEffectsManager::~SpecialEffectsManager(void)
{
	GameApp::getSingleton().RemoveUpdateable(this);
}

ISpecialEffects* SpecialEffectsManager::Create(const String& templateName,
											   const String& name)
{
	SpecialEffectsMap::iterator i = _templateMap.find(templateName);
	if(i != _templateMap.end())
	{
		if(_instanceMap.find(name) != _instanceMap.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"Special Effects already exist [" + name + "]", 
				"SpecialEffectsManager::Create");
		}
		else
		{
			ISpecialEffects* se = i->second;
			ISpecialEffects* newSe = se->Clone(name);
			_instanceMap.insert(std::make_pair(name, newSe));
			LogDebugIDf(SFXLogID, "SpecialEffects clone template[%s] name[%s]", templateName.c_str(), name.c_str());
			return newSe;
		}			
	}

	OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
		"Special Effects template not found [" + templateName + "]", 
		"SpecialEffectsManager::Create");
}

ISpecialEffects* SpecialEffectsManager::Create(const String& templateName)
{
	static uint32 autoID = 0;
	String autoName = templateName + formatString("[%u]", autoID);
	++autoID;
	return Create(templateName, autoName);
}


void SpecialEffectsManager::Destroy(ISpecialEffects* effects)
{
	SpecialEffectsMap::iterator i = _instanceMap.find(effects->GetName());
	if(i != _instanceMap.end())
	{
		_instanceMap.erase(i);
	}
	else
	{
		// 这个地方还要删除特效一般是对象生存周期没有管理好
		// 为避免crash直接返回
		return;
		//assert(false && "[SpecialEffectsManager::Destroy] effects not found when destroy!");
	}

	SpecialEffects* se = dynamic_cast<SpecialEffects*>(effects);
	assert(se);
	OGRE_DELETE se;
}

IEffectsElement* SpecialEffectsManager::CreateEffectsElement(const String& className,
															 const String& name, ISpecialEffects* parent)
{
	IEffectsElementFactory* f = GetEffectsElementFactory(className);
	assert(f);
	IEffectsElement* element = f->Create(name, parent);
	return element;
}

void SpecialEffectsManager::DestroyEffectsElement(IEffectsElement* effectsElement)
{
	IEffectsElementFactory* f = GetEffectsElementFactory(effectsElement->GetTypeName());
	assert(f);
	f->Destroy(effectsElement);
}

void SpecialEffectsManager::DestroyAllEffects()
{
	SpecialEffectsMap::iterator i = _instanceMap.begin();
	for (; i != _instanceMap.end();)
	{
		SpecialEffects* se = dynamic_cast<SpecialEffects*>(i->second);
		assert(se);
		if(!se->GetAutoLifeTimeManage())
		{
			OGRE_DELETE se;
			i = _instanceMap.erase(i);
		}
		else
		{
			++i;
		}
	}
	//_instanceMap.clear();
}

void SpecialEffectsManager::RegisterEffectsElementFactory(const String& classTypeName,
														  IEffectsElementFactory* factory, bool overrideExisting)
{
	EffectsElementFactoryMap::iterator i = _factoryMap.find(classTypeName);

	if(overrideExisting)
	{
		if(i != _factoryMap.end())
		{
			LogWarningIDf(SFXLogID, "[%s] Effect object factory[%s] class type[%s] is replaced by class type[%s]",
				__FUNCTION__, classTypeName.c_str(), getClassName(i->second), getClassName(factory));
		}
	}
	else if(i != _factoryMap.end())
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
			"Object factory already exist [" + classTypeName + "]", 
			"SpecialEffectsManager::RegisterEffectsElementFactory");
	}

	if(i != _factoryMap.end())
	{
		_factoryMap.erase(i);
	}

	_factoryMap.insert(std::make_pair(classTypeName, factory));
}

IEffectsElementFactory* SpecialEffectsManager::GetEffectsElementFactory(const String& classTypeName)
{
	EffectsElementFactoryMap::iterator i = _factoryMap.find(classTypeName);

	if(i != _factoryMap.end())
	{
		return i->second;
	}

	OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
		"Cannot find effect object factory with name [" + classTypeName + "]", 
		"SpecialEffectsManager::GetEffectsElementFactory");
}

void SpecialEffectsManager::PrintRegisteredFactory()
{
#ifndef _PRODUCT_
	LogConsoleIDf(SFXLogID, "---------Registered  SpecialEffects Object Factory---------\n");
	EffectsElementFactoryMap::iterator i = _factoryMap.begin();
	for(; i != _factoryMap.end(); ++i)
	{
		LogConsoleIDf("SFX", "[%s] -> [%s]\n", i->first.c_str(), getClassName(i->second).c_str());
	}
	LogConsoleIDf(SFXLogID, "------------------------------------------------\n");
#endif
}

void SpecialEffectsManager::LoadTemplateFromFile(const String& fileName)
{
	XMLDoc doc;
	if(doc.loadFromFile(fileName))
	{
		XMLElement e = doc.getRoot();
		LoadTemplateFromXMLRootNode(e);
	}
	else
	{
		LogErrorIDf(SFXLogID, "[SpecialEffectsManager::LoadTemplateFromFile] error xml file format:[%s]", fileName.c_str());
		assert(false && "[SpecialEffectsManager::LoadTemplateFromFile] error xml file format!");
	}
}

void SpecialEffectsManager::LoadTemplateFromResourceSystem(const String& fileName)
{
	if(ResourceGroupManager::getSingleton().resourceExists(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, fileName))
	{
		DataStreamPtr dat = ResourceGroupManager::getSingleton().openResource(fileName);
		XMLDoc doc;
		if(doc.loadFromXML(dat->getAsString()))
		{
			XMLElement e = doc.getRoot();
			LoadTemplateFromXMLRootNode(e);
		}
		else
		{
			LogErrorIDf(SFXLogID, "[SpecialEffectsManager::LoadTemplateFromResourceSystem] error xml file format:[%s]", fileName.c_str());
		}
	}
	else
	{
		LogErrorIDf(SFXLogID, "[SpecialEffectsManager::LoadTemplateFromResourceSystem] file not found:[%s]", fileName.c_str());
	}
}

void SpecialEffectsManager::LoadTemplateFromXMLRootNode(XMLElement& rootNode)
{
	XMLElement effectsNode = rootNode.firstChild();
	while(!effectsNode.isNull())
	{
		String effectName = effectsNode.getAttribute("Name");
		SpecialEffectsMap::iterator it = _templateMap.find(effectName);
		if(it != _templateMap.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"SpecialEffects template already exist [" + effectName + "]", 
				"SpecialEffectsManager::LoadTemplateFromXMLRootNode");
		}

		ISpecialEffects* effects = OGRE_NEW SpecialEffects(effectName);
		_templateMap.insert(std::make_pair(effectName, effects));

		XMLElement effectsPropNode = effectsNode.firstChild();
		while (!effectsPropNode.isNull())
		{
			String propName = effectsPropNode.name();
			assert(!propName.empty());
			if(propName == C_EffectElement)
			{
				String elementType = effectsPropNode.getAttribute("Type");
				String elementName = effectsPropNode.getAttribute("Name");

				IEffectsElement* effectsElemet = CreateEffectsElement(elementType, elementName, effects);

				XMLElement elementPropNode = effectsPropNode.firstChild();
				while (!elementPropNode.isNull())
				{
					String elemChildNodeName = elementPropNode.name();
					// load controller
					String propVal = elementPropNode.getAttribute("Value");
					String propName = elementPropNode.name();
					effectsElemet->SetProperty(propName, propVal);
					elementPropNode = elementPropNode.nextSibling();
				}

				effects->AddElement(effectsElemet);
			}
			else // load special effects properties
			{
				String propVal = effectsPropNode.getAttribute("Value");
				effects->SetProperty(propName, propVal);
			}

			effectsPropNode = effectsPropNode.nextSibling();
		}

		effectsNode = effectsNode.nextSibling();
	}
}

/*
*<SpecialEffects>
*	<Effect Name=...>
*		<PropName Value=.../>
*		<PropName Value=.../>
*		<EffectElement Type=... Name=...>
*			<PropName Value=.../>
*			<PropName Value=.../>
*		</EffectElement>
*	</Effect>
*</SpecialEffects>
*/
void SpecialEffectsManager::SaveTemplateToFile(const String& fileName)
{
	XMLDoc doc;
	doc.createRootNode("SpecialEffects");
	XMLElement root = doc.getRoot();

	for (SpecialEffectsMap::iterator i = _templateMap.begin(); i != _templateMap.end(); ++i)
	{
		ISpecialEffects* obj = i->second;
		const String& name = obj->GetName();
		XMLElement effectsNode = root.addChild("Effect");
		effectsNode.setAttribute("Name", name);

		// save special effect property
		const PropertyMap& effectPropMap = obj->GetProperties();
		for (PropertyMap::const_iterator i = effectPropMap.begin(); i != effectPropMap.end(); ++i)
		{
			const IPropertyPtr p = i->second;
			if(p->IsSerializeable())
			{
				String propName = p->GetName();
				String propValue = p->ToString();
				XMLElement eProp = effectsNode.addChild(propName);
				eProp.setAttribute("Value", propValue);
			} // if
		} // for

		// save effect elements

		size_t cnt = obj->GetElementsCount();
		for (size_t j = 0; j < cnt; ++j)
		{
			IEffectsElement* ee = obj->GetElementsByIndex(j);
			XMLElement elementNode = effectsNode.addChild(C_EffectElement);
			const String& elementType = ee->GetTypeName();
			elementNode.setAttribute("Type", elementType);
			elementNode.setAttribute("Name", ee->GetName());

			// save element properties
			const PropertyMap& m = ee->GetProperties();
			for (PropertyMap::const_iterator i = m.begin(); i != m.end(); ++i)
			{
				const IPropertyPtr p = i->second;
				if(p->IsSerializeable())
				{
					String propName = p->GetName();
					String propValue = p->ToString();
					XMLElement eProp = elementNode.addChild(propName);
					eProp.setAttribute("Value", propValue);
				} // if
			} // for
		} // for
	}

	doc.save(fileName);
}

void SpecialEffectsManager::Update(float deltaTime)
{
	typedef list<ISpecialEffects*>::type DeadList;
	DeadList deadList;

	SpecialEffectsMap::iterator i = _instanceMap.begin();
	for (; i != _instanceMap.end(); ++i)
	{
		ISpecialEffects* spe = i->second;
		spe->Update(deltaTime);
		if(spe->IsEnd() && spe->GetAutoLifeTimeManage())
		{
			deadList.push_back(spe);
		}
	}

	for (DeadList::iterator j = deadList.begin(); j != deadList.end(); ++j)
	{
		Destroy(*j);
	}

	deadList.clear();
}

void SpecialEffectsManager::ShutDown()
{
	SpecialEffectsMap::iterator i = _instanceMap.begin();
	for (; i != _instanceMap.end(); ++i)
	{
		ISpecialEffects* spe = i->second;
		SpecialEffects* se = dynamic_cast<SpecialEffects*>(spe);
		assert(se);
		OGRE_DELETE se;
	}

	i = _templateMap.begin();
	for (; i != _templateMap.end(); ++i)
	{
		ISpecialEffects* spe = i->second;
		SpecialEffects* se = dynamic_cast<SpecialEffects*>(spe);
		assert(se);
		OGRE_DELETE se;
	}

	_instanceMap.clear();
	_templateMap.clear();
}

void SpecialEffectsManager::AddTemplate(ISpecialEffects* effects)
{
	SpecialEffectsMap::iterator i = _templateMap.find(effects->GetName());
	if(i != _templateMap.end())
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
			"SpecialEffects template already exist [" + effects->GetName() + "]", 
			"SpecialEffectsManager::AddTemplate");
	}

	_templateMap.insert(std::make_pair(effects->GetName(), effects));
}

void SpecialEffectsManager::RemoveTemplate(const String& templateName)
{
	SpecialEffectsMap::iterator i = _templateMap.begin();
	if(i != _templateMap.end())
	{
		_templateMap.erase(i);
	}
}

bool SpecialEffectsManager::IsTemplateExists(const String& templateName)
{
	SpecialEffectsMap::iterator i = _templateMap.find(templateName);
	return i != _templateMap.end();
}

void SpecialEffectsManager::ReplaceTemplate( ISpecialEffects* eff, const String& templateName )
{
	SpecialEffectsMap::iterator i = _templateMap.find(templateName);
	if(i != _templateMap.end())
	{
		ISpecialEffects* ef = i->second;
		i->second = eff;
		SpecialEffects* se = dynamic_cast<SpecialEffects*>(ef);
		assert(se);
		OGRE_DELETE se;
	}
	else
	{
		_templateMap.insert(std::make_pair(templateName, eff));
	}
}