#pragma once
#include "PathFinderHelper.h"

class EditorGameApp : public GameApp, public OgeAlloc
{
public:

	EditorGameApp(IGame* pLogic, const String& configFile = "");
	virtual ~EditorGameApp(void);
	
	virtual void Quit();

	void Run(HWND hwndRender);

	virtual void windowResized(RenderWindow* rw);

	void SetMainWnd(HWND hwnd) { _renderHWND = hwnd; }

	virtual void Init();
	void AppConstruct();


protected:

	HWND	_renderHWND;
	bool	_quit;
	virtual void OnCreateRenderWnd();
	virtual void _RenderLoop();
	virtual bool OnInitOgre();
	virtual void OnShutDown();

	virtual bool OnConfigResourcePath();
};

//------------------------------------------------------------------------

enum ECursorType
{
	ECT_Normal,
	ECT_Select,
	ECT_Move,
	ECT_MoveX,
	ECT_MoveY,
	ECT_MoveZ,
	ECT_Rotate,
	ECT_RotateX,
	ECT_RotateY,
	ECT_RotateZ,
	ECT_Scale
};

class EditorGame
	: public GeneralGame
	, public Singleton<EditorGame>
{
public:

	EditorGame(void);
	virtual ~EditorGame(void);

	virtual void ShutDown();
	void SetCurrentCreateClassName(const String& name){ _currentCreateClassName = name; }
	const String& GetCurrentCreateClassName() const { return _currentCreateClassName; }
	
	static String GenerateObjectID(const String& typeName);
	void CreateDefaultSkyAndEnvironment();

	virtual void LoadMap(const String fileName);

	void CreateMap(const String mapName, int tileSize, int worldSize, long terrainXCnt,
		long terrainZCnt, float texWorldSz, uint16 blendMapSz, uint16 clrMapSz,
		uint16 lightMapSz, float compositeMapDistance, float skirtSz,
		uint16 compositeMapSz, float maxPixelErr, bool castDynShadows);

	void ResetCamera();

	String GetMapKeyName();

	void ShowCameraProperties();
	void SetSourceCameraForEdit(Camera* cam);

	void OnNewScene();
	void OnDeleteScene();
	void OnCloseScene();
	void OnOpenScene(const String& fileName);
	void SaveScene();

	void SaveServerMap(const String& serverFileName);
	void SaveClientMap(const String& clientFileName);

	void ShowGameObjectByType(const String& objType, bool show);

	void SnapshotMiniMap();
	void Snapshot();
	
	void SetShowObjectHelper(bool on);

	void AddUnManagedObject(IGameObject* obj);
	void RemoveUnManagedObject(IGameObject* obj);
	void DestroyAllUnManagedObject();

	String GetMaterialScheme() const { return _currentMaterialScheme; }
	void SetMaterialScheme(const String& m){ _currentMaterialScheme = m; }


protected:

	String	_currentMaterialScheme;
	String	_currentCreateClassName;
	bool			_modified;

	GameCamera*	_camProxy;

	GameObjectList	_serverObjects;

	typedef std::set<IGameObject*> GameObjectSet;
	GameObjectSet	_unmanagedObjects;

	virtual void Init();
	virtual void PostInit();
	virtual void Update(float fDeltaTime);
	void LoadPathFindingMap();
	void SavePathFindingMap(const String& outPath);
};
