#pragma once
#include "EditorCorePrerequisites.h"

class AbstractEditState;

#define EDITOR_MSGID_START (WM_USER + 100)
#define EDITOR_ID_START (WM_USER + 1)
#define EDITOR_ID_END (WM_USER + 99)

/// Return auto windows message ID
extern _EditorCoreExport UINT GetAutoWMID();
/// Create an auto edit state id
extern _EditorCoreExport int GetAutoEditStateID();


template<class T>
uint32 GetQueryMask()
{
	return GameObjectFactoryManager::getSingleton().GetQueryFlag<T>();
}

///
/// This the facade interface for each plugin
/// Register factories into engine/editor
/// Create the UI when notify events
///
class _EditorCoreExport EditorPlugin
	: public Plugin
	, public IAppListener
{
public:

	EditorPlugin(const Ogre::String& name);
	virtual ~EditorPlugin();
	const Ogre::String& getName() const;
	virtual void install();
	virtual void uninstall();
	virtual void initialise();
	virtual void shutdown();

	/// Get plugin description, author, copyright, copyleft...
	const String& getDescription() const;

	/// Notify when MainFrameWnd is created
	virtual void onCreateEditorMainWnd(CFrameWndEx* mainWnd);

	/// Notify when main docking panels created
	virtual void onCreateDockPanel(CFrameWndEx* mainWnd, CDockablePane* leftRoot,
		CDockablePane* rightRoot, CDockablePane* bottomRoot);

	/// Notify when main ribbon bar is created
	virtual void onCreateEditorMainToolbar(CMFCRibbonBar* mainToolbar);

	/// Notify when global object category created
	virtual void onCreateGlobalObjects(CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel);

	/// Notify when tools category created
	virtual void onCreateToolsCategory(CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel);

	/// Notify when state manager is created
	virtual void onCreateEditStates(IStateManager* sm);

	/// Notify after edit state category created
	virtual void onPostCreateEditStateCategory(CMFCRibbonCategory* cat);

	/// Notify when the main render window is resized.
	virtual void onWindowResized(RenderWindow* rw);

	virtual void onMapCreate(const String& mapName);
	virtual void onMapLoad(const String& mapName);
	virtual void onMapUnload(const String& mapName);
	virtual void onMapSave(const String& mapName);

protected:

	Ogre::String _name;
	Ogre::String _description;
};

//--------------------------------------------------------------------------------

typedef vector<EditorPlugin*>::type EditorPluginList;
typedef map<String, AbstractEditState*>::type StateNameMap;

struct _EditorCoreExport SPickGameObjectResult
{
	MovableObject*		RawObject;
	IGameObject*	GameObject;
	Vector3				IntersectPoint;
	RaySceneQueryResult	RawResult;

	SPickGameObjectResult()
		: RawObject(0)
		, GameObject(0)
		, IntersectPoint(0, 0, 0)
	{
	}
};


