// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-27
// *************************************************************************************

#include "StdAfx.h"
#include "CEGUIGame.h"
#include "UIController.h"
#include "CmdLineUI.h"

#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

CEGUI::MouseButton ConvertOISMouseButtonToCegui(int buttonID)
{
	switch (buttonID)
	{
	case 0: return CEGUI::LeftButton;
	case 1: return CEGUI::RightButton;
	case 2:	return CEGUI::MiddleButton;
	case 3: return CEGUI::X1Button;
	default: return CEGUI::LeftButton;
	}
}
//----------------------------------------------------------------------------------------------
CEGUIGame::CEGUIGame(void)
: _guiRender(0)
, _guiSystem(0)
{
}
//----------------------------------------------------------------------------------------------
CEGUIGame::~CEGUIGame(void)
{
}
//----------------------------------------------------------------------------------------------
void CEGUIGame::InitImp()
{
	GeneralGame::InitImp();

	GameApp::getSingleton().AddUpdateable(&OISInputManager::getSingleton());
	WindowEventUtilities::addWindowEventListener(GameApp::getSingleton().GetRenderWindow(), this);

	LoadingListener::getSingleton().SetLoadingText("Init all resource gropus...");
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	LoadingListener::getSingleton().SetLoadingText("Loading UI...");
	InitUI();

	LoadingListener::getSingleton().SetLoadingText("Init states...");
	InitStates();
}
//----------------------------------------------------------------------------------------------
void CEGUIGame::ShutDown()
{
	//zhangdeke date:2008-12-11
	//destroy Script Module
	//OGRE_DELETE CEGUI::System::getSingleton().getScriptingModule();

	WindowEventUtilities::removeWindowEventListener(GameApp::getSingleton().GetRenderWindow(), this);
	UIControllerManager::getSingleton().ShutDown();

	OISInputManager::getSingleton().RemoveGUIKeyListener();
	OISInputManager::getSingleton().RemoveGUIMouseListener();
	OISInputManager::getSingleton().ShutDown();


	if(_guiSystem)
	{
		CEGUI::OgreRenderer::destroySystem();
		//OGRE_DELETE _guiSystem;
	}

	_guiRender = 0;

	GeneralGame::ShutDown();
}
//----------------------------------------------------------------------------------------------
void CEGUIGame::PostInit()
{
	LoadingListener::getSingleton().SetLoadingText("Post init...");

	LoadingListener::getSingleton().SetLoadingText("Init input system...");
	OISInputManager::getSingleton().Init(GameApp::getSingleton().GetRenderWindow());
	OISInputManager::getSingleton().SetGUIKeyListener(this);
	OISInputManager::getSingleton().SetGUIMouseListener(this);

	AbstractGame::PostInit();
}
//----------------------------------------------------------------------------------------------
bool CEGUIGame::keyPressed( const OIS::KeyEvent &arg )
{
	if(CEGUI::System::getSingletonPtr())
	{
		bool r1 = CEGUI::System::getSingleton().injectKeyDown(arg.key);
		if(r1)
		{
			return true;
		}
	}

	if(GeneralGame::keyPressed(arg))
	{
		return true;
	}

#if !defined(_PRODUCT_) && 1
	if(arg.key == OIS::KC_GRAVE)
	{
		IUIControllerPtr spUI = UIControllerManager::getSingleton().GetUIController(("CommandLine"));
		if(spUI)
		{
			spUI->Show(!spUI->IsWindowVisible());
		}
		return true;
	}
	else if(arg.key == OIS::KC_F11)
	{
		if(_visualScene)
		{
			_visualScene->showBoundingBoxes(
				!_visualScene->getShowBoundingBoxes());
			return true;
		}
	}
	else if(arg.key == OIS::KC_F8)
	{
		_debugOverlay = OverlayManager::getSingleton().getByName(("Core/DebugOverlay"));

		OverlayContainer* oc = _debugOverlay->getChild("BorderPanel(Core/StatPanel)");
		if(oc)
		{
			int h = GameApp::getSingleton().GetRenderWindow()->getHeight();
			oc->setPosition(5, -h + oc->getHeight() + 5);
		}

		if (_debugOverlay)
		{
			ShowDebugOverlay(!_debugOverlay->isVisible());
			return true;
		}
	}		
	// 		else if(arg.key == OIS::KC_H)
	// 		{
	// 			SelectNextShadowTechnique();
	// 			return true;
	// 		}
	else if(arg.key == OIS::KC_PAUSE)
	{
		GameApp::getSingleton().Pause(!GameApp::getSingleton().IsPaused());
		return true;
	}
	else if(arg.key == OIS::KC_MINUS)
	{
		float curSpeed = GameApp::getSingleton().GetUpdateSpeed();
		GameApp::getSingleton().SetUpdateSpeed(curSpeed - 0.1f);
		return true;
	}
	else if(arg.key == OIS::KC_EQUALS)
	{
		float curSpeed = GameApp::getSingleton().GetUpdateSpeed();
		GameApp::getSingleton().SetUpdateSpeed(curSpeed + 0.1f);
		return true;
	}
	//else if(arg.key == OIS::KC_0)
	//{
	//	GameApp::getSingleton().SetUpdateSpeed(0.1f);
	//	return true;
	//}
	//else if(arg.key == OIS::KC_9)
	//{
	//	GameApp::getSingleton().SetUpdateSpeed(1.0f);
	//	return true;
	//}

	if(arg.key == OIS::KC_ESCAPE)
	{
		GameApp::getSingleton().Quit();
		return true;
	}
	else if(arg.key == OIS::KC_RETURN)
	{
		// 			if(OISInputManager::getSingleton().IsModifierDown(OIS::Keyboard::Alt))
		// 			{
		// 				RenderWindow* pRW = GameApp::getSingleton().GetRenderWindow();
		// 				pRW->setFullscreen(!pRW->isFullScreen(), pRW->getWidth(), pRW->getHeight());
		// 				return true;
		// 			}
	}
	else if(arg.key == OIS::KC_SYSRQ)
	{
		String path = GameApp::getSingleton().GetAppExePath() + "Snapshot/";
		if(!File::isDirExist(path))
		{
			File::makeDir(path);
		}

		String ext = GameApp::getSingleton().GetMainConfigFile().getSetting("SnapshotFormat", "Main");
		GameApp::getSingleton().SnapshotToFile(path + "Snapshot", ext);
	}

#endif

	return false;
}
//----------------------------------------------------------------------------------------------
bool CEGUIGame::keyReleased( const OIS::KeyEvent &arg )
{
	if(CEGUI::System::getSingletonPtr())
	{
		return CEGUI::System::getSingleton().injectKeyUp(arg.key);
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool CEGUIGame::mouseMoved( const OIS::MouseEvent &arg )
{
	if(CEGUI::System::getSingletonPtr())
	{
		//bool r = CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
		bool r = CEGUI::System::getSingleton().injectMousePosition((float)arg.state.X.abs, (float)arg.state.Y.abs);
		return r;
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool CEGUIGame::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(CEGUI::System::getSingletonPtr())
	{
		return CEGUI::System::getSingleton().injectMouseButtonDown(ConvertOISMouseButtonToCegui(id));
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool CEGUIGame::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(CEGUI::System::getSingletonPtr())
	{
		return CEGUI::System::getSingleton().injectMouseButtonUp(ConvertOISMouseButtonToCegui(id));
	}

	return false;
}
//----------------------------------------------------------------------------------------------
void CEGUIGame::Update(float deltaTime)
{
	if(CEGUI::System::getSingletonPtr())
	{
		CEGUI::System::getSingleton().injectTimePulse(deltaTime);
	}

	GeneralGame::Update(deltaTime);
}
//----------------------------------------------------------------------------------------------
void CEGUIGame::InitUI()
{
	// Render quad count
	ConfigFile& cfg = GameApp::getSingleton().GetMainConfigFile();
	String str = cfg.getSetting(("SchemeCnt"), ("GUI"));
	int n = StringConverter::parseInt(str);

	if(n <= 0)
	{
		return;
	}

	str = cfg.getSetting(("RenderQuadCnt"), ("GUI"));
	if(str.empty())
	{
		str = ("512");
	}

	int guiQuadCount = StringConverter::parseInt(str);

	LoadingListener::getSingleton().SetLoadingText("Creating GUI Render...");

	str = cfg.getSetting(("LastRender"), ("GUI"));
	if (str.empty())
	{
		str = ("false");
	}

	bool b = StringConverter::parseBool(str);

	LoadingListener::getSingleton().SetLoadingText("Creating GUI System...");

	_guiRender = &CEGUI::OgreRenderer::bootstrapSystem(
		*GameApp::getSingleton().GetRenderWindow());

	/*_guiRender = OGRE_NEW CEGUI::OgreRenderer(GameApp::getSingleton().GetRenderWindow(), 
	RENDER_QUEUE_OVERLAY, b, guiQuadCount);*/

	// xml parser module
	String xmlParser = cfg.getSetting(("XMLParser"), ("GUI"));
	if(xmlParser.empty())
	{
		xmlParser = ("ExpatParser");
	}

	CEGUI::System::setDefaultXMLParserName(xmlParser.c_str());

	str = cfg.getSetting(("LogFile"), ("GUI"));
	// 		if(!File::isDirExist("Log"))
	// 		{
	// 			File::makeDir("Log");
	// 		}

	if(str.empty())
	{
		str = ("CEGUI.log");
	}

	//LoadingListener::getSingleton().SetLoadingText("Creating GUI System...");

	//_guiSystem = OGRE_NEW CEGUI::System(_guiRender, 0, 0, 0, (""), str);
	_guiSystem = CEGUI::System::getSingletonPtr();

	// log level
	String ceLogLv = cfg.getSetting(("CELogLevel"), ("GUI"));

	CEGUI::LoggingLevel logLv = CEGUI::Informative;
	if(ceLogLv == ("Standard"))
	{
		logLv = CEGUI::Standard;
	}
	else if(ceLogLv == ("Insane"))
	{
		logLv = CEGUI::Insane;
	}	
	else if(ceLogLv == ("Errors"))
	{
		logLv = CEGUI::Errors;
	}

	CEGUI::Logger::getSingleton().setLoggingLevel(logLv);

	// mouse interval
	str = cfg.getSetting(("MouseClickInterval"), ("GUI"));
	double clickInterval = StringConverter::parseReal(str);
	if (clickInterval > CEGUI::System::DefaultSingleClickTimeout)
	{
		CEGUI::System::getSingleton().setSingleClickTimeout(clickInterval);
	}

	str = cfg.getSetting(("MouseDblClickInterval"), ("GUI"));
	double dblClickInterval = StringConverter::parseReal(str);
	if (dblClickInterval > CEGUI::System::DefaultMultiClickTimeout)
	{
		CEGUI::System::getSingleton().setMultiClickTimeout(dblClickInterval);
	}

	LoadingListener::getSingleton().SetLoadingText("Loading GUI shcemes...");

	// load scheme

	for (int i = 0; i < n; ++i)
	{
		String key = formatString(("Scheme[%d]"), i);
		str = cfg.getSetting(key, ("GUI"));
		assert(!str.empty());
		CEGUI::SchemeManager::getSingleton().create(str);
	}

	SetGUIRootWindow(CreateDefaultGUIRootWnd());

	//UIController::UIControllerManager::getSingleton().AddGUIController(
	//	OGRE_NEW UIController::CmdLineUI());

	CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(
		GameApp::getSingleton().GetRenderWindow()->getWidth() * 0.5f,
		GameApp::getSingleton().GetRenderWindow()->getHeight() * 0.5f));

	//String mouseStyle  = cfg.getSetting(("MouseCursorStyle"), ("GUI"));
	//String mouseImage  = cfg.getSetting(("MouseCursorImageName"), ("GUI"));
	//CEGUI::System::getSingleton().setDefaultMouseCursor(mouseStyle.c_str(), mouseImage.c_str());
	//CEGUI::MouseCursor::getSingleton().show();
	CEGUI::MouseCursor::getSingleton().hide();

	//Init Script Module
	InitScriptModule();
#ifdef _PRODUCT_               
	CEGUI::MouseCursor::getSingleton().hide();
#endif
}
//----------------------------------------------------------------------------------------------
void CEGUIGame::InitScriptModule()
{
	// create a script module.
	//		CEGUI::LuaScriptModule* scriptmod = OGRE_NEW CEGUI::LuaScriptModule();
	// tell CEGUI to use this scripting module
	// 		CEGUI::System::getSingleton().setScriptingModule(scriptmod);
	//Init FSMMO Game Script
	//tolua_FSMMO_open(scriptmod->getLuaState());
}
//----------------------------------------------------------------------------------------------
CEGUI::Window* CEGUIGame::CreateDefaultGUIRootWnd()
{
	return CEGUI::WindowManager::getSingleton().createWindow(("DefaultWindow"), ("RootWindow"));
}
//----------------------------------------------------------------------------------------------
void CEGUIGame::SetGUIRootWindow(CEGUI::Window* rootWnd)
{
	CEGUI::System::getSingleton().setGUISheet(rootWnd);
	//if(rootWnd)
	//{
	//	//CEGUI::OgreRenderer* pRender = dynamic_cast<CEGUI::OgreRenderer*>(_guiRender);
	//	//pRender->setTargetSceneManager(_visualScene);
	//	CEGUI::System::getSingleton().setGUISheet(rootWnd);
	//}
	//else
	//{
	//	//CEGUI::OgreRenderer* pRender = dynamic_cast<CEGUI::OgreRenderer*>(_guiRender);
	//	//pRender->setTargetSceneManager(0);
	//	CEGUI::System::getSingleton().setGUISheet(0);
	//}
}
//----------------------------------------------------------------------------------------------
void CEGUIGame::windowResized(RenderWindow* rw)
{
	GeneralGame::windowResized(rw);
	RenderWindow* pRenderWnd = GameApp::getSingleton().GetRenderWindow();
	CEGUI::Size sz;
	sz.d_width = (float)pRenderWnd->getWidth();
	sz.d_height = (float)pRenderWnd->getHeight();
	CEGUI::OgreRenderer* pRender = dynamic_cast<CEGUI::OgreRenderer*>(_guiRender);
	if(pRender)
	{
		pRender->setDisplaySize(sz);
	}
}
//----------------------------------------------------------------------------------------------
void CEGUIGame::windowFocusChange(RenderWindow* rw)
{
}
//----------------------------------------------------------------------------------------------
bool CEGUIGame::windowOnChar(uint32 uc)
{
	if(!CEGUI::System::getSingletonPtr())
		return false;

#ifdef _UNICODE
	return CEGUI::System::getSingleton().injectChar(uc);
#else
	bool bRet = false;

	static bool s_bHasHalf = false;
	static unsigned char ch0 = 0;
	unsigned char ch = unsigned char(uc);

	if(s_bHasHalf)
	{
		static unsigned char strBuffer[4];
		strBuffer[0] = ch0;
		strBuffer[1] = ch;

		static wchar_t wch[4];
		mbstowcs(wch, (const char*)strBuffer, 2);
		bRet = CEGUI::System::getSingleton().injectChar((CEGUI::utf32)wch[0]);

		s_bHasHalf = false;
	}
	else
	{
		if(ch >= 0x80)
		{
			s_bHasHalf = true;
			ch0 = ch;
		}
		else
		{
			bRet = CEGUI::System::getSingleton().injectChar(uc);
		}
	}

	return bRet;
#endif

}
//----------------------------------------------------------------------------------------------
void CEGUIGame::InitStates()
{

}

