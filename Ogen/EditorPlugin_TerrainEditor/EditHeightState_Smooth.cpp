#include "stdafx.h"
#include "EditHeightState_Smooth.h"
#include "TerrainBrush.h"
#include "Cmd_TerrainHeight.h"

//------------------------------------------------------------------------
// EditHeightState_Smooth
//------------------------------------------------------------------------
EditHeightState_Smooth::EditHeightState_Smooth(IStateManager* pMgr)
: AbstractEditHeightState(pMgr)
{
	_stateID = "Smooth";
	_commandID = (int)EHET_Smooth;
}
//--------------------------------------------------------------------------------
EditHeightState_Smooth::~EditHeightState_Smooth()
{

}
//--------------------------------------------------------------------------------
BOOL EditHeightState_Smooth::OnMouseMove(UINT nFlags, CPoint point)
{
	EditState_TerrainHeight& ts = EditState_TerrainHeight::getSingleton();

	if(_isPainting)
	{
		TerrainGroup::RayResult r =
			EditorApp::getSingleton().PickOnTerrain(point.x, point.y);
		if(r.hit)
		{
			TerrainGroup::TerrainList terrainList =
				ts.GetBrushAffectedTiles(r.position);

			float brushRadius = ts.GetBrushRadius();

			for (TerrainGroup::TerrainList::iterator ti = terrainList.begin();
				ti != terrainList.end(); ++ti)
			{
				Terrain* terrain = *ti;
				Vector3 tsPosMin, tsPosMax;
				terrain->getTerrainPosition(r.position
					+ Vector3(-brushRadius, 0, brushRadius), &tsPosMin);

				terrain->getTerrainPosition(r.position
					+ Vector3(brushRadius, 0, -brushRadius), &tsPosMax);

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

				for (long y = starty; y < endy; ++y)
				{
					Real dy = (float)(y - starty);
					for (long x = startx; x < endx; ++x)
					{
						Real dx = (float)(x - startx);
						float oldHeight = terrain->getHeightAtPoint(x, y);
						
						AddOldHF(terrain, x, y, oldHeight);

						float sum = oldHeight * 2;
						float num = 2.0f;
						
						if((x != startx) && (x != endx - 1))
						{
							sum += terrain->getHeightAtPoint(x - 1, y);
							sum += terrain->getHeightAtPoint(x + 1, y);
							sum += oldHeight;
							num += 3.0f;
						}

						if((y != starty) && (y != endy - 1))
						{
							sum += terrain->getHeightAtPoint(x, y - 1);
							sum += terrain->getHeightAtPoint(x, y + 1);
							sum += oldHeight;
							num += 3.0f;
						}

						float newheight = sum / num;
						terrain->setHeightAtPoint(x, y, newheight);
					}
				}

			}
		}
	}
	else
	{
		ts.UpdateBrush(point);
	}

	return AbstractEditHeightState::OnMouseMove(nFlags, point);
}
//--------------------------------------------------------------------------------