class _EditorCoreExport EditorApp
	: public Singleton<EditorApp>
	, public OgeAlloc
	, public IAppListener
{
public:

	EditorApp();
	virtual ~EditorApp();

	/// Return the game object
	GeneralGame* GetGame() { return _game; }
	/// Set the game object
	void SetGame(GeneralGame* game);

	/// Shutdown when closing edit 
	void Shutdown();

	/// Load plugins from config file
	virtual void LoadPlugins(const String& cfgFile);
	/// Get current loaded OGE plugin list
	const EditorPluginList& GetLoadedPluginList() const;
	/// Get a plugin by name
	const EditorPlugin* GetPlugin(const String& pluginName) const;
	/// Add a plugin int to list
	void AddPlugin(EditorPlugin* plugin);
	/// Remove a plugin from list
	void RemovePlugin(EditorPlugin* plugin);
	/// Unload a plugin
	void UnloadPlugin(const String& pluginName);
	/// Unload all plugin
	void UnloadAllPlugins();

	/// Get the default command history
	CommandHistory& GetCommandHistory() { return _cmdHistory; }

	/// Return the main state manager of editor game
	IStateManager& GetStateManager();
	/// Add a new edit state to editor game
	void AddState(IStatePtr s);
	/// Active state by name
	void ActiveState(const String& name);
	void ActiveState(int id);
	/// Get a state by name
	AbstractEditState* GetState(const String& name) const;
	AbstractEditState* GetState(int id) const;
	/// Return all the states
	const StateNameMap& GetStateList() const;
	/// Return state by command ID for toolbar event
	int GetActiveStateCommandID() const;

	/// Add a memento to list
	void AddMemento(IMementoPtr m);
	/// Remove a memento
	void RemoveMemento(IMementoPtr m);
	/// Remove all mementos
	void RemoveAllMemeto();

	/// Get current active camera object
	Camera* GetActiveCamera();
	/// Set current active camera object
	void SetActiveCamera(Camera* cam);
	void SetActiveViewport(Viewport* cam) { _activeViewport = cam; }
	Viewport* GetActiveViewport() { return _activeViewport; }


	/// Load local display string from resource system file
	void LoadDisplayStringCfg(const std::string& file = "DisplayString.cfg",
		const std::string& resGroup = EditorResourceGroup);

	/// Get local display string from a key string
	const String& GetDisplayString(const String& keyString);
	/// Get key string from local string
	const String& GetKeyString(const String& displayString);
	/// Save the current display string map to file
	void SaveDisplayStringToFile(const String& fileName);

	/// Notify the events to plugins
	void NotifyCreateEditorMainWnd(CFrameWndEx* mainWnd);
	void NotifyCreateMainToolbar(CMFCRibbonBar* mainToolbar);
	void NotifyCreateEditStates(IStateManager* sm);
	void NotifywindowResized(RenderWindow* rw);
	void NotifyMapCreated(const String& mapName);
	void NotifyMapLoad(const String& mapName);
	void NotifyMapUnload(const String& mapName);
	void NotifyMapSave(const String& mapName);
	void NotifyCreateDockPanel(CFrameWndEx* mainWnd, CDockablePane* leftRoot,
		CDockablePane* rightRoot, CDockablePane* bottomRoot);
	void NotifyCreateToolsCategory(CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel);
	void NotifyCreateGlobalObjectsCategory(CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel);
	void NotifyPostCreateEditStateCategory(CMFCRibbonCategory* cat);

	/// Notify un-processed window message to registered handler
	BOOL NotifyWndMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	void RegisterWMHandler(CCmdTarget* h);
	void UnRegisterWMHandler(CCmdTarget* h);

	/// Return the main toolbar
	CMFCRibbonBar* GetMainToolbar();
	/// Return the mfc app
	CWinApp* GetMFCApp();
	/// Init the mfc app object
	void SetMFCApp(CWinApp* app);

	/// Pick on terrain, if cam is null, use active camera
	TerrainGroup::RayResult PickOnTerrain(int mouseX, int mouseY, Camera* cam =  0);
	/// Pick game objects, if cam is null, use active camera
	SPickGameObjectResult PickGameObjects(int mouseX, int mouseY,
		uint32 queryMask = MovableObject::getDefaultQueryFlags(),
		uint32 queryTypeMask = MovableObject::getDefaultQueryFlags(),
		Camera* cam =  0);

	/// Set mouse cursor image
	void SetCursor(HCURSOR hcr);

	IGameObject* GetSelectedObject() { return _selectedObject; }
	void SetSelectObject(IGameObject* obj) { _selectedObject = obj; }

protected:

	Camera* _activeCamera;
	Viewport*	_activeViewport;

	IGameObject* _selectedObject;

	GeneralGame* _game;
	CMFCRibbonBar* _mainToolbar;
	CWinApp* _mfcApp;

	EditorPluginList _loadedPlugins;

	CommandHistory	_cmdHistory;

	typedef list<IMementoPtr>::type MementoList;
	MementoList		_mementoList;

	typedef map<String, String>::type String2StringMap;
	String2StringMap _key2DisplayString;
	String2StringMap _display2KeyString;

	ObjectSelection _selectedObjects;

	StateNameMap _states;

	RaySceneQuery*	_terrainRayQuery;

	typedef std::list<CCmdTarget*> CmdTargetList;
	CmdTargetList _handlerLst;

	virtual void OnPostGameInit();
};

#define _UIDisplayString(x) EditorApp::getSingleton().GetDisplayString(x)
#define _UIKeyString(x) EditorApp::getSingleton().GetKeyString(x)