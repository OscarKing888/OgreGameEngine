#include "stdafx.h"
#include "GeneralGame.h"
#include "GameApp.h"
#include "LoadingListener.h"
#include "Events.h"

GeneralGame::GeneralGame(void)
{
	_mainCamera = 0;
	_mainViewport = 0;
}
//----------------------------------------------------------------------------------------------
GeneralGame::~GeneralGame(void)
{
}
//----------------------------------------------------------------------------------------------
void GeneralGame::InitImp()
{
	AbstractGame::InitImp();

	LoadingListener::getSingleton().SetLoadingText("Init camera & viewport...");
	CreateCamera();
	CreateViewPort();
}
//----------------------------------------------------------------------------------------------
void GeneralGame::CreateCamera()
{		
	_mainCamera = _visualScene->createCamera(("MainCamera"));
	InitCamera(_mainCamera);
}
//----------------------------------------------------------------------------------------------
Camera* GeneralGame::GetMainCamera()
{
	return _mainCamera;
}
//----------------------------------------------------------------------------------------------
void GeneralGame::SetMainCamera(Camera* cam)
{
	_mainCamera = cam;
	Viewport* vp = GameApp::getSingleton().GetRenderWindow()->getViewport(0);
	vp->setCamera(_mainCamera);
	_mainCamera->setAutoAspectRatio(true);

	EventsManager::getSingleton().CreateEvent("SetMainCamera", Any(_mainCamera));
}
//----------------------------------------------------------------------------------------------
void GeneralGame::CreateViewPort()
{
	_mainViewport = GameApp::getSingleton().GetRenderWindow()->addViewport(_mainCamera);
	_mainViewport->setBackgroundColour(ColourValue(127.0f / 255.0f, 127.0f / 255.0f, 127.0f / 255.0f));
}
//----------------------------------------------------------------------------------------------
bool GeneralGame::keyPressed( const OIS::KeyEvent &arg )
{
	/*if(AbstractGame::keyPressed(arg))
	{
	return true;
	}		*/

#ifndef _PRODUCT_
	if(arg.key == OIS::KC_F10)
	{
		static int mode = 0;
		mode = (mode + 1) % 3;
		PolygonMode pm = PM_SOLID;
		switch(mode)
		{
		case 0 :
			{
				pm = PM_SOLID;
				break;
			}

		case 1 :
			{
				pm = PM_WIREFRAME;
				break;
			}

		case 2 :
			{
				pm = PM_POINTS;	
				break;
			}
		} // switch;

		SetPolygonMode(pm);

		return true;
	} 
#endif
	return false;
}	
//----------------------------------------------------------------------------------------------
void GeneralGame::ShutDown()
{
	if(_visualScene)
	{
		_visualScene->destroyCamera(_mainCamera);
	}	

	AbstractGame::ShutDown();
}
//----------------------------------------------------------------------------------------------
void GeneralGame::windowResized(RenderWindow* rw)
{
	//AbstractGame::windowResized(rw);

	float r = (float)rw->getWidth() / (float)rw->getHeight();

	if(_mainCamera)
	{
		_mainCamera->setAspectRatio(r);
	}
}
//----------------------------------------------------------------------------------------------
Camera* GeneralGame::GetActiveCamera()
{
	return _mainCamera;
}
//----------------------------------------------------------------------------------------------
Viewport* GeneralGame::GetActiveViewport()
{
	return _mainViewport;
}	
