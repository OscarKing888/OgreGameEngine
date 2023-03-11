// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-24
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"
#include "IState.h"
#include "EnumDescriptorManager.h"
#include "IGame.h"

class OgePlugin;
typedef vector<OgePlugin*>::type OgePluginList;

///
/// Runtime for identify in editor or game
///
enum _EngineExport ERunType
{
	ERT_Game,
	ERT_Editor
};

///
/// App listener for App important events
///
class _EngineExport IAppListener
{
public:

	virtual void OnAppInit();
	virtual void OnCreateSingletons();
	virtual void OnPreGameInit();
	virtual void OnPostGameInit();
	virtual void OnAppInitCompleted();	
	virtual void OnAppShutdown();
	virtual void OnDestroySingletons();
};

class IUpdateAble;
class IGame;

///
/// The core class for running the game
///
class _EngineExport GameApp
	: public WindowEventListener
	, public FrameListener
	, public Singleton<GameApp>
	, LogListener
{
public:

	static ERunType CurrentRunType;

	GameApp();
	GameApp(IGame* game, const String& configFile = "");
	virtual ~GameApp(void);

	/// Init app
	virtual void Init();
	/// Main loop function
	virtual void Run();
	/// Pause the game, and sound
	virtual void Pause(bool pause, bool playSound = true );
	/// Return if game is pauesed
	bool IsPaused() const { return _isPaused; }
	/// Quit app
	virtual void Quit();
	/// Shutdown engine
	void Shutdown();

	virtual void SetUpdateSpeed(float fSpeed = 1.0f);
	float GetUpdateSpeed();

	String GetAppExePath();
	void AddUpdateable(IUpdateAble* pU);
	void RemoveUpdateable(IUpdateAble* pU);

	void AddWindowEventListener(WindowEventListener* el);
	void RemoveWindowEventListener(WindowEventListener* el);

	RenderWindow* GameApp::GetRenderWindow();

	ConfigFile& GetMainConfigFile() { return _mainConfigFile; }
	IGame* GetGame() const { return _game; }

	// 精确更新，用更小的时间段Update
	void EnableStepUpdate(bool on){ _enableStepUpdate = on; }
	bool IsStepUpdateEnabled() const { return _enableStepUpdate; }
	void SetStepUpdateSize(float stepSize){ _stepUpdateSize = stepSize; }
	float GetStepUpdateSize() const { return _stepUpdateSize; }

	void SnapshotToFile(const String& filenamePrefix = "Snapshot", const String& filenameSuffix = ".jpg");

	void SetHICON(HICON hicon) { _hicon = hicon; }
	HICON GetHICON() const { return _hicon; }
	HWND GetGameWindowHWND();

	void SetFPSLimit(bool on){ _limitFPS = on; }
	bool GetFPSLimit() const { return _limitFPS; }

	void SetActiveMaxFPS(float fps){ _activeMaxFPS = fps; }
	void SetInctiveMaxFPS(float fps){ _inactiveMaxFPS = fps; }

	float GetActiveMaxFPS() const { return _activeMaxFPS; }
	float GetInactiveMaxFPS() const { return _inactiveMaxFPS; }

	void AddAppListener(IAppListener* listener);
	void RemoveAppListener(IAppListener* listener);

	/// Load plugins from config file
	virtual void LoadPlugins(const String& cfgFile);
	/// Get current loaded OGE plugin list
	const OgePluginList& GetLoadedPluginList() const;
	/// Get a plugin by name
	const OgePlugin* GetPlugin(const String& pluginName) const;
	/// Add a plugin int to list
	void AddPlugin(OgePlugin* plugin);
	/// Remove a plugin from list
	void RemovePlugin(OgePlugin* plugin);
	/// Unload a plugin
	void UnloadPlugin(const String& pluginName);
	/// Unload all plugin
	void UnloadAllPlugins();

protected:

	bool				_initOK;
	HICON				_hicon;
	bool				_isPaused;

	typedef	vector<IUpdateAble*>::type	UpdateableList;
	UpdateableList		_updateList;
	UpdateableList		_updateAddList;

	Root*			_ogreRoot;
	RenderWindow*	_renderWindow;
	String		_renderSystemName;
	ConfigFile	_mainConfigFile;
	IGame*				_game;
	StateManager		_stateManager;
	float				_updateSpeed;

	bool				_enableStepUpdate;
	float				_stepUpdateSize;
	bool				_quit;
	bool				_isActive;
	bool				_limitFPS;
	float				_activeMaxFPS;
	float				_inactiveMaxFPS;
	DWORD				_sleepTime;
	String				_configFile;

	OgePluginList _loadedPlugins;

	typedef list<IAppListener*>::type AppListenerList;
	AppListenerList		_appListener;

	void NotifyAppInit();
	void NotifyCreateSingletons();
	void NotifyPreGameInit();
	void NotifyPostGameInit();
	void NotifyAppInitCompleted();
	void NotifyAppShutdown();
	void NotifyDestroySingletons();


	virtual bool OnAppConstruct();
	virtual bool OnAppInit();
	virtual void OnShutDown();

	virtual bool OnInitOgre();
	virtual void OnCreateSingletons();
	virtual void OnDestroySingletons();

	virtual void OnCreateOgreRender();
	virtual void OnCreateRenderWnd();
	virtual bool OnConfigResourcePath();

	virtual bool frameStarted(const FrameEvent& evt);
	virtual bool frameRenderingQueued(const FrameEvent& evt);

	void _Update( const FrameEvent &evt );
	virtual void windowMoved(RenderWindow* rw);
	virtual void windowResized(RenderWindow* rw);
	virtual void windowClosed(RenderWindow* rw);
	virtual void windowFocusChange(RenderWindow* rw);

	void CheckUpdateLists();

	virtual void messageLogged( const String& message, LogMessageLevel lml, bool maskDebug, const String &logName );

	virtual void _RenderLoop();
};

class _EngineExport ShadowTechniqueType
	: public AbstractEnumDescriptor
	, public Singleton<ShadowTechniqueType>
{
public:

	ShadowTechniqueType();
	virtual ~ShadowTechniqueType(){}	
};

