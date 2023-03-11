#include "stdafx.h"
#include "EditHeightState_DynamicElevate.h"
#include "TerrainBrush.h"
#include "Cmd_TerrainHeight.h"

//------------------------------------------------------------------------
// EditHeightState_DynamicElevate
//------------------------------------------------------------------------
EditHeightState_DynamicElevate::EditHeightState_DynamicElevate(IStateManager* pMgr)
: AbstractEditHeightState(pMgr)
{
	_stateID = "DynamicElevate";
	_commandID = (int)EHET_DynamicElevate;
}
//--------------------------------------------------------------------------------
EditHeightState_DynamicElevate::~EditHeightState_DynamicElevate()
{

}
//--------------------------------------------------------------------------------
BOOL EditHeightState_DynamicElevate::OnLButtonDown(UINT nFlags, CPoint point)
{
	// This must call at first
	AbstractEditHeightState::OnLButtonDown(nFlags, point);

	EditState_TerrainHeight& ts = EditState_TerrainHeight::getSingleton();

	TerrainGroup::RayResult r =
		EditorApp::getSingleton().PickOnTerrain(point.x, point.y);
	if(r.hit)
	{
		EditorConfig& cfg = EditorConfig::getSingleton();
		if(cfg.GetSnapToGrid())
		{
			const Vector2& gridSz = cfg.GetSnapGridSize();
			float sx = r.position.x > 0 ? 0.5f : -0.5f;
			float sz = r.position.z > 0 ? 0.5f : -0.5f;

			r.position.x = ((int)(r.position.x) / (int)gridSz.x) * (int)gridSz.x + gridSz.x * sx;
			r.position.z = ((int)(r.position.z) / (int)gridSz.y) * (int)gridSz.y + gridSz.y * sz;
		}

		ts.SetBrushDownPosition(r.position);
		TerrainGroup::TerrainList terrainList =
			ts.GetBrushAffectedTiles(r.position);

		ITerrainBrush* brush = ts.GetSelectedBrush();
		float brushRadius = ts.GetBrushRadius();

		bool useSubtract = !ts.IsElevUp();

		for (TerrainGroup::TerrainList::iterator ti = terrainList.begin();
			ti != terrainList.end(); ++ti)
		{
			Terrain* terrain = *ti;
			PaintHeight(terrain, r.position, brushRadius,
				ts.GetStrength(), brush, useSubtract);

		}
	}

	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL EditHeightState_DynamicElevate::OnMouseMove(UINT nFlags, CPoint point)
{
	EditState_TerrainHeight& ts = EditState_TerrainHeight::getSingleton();

	if(_isPainting)
	{
		Vector3 mouseDownPos = ts.GetBrushDownPosition();
		// 如果按下Shift键，固定以按下的点拉伸
		if(!MFCInputManager::getSingleton().IsKeyDown(VK_SHIFT))
		{
			TerrainGroup::RayResult r =
				EditorApp::getSingleton().PickOnTerrain(point.x, point.y);
			if(r.hit)
			{
				mouseDownPos = r.position;
			}
		}

		EditorConfig& cfg = EditorConfig::getSingleton();
		if(cfg.GetSnapToGrid())
		{
			const Vector2& gridSz = cfg.GetSnapGridSize();

			float sx = mouseDownPos.x > 0 ? 0.5f : -0.5f;
			float sz = mouseDownPos.z > 0 ? 0.5f : -0.5f;

			mouseDownPos.x = ((int)mouseDownPos.x / (int)gridSz.x) * (int)gridSz.x + gridSz.x * sx;
			mouseDownPos.z = ((int)mouseDownPos.z / (int)gridSz.y) * (int)gridSz.y + gridSz.y * sz;
		}

		TerrainGroup::TerrainList terrainList =
			ts.GetBrushAffectedTiles(mouseDownPos);

		ITerrainBrush* brush = ts.GetSelectedBrush();
		float brushRadius = ts.GetBrushRadius();

		bool useSubtract = !ts.IsElevUp();

		for (TerrainGroup::TerrainList::iterator ti = terrainList.begin();
			ti != terrainList.end(); ++ti)
		{
			Terrain* terrain = *ti;
			PaintHeight(terrain, mouseDownPos, brushRadius,
				ts.GetStrength(), brush, useSubtract);
		}
	}
	else
	{
		ts.UpdateBrush(point);
	}

	return AbstractEditHeightState::OnMouseMove(nFlags, point);
}
//--------------------------------------------------------------------------------
void EditHeightState_DynamicElevate::PaintHeight( 
	Terrain* terrain, const Vector3& position, float brushRadius,
	float strength, ITerrainBrush* brush, bool useSubtract )
{
	Vector3 tsPosMin, tsPosMax;
	terrain->getTerrainPosition(position +
		Vector3(-brushRadius, 0, brushRadius), &tsPosMin);

	terrain->getTerrainPosition(position +
		Vector3(brushRadius, 0, -brushRadius), &tsPosMax);

	// Here returns the relative size of terrain coord

	Real terrainSize = terrain->getSize();
	long startx = tsPosMin.x * terrainSize;
	long starty = tsPosMin.y * terrainSize;
	long endx = tsPosMax.x * terrainSize;
	long endy = tsPosMax.y * terrainSize;

	float dwx = endx - startx;
	float dwy = endy - starty;

	long realStartX = startx;
	long realStartY = starty;

	startx = std::max(startx, 0L);
	starty = std::max(starty, 0L);
	endx = std::min(endx, (long)terrainSize);
	endy = std::min(endy, (long)terrainSize);


	for (long y = starty; y < endy; ++y)
	{
		Real dy = (float)(y - realStartY);
		for (long x = startx; x < endx; ++x)
		{
			Real dx = (float)(x - realStartX);

			float weight = brush->GetWeight(dx, dy, dwx, dwy);

			float addedHeight = weight * strength;
			float newheight = 0.0f;
			float oldHeight = terrain->getHeightAtPoint(x, y);

			AddOldHF(terrain, x, y, oldHeight);

			if (useSubtract)
			{
				newheight = oldHeight - addedHeight;
			}
			else
			{
				newheight = oldHeight + addedHeight;
			}

			terrain->setHeightAtPoint(x, y, newheight);
		}
	}
}
