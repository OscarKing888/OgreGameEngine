// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-03-31
// *************************************************************************************
#include "stdafx.h"
#include "LoadingListener.h"



template<> LoadingListener* Singleton<LoadingListener>::ms_Singleton = 0;

LoadingListener::LoadingListener(void)
{
}

LoadingListener::~LoadingListener(void)
{
}

void LoadingListener::Start(RenderWindow* window,
							SceneManager* sm,
				   unsigned short numGroupsInit, 
				   unsigned short numGroupsLoad, 
				   float initProportion)
{
	_window = window;
	_numGroupsInit = numGroupsInit;
	_numGroupsLoad = numGroupsLoad;
	_initProportion = initProportion;
	_sceneMgr = sm;

	ShowUI(true);

	SetLoadingProgress(0);
	// self is listener
	ResourceGroupManager::getSingleton().addResourceGroupListener(this);

	if(_sceneMgr)
	{
		_sceneMgr->clearSpecialCaseRenderQueues();
		_sceneMgr->addSpecialCaseRenderQueue(RENDER_QUEUE_OVERLAY);
		_sceneMgr->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_INCLUDE);
	}

	Update();
}

void LoadingListener::Finish(void)
{
	// hide loading screen
	ShowUI(false);

	// Back to full rendering
	if(_sceneMgr)
	{
		_sceneMgr->clearSpecialCaseRenderQueues();
		_sceneMgr->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_EXCLUDE);
	}

	// Unregister listener
	ResourceGroupManager::getSingleton().removeResourceGroupListener(this);

}


void LoadingListener::resourceGroupScriptingStarted(const String& groupName, size_t scriptCount)
{
	assert(_numGroupsInit > 0 && "You stated you were not going to init "
		"any groups, but you did! Divide by zero would follow...");

	// Lets assume script loading is 70%
	_progressBarInc = _progressBarMaxSize * _initProportion / (Real)scriptCount;
	_progressBarInc /= _numGroupsInit;

	SetLoadingText("Parsing scripts...");

	Update();
}

void LoadingListener::scriptParseStarted(const String& scriptName, bool& skipThisScript)
{
	SetLoadingText(scriptName);
}

void LoadingListener::scriptParseEnded(const String& scriptName, bool skipped)
{
	AdvanceProgress(_progressBarInc);
	Update();
}

void LoadingListener::resourceGroupScriptingEnded(const String& groupName)
{
}

void LoadingListener::resourceGroupLoadStarted(const String& groupName, size_t resourceCount)
{
	assert(_numGroupsLoad > 0 && "You stated you were not going to load "
		"any groups, but you did! Divide by zero would follow...");

	_progressBarInc = _progressBarMaxSize * (1 - _initProportion) / (float)resourceCount;
	_progressBarInc /= _numGroupsLoad;
	SetLoadingText("Loading resources...");
}

void LoadingListener::resourceLoadStarted(const ResourcePtr& resource)
{
	SetLoadingText(resource->getName());
}

void LoadingListener::resourceLoadEnded(void)
{

}

void LoadingListener::worldGeometryStageStarted(const String& description)
{
	SetLoadingText(description);
}

void LoadingListener::worldGeometryStageEnded(void)
{

}

void LoadingListener::resourceGroupLoadEnded(const String& groupName)
{
	AdvanceProgress(_progressBarInc);
	Update();
}

void LoadingListener::Update()
{
	_window->update();
}

//-------------------------------------------------------------------------------------------------

DefaultLoadingListener::DefaultLoadingListener()
: _loadOverlay(0)
, _loadingBarElement(0)
, _loadingDescriptionElement(0)
, _loadingCommentElement(0)
{

}

DefaultLoadingListener::~DefaultLoadingListener()
{

}

void DefaultLoadingListener::SetLoadingText(const String& txt)
{
	try
	{
		//_loadingCommentElement->setCaption(txt.c_str());
	}	
	catch (...)
	{
	}

	Update();
}

void DefaultLoadingListener::SetLoadingProgress(int progressPercent)
{
	_loadingBarElement->setWidth(
		_progressBarMaxSize * float(progressPercent) / 100.0f);
}

void DefaultLoadingListener::AdvanceProgress(int progressDeltaPercent)
{
	float fWidth = _loadingBarElement->getWidth() + progressDeltaPercent;
	if (fWidth > _progressBarMaxSize)
	{
		fWidth = _progressBarMaxSize;
	}
	_loadingBarElement->setWidth(fWidth);
}

void DefaultLoadingListener::Init()
{
	// We need to pre-initialise the 'Bootstrap' group so we can use
	// the basic contents in the loading screen
	ResourceGroupManager::getSingleton().initialiseResourceGroup("Bootstrap");

	OverlayManager& omgr = OverlayManager::getSingleton();

	_loadOverlay = (Overlay*)omgr.getByName("Core/LoadOverlay");

	if (!_loadOverlay)
	{
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
			"Cannot find loading overlay", "ExampleLoadingBar::start");
	}

	// Save links to the bar and to the loading text, for updates as we go
	_loadingBarElement = omgr.getOverlayElement("Core/LoadPanel/Bar/Progress");
	_loadingCommentElement = omgr.getOverlayElement("Core/LoadPanel/Comment");
	_loadingDescriptionElement = omgr.getOverlayElement("Core/LoadPanel/Description");
	OverlayElement* barContainer = omgr.getOverlayElement("Core/LoadPanel/Bar");
	_progressBarMaxSize = barContainer->getWidth();
}


void DefaultLoadingListener::ShowUI(bool show)
{
	if(show)
	{
		_loadOverlay->show();
	}
	else
	{
		_loadOverlay->hide();
	}
}
