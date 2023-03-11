#include "stdafx.h"
#include "SceneInfo.h"

template<> SceneInfo* Singleton<SceneInfo>::ms_Singleton = 0;

class SceneInfoFactory : public TGameObjectFactory<SceneInfo>
{
public:

	virtual void Destroy(IGameObject* obj)
	{
		// 不要删除singleton
		//SDestroy<SceneEnvironment> d(obj);
	}

	virtual IGameObject* CreateImp(const XGUID& objID, const NameValuePairList* para = 0)
	{
		// 返回已经创建的singleton
		if(SceneInfo::getSingletonPtr())
		{
			return SceneInfo::getSingletonPtr();
		}

		IGameObject* obj = OGRE_NEW SceneInfo();
		obj->SetGUID(String("0xFF88FF88FFFFFFF1"));
		return obj;
	}

	virtual bool IsShownInEditorClassList() const
	{
		return true;
	}
};


M_REG_FACTORY(SceneInfo, SceneInfoFactory, Scene)

SceneInfo::SceneInfo(void)
{

}

SceneInfo::~SceneInfo(void)
{

}

const String& SceneInfo::GetTypeName() const
{
	static String sName("SceneInfo");
	return sName;
}

void SceneInfo::BuildPropertyMap()
{	
	_propMap.clear();
	AddProp(Property_Int, MapWidth, SceneInfo, "MapWidth", "SceneInfo");
	AddProp(Property_Int, MapHeight, SceneInfo, "MapHeight", "SceneInfo");
}

int SceneInfo::GetMapWidth() const
{
	// TERRAIN:
	return 1024;
	//return (int)Terrain::getSingleton().GetTerrainWidthX();
}

int SceneInfo::GetMapHeight() const
{
	// TERRAIN:
	return 1024;
	//return (int)Terrain::getSingleton().GetTerrainWidthZ();
}
