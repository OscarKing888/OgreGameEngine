// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-03-31
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"



	class _EngineExport LoadingListener
		: public ResourceGroupListener
		, public Singleton<LoadingListener>
	{
	public:

		LoadingListener(void);
		virtual ~LoadingListener(void);

		virtual void Init(){}

		virtual void LoadingListener::Start(RenderWindow* window,
			SceneManager* sm = 0,
			unsigned short numGroupsInit = 1, 
			unsigned short numGroupsLoad = 1, 
			float initProportion = 0.70f);

		virtual void Finish(void);

		virtual void resourceGroupScriptingStarted(const String& groupName, size_t scriptCount);
		virtual void scriptParseStarted(const String& scriptName, bool& skipThisScript);
		virtual void scriptParseEnded(const String& scriptName, bool skipped);
		virtual void resourceGroupScriptingEnded(const String& groupName);
		virtual void resourceGroupLoadStarted(const String& groupName, size_t resourceCount);
		virtual void resourceLoadStarted(const ResourcePtr& resource);
		virtual void resourceLoadEnded(void);
		virtual void worldGeometryStageStarted(const String& description);
		virtual void worldGeometryStageEnded(void);
		virtual void resourceGroupLoadEnded(const String& groupName);


		/**	显示字符信息 */
		virtual void SetLoadingText(const String& txt){}

		/**	设置加载进度，范围[0-100]，百分比 */
		virtual void SetLoadingProgress(int progressPercent){}

		/**	进度增加，百分比 */
		virtual void AdvanceProgress(int progressDeltaPercent){}

	protected:

		SceneManager* _sceneMgr;
		RenderWindow* _window;

		float _initProportion;
		unsigned short _numGroupsInit;
		unsigned short _numGroupsLoad;
		float _progressBarMaxSize;
		float _progressBarScriptSize;
		float _progressBarInc;

		virtual void ShowUI(bool show){}
		virtual void Update();
	};

	class _EngineExport DefaultLoadingListener : public LoadingListener
	{
	public:

		DefaultLoadingListener();
		virtual ~DefaultLoadingListener();

		virtual void Init();

		/**	显示字符信息 */
		virtual void SetLoadingText(const String& txt);

		/**	设置加载进度，范围[0-100]，百分比 */
		virtual void SetLoadingProgress(int progressPercent);

		/**	进度增加，百分比 */
		virtual void AdvanceProgress(int progressDeltaPercent);

	protected:

		Overlay* _loadOverlay;
		OverlayElement* _loadingBarElement;
		OverlayElement* _loadingDescriptionElement;
		OverlayElement* _loadingCommentElement;

		virtual void ShowUI(bool show);
	};
