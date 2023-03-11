#include "stdafx.h"
#include "Cmd_TerrainHeight.h"

Cmd_TerrainHeight::Cmd_TerrainHeight(Terrain* terrain,
	const TerrainHFMap& newData)
: _terrain(terrain)
, _newData(newData)
{
}

Cmd_TerrainHeight::~Cmd_TerrainHeight()
{

}

ICommandXPtr Cmd_TerrainHeight::Execute()
{
	TerrainHFMap oldData;

	TerrainHFMap::iterator x = _newData.begin();
	for (; x != _newData.end(); ++x)
	{
		long tx = x->first;

		FloatMap::iterator y = x->second.begin();
		for (; y != x->second.end(); ++y)
		{
			long ty = y->first;
			oldData[tx][ty] = _terrain->getHeightAtPoint(tx, ty);
			_terrain->setHeightAtPoint(tx, ty, y->second);
		}
	}

	_terrain->update();

	return new Cmd_TerrainHeight(_terrain, oldData);
}