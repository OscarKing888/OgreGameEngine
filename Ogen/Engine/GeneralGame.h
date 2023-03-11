#pragma once
#include "AbstractGame.h"

class _EngineExport GeneralGame : public AbstractGame
{
public:

	GeneralGame(void);
	virtual ~GeneralGame(void);

	virtual Camera* GetMainCamera();
	virtual void SetMainCamera(Camera *cam);

	virtual Camera* GetActiveCamera();
	virtual Viewport* GetActiveViewport();

protected:

	Camera*	_mainCamera;
	Viewport*	_mainViewport;

	virtual void InitImp();

	virtual void CreateCamera();
	virtual void CreateViewPort();
	virtual void ShutDown();

	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual void windowResized(RenderWindow* rw);
};

