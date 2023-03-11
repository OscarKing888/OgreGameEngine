// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-24
// *************************************************************************************

#include "StdAfx.h"
#include "GameApp.h"
#include "File.h"
#include "IUpdateAble.h"
#include "DisplayDevice.h"
#include "IGame.h"
#include "LoadingListener.h"
#include "GameObjectFactoryManager.h"
#include "MeshDataManager.h"
#include "ISound.h"
#include "OgePlugin.h"
#include "LogicObject.h"
#include "Events.h"

template<> GameApp* Singleton<GameApp>::ms_Singleton = 0;
template<> ShadowTechniqueType* Singleton<ShadowTechniqueType>::ms_Singleton = 0;

ERunType GameApp::CurrentRunType = ERT_Game;
static ShadowTechniqueType _ShadowTechniqueTypeInstance;
//--------------------------------------------------------------------------------
ShadowTechniqueType::ShadowTechniqueType()
{
	M_AddEnumToDescriptorEx(SHADOWTYPE_NONE, None);
	M_AddEnumToDescriptorEx(SHADOWTYPE_TEXTURE_ADDITIVE, TextureAdditive);
	M_AddEnumToDescriptorEx(SHADOWTYPE_STENCIL_MODULATIVE, StencilModulative);
	M_AddEnumToDescriptorEx(SHADOWTYPE_STENCIL_ADDITIVE, StencilAdditive);
	M_AddEnumToDescriptorEx(SHADOWTYPE_TEXTURE_MODULATIVE, TextureModulative);
	M_AddEnumToDescriptorEx(SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED, TextureAdditiveIntegrated);
	M_AddEnumToDescriptorEx(SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED, TextureModulativeIntegrated);

	AbstractEnumDescriptor::Init();
}
//--------------------------------------------------------------------------------
GameApp::GameApp(IGame* game, const String& configFile)
: _isPaused(false)
, _ogreRoot(0)
, _renderWindow(0)
, _renderSystemName(("Direct3D9"))
, _game(game)
, _updateSpeed(1.0f)
, _enableStepUpdate(false)
, _stepUpdateSize(0.01f)
, _hicon(0)
, _quit(false)
, _isActive(true)
, _activeMaxFPS(1.0f / 40.0f)
, _inactiveMaxFPS(1.0f / 15.0f)
, _limitFPS(false)
, _sleepTime(2)
, _configFile(configFile)
{	
}
//--------------------------------------------------------------------------------
GameApp::GameApp()
{

}
//--------------------------------------------------------------------------------
GameApp::~GameApp(void)
{
}
//--------------------------------------------------------------------------------
bool GameApp::OnAppConstruct()
{
	if(!OnAppInit())
	{
		return false;
	}

	if(!OnInitOgre())
	{
		return false;
	}

	if(!OnConfigResourcePath())
	{
		return false;
	}

	OnCreateSingletons();

	return true;
}
//--------------------------------------------------------------------------------
void GameApp::Init()
{

#ifdef _PRODUCT_
	try
	{
#endif
		if(!OnAppConstruct())
		{
			return;
		}

		GameObjectFactoryManager::getSingleton().PrintRegisteredFactory();

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
	//catch (const CEGUI::Exception& ex)
	//{
	//	LogErrorIDf("Engine", (("%s"), ex.getMessage().c_str());
	//	_renderWindow->setFullscreen(false, 800, 600);
	//	MessageBox_Error(ex.getMessage());
	//}
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
//--------------------------------------------------------------------------------
void GameApp::Run()
{
#ifdef _PRODUCT_
	try
	{
#endif
		_RenderLoop();
#ifdef _PRODUCT_
	}
	catch (const std::exception& ex)
	{
		LogErrorIDf("Engine", (("%s"), ex.what());
		_renderWindow->setFullscreen(false, 800, 600);
		MessageBox_Error(ex.what());
	}
	//catch (const CEGUI::Exception& ex)
	//{
	//	LogErrorIDf("Engine", (("%s"), ex.getMessage().c_str());
	//	_renderWindow->setFullscreen(false, 800, 600);
	//	MessageBox_Error(ex.getMessage());
	//}
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
	OnShutDown();
}
//--------------------------------------------------------------------------------
void GameApp::_RenderLoop()
{
	WindowEventUtilities::addWindowEventListener(_renderWindow, this);

	//Root::getSingleton().startRendering();

	Root& root = *_ogreRoot;
	root.getRenderSystem()->_initRenderTargets();
	root.clearEventTimes();
	Timer* timer = root.getTimer();
	unsigned long startTime = timer->getMilliseconds();
	float timeElapsed = 0.0f;

	while (!_quit)
	{
		//Pump messages in all registered RenderWindow windows
		WindowEventUtilities::messagePump();

		if(_limitFPS)
		{
			unsigned long now = timer->getMilliseconds();
			float t = (now - startTime) * 0.001f;
			startTime = now;
			timeElapsed += t;

			float fpsLimit = _activeMaxFPS;
			if(!_isActive)
			{
				fpsLimit = _inactiveMaxFPS;
			}

			if(timeElapsed >= fpsLimit)
			{
				timeElapsed = 0.0f;
				if (!root.renderOneFrame())
					break;
			}
		}
		else
		{
			if (!root.renderOneFrame())
				break;
		}

		::Sleep(_sleepTime);
	}
}
//--------------------------------------------------------------------------------
void GameApp::OnShutDown()
{
#ifndef _PRODUCT_
	String msg;
	int c = 0;
	for(LogIDMap::iterator i = g_LogIDMap.begin(); i != g_LogIDMap.end(); ++i)
	{
		String tmp = formatString(("%-18s  "),
			(i->first + ("=") + (i->second ? ("1") : ("0"))).c_str());
		msg += tmp;

		++c;
		if(c == 4)
		{
			msg += ("\n");
			c = 0;
		}
	}

	LogManager::getSingleton().logMessage("Log ID(s)-------------------------------------------------------------------------------");
	LogManager::getSingleton().logMessage(msg.c_str());
	LogManager::getSingleton().logMessage("-------------------------------------------------------------------------------");
#endif

	NotifyAppShutdown();

	WindowEventUtilities::removeWindowEventListener(_renderWindow, this);
	_game->ShutDown();

	NotifyDestroySingletons();
	UnloadAllPlugins();

	_ogreRoot->removeFrameListener(this);
	if(_ogreRoot)
	{
		OGRE_DELETE _ogreRoot;
	}
}
//--------------------------------------------------------------------------------
void GameApp::Pause(bool pause, bool playSound )
{
	if(_isPaused == pause)
	{
		return;
	}

	_isPaused = pause;
	if (_isPaused)
	{
		if ( playSound && SoundManager::getSingletonPtr() )
		{
			SoundManager::getSingleton().PauseAll(true);
		}

		ControllerManager::getSingleton().setTimeFactor(0.0f);
	}
	else
	{
		if ( playSound && SoundManager::getSingletonPtr() )
		{
			SoundManager::getSingleton().PauseAll(false);
		}

		ControllerManager::getSingleton().setTimeFactor(_updateSpeed);
	}
}
//--------------------------------------------------------------------------------
bool GameApp::OnAppInit()
{
	String appPath = GetAppExePath();
	String exeName = __argv[0];
	String cfgFile;
#ifndef USE_CMDLINE_ARGUMENTS
	exeName = exeName.substr(1 + exeName.find_last_of(("\\")));
#ifdef	_DEBUG
	exeName.resize(exeName.size() - 5);
#else
	exeName.resize(exeName.size() - 4);
#endif
	cfgFile = exeName + ("\\") + exeName + (".cfg");
#else
	if(__argc == 2)
	{
		cfgFile = __argv[1];
	}
	else
	{
		cfgFile = /*appPath +*/ ("MainConfig.cfg");
	}
#endif
	SetCurrentDirectory(appPath.c_str());
	if(!_configFile.empty())
	{
		cfgFile = _configFile;
	}

	if(File::isFileExist(cfgFile))
	{
		_mainConfigFile.load(cfgFile);

#if !defined(_PRODUCT_)
		String str = _mainConfigFile.getSetting(("ShowConsole"), ("Main"));
		if(!str.empty())
		{
			bool r = StringConverter::parseBool(str);
			if(r)
			{
				AllocConsole();   //打开控制台窗口
				_tfreopen(("CON"), ("w"), stdout);  //将标准输出重定向到控制台窗口上
			}
		}
		_tprintf(("Using config file: %s\n\n"), cfgFile.c_str());
#endif

		// Load log id config
		ConfigFile::SettingsIterator sit = _mainConfigFile.getSettingsIterator("LogID");
		while (sit.hasMoreElements())
		{
			bool r = StringConverter::parseBool(sit.peekNextValue());
			EnableLogID(sit.peekNextKey(), r);
			sit.moveNext();
		}
	}

	NotifyAppInit();

	return true;
}
//--------------------------------------------------------------------------------
bool GameApp::OnInitOgre()
{
	String appPath = GetAppExePath();

	String plugins =  _mainConfigFile.getSetting(("PluginsCfg"), ("Main"));
	String logFile =  _mainConfigFile.getSetting(("LogFile"), ("Main"));

	try
	{
		_ogreRoot = OGRE_NEW Root(/*appPath +*/ plugins, (""), /*appPath + "Log/" + */logFile);
	}
	catch(const Exception& ex)
	{
		MessageBox_Error(ex.getFullDescription());
		return false;
	}

	LogManager::getSingleton().getDefaultLog()->addListener(this);

	String ogreLogLv = _mainConfigFile.getSetting(("OgreLogLevel"), ("Log"));

	LoggingLevel logLv = LL_NORMAL;
	if(ogreLogLv == ("LL_LOW"))
	{
		logLv = LL_LOW;
	}
	else if(ogreLogLv == ("LL_BOREME"))
	{
		logLv = LL_BOREME;
	}

	LogSysIDf("Engine", "Log detail:[%s]", ogreLogLv.c_str());

	LogSysIDf("Engine", "App path:[%s]", appPath.c_str());

	LogManager::getSingleton().setLogDetail(logLv);
	_ogreRoot->addFrameListener(this);

	OSVERSIONINFO osinfo = {0};
	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(& osinfo);
	LogSysIDf("Engine", "Operation system info, MajorVersion[%d] MinorVersion[%d] BuildNumber[%d] PlatformId[%d] CSDVersion[%s]",
		osinfo.dwMajorVersion, osinfo.dwMinorVersion, osinfo.dwBuildNumber, osinfo.dwPlatformId, osinfo.szCSDVersion);


	//
	// Load Engine Plugins
	//
	LoadPlugins(_mainConfigFile.getSetting("EnginePlugins", "Main"));

	return true;
}
//--------------------------------------------------------------------------------
bool GameApp::OnConfigResourcePath()
{
	String appPath = GetAppExePath();

	String langStr = _mainConfigFile.getSetting(("Language"), ("GUI"));

	String resCfg =  _mainConfigFile.getSetting(langStr, "ResCfgFile");

	LogSysIDf("Engine", "Loading resource config [%s]", resCfg.c_str());

	// Load resource paths from config file
	ConfigFile cf;
	cf.load(resCfg);

	// Go through all sections & settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}

#if defined(_WIN32) && 0
	// add fonts
	TCHAR buf[OGE_MAX_PATH];
	::GetWindowsDirectory(buf, OGE_MAX_PATH);
	String windowsFontDir = buf;
	windowsFontDir += ("\\Fonts");

	if(File::isDirExist(windowsFontDir))
	{
		ResourceGroupManager::getSingleton()
			.addResourceLocation(windowsFontDir, "FileSystem");
		LogSysIDf("Engine", ("Font directory:[%s]"), windowsFontDir.c_str());
	}
#endif

	// load FPS config

	ConfigFile fpsCfg;
	fpsCfg.loadFromResourceSystem("FPS.cfg", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	String val = fpsCfg.getSetting("LimitFPS");

	if(!val.empty())
	{
		_limitFPS = StringConverter::parseBool(val);
	}


	val = fpsCfg.getSetting("ActiveMaxFPS");

	if(!val.empty())
	{
		_activeMaxFPS = 1.0f / StringConverter::parseReal(val);
	}

	val = fpsCfg.getSetting("InActiveMaxFPS");

	if(!val.empty())
	{
		_inactiveMaxFPS = 1.0f / StringConverter::parseReal(val);
	}

	val = fpsCfg.getSetting("SleepTime");

	if(!val.empty())
	{
		_sleepTime = StringConverter::parseUnsignedLong(val);
	}

	return true;
}
//--------------------------------------------------------------------------------
void GameApp::OnCreateOgreRender()
{
#if 1
	String devStr = DisplayDevice::getSingleton().GetMainDisplayDevice().DeviceString;
	if(devStr.empty()) // 没用可用显示设备,可能是没有安装正确的显卡驱动程序
	{
		String msg = ("没用可用显示设备,可能是没有安装正确的显卡驱动程序,自动尝试使用OpenGL,可能不能进行游戏!");
		LogInfoID("Engine", msg.c_str());
		//::MessageBox(NULL, msg.c_str(), ("警告"), MB_OK | MB_ICONWARNING);
	}

	try
	{
		String plugin = ("RenderSystem_Direct3D9");
#ifdef _DEBUG
		plugin += ("_d");
#endif
		Root::getSingleton().loadPlugin(plugin);
	}
	catch (...)
	{
		LogErrorID("Engine", ("加载RenderSystem_Direct3D9出错!"));			

		/*if(::MessageBox(NULL, ("您还未安装DirectX9.0,是否采用备用的OpenGL渲染引擎启动？\n选择否退出游戏."), (""),
		MB_YESNO | MB_ICONINFORMATION) == IDYES)
		{*/
		// 				try
		// 				{
		// 					String plugin = ("RenderSystem_GL");
		// #ifdef _DEBUG
		// 					plugin += ("_d");
		// #endif
		// 					Root::getSingleton().loadPlugin(plugin);
		// 					_renderSystemName = ("OpenGL");
		// 				}
		// 				catch (...)
		// 				{
		OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Can't create render system!", "GameApp::OnCreateOgreRender");
		// 				}
		/*}
		else
		{
		throw throwf(("不能启动引擎,请确认您安装了正确的显卡驱动程序。系统默认的显卡驱动程序不能启动游戏!"));
		return;
		}*/
	}

	RenderSystemList rsList = _ogreRoot->getAvailableRenderers();
	assert(!rsList.empty());
	RenderSystem* selectedRenderSystem = rsList.front();
	assert(selectedRenderSystem != 0);

	String renderCfg =  _mainConfigFile.getSetting(("RenderCfg"), ("Main"));

	LogSysIDf("Engine", ("Render config file:[%s]"), renderCfg.c_str());

	ConfigFile cfg;
	cfg.load(renderCfg);

	ConfigFile::SettingsIterator seci = cfg.getSettingsIterator(_renderSystemName);
	while(seci.hasMoreElements())
	{
		String cfgKey = seci.peekNextKey();
		String cfgVal = seci.peekNextValue();
		selectedRenderSystem->setConfigOption(cfgKey, cfgVal);
		seci.moveNext();
	}

	_ogreRoot->setRenderSystem(selectedRenderSystem);

#else
	_ogreRoot->showConfigDialog();
#endif
}
//--------------------------------------------------------------------------------
void GameApp::OnCreateRenderWnd()
{
	String titleCfg =  _mainConfigFile.getSetting(("WindowTitle"), ("Main"));
	_renderWindow = _ogreRoot->initialise(true, titleCfg);

	HWND hwnd = GetGameWindowHWND();
	if(::IsWindow(hwnd) == TRUE)
	{
		::SendMessage(hwnd, WM_SETICON, TRUE, (LPARAM)_hicon);
		::SendMessage(hwnd, WM_SETICON, FALSE, (LPARAM)_hicon);
	}

	//LogConsolef("hwnd : %x  %x icon:%x", hwnd, ::GetDesktopWindow(), _hicon);
}
//--------------------------------------------------------------------------------
HWND GameApp::GetGameWindowHWND()
{
	return (HWND)_renderWindow->_getWindowHandle();
	/*D3D9RenderWindow* r = dynamic_cast<D3D9RenderWindow*>(_renderWindow);
	if(r)
	{
	HWND hwnd = r->getWindowHandle();
	return hwnd;
	}

	return 0;*/
}
//--------------------------------------------------------------------------------
String GameApp::GetAppExePath()
{
	TCHAR buf[OGE_MAX_PATH];
	memset(buf, 0, OGE_MAX_PATH * sizeof(TCHAR));
	::GetModuleFileName(NULL, buf, OGE_MAX_PATH - 1);		
	String strT = File::getDirFromFullPath(buf);
	return strT;
}
//--------------------------------------------------------------------------------
bool GameApp::frameStarted(const FrameEvent& evt)
{
	_Update(evt);
	return true;
}

bool GameApp::frameRenderingQueued(const FrameEvent& evt)
{
	/*if(_isPaused)
	{
	return true;
	}*/

	//_Update(evt);


	return true;
}
//--------------------------------------------------------------------------------
void GameApp::AddUpdateable(IUpdateAble* pU)
{
	UpdateableList::iterator i = std::find(_updateAddList.begin(), _updateAddList.end(), pU);
	if(i == _updateAddList.end())
	{
		i = std::find(_updateList.begin(), _updateList.end(), pU);
		if (i == _updateList.end())
		{
			_updateAddList.push_back(pU);
		}
	}

#ifdef _DEBUG
	const type_info* pTypeInfo = &typeid(*pU);
	const char* pszClassName = pTypeInfo->name();
	LogDebugIDf("Engine", "[GameApp::RemoveUpdateable][%s]", pszClassName);
#endif
}
//--------------------------------------------------------------------------------
void GameApp::RemoveUpdateable(IUpdateAble* pU)
{
	UpdateableList::iterator i = std::find(_updateList.begin(), _updateList.end(), pU);
	if(i != _updateList.end())
	{
		*i = 0;
	}

	i =	std::find(_updateAddList.begin(), _updateAddList.end(), pU);

	if(i != _updateAddList.end())
	{
		*i = 0;
	}

#ifdef _DEBUG
	const type_info* pTypeInfo = &typeid(*pU);
	const char* pszClassName = pTypeInfo->name();
	LogDebugIDf("Engine", "[GameApp::RemoveUpdateable][%s]", pszClassName);
#endif
}
//--------------------------------------------------------------------------------
void GameApp::CheckUpdateLists()
{
	if (!_updateAddList.empty())
	{
		UpdateableList::iterator itAdding = _updateAddList.begin();
		_updateList.insert(_updateList.end(), _updateAddList.begin(), _updateAddList.end());
		_updateAddList.clear();
	}

	UpdateableList::iterator itRemoving = _updateList.begin();
	while(itRemoving != _updateList.end())
	{
		if (*itRemoving == 0)
		{
			itRemoving = _updateList.erase(itRemoving);
		}
		else
		{
			++itRemoving;
		}
	}
}
//--------------------------------------------------------------------------------
void GameApp::windowMoved(RenderWindow* rw)
{

}
//--------------------------------------------------------------------------------
void GameApp::windowResized(RenderWindow* rw)
{
	if(_renderWindow)
	{
		_renderWindow->windowMovedOrResized();
	}
}
//--------------------------------------------------------------------------------
void GameApp::windowClosed(RenderWindow* rw)
{
	Root::getSingleton().queueEndRendering();
	_quit = true;
}
//--------------------------------------------------------------------------------
void GameApp::windowFocusChange(RenderWindow* rw)
{
	if(_game != 0)
	{
		if(rw->isActive())
		{
			_game->OnGameWindowSetFocus();
			_isActive = true;
		}
		else
		{
			_game->OnGameWindowLostFocus();
			_isActive = false;
		}
	}
	//LogConsolef("%s", rw->isActive() ? "set focus" : "lost focus");
#ifndef _PRODUCT_
	rw->setActive(true);
#endif
}
//--------------------------------------------------------------------------------
void GameApp::AddWindowEventListener(WindowEventListener* el)
{
	WindowEventUtilities::addWindowEventListener(_renderWindow, el);
}
//--------------------------------------------------------------------------------
void GameApp::RemoveWindowEventListener(WindowEventListener* el)
{
	WindowEventUtilities::removeWindowEventListener(_renderWindow, el);
}
//--------------------------------------------------------------------------------
RenderWindow* GameApp::GetRenderWindow()
{
	return _renderWindow;
}
//--------------------------------------------------------------------------------
void GameApp::SetUpdateSpeed(float fSpeed)
{
#if !defined(_PRODUCT_) && 1
	_updateSpeed = std::max(0.1f, fSpeed);
	ControllerManager::getSingleton().setTimeFactor(_updateSpeed);
	LogConsoleIDf("Engine", ("SpeedRate:[%f]"), _updateSpeed);
#endif
}
//--------------------------------------------------------------------------------
float GameApp::GetUpdateSpeed()
{
	return _updateSpeed;
}
//--------------------------------------------------------------------------------
void GameApp::messageLogged( const String& message, LogMessageLevel lml, bool maskDebug, const String &logName )
{
	//if (lml == LML_CRITICAL)
	{
		LogConsoleID("Engine", message.c_str());
	}
}
//--------------------------------------------------------------------------------
void GameApp::Quit()
{
	Root::getSingleton().queueEndRendering();
	_quit = true;
}
//--------------------------------------------------------------------------------
void GameApp::SnapshotToFile(const String& filenamePrefix, const String& filenameSuffix)
{
	if(_renderWindow)
	{
		_renderWindow->writeContentsToTimestampedFile(filenamePrefix, filenameSuffix);
	}
}
//--------------------------------------------------------------------------------
void GameApp::AddAppListener(IAppListener* listener)
{
	_appListener.push_back(listener);
}
//--------------------------------------------------------------------------------
void GameApp::RemoveAppListener(IAppListener* listener)
{
	AppListenerList::iterator i = std::find(_appListener.begin(),
		_appListener.end(), listener);
	if(i != _appListener.end())
	{
		_appListener.erase(i);
	}
	else
	{
		assert(false && "[GameApp::RemoveAppListener] listener not found!");
	}
}
//--------------------------------------------------------------------------------
void GameApp::NotifyAppInit()
{
	AppListenerList::iterator i = _appListener.begin();
	for (; i != _appListener.end(); ++i)
	{
		IAppListener* l = *i;
		l->OnAppInit();
	}
}
//--------------------------------------------------------------------------------
void GameApp::NotifyCreateSingletons()
{
	AppListenerList::iterator i = _appListener.begin();
	for (; i != _appListener.end(); ++i)
	{
		IAppListener* l = *i;
		l->OnCreateSingletons();
	}
}
//--------------------------------------------------------------------------------
void GameApp::NotifyPreGameInit()
{
	AppListenerList::iterator i = _appListener.begin();
	for (; i != _appListener.end(); ++i)
	{
		IAppListener* l = *i;
		l->OnPreGameInit();
	}
}
//--------------------------------------------------------------------------------
void GameApp::NotifyPostGameInit()
{
	AppListenerList::iterator i = _appListener.begin();
	for (; i != _appListener.end(); ++i)
	{
		IAppListener* l = *i;
		l->OnPostGameInit();
	}
}
//--------------------------------------------------------------------------------
void GameApp::NotifyAppInitCompleted()
{
	AppListenerList::iterator i = _appListener.begin();
	for (; i != _appListener.end(); ++i)
	{
		IAppListener* l = *i;
		l->OnAppInitCompleted();
	}
}
//--------------------------------------------------------------------------------
void GameApp::NotifyAppShutdown()
{
	AppListenerList::iterator i = _appListener.begin();
	for (; i != _appListener.end(); ++i)
	{
		IAppListener* l = *i;
		l->OnAppShutdown();
	}
}
//--------------------------------------------------------------------------------
void GameApp::NotifyDestroySingletons()
{
	AppListenerList::iterator i = _appListener.begin();
	for (; i != _appListener.end(); ++i)
	{
		IAppListener* l = *i;
		l->OnDestroySingletons();
	}
}
//--------------------------------------------------------------------------------
void GameApp::LoadPlugins(const String& cfgFile)
{
	if(cfgFile.empty())
	{
		LogWarningIDf("Engine", "[%s] config file[%s] is empty!", cfgFile.c_str());
		return;
	}

	StringVector pluginList;
	String pluginDir;
	ConfigFile cfg;

	try 
	{
		cfg.load( cfgFile );
	}
	catch (Exception)
	{
		LogErrorIDf("Engine",
			"GameApp::LoadPlugins[%s] not found, automatic plugin loading disabled.",
			cfgFile.c_str());

		return;
	}

	pluginDir = cfg.getSetting("PluginFolder"); // Ignored on Mac OS X, uses Resources/ directory
	pluginList = cfg.getMultiSetting("Plugin");

	if (pluginDir.empty())
	{
		// User didn't specify plugins folder, try current one
		pluginDir = ".";
	}

	char last_char = pluginDir[pluginDir.length()-1];
	if (last_char != '/' && last_char != '\\')
	{
		pluginDir += "\\";
	}

	for( StringVector::iterator it = pluginList.begin(); it != pluginList.end(); ++it )
	{
		Root::getSingleton().loadPlugin(*it);
	}
}
//--------------------------------------------------------------------------------
const OgePluginList& GameApp::GetLoadedPluginList() const
{
	return _loadedPlugins;
}
//--------------------------------------------------------------------------------
const OgePlugin* GameApp::GetPlugin(const String& pluginName) const
{
	OgePluginList::const_iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		Plugin* p = *i;
		if(p->getName() == pluginName)
		{
			return dynamic_cast<OgePlugin*>(p);
		}
	}

	LogErrorIDf("Engine", "[%s]Plugin [%s] not found!", __FUNCTION__, pluginName.c_str());
	return 0;
}
//--------------------------------------------------------------------------------
void GameApp::AddPlugin(OgePlugin* plugin)
{
	_loadedPlugins.push_back(plugin);
}
//--------------------------------------------------------------------------------
void GameApp::RemovePlugin(OgePlugin* plugin)
{
	OgePluginList::iterator i = std::find(
		_loadedPlugins.begin(), _loadedPlugins.end(), plugin);
	if(i != _loadedPlugins.end())
	{
		_loadedPlugins.erase(i);
	}
	else
	{
		LogWarningIDf("Engine", "[%s]Plugin [%s] not found!", __FUNCTION__, plugin->getName().c_str());
	}
}
//--------------------------------------------------------------------------------
void GameApp::UnloadPlugin(const String& pluginName)
{
	Root::getSingleton().unloadPlugin(pluginName);
}
//--------------------------------------------------------------------------------
void GameApp::UnloadAllPlugins()
{
	OgePluginList::iterator i = _loadedPlugins.begin();
	for (; i != _loadedPlugins.end(); ++i)
	{
		UnloadPlugin((*i)->getName());
	}

	_loadedPlugins.clear();
}
//--------------------------------------------------------------------------------
void GameApp::OnCreateSingletons()
{
	OGRE_NEW LogicObjectManager();
	OGRE_NEW MeshDataManager();
	OGRE_NEW EventsManager();
	NotifyCreateSingletons();
}
//--------------------------------------------------------------------------------
void GameApp::OnDestroySingletons()
{
	OGRE_DELETE LogicObjectManager::getSingletonPtr();
	OGRE_DELETE MeshDataManager::getSingletonPtr();
	OGRE_DELETE EventsManager::getSingletonPtr();
	NotifyDestroySingletons();
}
//--------------------------------------------------------------------------------
void GameApp::Shutdown()
{
	OnShutDown();
}
//--------------------------------------------------------------------------------
void GameApp::_Update( const FrameEvent &evt )
{
	if(_enableStepUpdate)
	{
		static float leftOverTime = 0.0f;

		float time = evt.timeSinceLastFrame + leftOverTime;
		unsigned int steps = (unsigned int)(time / _stepUpdateSize);
		float realTime = _updateSpeed * _stepUpdateSize;

		if(!_isPaused)
		{
			_game->Update(realTime);
		}

		for(unsigned int  i = 0; i < steps; ++i)
		{
			CheckUpdateLists();

			for(UpdateableList::iterator i = _updateList.begin(); i != _updateList.end(); ++i)
			{
				IUpdateAble* pU = *i;
				if (pU != 0)
				{
					// TODO: optimize here
					if(_isPaused)
					{
						INonPauseUpdateAble* p = dynamic_cast<INonPauseUpdateAble*>(pU);
						if(p)
						{
							p->Update(realTime);
						}
					}
					else
					{
						(pU)->Update(realTime);
					}
				}
			}
		}

		leftOverTime = time - (steps * _stepUpdateSize);	
	}
	else
	{
		CheckUpdateLists();

		float realTime = _updateSpeed * evt.timeSinceLastFrame;
		if(!_isPaused)
		{
			_game->Update(realTime);
		}

		for(UpdateableList::iterator i = _updateList.begin(); i != _updateList.end(); ++i)
		{
			IUpdateAble* pU = *i;
			if (pU != 0)
			{
				// TODO: optimize here
				if(_isPaused)
				{
					INonPauseUpdateAble* p = dynamic_cast<INonPauseUpdateAble*>(pU);
					if(p)
					{
						p->Update(realTime);
					}
				}
				else
				{
					(pU)->Update(realTime);
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------
// IAppListener
//--------------------------------------------------------------------------------
void IAppListener::OnAppInitCompleted()
{

}
//--------------------------------------------------------------------------------
void IAppListener::OnAppInit()
{

}
//--------------------------------------------------------------------------------
void IAppListener::OnAppShutdown()
{

}
//--------------------------------------------------------------------------------
void IAppListener::OnPreGameInit()
{

}
//--------------------------------------------------------------------------------
void IAppListener::OnPostGameInit()
{

}
//--------------------------------------------------------------------------------
void IAppListener::OnCreateSingletons()
{

}
//--------------------------------------------------------------------------------
void IAppListener::OnDestroySingletons()
{

}