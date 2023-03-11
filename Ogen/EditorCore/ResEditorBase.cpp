#include "stdafx.h"
#include "RenderWnd.h"
#include "ResEditorBase.h"
#include "Grid.h"

ResEditorBase::ResEditorBase()
: _axes(0)
, _showGrid(true)
, _renderWindow(0)
, _bgColour(EditorConfig::getSingleton().GetViewportColour())
, _showWire(false)
, _axesNode(0)
, _grid(0)
{

}

ResEditorBase::~ResEditorBase()
{

}


void ResEditorBase::BuildPropertyMap()
{
	AddProp(Property_Bool, ShowGrid, ResEditorBase, "ShowGrid", "View");
	AddProp(Property_Bool, ShowWireframe, ResEditorBase, "ShowWireframe", "View");
	AddProp(Property_ColourValue, ViewportColour, ResEditorBase, "ViewportColour", "View");
}

void ResEditorBase::Init(uint visibilityFlag, const String& name)
{
	_renderWindow = dynamic_cast<RenderWnd*>(_GetActiveView());// OGRE_NEW RenderWnd("ParticleEditor", this);
	//_renderWindow->OnInitialUpdate();
	_renderWindow->GetViewport()->setVisibilityMask(visibilityFlag);
	_renderWindow->GetViewport()->setSkiesEnabled(false);

	_renderWindow->SetCameraController(OGRE_NEW LookAtCamera());

	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();

	_axes = sm->createEntity(name + "Axes", "axes.mesh");
	_axes->setVisibilityFlags(visibilityFlag);

	_axesNode = sm->getRootSceneNode()->createChildSceneNode();
	_axesNode->attachObject(_axes);
	
	ShowAxes(false);

	_grid = OGRE_NEW Grid();

	BuildPropertyMap();
}

void ResEditorBase::ShowAxes(bool on)
{
	_axes->setVisible(on);
}

void ResEditorBase::Shutdown()
{
	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	if(_axes)
	{
		_axesNode->detachAllObjects();
		sm->getRootSceneNode()->removeChild(_axesNode);
		sm->destroySceneNode(_axesNode);
		_axesNode = 0;

		sm->destroyEntity(_axes);
	}

	_renderWindow->DestroyWindow();

	if(_grid)
	{
		OGRE_DELETE _grid;
	}
}

void ResEditorBase::SetShowGrid(bool on)
{
	_showGrid = on;
	if(_grid)
	{
		_grid->SetVisible(on);
	}
}

bool ResEditorBase::GetShowGrid() const
{
	return _showGrid;
}


void ResEditorBase::SetViewportColour(const ColourValue& clr)
{
	_bgColour = clr;
	_renderWindow->GetViewport()->setBackgroundColour(_bgColour);
}

const ColourValue& ResEditorBase::GetViewportColour() const
{
	return _bgColour;
}

void ResEditorBase::SetShowWireframe(bool on)
{
	_showWire = on;

	PolygonMode pm = PM_SOLID;
	if(_showWire)
	{
		pm = PM_WIREFRAME;
	}

	_renderWindow->GetCamera()->setPolygonMode(pm);
}

void ResEditorBase::CaptureThumbImage(const String& thumbFile)
{
	bool showAxes = IsAxesVisible();
	bool showGrid = GetShowGrid();

	ShowAxes(false);
	SetShowGrid(false);

	bool skyOn = false;
	if(SkyBox::getSingletonPtr())
	{
		skyOn = SkyBox::getSingleton().GetEnableSkyBox();
		SkyBox::getSingleton().SetEnableSkyBox(false);
	}

	RenderWindow* rw = _renderWindow->GetRenderWindow();
	int w = rw->getWidth();
	int h = rw->getHeight();

	rw->resize(128 + 4, 128 + 4);
	rw->windowMovedOrResized();
	rw->update();

	PixelFormat pf = rw->suggestPixelFormat();

	uchar *data = OGE_ALLOC_T(uchar, rw->getWidth() * rw->getHeight() * PixelUtil::getNumElemBytes(pf));
	PixelBox pb(rw->getWidth(), rw->getHeight(), 1, pf, data);

	rw->copyContentsToMemory(pb);

	Ogre::Image img;
	img.loadDynamicImage(data, rw->getWidth(),  rw->getHeight(), 1, pf, true, 1, 0);
	//img.resize(128, 128);
	img.save(thumbFile);

	//OGE_FREE(data);

	ShowAxes(true);

	if(SkyBox::getSingletonPtr())
	{
		SkyBox::getSingleton().SetEnableSkyBox(skyOn);
	}

	rw->resize(w, h);
	rw->windowMovedOrResized();
	rw->update();

	ShowAxes(showAxes);
	SetShowGrid(showGrid);
}

bool ResEditorBase::IsAxesVisible() const
{
	return _axes->isVisible();
}