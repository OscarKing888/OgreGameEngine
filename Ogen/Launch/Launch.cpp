// Launch.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Launch.h"
#include <Engine/GameApp.h>
#include <Engine/LoadingListener.h>
#include <objbase.h>

#define MAX_LOADSTRING 100

//#ifdef _DEBUG
//#	define GameDLL "Game_Debug.dll"
//#else
//#	define GameDLL "Game_Release.dll"
//#endif

#ifdef _DEBUG
#	define GameDLL "MechArena_Debug.dll"
#else
#	define GameDLL "MechArena_Release.dll"
#endif

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	setlocale(LC_CTYPE, "");

	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	DefaultLoadingListener loading;

	HMODULE h = ::LoadLibrary(GameDLL);

	typedef IGame* (*CreateGameFun)(void);
	CreateGameFun createGame = (CreateGameFun)GetProcAddress(h, "CreateGame");
	IGame* game = createGame();
	GameApp theApp(game, "MechArena/Main.cfg");

	HICON hicon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	theApp.SetHICON(hicon);

	theApp.Init();
	theApp.Run();


	typedef void (*DestroyGameFun)(IGame*);
	DestroyGameFun destroyGame = (DestroyGameFun)GetProcAddress(h, "DestroyGame");
	destroyGame(game);
	::FreeLibrary(h);
	h = 0;

	CoUninitialize();
	return 0;
}