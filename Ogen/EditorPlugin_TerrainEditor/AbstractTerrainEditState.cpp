#include "stdafx.h"
#include "AbstractTerrainEditState.h"
#include "TerrainBrush.h"
#include "Property_TerrainBrush.h"
#include "EditorCore/Grid.h"

Grid* AbstractTerrainEditState::_grid = 0;

AbstractTerrainEditState::AbstractTerrainEditState( IStateManager* pMgr )
: AbstractEditState(pMgr)
, _brushRadius(200.0f)
, _brushRange(0)
, _brushNode(0)
, _selectedBrush(0)
, _brushAnchor(0)
, _showTileBoundingBox(true)
{
	_selectedBrush = TerrainBrushManager::getSingleton().GetBrush(0);
}
//--------------------------------------------------------------------------------
AbstractTerrainEditState::~AbstractTerrainEditState()
{
	DestroyTerrainBoundingBox();

	_brushNode->detachAllObjects();
	GetSceneManager()->destroyEntity(_brushAnchor);
	_brushNode->getParentSceneNode()->removeAndDestroyChild(_brushNode->getName());
	OGRE_DELETE _brushRange;
}
//--------------------------------------------------------------------------------
void AbstractTerrainEditState::BuildPropertyMap()
{
	AddProp(Property_Float, BrushRadius, AbstractTerrainEditState, "BrushRadius", "TerrainBrush");
	
	AddProp(Property_TerrainBrush, SelectedBrush, AbstractTerrainEditState, "SelectedBrush", "Brush");

	AddProp(Property_Bool, ShowTileBoundingBox, AbstractTerrainEditState, "ShowTileBoundingBox", "View");

}
//--------------------------------------------------------------------------------
void AbstractTerrainEditState::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(eventName == "PickOnTerrain")
	{
		Vector3 pos = any_cast<Vector3>(eventContex);
		UpdateBrush(pos);
	}
	else if(eventName == "ShowTerrainBounding")
	{
		bool on = any_cast<bool>(eventContex);
		SetShowTileBoundingBox(on);
	}
	else if(eventName == "RefreshBrush")
	{
		TerrainBrushManager::getSingleton().RefreshBrushList();
	}
}
//--------------------------------------------------------------------------------
void AbstractTerrainEditState::SetBrushRadius(float radius)
{
	_brushRadius = radius;
	UpdateBrush(_brushNode->getPosition());
	EventsManager::getSingleton().CreateEvent(PropertyEvents::PropertyChanged, Any(String("BrushRadius")));
}

