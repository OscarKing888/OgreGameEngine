#include "stdafx.h"
#include "EditHeightState_FixedHeight.h"
#include "TerrainBrush.h"
#include "Cmd_TerrainHeight.h"

//------------------------------------------------------------------------
// EditHeightState_FixedHeight
//------------------------------------------------------------------------
EditHeightState_FixedHeight::EditHeightState_FixedHeight(IStateManager* pMgr)
: AbstractEditHeightState(pMgr)
{
	_stateID = "FixedHeight";
	_commandID = (int)EHET_FixedHeight;
}
//--------------------------------------------------------------------------------
EditHeightState_FixedHeight::~EditHeightState_FixedHeight()
{

}
//--------------------------------------------------------------------------------
BOOL EditHeightState_FixedHeight::OnLButtonDown(UINT nFlags, CPoint point)
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

		if(MFCInputManager::getSingleton().IsKeyDown(VK_CONTROL))
		{
			ts.SetFixedHeight(r.position.y);
		}
		else
		{
			ts.SetBrushDownPosition(r.position);
			TerrainGroup::TerrainList terrainList = ts.GetBrushAffectedTiles(r.position);

			ITerrainBrush* brush = ts.GetSelectedBrush();
			float brushRadius = ts.GetBrushRadius();
			float fixHeight = ts.GetFixedHeight();

			for (TerrainGroup::TerrainList::iterator ti = terrainList.begin();
				ti != terrainList.end(); ++ti)
			{
				PaintHeight(*ti, r.position, brushRadius, brush, fixHeight);
			}
		}
	}

	return AbstractEditHeightState::OnLButtonDown(nFlags, point);
}
//--------------------------------------------------------------------------------
BOOL EditHeightState_FixedHeight::OnMouseMove(UINT nFlags, CPoint point)
{
	EditState_TerrainHeight& ts = EditState_TerrainHeight::getSingleton();

	if(_isPainting)
	{
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

			TerrainGroup::TerrainList terrainList = ts.GetBrushAffectedTiles(r.position);

			ITerrainBrush* brush = ts.GetSelectedBrush();
			float brushRadius = ts.GetBrushRadius();
			float fixHeight = ts.GetFixedHeight();

			for (TerrainGroup::TerrainList::iterator ti = terrainList.begin();
				ti != terrainList.end(); ++ti)
			{
				PaintHeight(*ti, r.position, brushRadius, brush, fixHeight);				
			}
		}
	}
	else
	{
		ts.UpdateBrush(point);
	}


	return AbstractEditHeightState::OnMouseMove(nFlags, point);
}

void EditHeightState_FixedHeight::PaintHeight(
	Terrain* terrain, const Vector3& position, float brushRadius,
	ITerrainBrush* brush, float fixHeight)
{
	Vector3 tsPosMin, tsPosMax;
	terrain->getTerrainPosition(position + Vector3(-brushRadius, 0, brushRadius), &tsPosMin);
	terrain->getTerrainPosition(position + Vector3(brushRadius, 0, -brushRadius), &tsPosMax);

	// Here returns the relative size of terrain coord

	Real terrainSize = terrain->getSize();
	long startx = tsPosMin.x * terrainSize;
	long starty = tsPosMin.y * terrainSize;
	long endx = tsPosMax.x * terrainSize;
	long endy = tsPosMax.y * terrainSize;

	startx = std::max(startx, 0L);
	starty = std::max(starty, 0L);
	endx = std::min(endx, (long)terrainSize);
	endy = std::min(endy, (long)terrainSize);

	float dwx = endx - startx;
	float dwy = endy - starty;

	for (long y = starty; y < endy; ++y)
	{
		Real dy = (float)(y - starty);
		for (long x = startx; x < endx; ++x)
		{
			Real dx = (float)(x - startx);
			float weight = brush->GetWeight(dx, dy, dwx, dwy);
			float oldHeight = terrain->getHeightAtPoint(x, y);

			AddOldHF(terrain, x, y, oldHeight);

			float newheight = oldHeight + (fixHeight - oldHeight) * weight;
			terrain->setHeightAtPoint(x, y, newheight);
		}
	}
}
//--------------------------------------------------------------------------------