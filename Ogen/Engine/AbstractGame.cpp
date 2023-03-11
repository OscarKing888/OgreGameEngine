
#include "stdafx.h"
#include "AbstractGame.h"
#include "GameObjectFactoryManager.h"
#include "LoadingListener.h"
#include "EffectManager.h"
#include "GameApp.h"
#include "LoadingListener.h"
#include "ClassUtil.h"
#include "SkyBox.h"
#include "SceneEnvironment.h"
#include "StaticModel.h"
#include "File.h"
#include "SceneInfo.h"
#include "PlatformAPI.h"
#include "Tile.h"
#include "GameLight.h"
#include "HelperLogics.h"
#include "Events.h"

String AbstractGame::SceneResourceGroupName = "__Scene__";
String AbstractGame::Event_PostSceneLoad = "__Post_Scene_Load__";
const int C_ShadowTechCount = 7;
const String C_GlobalFileExt = ".global";

static ShadowTechnique g_ShadowTech[C_ShadowTechCount] = 
{
	SHADOWTYPE_NONE,
	SHADOWTYPE_TEXTURE_ADDITIVE,
	SHADOWTYPE_STENCIL_MODULATIVE,
	SHADOWTYPE_STENCIL_ADDITIVE,
	SHADOWTYPE_TEXTURE_MODULATIVE,
	SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED,
	SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED
};