//--------------------------------------------------------------------------------
void AbstractTerrainEditState::UpdateBrush(const Vector3& brushPosition)
{
	Vector3 pos = brushPosition;

	EditorConfig& cfg = EditorConfig::getSingleton();
	if(cfg.GetSnapToGrid())
	{
		const Vector2& gridSz = cfg.GetSnapGridSize();

		float sx = pos.x < 0 ? 0.5f : -0.5f;
		float sz = pos.z > 0 ? 0.5f : -0.5f;

		pos.x = ((int)(pos.x) / (int)gridSz.x) * (int)gridSz.x - gridSz.x * sx;
		pos.z = ((int)(pos.z) / (int)gridSz.y) * (int)gridSz.y + gridSz.y * sz;
	}

	//pos.y = 0.0f;
	_brushNode->setPosition(pos);

	const int C_Segments = 40;
	float C_Radius = _brushRadius;
	const float C_DeltaAng = Math::TWO_PI / (float)C_Segments;

	SceneManager* sm = GetSceneManager();
	IGame* game = GameApp::getSingleton().GetGame();
	Vector3List points;
	for (int i = 0; i < C_Segments; ++i)
	{
		float da = C_DeltaAng * (float)i;
		float x = C_Radius * cosf(da);
		float z = C_Radius * sinf(da);
		float y = game->GetTerrainHeight(x + brushPosition.x, z + brushPosition.z);
		points.push_back(Vector3(x, y - pos.y, z));
	}

	float x = C_Radius * cosf(0.0f);
	float z = C_Radius * sinf(0.0f);
	float y = game->GetTerrainHeight(x + brushPosition.x, z + brushPosition.z);
	points.push_back(Vector3(x, y - pos.y, z));
	_brushRange->setPoint(points);
}
//--------------------------------------------------------------------------------
void AbstractTerrainEditState::UpdateBrush( const CPoint& mousePoint )
{
	TerrainGroup::RayResult r =
		EditorApp::getSingleton().PickOnTerrain(mousePoint.x, mousePoint.y);
}
//--------------------------------------------------------------------------------
void AbstractTerrainEditState::OnActive()
{
	AbstractEditState::OnActive();

	_brushNode->setVisible(true);

	if(SceneEnvironment::getSingletonPtr())
	{
		_fogMode = SceneEnvironment::getSingleton().GetFogType();
		SceneEnvironment::getSingleton().SetFogType(FOG_NONE);
	}

	EventsManager::getSingleton().AddListener(this);

	ObjectPropertiesWnd::getSingleton().SetObject(this);
	ObjectPropertiesWnd::getSingleton().SetPropertyChangeUndo(false);

	SetShowTileBoundingBox(true);
}
//--------------------------------------------------------------------------------
void AbstractTerrainEditState::OnInActive()
{
	AbstractEditState::OnInActive();	
	_brushNode->setVisible(false);

	if(SceneEnvironment::getSingletonPtr())
	{
		SceneEnvironment::getSingleton().SetFogType(_fogMode);
	}

	EventsManager::getSingleton().RemoveListener(this);
	ObjectPropertiesWnd::getSingleton().SetPropertyChangeUndo(true);

	SetShowTileBoundingBox(false);
}
//--------------------------------------------------------------------------------
SceneManager* AbstractTerrainEditState::GetSceneManager()
{
	SceneManager* sm = EditorApp::getSingleton().GetGame()->GetVisualScene();	
	return sm;
}
//--------------------------------------------------------------------------------
TerrainGroup::TerrainList AbstractTerrainEditState::GetBrushAffectedTiles(const Vector3& pos)
{
	TerrainGroup* tg = GameApp::getSingleton().GetGame()->GetTerrainGroup();
	TerrainGroup::TerrainList terrainList;
	Real brushSizeWorldSpace =  _brushRadius;
	Sphere sphere(pos, brushSizeWorldSpace);
	tg->sphereIntersects(sphere, &terrainList);
	return terrainList;
}
//--------------------------------------------------------------------------------
BOOL AbstractTerrainEditState::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	AbstractEditState::OnKeyDown(nChar, nRepCnt, nFlags);

	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL AbstractTerrainEditState::OnMouseMove(UINT nFlags, CPoint point)
{	
	AbstractEditState::OnMouseMove(nFlags, point);

	return FALSE;
}
//------------------------------------------------------------------------
BOOL AbstractTerrainEditState::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{	
	if(MFCInputManager::getSingleton().IsKeyDown(VK_SPACE))
	{
		SetBrushRadius(Math::Clamp(
			_brushRadius + (float)zDelta, 20.0f, 512 * 512 * 0.7f));

		return TRUE;
	}

	return AbstractEditState::OnMouseWheel(nFlags, zDelta, pt);
}
//--------------------------------------------------------------------------------
void AbstractTerrainEditState::UpdateWholeTerrain()
{
	Ogre::TerrainGroup* t = GameApp::getSingleton().GetGame()->GetTerrainGroup();
	t->update();
}
//--------------------------------------------------------------------------------
void AbstractTerrainEditState::CreateBrushModel( String name )
{
	_brushNode = GetSceneManager()->getRootSceneNode()->createChildSceneNode(name + ".Node");

	_brushRange = OGRE_NEW DynamicLines(name + ".Circle");
	_brushRange->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);
	_brushRange->setMaterial("TerrainBrush");
	_brushRange->setLineColour(ColourValue(1, 1, 0));

	_brushNode->attachObject(_brushRange);

	_brushAnchor = GetSceneManager()->createEntity(name + ".Anchor", "EditorPickTag.mesh");
	_brushAnchor->setQueryFlags(0);	
	_brushAnchor->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);

	_brushNode->attachObject(_brushAnchor);
	//_brushNode->setScale(Vector3(_brushRadius));
	_brushNode->setVisible(false);
	UpdateBrush(Vector3::ZERO);
}
//--------------------------------------------------------------------------------
StringVector AbstractTerrainEditState::GetBrushList() const
{
	return TerrainBrushManager::getSingleton().GetBrushNameList();
}
//--------------------------------------------------------------------------------
void AbstractTerrainEditState::SetBrushList(const StringVector& lst)
{

}
//--------------------------------------------------------------------------------
void AbstractTerrainEditState::CreateTerrainBoundingBox()
{
	IGame* game = GameApp::getSingleton().GetGame();
	SceneManager* sm = game->GetVisualScene();
	SceneNode* root = sm->getRootSceneNode();
	long xCnt = game->GetTerrainXCount()/* / 2*/;
	long zCnt = game->GetTerrainZCount()/* / 2*/;
	long tileSize = game->GetTerrainWorldSize();
	long hfTileSz = tileSize / 2;

	TerrainGroup* tg = game->GetTerrainGroup();

	if(!_grid)
	{
		_grid = OGRE_NEW Grid();
	}

	_grid->Offset(Vector3(-hfTileSz, 0, -hfTileSz));
	_grid->CellSizeOnX(tileSize);
	_grid->CellSizeOnZ(tileSize);
	_grid->BigCellCntOnX(3);
	_grid->BigCellCntOnZ(3);
	_grid->XCellCnt(xCnt);
	_grid->ZCellCnt(zCnt);
}
//--------------------------------------------------------------------------------
void AbstractTerrainEditState::ShowTerrainBoundingBox( bool on )
{
	if(_grid)
	{
		_grid->SetVisible(on);
	}
}
//--------------------------------------------------------------------------------
void AbstractTerrainEditState::DestroyTerrainBoundingBox()
{
	if(_grid)
	{
		OGRE_DELETE _grid;
		_grid = 0;
	}
}

void AbstractTerrainEditState::SetShowTileBoundingBox( bool val )
{
	_showTileBoundingBox = val;
	ShowTerrainBoundingBox(val);
}