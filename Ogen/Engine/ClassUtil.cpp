#include "stdafx.h"
#include "ClassUtil.h"
#include "IProperty.h"
#include "GameObjectFactoryManager.h"
#include "GameApp.h"

SerializePriorityMap g_serializePriorityMap;

struct SSerializePriority
{
	String ClassName;
	int Proprity;
};

bool InitPriorityMap()
{
	SSerializePriority vals[] =
	{
		{"SceneEnvironment",  0 },
		{"SkyBox",  0 },
		{"Terrain",  0 },

		{"MoveActor_PathFollow", 10 },
		{"Actor",  15 },
	};

	int cnt = sizeof(vals) / sizeof(SSerializePriority);

	for (int i = 0; i < cnt; ++i)
	{
		SSerializePriority& val = vals[i];
		g_serializePriorityMap.insert(std::make_pair(val.ClassName, val.Proprity));
	}

	return true;
}

static bool _initPri = InitPriorityMap();

int GetClassSerializePriority(const String& className)
{
	SerializePriorityMap::iterator i = g_serializePriorityMap.find(className);
	if(i != g_serializePriorityMap.end())
	{
		return i->second;
	}

	return 1;
}


bool CompareSerializePriority(IGameObject* lhs, IGameObject* rhs)
{
	int p1 = GetClassSerializePriority(lhs->GetTypeName());
	int p2 = GetClassSerializePriority(rhs->GetTypeName());
	return p2 > p1;
}

//------------------------------------------------------------------------

ClassXMLSerializer::ClassXMLSerializer()
{

}

ClassXMLSerializer::~ClassXMLSerializer()
{

}


void ClassXMLSerializer::Save(const GameObjectList& objectList, const String& fileName, ISerializationMask* query)
{
	GameObjectList sortObjects = objectList;
	std::sort(sortObjects.begin(), sortObjects.end(), CompareSerializePriority);

	XMLDoc doc;
	doc.createRootNode("GameObjects");
	for (size_t i = 0; i < sortObjects.size(); ++i)
	{
		IGameObject* obj = sortObjects[i];
		if(!query || (query && query->IsObjectCouldSave(obj)))
		{
			obj->PreSave();
			Save(obj, doc, query);
			obj->PostSave();
		}
	}

	doc.save(fileName);
}

XMLElement ClassXMLSerializer::Save(const IGameObject* obj, XMLDoc& doc, ISerializationMask* query)
{
	String strTypeID = getClassName(obj);

	XMLElement e = doc.getRoot().addChild(strTypeID);
	e.setAttribute("GUID", obj->GetGUID().toString());

	const PropertyMap& m = obj->GetProperties();
	for (PropertyMap::const_iterator i = m.begin(); i != m.end(); ++i)
	{
		const IPropertyPtr p = i->second;
		if(p->IsSerializeable() && ((p->GetDistributeFlag() & query->GetPropertySaveMask()) != 0))
		{
			String propName = p->GetName();
			String propValue = p->ToString();
			XMLElement eName = e.addChild(propName);
			eName.setAttribute("Value", propValue);
		}
	}

	return e;
}

GameObjectList ClassXMLSerializer::LoadFromXMLRootNode(XMLElement& objectsNode)
{
	GameObjectList lst;
	XMLElement e = objectsNode.firstChild();
	IGame* game = GameApp::getSingleton().GetGame();
	while(!e.isNull())
	{
		IGameObjectFactory* f = GameObjectFactoryManager::getSingleton().GetFactory(e.name());
		if(!f)
		{
			e = e.nextSibling();
			continue;
		}

#if 0
		IGameObject* obj = f->Create(e.getAttribute("Name"));
#else
		XGUID guid;
		guid.fromString(e.getAttribute("GUID"));
		IGameObject* obj = game->CreateGameObject(e.name(), guid, false);
#endif
		lst.push_back(obj);
		obj->PreLoad();

		XMLElement pe = e.firstChild();
		while (!pe.isNull())
		{
			String propName = pe.name();
			assert(!propName.empty());
			String propVal = pe.getAttribute("Value");
			obj->SetProperty(propName, propVal);

			pe = pe.nextSibling();
		}

		obj->PostLoad();

		e = e.nextSibling();
	}

	return lst;
}

GameObjectList ClassXMLSerializer::LoadDirect(const String& fileName)
{
	XMLDoc doc;
	if(doc.loadFromFile(fileName))
	{
		XMLElement e = doc.getRoot();
		return LoadFromXMLRootNode(e);
	}
	else
	{
		LogErrorIDf("Engine", "[ClassXMLSerializer::LoadDirect] error xml file format:[%s]", fileName.c_str());
		GameObjectList lst;
		return lst;
	}
}

GameObjectList ClassXMLSerializer::LoadFromResourceSystem(const String& fileName, const String& groupName)
{
	if(ResourceGroupManager::getSingleton().resourceExists(groupName, fileName))
	{
		DataStreamPtr dat = ResourceGroupManager::getSingleton().openResource(fileName);
		XMLDoc doc;
		if(doc.loadFromXML(dat->getAsString()))
		{
			XMLElement e = doc.getRoot();
			return LoadFromXMLRootNode(e);
		}
		else
		{
			LogErrorIDf("Engine",
				"[ClassXMLSerializer::LoadFromResourceSystem] error xml file format:[%s]",
				fileName.c_str());
		}
	}
	else
	{
		LogErrorIDf("Engine",
			"[ClassXMLSerializer::LoadFromResourceSystem] file not found:[%s]",
			fileName.c_str());
	}


	GameObjectList lst;
	return lst;
}

//--------------------------------------------------------------------------------

ClassBinarySerializer::ClassBinarySerializer()
{

}

ClassBinarySerializer::~ClassBinarySerializer()
{

}


void ClassBinarySerializer::Save(const GameObjectList& objectList, const String& fileName, ISerializationMask* query)
{

}

GameObjectList ClassBinarySerializer::LoadDirect(const String& fileName)
{
	GameObjectList lst;
	return lst;
}

GameObjectList ClassBinarySerializer::LoadFromResourceSystem(const String& fileName, const String& groupName)
{
	GameObjectList lst;
	return lst;
}

GameObjectList ClassBinarySerializer::LoadFromDataStream(DataStreamPtr s)
{
	GameObjectList lst;
	return lst;
}