//--------------------------------------------------------------------------------
// AbstractGame
//--------------------------------------------------------------------------------
AbstractGame::AbstractGame(void)
: _debugOverlay(0)
, _visualScene(0)
, _shadowTechIdx(0)
, _softShadow(false)
, _terrainGroup(0)
, _initMapResGrpTicket(0)
, _terrainGlobalOpt(0)
, _terrainXCnt(1)
, _terrainZCnt(1)
, _terrainSize(513)
, _terrainWorldSize(10000.0f)
{
	EnableScreenSaver(false);
}
//--------------------------------------------------------------------------------
AbstractGame::~AbstractGame(void)
{
	EnableScreenSaver(true);
}
//--------------------------------------------------------------------------------
void AbstractGame::Init()
{
	_stateManager = OGRE_NEW StateManager();
	_effectManager = OGRE_NEW EffectManager();

	PreInit();
	CreateVisualScene();

	InitImp();

	PostInit();
}
//--------------------------------------------------------------------------------
void AbstractGame::PreInit()
{
	InitTerrain();
}
//--------------------------------------------------------------------------------
void AbstractGame::InitImp()
{
}
//--------------------------------------------------------------------------------
void AbstractGame::PostInit()
{
	// Register the compositor logics
	// See comment in beginning of HelperLogics.h for explanation
	Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();
	compMgr.registerCompositorLogic("GaussianBlur", OGRE_NEW GaussianBlurLogic);
	compMgr.registerCompositorLogic("HDR", OGRE_NEW HDRLogic);
	compMgr.registerCompositorLogic("HeatVision", OGRE_NEW HeatVisionLogic);

	LoadingListener::getSingleton().SetLoadingProgress(100);
	LoadingListener::getSingleton().SetLoadingText("Init completed!");

#if _ENABLE_PROFILER_
	OGRE_NEW Profiler();
	Profiler::getSingleton().setEnabled(true);
	Profiler::getSingleton().setUpdateDisplayFrequency(_PROFILE_FREQUENCY_);
	Profiler::getSingleton().setTimer(Root::getSingleton().getTimer());
#endif

	LoadingListener::getSingleton().Finish();
}
//--------------------------------------------------------------------------------
void AbstractGame::ShutDown()
{
	DestroyAllGameObject();

	if(_terrainGroup)
	{		
		OGRE_DELETE _terrainGroup;
	}

	if(SceneInfo::getSingletonPtr())
	{
		OGRE_DELETE SceneInfo::getSingletonPtr();
	}

	if(SkyBox::getSingletonPtr())
	{
		OGRE_DELETE SkyBox::getSingletonPtr();
	}

	if(SceneEnvironment::getSingletonPtr())
	{
		OGRE_DELETE SceneEnvironment::getSingletonPtr();
	}

	if(MainLight::getSingletonPtr())
	{
		OGRE_DELETE MainLight::getSingletonPtr();
	}

	_stateManager->RemoveAllStates();
	OGRE_DELETE _stateManager;
	_stateManager = 0;

	_effectManager->RemoveAll();
	OGRE_DELETE _effectManager;
	_effectManager = 0;


	DestroySceneManager();

	if(_terrainGlobalOpt)
	{
		OGRE_DELETE _terrainGlobalOpt;
		_terrainGlobalOpt = 0;
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::DestroySceneManager()
{
	if(_visualScene)
	{
		Root::getSingleton().destroySceneManager(_visualScene);
		_visualScene = 0;
	}
}
//--------------------------------------------------------------------------------
IStateManager& AbstractGame::GetStateManager()
{
	return *_stateManager;
}
//--------------------------------------------------------------------------------
IEffectManager& AbstractGame::GetEffectManager()
{
	return *_effectManager;
}
//--------------------------------------------------------------------------------
SceneManager* AbstractGame::GetVisualScene()
{
	return _visualScene;
}
//--------------------------------------------------------------------------------
void AbstractGame::OnGameWindowLostFocus()
{
}
//--------------------------------------------------------------------------------
void AbstractGame::OnGameWindowSetFocus()
{
}
//--------------------------------------------------------------------------------
void AbstractGame::RegisterGameObjectFactory(const String& classTypeName,
											 IGameObjectFactory* factory, bool overrideExisting)
{
	GameObjectFactoryManager::getSingleton().RegisterFactory(classTypeName, factory, overrideExisting);
}
//--------------------------------------------------------------------------------
IGameObject* AbstractGame::CreateGameObject(const String& classTypeName, const XGUID& guid,
											bool addToGame,	const NameValuePairList* para)
{		
	IGameObjectFactory* f = GameObjectFactoryManager::getSingleton().GetFactory(classTypeName);
	IGameObject* obj = f->Create(guid, para);
	if(addToGame)
	{
		AddGameObject(obj);
		obj->OnAddToScene(_visualScene->getRootSceneNode());
	}

	//LogDebugIDf("GameLogic", "[AbstractGame::CreateGameObject] [%s] [%s] created!", classTypeName.c_str(), objectName.c_str());
	return obj;
}
//--------------------------------------------------------------------------------
void AbstractGame::AddGameObject(IGameObject* obj)
{
	String typeName = getClassName(obj);
	if(HasGameObject(obj->GetGUID()))
	{
		String msg = formatString("Object already exist [%s], type [%s]",
			obj->GetGUID().toString().c_str(), typeName.c_str());

		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, msg, "AbstractGame::AddGameObject");
	}

	TypedGameObjectMap::iterator i = _objects.find(typeName);
	if(i == _objects.end())
	{
		i = _objects.insert(std::make_pair(typeName, GameObjectMap())).first;
	}

	i->second.insert(std::make_pair(obj->GetGUID(), obj));
// 	obj->OnAddToScene(_visualScene->getRootSceneNode());
}
//--------------------------------------------------------------------------------
void AbstractGame::RemoveGameObject(IGameObject* obj)
{
	TypedGameObjectMap::iterator i = _objects.find(obj->GetTypeName());
	if(i != _objects.end())
	{
		GameObjectMap& m = i->second;
		GameObjectMap::iterator oi = m.find(obj->GetGUID());
		if(oi != m.end())
		{
			m.erase(oi);
			obj->OnRemoveFromScene(_visualScene->getRootSceneNode());
		}
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::RemoveGameObject( const GameObjectList& lst )
{
	GameObjectList::const_iterator i = lst.begin();
	for (; i != lst.end(); ++i)
	{
		RemoveGameObject(*i);
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::DestroyGameObject(const XGUID& guid)
{
	TypedGameObjectMap::iterator i = _objects.begin();
	for (; i != _objects.end(); ++i)
	{
		GameObjectMap& m = i->second;
		GameObjectMap::iterator oi = m.find(guid);
		if(oi != m.end())
		{
			IGameObject* o = oi->second;
			m.erase(oi);
			//o->OnRemoveFromTile(ITile* tile);

			// 				GameObjectList::iterator it = std::find(_loadedGameObjectList.begin(), _loadedGameObjectList.end(), o);
			// 				if(it != _loadedGameObjectList.end())
			// 				{
			// 					_loadedGameObjectList.erase(it);
			// 				}

			String typeName = getClassName(o);
			IGameObjectFactory* f = GameObjectFactoryManager::getSingleton().GetFactory(typeName);
			f->Destroy(o);
			return;
		}		
	}

	LogWarningIDf("GameLogic", "[AbstractGame::DestroyGameObject] object [%s] not found!", guid.toString().c_str());
}
//--------------------------------------------------------------------------------
void AbstractGame::DestroyGameObject(IGameObject* obj)
{
	DestroyGameObject(obj->GetGUID());
}
//--------------------------------------------------------------------------------
void AbstractGame::DestoryAllGameObjectByType(const String& classTypeName)
{
	TypedGameObjectMap::iterator i = _objects.find(classTypeName);
	if(i != _objects.end())
	{
		GameObjectMap::iterator j = i->second.begin();
		for (; j != i->second.end(); ++j)
		{
			IGameObject* o = j->second;
			GameObjectList::iterator it = std::find(_loadedGameObjectList.begin(), _loadedGameObjectList.end(), o);
			if(it != _loadedGameObjectList.end())
			{
				_loadedGameObjectList.erase(it);
			}

			String typeName = getClassName(o);
			IGameObjectFactory* f = GameObjectFactoryManager::getSingleton().GetFactory(typeName);
			f->Destroy(o);
		}
		_objects.erase(i);
		return;
	}

	LogWarningIDf("GameLogic", "[AbstractGame::DestoryAllGameObjectByType] type [%s] not found!", classTypeName.c_str());
}
//--------------------------------------------------------------------------------
void AbstractGame::DestroyAllGameObject()
{
	TypedGameObjectMap::iterator i = _objects.begin();
	for (; i != _objects.end(); ++i)
	{
		GameObjectMap::iterator j = i->second.begin();
		for (; j != i->second.end(); ++j)
		{
			IGameObject* o = j->second;
			String typeName = getClassName(o);
			IGameObjectFactory* f = GameObjectFactoryManager::getSingleton().GetFactory(typeName);
			f->Destroy(o);
		}
	}

	_loadedGameObjectList.clear();
	_objects.clear();
}
//--------------------------------------------------------------------------------
const TypedGameObjectMap& AbstractGame::GetAllGameObjects() const
{
	return _objects;
}
//--------------------------------------------------------------------------------
const GameObjectMap& AbstractGame::GetGameObjectsByType(const String& typeName) const
{
	TypedGameObjectMap::const_iterator i = _objects.find(typeName);
	if(i != _objects.end())
	{
		return i->second;
	}

	static GameObjectMap EmptyMap;
	return EmptyMap;
#if 0
	OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
		"Object type not found [" + typeName + "]", 
		"AbstractGame::GetGameObjectsByType");
#endif
}
//--------------------------------------------------------------------------------
IGameObject* AbstractGame::GetGameObject(const XGUID& guid)
{
	TypedGameObjectMap::const_iterator i = _objects.begin();
	for (; i != _objects.end(); ++i)
	{
		const GameObjectMap& m = i->second;
		GameObjectMap::const_iterator oi = m.find(guid);
		if(oi != m.end())
		{
			return oi->second;
		}			
	}

// 	TileMap::const_iterator i2 = _tileMap.begin();
// 	for (; i2 != _tileMap.end(); ++i2)
// 	{
// 		TileZMap::const_iterator i3 = i2->second.begin();
// 		for (; i3 != i2->second.end(); ++i3)
// 		{
// 			ITile* tile = i3->second;
// 			IGameObject* o = tile->GetGameObject(guid);
// 			if(o)
// 			{
// 				return o;
// 			}
// 		}
// 	}

	return 0;
}
//--------------------------------------------------------------------------------
bool AbstractGame::HasGameObject(const XGUID& guid)
{
	return GetGameObject(guid) != 0;
}
//--------------------------------------------------------------------------------
void AbstractGame::CreateVisualScene()
{
	_visualScene = _CreateVisualScene();

	LoadingListener::getSingleton().Init();
	LoadingListener::getSingleton().Start(GameApp::getSingleton().GetRenderWindow(), _visualScene);

#ifndef _PRODUCT_
	_debugOverlay = OverlayManager::getSingleton().getByName(("Core/DebugOverlay"));		
#endif

	ConfigFile& cfg = GameApp::getSingleton().GetMainConfigFile();
	_softShadow = StringConverter::parseBool(cfg.getSetting(("Soft"), ("Shadow")));

	String shadow = cfg.getSetting(("Type"), ("Shadow"));
	ShadowTechnique st = (ShadowTechnique)ShadowTechniqueType::getSingleton().FromString(shadow);

	SetShadowTechnique(st, _softShadow);

	ColourValue shaowClr = StringConverter::parseColourValue(
		cfg.getSetting(("Color"), ("Shadow")));
	_visualScene->setShadowColour(shaowClr);

	int shadowDist = 30;
	String tmp = cfg.getSetting(("ShadowDistance"), ("Shadow"));
	if(!tmp.empty())
	{
		shadowDist = StringConverter::parseReal(tmp);
	}

	_visualScene->setShadowFarDistance(U_M(shadowDist));

	bool selfShadow = false;
	tmp = cfg.getSetting(("SelfShadow"), ("Shadow"));
	if(!tmp.empty())
	{
		selfShadow = StringConverter::parseBool(tmp);
	}

	_visualScene->setShadowTextureSelfShadow(selfShadow);		

	if (Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_HWRENDER_TO_TEXTURE))
	{
		// In D3D, use a 1024x1024 shadow texture
		int sz = 1024;
		String tmp = cfg.getSetting(("D3DShadowTexSize"), ("Shadow"));
		if(!tmp.empty())
		{
			sz = StringConverter::parseInt(tmp);
		}

		int cnt = 2;
		tmp = cfg.getSetting(("D3DShadowTexCnt"), ("Shadow"));
		if(!tmp.empty())
		{
			cnt = StringConverter::parseInt(tmp);
		}

		_visualScene->setShadowTextureSettings(sz, cnt);
	}
	else
	{
		// Use 512x512 texture in GL since we can't go higher than the window res

		int sz = 512;
		String tmp = cfg.getSetting(("GLShadowTexSize"), ("Shadow"));
		if(!tmp.empty())
		{
			sz = StringConverter::parseInt(tmp);
		}

		int cnt = 2;
		tmp = cfg.getSetting(("GLShadowTexCnt"), ("Shadow"));
		if(!tmp.empty())
		{
			cnt = StringConverter::parseInt(tmp);
		}

		_visualScene->setShadowTextureSettings(sz, cnt);
	}
}
//--------------------------------------------------------------------------------
SceneManager* AbstractGame::_CreateVisualScene()
{
	return Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);
}
//--------------------------------------------------------------------------------
void AbstractGame::SetShadowTechnique(ShadowTechnique t, bool soft)
{
	LogSysIDf("GameLogic", "Shadow type:%s",
		ShadowTechniqueType::getSingleton().ToString((int)t).c_str());

	_visualScene->setShadowTechnique(t);
	_softShadow = soft;

	if(soft && (t != SHADOWTYPE_TEXTURE_MODULATIVE))
	{
		// TODO: 
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::SelectNextShadowTechnique()
{
	_shadowTechIdx = (_shadowTechIdx + 1) % C_ShadowTechCount;
	ShadowTechnique st = g_ShadowTech[_shadowTechIdx];

	SetShadowTechnique(st, true);
}
//--------------------------------------------------------------------------------
void AbstractGame::Update(float deltaTime)
{
	_effectManager->Update(deltaTime);
	UpdateStats(deltaTime);
	CheckTriggers();
}
//--------------------------------------------------------------------------------
void AbstractGame::ShowDebugOverlay(bool show)
{		
	if(_debugOverlay)
	{
		if (show)
		{
			_debugOverlay->show();
		}
		else
		{
			_debugOverlay->hide();
		}
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::UpdateStats(float deltaTime)
{
	static String currFps = ("Current FPS: ");
	static String avgFps = ("Average FPS: ");
	static String bestFps = ("Best FPS: ");
	static String worstFps = ("Worst FPS: ");
	static String tris = ("Triangle Count: ");
	static String batch = ("Batch Count: ");

	static float updateInterval = 0.0f;

	updateInterval += deltaTime;
	if(updateInterval < 2.0f) // 2秒更新一次，以免影响性能
	{
		return;
	}

	updateInterval = 0.0f;

	// update stats when necessary
	OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement(("Core/AverageFps"));
	if(!guiAvg->isVisible())
	{
		return;
	}

	OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement(("Core/CurrFps"));
	OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement(("Core/BestFps"));
	OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement(("Core/WorstFps"));
	OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement(("Core/NumTris"));
	OverlayElement* guiBatch = OverlayManager::getSingleton().getOverlayElement(("Core/NumBatches"));

	RenderWindow* rw = GameApp::getSingleton().GetRenderWindow();
	guiAvg->setCaption(avgFps + StringConverter::toString(rw->getAverageFPS()));
	guiCurr->setCaption(currFps + StringConverter::toString(rw->getLastFPS()));

	guiBest->setCaption(bestFps + StringConverter::toString(rw->getBestFPS())
		+ (" ") + StringConverter::toString(rw->getBestFrameTime()) + (" ms"));

	guiWorst->setCaption(worstFps + StringConverter::toString(rw->getWorstFPS())
		+ (" ") + StringConverter::toString(rw->getWorstFrameTime()) + (" ms"));

	guiTris->setCaption(tris + StringConverter::toString(rw->getTriangleCount()));
	guiBatch->setCaption(batch + StringConverter::toString(rw->getBatchCount()));
}
//--------------------------------------------------------------------------------
void AbstractGame::SetDebugText(const String& debugTxt)
{
	OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement(("Core/DebugText"));
	guiDbg->setCaption(debugTxt);
}
//--------------------------------------------------------------------------------
void AbstractGame::SetPolygonMode(PolygonMode mode)
{
	SceneManager::CameraIterator cit = _visualScene->getCameraIterator();
	while(cit.hasMoreElements())
	{
		Camera* cam = cit.peekNextValue();
		cam->setPolygonMode(mode);
		cit.moveNext();
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::InitCamera(Camera* cam)
{
	cam->setPosition(Vector3(0, 0, 500));
	cam->lookAt(Vector3(0,0,0));
	cam->setNearClipDistance(1.0f);
	// 		cam->setFarClipDistance(1500.0f);
	ConfigFile& cfg = GameApp::getSingleton().GetMainConfigFile();
	String fovDeg = cfg.getSetting(("FOV"), ("Camera"));

	if(fovDeg.empty())
	{
		fovDeg = ("40");
	}

	float fov = StringConverter::parseReal(fovDeg);
	cam->setFOVy(Radian(Degree(fov).valueRadians()));
}
//--------------------------------------------------------------------------------
void AbstractGame::AddGameObjects(const GameObjectList& lst)
{
	for (size_t i = 0; i < lst.size(); ++i)
	{
		AddGameObject(lst[i]);
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::DrawString(const String& name, const String& text, int x, int y)
{

	Overlay* o = OverlayManager::getSingleton().getByName(name);

	String pixelWidth = GameApp::getSingleton().GetMainConfigFile().getSetting("WidthPixelPerChar", "ScreenText");
	String pixelHeight = GameApp::getSingleton().GetMainConfigFile().getSetting("HeightPixel", "ScreenText");

	size_t w = text.size() * StringConverter::parseInt(pixelWidth);
	size_t h = StringConverter::parseInt(pixelHeight);

	if(o)
	{
		OverlayElement* textArea = 
			OverlayManager::getSingleton().getOverlayElement(name);
		textArea->setCaption(text);
		textArea->setTop(0);
		textArea->setLeft(0);
		textArea->setWidth(w);

		OverlayContainer* container = dynamic_cast<OverlayContainer*>
			(OverlayManager::getSingleton().getOverlayElement(name + "Container"));

		container->setLeft(x);
		container->setTop(y);

		container->setWidth(w);
	}
	else
	{
		OverlayElement* textArea = 
			OverlayManager::getSingleton().createOverlayElement("TextArea", name);
		textArea->setMetricsMode(GMM_PIXELS);
		textArea->setWidth(w);
		textArea->setHeight(h);
		textArea->setTop(y);
		textArea->setLeft(x);

		String fontName = GameApp::getSingleton().GetMainConfigFile().getSetting("FontName", "ScreenText");
		textArea->setParameter("font_name", fontName);

		String fontSize = GameApp::getSingleton().GetMainConfigFile().getSetting("FontSize", "ScreenText");
		textArea->setParameter("char_height", fontSize);
		textArea->setCaption(text);
		//textArea->setParameter("colour_top", "0 0 0");
		//textArea->setParameter("colour_bottom", "0 0 0");

		textArea->show();

		OverlayContainer* container = dynamic_cast<OverlayContainer*>
			(OverlayManager::getSingleton().createOverlayElement("BorderPanel", name + "Container"));
		container->setMetricsMode(GMM_PIXELS);
		//container->setMaterialName("Core/StatsBlockCenter");
		container->setHeight(h);
		container->setWidth(w);
		container->setParameter("border_size", "0 0 0 0");
		//container->setParameter("border_material", "Core/StatsBlockBorder");
		container->setParameter("border_topleft_uv", "0.0000 1.0000 0.0039 0.9961");
		container->setParameter("border_top_uv", "0.0039 1.0000 0.9961 0.9961");
		container->setParameter("border_topright_uv", "0.9961 1.0000 1.0000 0.9961");
		container->setParameter("border_left_uv","0.0000 0.9961 0.0039 0.0039");
		container->setParameter("border_right_uv","0.9961 0.9961 1.0000 0.0039");
		container->setParameter("border_bottomleft_uv","0.0000 0.0039 0.0039 0.0000");
		container->setParameter("border_bottom_uv","0.0039 0.0039 0.9961 0.0000");
		container->setParameter("border_bottomright_uv","0.9961 0.0039 1.0000 0.0000");
		container->setLeft(x);
		container->setTop(y);

		container->addChild(textArea);
		container->show();

		Overlay* o = OverlayManager::getSingleton().create(name);
		o->add2D(container);
		o->show();
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::CreateSceneManager(const String& sceneType)
{
	DestroySceneManager();
	_visualScene = Root::getSingleton().createSceneManager(sceneType);
}
//--------------------------------------------------------------------------------
void AbstractGame::SaveMap( const String& mapName, ISerializationMask* query /*= 0*/ )
{
	GameObjectList objs;
	
	if(SceneEnvironment::getSingletonPtr())
	{
		objs.push_back(SceneEnvironment::getSingletonPtr());
	}

	if(MainLight::getSingletonPtr())
	{
		objs.push_back(MainLight::getSingletonPtr());
	}

	if(SkyBox::getSingletonPtr())
	{
		objs.push_back(SkyBox::getSingletonPtr());
	}

	String globalObjFile = GetSceneDir() + mapName + "/" + mapName + C_GlobalFileExt;
	ClassXMLSerializer globalObjs;
	globalObjs.Save(objs, globalObjFile, query);

	String sceneDir = this->GetSceneDir() + mapName + "/";
	TileMap::iterator it = _tileMap.begin();
	for (; it != _tileMap.end(); ++it)
	{
		TileZMap::iterator itz = it->second.begin();
		for (; itz != it->second.end(); ++itz)
		{
			ITile* tile = itz->second;
			tile->Save(sceneDir, query, true);
		}
	}

	if(_terrainGroup)
	{
		_terrainGroup->saveAllTerrains(true);
	}
}
//--------------------------------------------------------------------------------
#ifndef _PRODUCT_
int formatIndent = 0;
void PrintSceneNode(SceneNode* node, String& msg)
{
	/*for (int i = 0; i < formatIndent; ++i)
	{
	msg += " ";
	}*/

	++formatIndent;
	msg += "<SceneNode name=\"";
	msg += node->getName() + "\">";

	SceneNode::ObjectIterator oi = node->getAttachedObjectIterator();
	while(oi.hasMoreElements())
	{
		for (int i = 0; i < formatIndent + 1; ++i)
		{
			msg += " ";
		}

		MovableObject* obj = oi.peekNextValue();
		String objType = getClassName(obj);

		msg += "<" + objType + " name=\"";

		msg += obj->getName() + "\"/>\r\n";

		oi.moveNext();
	}

	SceneNode::ChildNodeIterator ni = node->getChildIterator();
	while(ni.hasMoreElements())
	{
		PrintSceneNode((SceneNode*)ni.peekNextValue(), msg);
		ni.moveNext();
	}

	msg += "</SceneNode>\r\n";

	--formatIndent;
}
//--------------------------------------------------------------------------------
void PrintSceneNode(const String& fileName, SceneNode* rootNode)
{
	formatIndent = 0;
	String sceneGraph;
	PrintSceneNode(rootNode, sceneGraph);

	FILE* fp = 0;
	fopen_s(&fp, fileName.c_str(), "w+");
	fputs(sceneGraph.c_str(), fp);
	fclose(fp);
	LogConsoleIDf("GameLogic", "Scene graph saved:%s", fileName.c_str());
}
#endif
//--------------------------------------------------------------------------------
void AbstractGame::LoadMap( const String mapName )
{
	UnloadMap();

	_mapName = mapName;

	String mapDir = GetSceneDir() + mapName;
	// TODO: change to release package type
	ResourceGroupManager::getSingleton().addResourceLocation(mapDir, "FileSystem", SceneResourceGroupName);
// 	if(GameApp::CurrentRunType == ERT_Game)
// 	{
// 		_initMapResGrpTicket = ResourceBackgroundQueue::getSingleton().initialiseResourceGroup(SceneResourceGroupName, this);
// 	}
// 	else
	{
		ResourceGroupManager::getSingleton().initialiseResourceGroup(SceneResourceGroupName);
		OnMapResourceGroupReady();
	}

//#if !defined(_PRODUCT_) && defined(_DEBUG)
//	File::removeReadonly("AutoDebugScene.xml");
//	PrintSceneNode("AutoDebugScene.xml", _visualScene->getRootSceneNode());
//
//	class SaveAllMask : public ISerializationMask
//	{
//	public:
//
//		virtual bool IsObjectCouldSave(IGameObject* obj)
//		{
//			return true;
//		}
//
//		virtual int GetPropertySaveMask() const
//		{
//			return 0xffffffff;
//		}
//	};
//
//	SaveAllMask query;
//	SaveMap("AutoSaveScene.xml", &query);
//#endif
}
//--------------------------------------------------------------------------------
void AbstractGame::UnloadMap()
{
	if(_terrainGroup)
	{
		OGRE_DELETE _terrainGroup;
		_terrainGroup = 0;
	}

	if(!_mapName.empty())
	{
		String mapDir = GetSceneDir() + _mapName;
		DestroyAllTile();
		UnloadUnreferencedReceoruces(SceneResourceGroupName);
		ResourceGroupManager::getSingleton().removeResourceLocation(mapDir);
//		if(!_loadedGameObjectList.empty())
//		{
//			for(GameObjectList::iterator i = _loadedGameObjectList.begin(); i != _loadedGameObjectList.end(); ++i)
//			{
//				IGameObject* obj = *i;
//				DestroyGameObject(obj);
//			}
//
//			_loadedGameObjectList.clear();
//
//		}
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::OnTouch(Trigger* trigger, IGameObject* object)
{
	LogDebugIDf("GameLogic", "[Trigger:OnTouch] Trigger[%s] Object[%s]", trigger->GetGUID().toString().c_str(), object->GetGUID().toString().c_str());
}
//--------------------------------------------------------------------------------
void AbstractGame::OnStay(Trigger* trigger, IGameObject* object)
{
	LogDebugIDf("GameLogic", "[Trigger:OnStay] Trigger[%s] Object[%s]", trigger->GetGUID().toString().c_str(), object->GetGUID().toString().c_str());
}
//--------------------------------------------------------------------------------
void AbstractGame::OnLeave(Trigger* trigger, IGameObject* object)
{
	LogDebugIDf("GameLogic", "[Trigger:OnLeave] Trigger[%s] Object[%s]", trigger->GetGUID().toString().c_str(), object->GetGUID().toString().c_str());
}
//--------------------------------------------------------------------------------
void AbstractGame::CheckTriggers()
{
	Trigger::TriggerMap::iterator i = Trigger::S_Triggers.begin();
	for (; i != Trigger::S_Triggers.end(); ++i)
	{
		i->second->CheckCollision(this);
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::LoadTile( long x, long z, bool synchronous/* = false*/ )
{
	ITile* tile = GetTile(x, z);
	tile->Load(synchronous);
}
//--------------------------------------------------------------------------------
void AbstractGame::UnloadTile(long x, long z)
{
	ITile* tile = GetTile(x, z);
	tile->Unload();
}
//--------------------------------------------------------------------------------
void AbstractGame::UnloadAllTiles()
{
	TileMap::const_iterator it = _tileMap.begin();
	for (; it != _tileMap.end(); ++it)
	{
		TileZMap::const_iterator itz = it->second.begin();
		for (; itz != it->second.end(); ++itz)
		{
			ITile* t = itz->second;
			t->Unload();
		}
	}
}
//--------------------------------------------------------------------------------
TerrainGroup* AbstractGame::GetTerrainGroup()
{
	return _terrainGroup;
}
//--------------------------------------------------------------------------------
float AbstractGame::GetTerrainHeight(const Vector3& pos)
{
	if(_terrainGroup)
	{
		return _terrainGroup->getHeightAtWorldPosition(pos);
	}
	return 0.0f;
}
//--------------------------------------------------------------------------------
float AbstractGame::GetTerrainHeight(float x, float z)
{
	Vector3 pos(x, 0, z);
	return GetTerrainHeight(pos);
}
//--------------------------------------------------------------------------------
TerrainGroup::RayResult AbstractGame::RayIntersects(const Ray& ray, Real distanceLimit)
{
	if(_terrainGroup && (_terrainGroup->getTerrainWorldSize() > 0.0f))
	{
		return _terrainGroup->rayIntersects(ray, distanceLimit);
	}

	TerrainGroup::RayResult r(false, 0, Vector3::ZERO);
	return r;
}
//--------------------------------------------------------------------------------
const String& AbstractGame::GetSceneDir() const
{
	static String path;

	if(path.empty())
	{
		String baseName;
		String file = FileEx::GetResourceFilePath("__EDITOR_SCENE__");		
		StringUtil::splitFilename(file, baseName, path);
		path = GameApp::getSingleton().GetAppExePath() + path;
	}
	
	return path;
}
//--------------------------------------------------------------------------------
void AbstractGame::operationCompleted( BackgroundProcessTicket ticket,
									  const BackgroundProcessResult& result )
{
	if(!result.error)
	{
		if(_initMapResGrpTicket == ticket)
		{
			_initMapResGrpTicket = (BackgroundProcessTicket)~0;
			OnMapResourceGroupReady();
		}
	}
	else
	{
		LogErrorIDf("Engine", "[%s] Init resource group error:%s", __FUNCTION__, result.message.c_str());
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::UnloadUnreferencedReceoruces(const String& resGroupName)
{
	ResourceGroupManager::getSingleton().unloadUnreferencedResourcesInGroup(resGroupName);
}
//--------------------------------------------------------------------------------
void AbstractGame::OnMapResourceGroupReady()
{
	String mapCfg = _mapName + ".xml";
	DataStreamPtr ds = ResourceGroupManager::getSingleton()
		.openResource(mapCfg, SceneResourceGroupName);

	XMLDoc doc;
	String xmlData = ds->getAsString();
	if(doc.loadFromXML(xmlData))
	{
		XMLElement e = doc.getRoot().firstChild();
		while (!e.isNull())
		{
			if(e.name() == "terrain")
			{
				assert(!_terrainGroup);

				_terrainSize = getIntAttribuate("terrainSize", e);
				_terrainWorldSize = getFloatAttribuate("terrainWorldSize", e);
				_terrainXCnt = getIntAttribuate("terrainXCnt", e);
				_terrainZCnt = getIntAttribuate("terrainZCnt", e);

				float texWorldSz = getFloatAttribuate("texWorldSz", e);
				uint16 blendMapSz = getIntAttribuate("blendMapSz", e);
				uint16 clrMapSz = getIntAttribuate("clrMapSz", e);
				uint16 lightMapSz = getIntAttribuate("lightMapSz", e);

				float compositeMapDistance = getFloatAttribuate("compositeMapDistance", e);
				float skirtSz = getFloatAttribuate("skirtSz", e);
				uint16 compositeMapSz = getIntAttribuate("compositeMapSz", e);
				float maxPixelErr = getFloatAttribuate("maxPixelErr", e);

				bool castDynShadows = getBoolAttribuate("castDynShadows", e);

				_terrainGroup = OGRE_NEW Ogre::TerrainGroup(_visualScene,
					Terrain::ALIGN_X_Z, _terrainSize, _terrainWorldSize);

				_terrainGlobalOpt->setDefaultLayerTextureWorldSize(texWorldSz);
				_terrainGlobalOpt->setLayerBlendMapSize(blendMapSz);
				_terrainGlobalOpt->setDefaultGlobalColourMapSize(clrMapSz);
				_terrainGlobalOpt->setLightMapSize(lightMapSz);

				_terrainGlobalOpt->setCompositeMapDistance(compositeMapDistance);
				_terrainGlobalOpt->setSkirtSize(skirtSz);
				_terrainGlobalOpt->setCompositeMapSize(compositeMapSz);
				_terrainGlobalOpt->setMaxPixelError(maxPixelErr);

				_terrainGlobalOpt->setCastsDynamicShadows(castDynShadows);

				_terrainGroup->setResourceGroup(AbstractGame::SceneResourceGroupName);
				_terrainGroup->setOrigin(Vector3::ZERO);			
			}
			e = e.nextSibling();
		}
	}

	long halfXW = _terrainXCnt / 2;
	long halfZW = _terrainZCnt / 2;
	for (long x = -halfXW; x < halfXW; ++x)
	{
		for (long z = -halfZW; z < halfXW; ++z)
		{
			if(_terrainGroup)
			{

				String filename = _terrainGroup->generateFilename(x, z);
				if (ResourceGroupManager::getSingleton().resourceExists(
					_terrainGroup->getResourceGroup(), filename))
				{
					_terrainGroup->defineTerrain(x, z, filename);
				}
				else
				{
					_terrainGroup->defineTerrain(x, z, 0.0f);
				}
			}
			CreateTile(x, z);
		}
	}

	// load global objects
	ClassXMLSerializer globalObjs;
	GameObjectList objs = globalObjs.LoadFromResourceSystem(
		_mapName + C_GlobalFileExt, SceneResourceGroupName);
	AddGameObjects(objs);

	// notify post scene load completed
	TypedGameObjectMap::iterator it = _objects.begin();
	for ( ; it != _objects.end(); ++it)
	{
		GameObjectMap& objMap = it->second;
		GameObjectMap::iterator it2 = objMap.begin();
		for ( ; it2 != objMap.end(); ++it2)
		{
			IGameObject* obj = it2->second;
			obj->PostSceneLoad();
		}
	}

	EventsManager::getSingleton().CreateEvent(Event_PostSceneLoad, Any());
}
//--------------------------------------------------------------------------------
ITile* AbstractGame::GetTileFromPosition( const Vector3& worldPos )
{
	long x = 0, z = 0;
	if(_terrainGroup)
	{
		_terrainGroup->convertWorldPositionToTerrainSlot(worldPos, &x, &z);
	}

	return GetTile(x, z);
}
//--------------------------------------------------------------------------------
void AbstractGame::AddTile(ITile* tile)
{
	TileMap::iterator it = _tileMap.find(tile->GetX());
	if(it == _tileMap.end())
	{
		it = _tileMap.insert(std::make_pair(tile->GetX(), TileZMap())).first;
	}

	TileZMap::iterator itz = it->second.find(tile->GetZ());
	if(itz == it->second.end())
	{
		it->second.insert(std::make_pair(tile->GetZ(), tile));
	}
	else
	{
		assert(false && "[AbstractGame::AddTile] Tile already exist!");
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::RemoveTile(long x, long z)
{
	TileMap::iterator it = _tileMap.find(x);
	if(it != _tileMap.end())
	{
		TileZMap::iterator itz = it->second.find(z);
		if(itz != it->second.end())
		{
			it->second.erase(itz);
		}
	}
}
//--------------------------------------------------------------------------------
TilesList AbstractGame::GetTileList() const
{
	TilesList lst;
	TileMap::const_iterator it = _tileMap.begin();
	for (; it != _tileMap.end(); ++it)
	{
		TileZMap::const_iterator itz = it->second.begin();
		for (; itz != it->second.end(); ++itz)
		{
			lst.push_back(itz->second);
		}
	}

	return lst;
}
//--------------------------------------------------------------------------------
bool AbstractGame::HasTile(long x, long z) const
{
	return GetTile(x, z) != 0;
}
//--------------------------------------------------------------------------------
ITile* AbstractGame::GetTile(long x, long z) const
{
	TileMap::const_iterator it = _tileMap.find(x);
	if(it != _tileMap.end())
	{
		TileZMap::const_iterator itz = it->second.find(z);
		if(itz != it->second.end())
		{
			return itz->second;
		}
	}

	return 0;
}
//--------------------------------------------------------------------------------
void AbstractGame::DestroyTile(long x, long z)
{
	TileMap::iterator it = _tileMap.find(x);
	if(it != _tileMap.end())
	{
		TileZMap::iterator itz = it->second.find(z);
		if(itz != it->second.end())
		{
			ITile* t = itz->second;
			t->Destroy();
			it->second.erase(itz);
		}
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::DestroyAllTile()
{
	TileMap::const_iterator it = _tileMap.begin();
	for (; it != _tileMap.end(); ++it)
	{
		TileZMap::const_iterator itz = it->second.begin();
		for (; itz != it->second.end(); ++itz)
		{
			ITile* t = itz->second;
			t->Destroy();
		}
	}

	_tileMap.clear();
}
//--------------------------------------------------------------------------------
void AbstractGame::InitTerrain()
{
	_terrainGlobalOpt = OGRE_NEW TerrainGlobalOptions();
	// Configure global
	_terrainGlobalOpt->setMaxPixelError(8);
	// testing composite map
	_terrainGlobalOpt->setCompositeMapDistance(3000);
	_terrainGlobalOpt->setDefaultLayerTextureWorldSize(100);
}
//--------------------------------------------------------------------------------
void AbstractGame::LoadTileAndTerrain( long x, long z, bool synchronous /*= false*/ )
{
	LoadTile(x, z, synchronous);

	if(_terrainGroup)
	{
		_terrainGroup->loadTerrain(x, z, synchronous);
	}
}
//--------------------------------------------------------------------------------
void AbstractGame::UnloadTileAndTerrain( long x, long z )
{
	UnloadTile(x, z);

	if(_terrainGroup)
	{
		_terrainGroup->unloadTerrain(x, z);
	}
}
//--------------------------------------------------------------------------------
long AbstractGame::GetTerrainXCount() const
{
	return _terrainXCnt;
}
//--------------------------------------------------------------------------------
long AbstractGame::GetTerrainZCount() const
{
	return _terrainZCnt;
}
//--------------------------------------------------------------------------------
Ogre::uint16 AbstractGame::GetTerrainSize() const
{
	return _terrainSize;
}
//--------------------------------------------------------------------------------
float AbstractGame::GetTerrainWorldSize() const
{
	return _terrainWorldSize;
}
//--------------------------------------------------------------------------------
const String& AbstractGame::GetLoadMapName() const
{
	return _mapName;
}
//--------------------------------------------------------------------------------
TerrainGlobalOptions* AbstractGame::GetTerrainGlobalOptions()
{
	return _terrainGlobalOpt;
}
//--------------------------------------------------------------------------------
ITile* AbstractGame::CreateTile( long x, long z )
{
	assert(!HasTile(x, z));

	Tile* tile = OGRE_NEW Tile(_visualScene, this);
	tile->SetSize(_terrainWorldSize);
	tile->SetCoord(x, z);
	AddTile(tile);
	return tile;
}
//--------------------------------------------------------------------------------