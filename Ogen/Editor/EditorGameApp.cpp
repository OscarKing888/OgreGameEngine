#include "stdafx.h"

#include "EditorGameApp.h"
#include "EditorView.h"
#include "Editor.h"
#include "MainFrm.h"
#include "OutputWnd.h"
#include <OgreParticleSystemRenderer.h>
#include <winuser.h>
#include <OgreMaterialSerializer.h>
//#include "Oge_SpecialEffects/SpecialEffectsManager.h"
#include "Engine/OgreLib.h"

#include "OgreTerrainMaterialGeneratorA.h"
#include "MFCCameraController.h"


const unsigned short C_MaxHeight = 0xffff;
unsigned short C_TerrainZero = C_MaxHeight * 0.5f;


//------------------------------------------------------------------------

EditorGameApp::EditorGameApp(IGame* pLogic, const String& configFile)
: GameApp(pLogic, configFile)
, _renderHWND(0)
{
	GameApp::CurrentRunType = ERT_Editor;
}
//--------------------------------------------------------------------------------
EditorGameApp::~EditorGameApp(void)
{
}
//--------------------------------------------------------------------------------
void EditorGameApp::Run(HWND hwndRender)
{
	_renderHWND = hwndRender;
	////GameApp::Run();

	//if(!OnAppConstruct())
	//{
	//	return;
	//}

	//GameObjectFactoryManager::getSingleton().PrintRegisteredFactory();

	//OnCreateOgreRender();
	//OnCreateRenderWnd();

	//_game->Init();

	//NotifyAppInitCompleted();

	//LoadingListener::getSingleton().Finish();

	_RenderLoop();
}
//--------------------------------------------------------------------------------
void EditorGameApp::OnCreateRenderWnd()
{
	_renderWindow = _ogreRoot->initialise(false, "Editor");
	NameValuePairList para;
	String k = "externalWindowHandle";
	String v = StringConverter::toString((uint)_renderHWND);
	para.insert(std::make_pair(k, v));

	k = "vsync";
	v = "true";
	para.insert(std::make_pair(k, v));

	_renderWindow = _ogreRoot->createRenderWindow("Editor", 800, 600, false, &para);

	ResourceBackgroundQueue::getSingleton().initialiseAllResourceGroups();
}
//--------------------------------------------------------------------------------
void EditorGameApp::_RenderLoop()
{
	EditorConfig::getSingleton().LoadConfig();

	// Nothing to do, render in WinApp::Run()
	//GameApp::_RenderLoop();
	Root& root = Root::getSingleton();
	root.getRenderSystem()->_initRenderTargets();
	root.clearEventTimes();

	// Infinite loop, until broken out of by frame listeners
	// or break out by calling queueEndRendering()
	_quit = false;

	MainFrame* pMainWnd = dynamic_cast<MainFrame*>(AfxGetMainWnd());
	EditorView* mainView = dynamic_cast<EditorView*>(pMainWnd->GetActiveView());
 	mainView->SetMainRenderWnd(_renderWindow);
 	mainView->CreateViewports(EVS_Max);	

	//SetTimer(AfxGetMainWnd()->GetSafeHwnd(), 0, 1000/80, NULL);
	//AfxGetApp()->Run();	
}
//--------------------------------------------------------------------------------
void EditorGameApp::Quit()
{
	_quit = true;
	MFCEditorApp& a = (MFCEditorApp&)*AfxGetApp();
	a._quit = true;
	//::AfxPostQuitMessage(0);
}
//--------------------------------------------------------------------------------
bool EditorGameApp::OnInitOgre()
{
	return GameApp::OnInitOgre();
}
//--------------------------------------------------------------------------------
void EditorGameApp::OnShutDown()
{
	EditorApp::getSingleton().Shutdown();
	GameApp::OnShutDown();
	OGRE_DELETE EditorApp::getSingletonPtr();
}
//--------------------------------------------------------------------------------
void EditorGameApp::windowResized(RenderWindow* rw)
{
	GameApp::windowResized(rw);
}
//--------------------------------------------------------------------------------
bool EditorGameApp::OnConfigResourcePath()
{
	bool r = GameApp::OnConfigResourcePath();

	EditorApp::getSingleton().LoadDisplayStringCfg();

	return r;
}
//--------------------------------------------------------------------------------
void EditorGameApp::Init()
{
#ifdef _PRODUCT_
	try
	{
#endif

		OnCreateOgreRender();
		OnCreateRenderWnd();

		NotifyPreGameInit();
		_game->Init();
		NotifyPostGameInit();

		NotifyAppInitCompleted();

		LoadingListener::getSingleton().Finish();
#ifdef _PRODUCT_
	}
	catch (const std::exception& ex)
	{
		LogErrorIDf("Engine", (("%s"), ex.what());
		_renderWindow->setFullscreen(false, 800, 600);
		MessageBox_Error(ex.what());
	}
	catch (const Exception& ex)
	{
		LogErrorIDf("Engine", (("%s"), ex.getFullDescription().c_str());
		_renderWindow->setFullscreen(false, 800, 600);
		MessageBox_Error(ex.getFullDescription());
	}
	catch (...)
	{
		LogErrorIDf("Engine", "Unknown exceptions!");
		_renderWindow->setFullscreen(false, 800, 600);
		MessageBox_Error("Unknown exceptions!");
	}
#endif
}
//------------------------------------------------------------------------
void EditorGameApp::AppConstruct()
{
	OnAppConstruct();
}
//------------------------------------------------------------------------
// EditorGame
//------------------------------------------------------------------------
template<> EditorGame* Singleton<EditorGame>::ms_Singleton = 0;

EditorGame::EditorGame(void)
: _camProxy(0)
{
}
//--------------------------------------------------------------------------------
EditorGame::~EditorGame(void)
{

}
//--------------------------------------------------------------------------------
void EditorGame::PostInit()
{
	String plugins = GameApp::getSingleton().GetMainConfigFile()
		.getSetting("EditorPlugins", "Main");

	EditorApp::getSingleton().LoadPlugins(plugins);

	GeneralGame::PostInit();
	_mainViewport->setBackgroundColour(
		EditorConfig::getSingleton().GetViewportColour());	

	Camera* cam = _mainViewport->getCamera();
	cam->setAutoAspectRatio(true);

	EventsManager::getSingleton().CreateEvent("SetMainCamera", Any(cam));

	EditorApp::getSingleton().SetActiveCamera(_mainViewport->getCamera());
	EditorApp::getSingleton().SetActiveViewport(_mainViewport);

	IStateManager* stm = this->_stateManager;
	//
	// 在此初始化所有的编辑器状态
	//
	EditorApp::getSingleton().NotifyCreateEditStates(stm);

#if 1
	//LoadMap(GameApp::getSingleton().GetMainConfigFile().getSetting("StartupScene", "Main"));
#else
	GameObjectList lst;

	const GameObjectFactoryManager::FactoryMap& fmap =
		GameObjectFactoryManager::getSingleton().GetFactoryMap();

	for (GameObjectFactoryManager::FactoryMap::const_iterator i = fmap.begin(); i != fmap.end(); ++i)
	{
		String objTypeName = i->first;
		IGameObject* obj = CreateGameObject(objTypeName, objTypeName);
		lst.push_back(obj);
	}
//	ObjectPropertiesWnd::getSingleton().SetObject(obj);

	ClassXMLSerializer ser;
	ser.Save(lst, "debug.ogremap");
#endif

	//SpecialEffectsManager::getSingleton().PrintRegisteredFactory();

	// 初始化各种资源管理窗口

	ObjectPropertiesWnd::getSingleton().SetObject(SkyBox::getSingletonPtr());

	_camProxy = OGRE_NEW GameCamera();
	((IGameObject*)(_camProxy))->BuildPropertyMap();
	_camProxy->SetCamera(cam);

	new MFCCameraController();
	MFCCameraController::getSingleton().SetCamera(cam);

	MainFrame* pMainWnd = dynamic_cast<MainFrame*>(AfxGetMainWnd());
	EditorApp::getSingleton().GetCommandHistory().AddCommandExecuteObserver(pMainWnd);
	//pMainWnd->UpdateCompositorList();
	CMFCToolBar::ResetAll();
}
//--------------------------------------------------------------------------------
void EditorGame::ShutDown()
{
	delete MFCCameraController::getSingletonPtr();

	_camProxy->SetCamera(0);
	OGRE_DELETE _camProxy;

	GeneralGame::ShutDown();

	EditorApp::getSingleton().SaveDisplayStringToFile("Prop.txt");
}
//--------------------------------------------------------------------------------
void EditorGame::Update(float fDeltaTime)
{
	GeneralGame::Update(fDeltaTime);
	if(_terrainGroup)
	{
		_terrainGroup->update();
	}
}
//--------------------------------------------------------------------------------
String EditorGame::GenerateObjectID(const String& typeName)
{
	static unsigned int autoID = 0;
	CTime time = CTime::GetCurrentTime();
	// typeName-year-month-day-hh-mm-ss-autoID
	String id = formatString("%s-%04d-%02d-%02d-%02d-%02d-%02d-%d",
		typeName.c_str(), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(),
		time.GetMinute(), time.GetSecond(), autoID);
	++autoID;
	return id;
}
//--------------------------------------------------------------------------------
ShadowCameraSetupPtr mPSSMSetup;

#pragma warning(push)
#pragma warning(disable : 4305)
void EditorGame::CreateDefaultSkyAndEnvironment()
{
	if(GetVisualScene())
	{
		if(!SceneEnvironment::getSingletonPtr())
		{
			SceneEnvironment* se = CreateGameObject<SceneEnvironment>(
				XGUID::createGUID());

			se->SetDescription("SceneEnvironment");
		}

		SceneEnvironment::getSingleton().SetAmbientLight(
			ColourValue(0.49f, 0.49f, 0.49f));
		

		if(!SkyBox::getSingletonPtr())
		{
			CreateGameObject<SkyBox>(XGUID::createGUID());
		}

		SkyBox* sky = SkyBox::getSingletonPtr();
		sky->SetDescription("SkyBox"); 
		sky->SetSkyType(EST_Box);

		if(!MainLight::getSingletonPtr())
		{
			CreateGameObject<MainLight>(XGUID::createGUID());			
		}

		// General scene setup
		_visualScene->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
		_visualScene->setShadowFarDistance(60000);
		
		
		MainLight* gl = MainLight::getSingletonPtr();

		Vector3 lightdir(0.55, -0.3, 0.75);
		lightdir.normalise();

		//gl->SetDirection(Vector3(0, -0.5, 0));
		gl->SetDirection(lightdir);
		gl->SetSpecularColour(ColourValue(0.4, 0.4, 0.4));
		gl->SetLightType(Light::LT_DIRECTIONAL);
		gl->SetPosition(0, 0, 0);
		//gl->SetDiffuseColour(ColourValue(0.49, 0.49, 0.49));
		gl->SetDiffuseColour(ColourValue(0.6, 0.6, 0.6));

		_terrainGlobalOpt->setLightMapDirection(gl->GetDirection());
		_terrainGlobalOpt->setCompositeMapAmbient(_visualScene->getAmbientLight());
		_terrainGlobalOpt->setCompositeMapDiffuse(gl->GetDiffuseColour());

#if 1
		if(_terrainGlobalOpt)
		{

			TerrainMaterialGeneratorA::SM2Profile* matProfile = 
				static_cast<TerrainMaterialGeneratorA::SM2Profile*>(
				_terrainGlobalOpt->getDefaultMaterialGenerator()->getActiveProfile());
			matProfile->setReceiveDynamicShadowsEnabled(true);


			// 3 textures per directional light (PSSM)
			_visualScene->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);

			_mainCamera->setNearClipDistance(0.1);
			_mainCamera->setFarClipDistance(100000);

			if (Root::getSingleton().getRenderSystem()
				->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
			{
				_mainCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
			}

			if (mPSSMSetup.isNull())
			{
				// shadow camera setup
				PSSMShadowCameraSetup* pssmSetup = new PSSMShadowCameraSetup();
				pssmSetup->setSplitPadding(_mainCamera->getNearClipDistance());
				pssmSetup->calculateSplitPoints(3, _mainCamera->getNearClipDistance(),
					_visualScene->getShadowFarDistance());

				pssmSetup->setOptimalAdjustFactor(0, 2);
				pssmSetup->setOptimalAdjustFactor(1, 1);
				pssmSetup->setOptimalAdjustFactor(2, 0.5);

				mPSSMSetup.bind(pssmSetup);

			}
			_visualScene->setShadowCameraSetup(mPSSMSetup);

			bool depthShadows = false;
			if (depthShadows)
			{
				_visualScene->setShadowTextureCount(3);
				_visualScene->setShadowTextureConfig(0, 2048, 2048, PF_FLOAT32_R);
				_visualScene->setShadowTextureConfig(1, 1024, 1024, PF_FLOAT32_R);
				_visualScene->setShadowTextureConfig(2, 1024, 1024, PF_FLOAT32_R);
				_visualScene->setShadowTextureSelfShadow(true);
				_visualScene->setShadowCasterRenderBackFaces(true);
				_visualScene->setShadowTextureCasterMaterial("PSSM/shadow_caster");

			}
			else
			{
				_visualScene->setShadowTextureCount(3);
				_visualScene->setShadowTextureConfig(0, 2048, 2048, PF_X8B8G8R8);
				_visualScene->setShadowTextureConfig(1, 1024, 1024, PF_X8B8G8R8);
				_visualScene->setShadowTextureConfig(2, 1024, 1024, PF_X8B8G8R8);
				_visualScene->setShadowTextureSelfShadow(false);
				_visualScene->setShadowCasterRenderBackFaces(false);
				_visualScene->setShadowTextureCasterMaterial(StringUtil::BLANK);
			}

			matProfile->setReceiveDynamicShadowsDepth(depthShadows);
			matProfile->setReceiveDynamicShadowsPSSM(static_cast<PSSMShadowCameraSetup*>(mPSSMSetup.get()));
		}
#endif

		if(SceneInfo::getSingletonPtr())
		{
			CreateGameObject("SceneInfo", XGUID::createGUID());
		}

		_mainCamera->setAutoAspectRatio(true);
		ObjectPropertiesWnd::getSingleton().SetObject(SkyBox::getSingletonPtr());
	}
}
#pragma warning(pop)

//--------------------------------------------------------------------------------
#include "Engine/ActorComponent_Mesh.h"
#include "Engine/ActorComponent_Light.h"
#include "Engine/ActorComponent_ParticleSystem.h"

void EditorGame::LoadMap(const String fileName)
{
	CFrameWndEx* mainWnd = dynamic_cast<CFrameWndEx*>(AfxGetMainWnd());
	mainWnd->GetActiveDocument()->SetTitle(fileName.c_str());

	GeneralGame::LoadMap(fileName);
	LoadPathFindingMap();

	LoadTileAndTerrain(0, 0);
	_terrainGroup->freeTemporaryResources();

	ResetCamera();

	EditorApp::getSingleton().NotifyMapLoad(_mapName);

	XGUID::createGUID();
	XGUID::createGUID();
	XGUID::createGUID();
	XGUID::createGUID();
	XGUID::createGUID();
	XGUID::createGUID();

	Actor* actor = CreateGameObject<Actor>(XGUID::createGUID());
	{
		ActorComponent_Mesh* comp = OGRE_NEW ActorComponent_Mesh();
		comp->SetName("MainBody");
		comp->SetMeshFile("Mesh_NPC_xiaoxue.mesh");
		actor->AddComponent(comp);
	}
	{
		ActorComponent_Mesh* comp = OGRE_NEW ActorComponent_Mesh();
		comp->SetName("TestAttach");
		comp->SetMeshFile("Mesh_NPC_xiaoxue.mesh");
		comp->SetAttachToSkeletonMesh(true);
		comp->SetAttachSource(OGRE_NEW AttachSource_ActorComponentMesh(
			(ActorComponent_Mesh*)actor->GetComponent(0)));
		comp->SetAttachTargetBoneName("Bip01 L Hand");
		actor->AddComponent(comp);
	}
	{
		ActorComponent_Light* comp = OGRE_NEW ActorComponent_Light();
		comp->SetName("TestLight");
		comp->SetOffsetPosition(Vector3(0, 200, 0));
		actor->AddComponent(comp);
	}
// 	{
// 		ActorComponent_ParticleSystem* comp = OGRE_NEW ActorComponent_ParticleSystem();
// 		comp->SetName("TestParticle");
// 		comp->SetParticleTemplate("shedengguangzhu");
// 		comp->SetOffsetPosition(Vector3(100, 200, 0));
// 		actor->AddComponent(comp);
// 	}

}
//--------------------------------------------------------------------------------
void EditorGame::CreateMap( const String mapName, int tileSize, int worldSize,
						   long terrainXCnt, long terrainZCnt, float texWorldSz,
						   uint16 blendMapSz, uint16 clrMapSz,
						   uint16 lightMapSz, float compositeMapDistance, float skirtSz,
						   uint16 compositeMapSz, float maxPixelErr, bool castDynShadows )
{
	String scenePath = GetSceneDir();
	String mapPath = scenePath + mapName + "/" ;
	if(FileEx::isDirExist(mapPath))
	{
		CString msg;
		msg.Format("Map[%s] already exist, path:[%s]", mapName.c_str(), mapPath.c_str());
		::AfxMessageBox(msg);
		return;
	}

	FileEx::makeDir(mapPath);

	String mapCfg = mapPath + mapName + ".xml";
	XMLDoc doc;
	doc.createRootNode("map");
	XMLElement rootNode = doc.getRoot();
	XMLElement terrainNode = rootNode.addChild("terrain");
	terrainNode.setAttribute("terrainXCnt", StringConverter::toString(terrainXCnt));
	terrainNode.setAttribute("terrainZCnt", StringConverter::toString(terrainZCnt));
	terrainNode.setAttribute("terrainSize", StringConverter::toString(tileSize + 1));
	terrainNode.setAttribute("terrainWorldSize", StringConverter::toString(worldSize));
	terrainNode.setAttribute("texWorldSz", StringConverter::toString(texWorldSz));
	terrainNode.setAttribute("blendMapSz", StringConverter::toString(blendMapSz));
	terrainNode.setAttribute("clrMapSz", StringConverter::toString(clrMapSz));
	terrainNode.setAttribute("lightMapSz", StringConverter::toString(lightMapSz));
	terrainNode.setAttribute("compositeMapDistance", StringConverter::toString(compositeMapDistance));
	terrainNode.setAttribute("skirtSz", StringConverter::toString(skirtSz));
	terrainNode.setAttribute("compositeMapSz", StringConverter::toString(compositeMapSz));
	terrainNode.setAttribute("maxPixelErr", StringConverter::toString(maxPixelErr));
	terrainNode.setAttribute("castDynShadows", StringConverter::toString(castDynShadows));
	doc.save(mapCfg);

	GeneralGame::LoadMap(mapName);
	CreateDefaultSkyAndEnvironment();


	EditorApp::getSingleton().NotifyMapCreated(mapName);

	//if(!mTerrainGlobals)
	//{
	//	mTerrainGlobals = OGRE_NEW TerrainGlobalOptions();

	//	// Configure global
	//	mTerrainGlobals->setMaxPixelError(8);
	//	// testing composite map
	//	mTerrainGlobals->setCompositeMapDistance(3000);
	//	//mTerrainGlobals->setUseRayBoxDistanceCalculation(true);
	//	//mTerrainGlobals->getDefaultMaterialGenerator()->setDebugLevel(1);
	//	//mTerrainGlobals->setLightMapSize(256);

	//	//matProfile->setLightmapEnabled(false);
	//	// Important to set these so that the terrain knows what to use for derived (non-realtime) data
	//	//mTerrainGlobals->setLightMapDirection(l->getDerivedDirection());
	//	//mTerrainGlobals->setCompositeMapAmbient(_visualScene->getAmbientLight());
	//	//mTerrainGlobals->setCompositeMapAmbient(ColourValue::Red);
	//	//mTerrainGlobals->setCompositeMapDiffuse(l->getDiffuseColour());

	//	// Configure default import settings for if we use imported image
	//	Terrain::ImportData& defaultimp = _terrainGroup->getDefaultImportSettings();
	//	defaultimp.terrainSize = tileSize + 1;
	//	defaultimp.worldSize = worldSize;
	//	defaultimp.inputScale = 600;
	//	defaultimp.minBatchSize = 33;
	//	defaultimp.maxBatchSize = 65;
	//	defaultimp.constantHeight = 0.0f;

	//	defaultimp.layerList.resize(3);
	//	defaultimp.layerList[0].worldSize = 100;
	//	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	//	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
	//	defaultimp.layerList[1].worldSize = 30;
	//	defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
	//	defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
	//	defaultimp.layerList[2].worldSize = 200;
	//	defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
	//	defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
	//}

	_terrainGroup->loadTerrain(0, 0);
	_terrainGroup->freeTemporaryResources();

	ResetCamera();
}
//--------------------------------------------------------------------------------
void EditorGame::ShowCameraProperties()
{
	ObjectPropertiesWnd::getSingleton().SetObject(_camProxy);
}
//--------------------------------------------------------------------------------
void EditorGame::SetSourceCameraForEdit(Camera* cam)
{
	_camProxy->SetCamera(cam);
}
// 
// TerrainSceneManager* EditorGame::GetTerrainSceneManager()
// {
// 	TerrainSceneManager* ts = dynamic_cast<TerrainSceneManager*>(_visualScene);
// 	return ts;
// }
//--------------------------------------------------------------------------------
void EditorGame::SavePathFindingMap(const String& outPath)
{
	String fileName = outPath + GetMapKeyName() + ".navmap";
	File::removeReadonly(fileName);
//	_pathFinder.SaveTerrainBrushTypeMap(fileName);
}
//--------------------------------------------------------------------------------
void EditorGame::LoadPathFindingMap()
{
	String fileName = GetMapKeyName() + ".navmap";
	//_pathFinder.LoadTerrainBrushTypeMapFromResourceSystem(fileName);
}
//--------------------------------------------------------------------------------
void EditorGame::SaveServerMap(const String& serverFileName)
{
	GameObjectList lst;

	const TypedGameObjectMap& objects = GetAllGameObjects();
	for (TypedGameObjectMap::const_iterator i = objects.begin(); i != objects.end(); ++i)
	{
		const GameObjectMap& objs = i->second;
		for(GameObjectMap::const_iterator j = objs.begin(); j != objs.end(); ++j)
		{
			IGameObject* obj = j->second;
			if(obj->HasDistributeFlag(eDF_Server))
			{
				lst.push_back(obj);
			}
		}
	}

	File::removeReadonly(serverFileName);

	ClassXMLSerializer ser;
	DefaultSerializationMask query(eDF_Server, eDF_Server);
	ser.Save(lst, serverFileName, &query);
}
//--------------------------------------------------------------------------------
void EditorGame::SaveClientMap(const String& clientFileName)
{
	GameObjectList lst;

	const TypedGameObjectMap& objects = GetAllGameObjects();
	for (TypedGameObjectMap::const_iterator i = objects.begin(); i != objects.end(); ++i)
	{
		const GameObjectMap& objs = i->second;
		for(GameObjectMap::const_iterator j = objs.begin(); j != objs.end(); ++j)
		{
			IGameObject* obj = j->second;
			if(obj->HasDistributeFlag(eDF_Client))
			{
				lst.push_back(obj);
			}
		}
	}

	File::removeReadonly(clientFileName);

	DefaultSerializationMask query(eDF_Client, eDF_Client);
	ClassXMLSerializer ser;
	ser.Save(lst, clientFileName, &query);
}
//--------------------------------------------------------------------------------

void EditorGame::OnNewScene()
{
	CreateDefaultSkyAndEnvironment();
	EditorApp::getSingleton().GetCommandHistory().Clear();
}
//--------------------------------------------------------------------------------
void EditorGame::OnDeleteScene()
{
	EditorApp::getSingleton().RemoveAllMemeto();

// 	if(EditState_Select::getSingletonPtr())
// 	{
// 		EditState_Select::getSingleton().SetSelectObject(0);
// 	}

	EditorApp::getSingleton().GetCommandHistory().Clear();
	DestroyAllGameObject();
}
//--------------------------------------------------------------------------------
void EditorGame::OnCloseScene()
{
	EditorApp::getSingleton().RemoveAllMemeto();
	EditorApp::getSingleton().GetCommandHistory().Clear();
	EditorApp::getSingleton().NotifyMapUnload(_mapName);
	UnloadMap();
}

void EditorGame::OnOpenScene(const String& fileName)
{
	DestroyAllUnManagedObject();

	//_pathFinder.Clear();
	SetCurrentDirectory(GameApp::getSingleton().GetAppExePath().c_str());
	String outBase, outExt, outPath;
	StringUtil::splitFullFilename(fileName, outBase, outExt, outPath);
	String clientMap = outBase + "." + outExt;
	LoadMap(clientMap);
	//GetStateManager().ActiveState(EES_Select);
	EditorApp::getSingleton().GetCommandHistory().Clear();
}
//--------------------------------------------------------------------------------
void EditorGame::SaveScene()
{
	//String outBase, outExt, outPath;
	//StringUtil::splitFullFilename(fileName, outBase, outExt, outPath);

	//SetCurrentDirectory(outPath.c_str());
	//SetCurrentDirectory(GameApp::getSingleton().GetAppExePath().c_str());

	//LogDebugIDf("Editor", "----[EditorGame::OnSaveScene] Current directory:[%s]", outPath.c_str());
	//_mapName = outBase + "." + outExt;

	//File::removeReadonly(fileName);

	DefaultSerializationMask query(eDF_All, eDF_All);
	SaveMap(_mapName, &query);


	//String clientMap = outPath + outBase + ".ClientMap";
	//SaveClientMap(clientMap);

	//String serverMap = outPath + outBase + ".ServerMap";
	//SaveServerMap(serverMap);

	////SavePathFindingMap();
	//SavePathFindingMap(outPath);

	////GetStateManager().RevertToPreviousState();
}
//--------------------------------------------------------------------------------
String EditorGame::GetMapKeyName()
{
	String outBase, outExt, outPath;
	StringUtil::splitFullFilename(_mapName, outBase, outExt, outPath);
	return outBase;
}
//--------------------------------------------------------------------------------
void EditorGame::ShowGameObjectByType(const String& objType, bool show)
{
	TypedGameObjectMap::iterator i = this->_objects.find(objType);
	if(i != _objects.end())
	{
		GameObjectMap::iterator j = i->second.begin();
		for (; j != i->second.end(); ++j)
		{
			IGameObject* obj = j->second;
			VisualObject* vo = dynamic_cast<VisualObject*>(obj);
			if(vo)
			{
				vo->SetVisible(show);
			}
		}
	}
}
//--------------------------------------------------------------------------------
void EditorGame::SnapshotMiniMap()
{
#if OCT_TERRAIN // TERRAIN:
	FogMode fogMode = SceneEnvironment::getSingleton().GetFogType();
	SceneEnvironment::getSingleton().SetFogType(FOG_NONE);
	IGameObject* obj = EditState_Select::getSingleton().GetSelecteObject();
	EditState_Select::getSingleton().SetSelectObject(0);

// 	// freeze time
// 	MSky& sky = MSky::getSingleton();
// 	float timeScale = sky.GetTimeScale();
// 	float  dayTime = sky.GetTime();
// 
 	float lodBias = Terrain::getSingleton().GetLodBias();
 	Terrain::getSingleton().SetLodBias(0.0f);
 	Terrain::getSingleton().UpdateTerrainData();
// 
// 	sky.SetTime(10.0f);
// 	sky.SetTimeScale(0.0f);

	SetShowObjectHelper(false);

	float w = (float)Terrain::getSingleton().GetTerrainWidthX();
	float h = (float)Terrain::getSingleton().GetTerrainWidthZ();

	TexturePtr tex = TextureManager::getSingleton().createManual(
		"__SNPASHOT__",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D,
		2048, 2048, 0,
		PF_A8B8G8R8,
		TU_RENDERTARGET);

	RenderTexture* rt = tex->getBuffer()->getRenderTarget();
	rt->setActive(true);
	rt->setAutoUpdated(false);

	Camera* cam = _visualScene->createCamera("__SNAPSHOT__");
	cam->setAutoAspectRatio(true);
	Viewport* vp = rt->addViewport(cam);
	vp->setMaterialScheme("Default");

	float y = sqrt(w * w + h * h) * 0.5f;
	Vector3 camPos(w * 0.5f, y, h * 0.5f);
	cam->setPosition(camPos);
	cam->setOrientation(Quaternion(Radian(Degree(-90.0f)), Vector3::UNIT_X));
	cam->setProjectionType(PT_ORTHOGRAPHIC);
	cam->setOrthoWindowHeight(std::max(w, h));

	rt->update();
	rt->writeContentsToFile(GetMapKeyName() + ".bmp");

	rt->removeAllViewports();
	_visualScene->destroyCamera(cam);
	TextureManager::getSingleton().remove("__SNPASHOT__");

	// restore

	SceneEnvironment::getSingleton().SetFogType(fogMode);

// 	sky.SetTimeScale(timeScale);
// 	sky.SetTime(dayTime);
	EditState_Select::getSingleton().SetSelectObject(obj);

	SetShowObjectHelper(true);

	Terrain::getSingleton().SetLodBias(lodBias);
	Terrain::getSingleton().UpdateTerrainData();
#endif
}
//--------------------------------------------------------------------------------
void EditorGame::Snapshot()
{
#if OCT_TERRAIN // TERRAIN:
	FogMode fogMode = SceneEnvironment::getSingleton().GetFogType();
	SceneEnvironment::getSingleton().SetFogType(FOG_NONE);
	IGameObject* obj = EditState_Select::getSingleton().GetSelecteObject();
	EditState_Select::getSingleton().SetSelectObject(0);

	// 	// freeze time
	// 	MSky& sky = MSky::getSingleton();
	// 	float timeScale = sky.GetTimeScale();
	// 	float  dayTime = sky.GetTime();
	//

	// 
	// 	sky.SetTime(10.0f);
	// 	sky.SetTimeScale(0.0f);

	SetShowObjectHelper(false);

#if OCT_TERRAIN
	float w = (float)Terrain::getSingleton().GetTerrainWidthX();
	float h = (float)Terrain::getSingleton().GetTerrainWidthZ();
#endif

	int texSz = (int)CEditorConfig::getSingleton().GetSnapshotSize();
	TexturePtr tex = TextureManager::getSingleton().createManual(
		"__SNPASHOT__",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D,
		texSz, texSz, 0,
		PF_A8B8G8R8,
		TU_RENDERTARGET);

	RenderTexture* rt = tex->getBuffer()->getRenderTarget();
	rt->setActive(true);
	rt->setAutoUpdated(false);

	Camera* cam = _visualScene->createCamera("__SNAPSHOT__");
	cam->setAutoAspectRatio(true);
	cam->setFOVy(CEditorConfig::getSingleton().GetSnapshotFov());

	Viewport* vp = rt->addViewport(cam);

	String materialScheme = EditorGame::getSingleton().GetMaterialScheme();
	LogDebugIDf("Editor", "Snap material scheme:%s", materialScheme.c_str());

	vp->setMaterialScheme(materialScheme);

	float y = sqrt(w * w + h * h) * 0.5f;
	Vector3 camPos(w * 0.5f, y, h);
	camPos *= CEditorConfig::getSingleton().GetCameraOffsetScale();

	EditorGame::getSingleton().GetVisualScene()->setShadowFarDistance(std::numeric_limits<float>::max());

	cam->setPosition(camPos);
	cam->setOrientation(Quaternion(CEditorConfig::getSingleton().GetSnapshotAngle(), -Vector3::UNIT_X));
	//cam->setProjectionType(PT_ORTHOGRAPHIC);
	//cam->setOrthoWindowHeight(std::max(w, h));

	rt->update();
	rt->writeContentsToFile(GetMapKeyName() + "_snap." + CEditorConfig::getSingleton().GetImageType());

	rt->removeAllViewports();
	_visualScene->destroyCamera(cam);
	TextureManager::getSingleton().remove("__SNPASHOT__");

	// restore

	SceneEnvironment::getSingleton().SetFogType(fogMode);

	// 	sky.SetTimeScale(timeScale);
	// 	sky.SetTime(dayTime);
	EditState_Select::getSingleton().SetSelectObject(obj);

	SetShowObjectHelper(true);

#endif
}
//--------------------------------------------------------------------------------
void EditorGame::SetShowObjectHelper(bool on)
{
	GameObjectMap objs = this->GetGameObjectsByType("Teleport");
	for (GameObjectMap::iterator i = objs.begin(); i != objs.end(); ++i)
	{
		Teleport* t = dynamic_cast<Teleport*>(i->second);
		t->SetShowCollisionBox(on);
	}

// 	objs = this->GetGameObjectsByType("MonsterSpawnPoint");
// 	for (GameObjectMap::iterator i = objs.begin(); i != objs.end(); ++i)
// 	{
// 		MonsterSpawnPoint* t = dynamic_cast<MonsterSpawnPoint*>(i->second);
// 		t->SetShowPreview(on);
// 	}
}
//--------------------------------------------------------------------------------
void EditorGame::AddUnManagedObject(IGameObject* obj)
{
	_unmanagedObjects.insert(obj);
}
//--------------------------------------------------------------------------------
void EditorGame::RemoveUnManagedObject(IGameObject* obj)
{
	GameObjectSet::iterator i = _unmanagedObjects.find(obj);
	if(i != _unmanagedObjects.end())
	{
		_unmanagedObjects.erase(i);
	}
}
//--------------------------------------------------------------------------------
void EditorGame::DestroyAllUnManagedObject()
{
	GameObjectSet::iterator i = _unmanagedObjects.begin();
	for (; i != _unmanagedObjects.end(); ++i)
	{
		IGameObject* obj = *i;

		obj->Destroy();
		String typeName = getClassName(obj);
		IGameObjectFactory* f = GameObjectFactoryManager::getSingleton().GetFactory(typeName);
		f->Destroy(obj);
	}
}

void EditorGame::ResetCamera()
{
	_mainCamera->setPosition(0, 300, 0);
	_mainCamera->setDirection(0, 0, -1);
}

void EditorGame::Init()
{
	GeneralGame::Init();
}