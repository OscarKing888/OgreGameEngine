#pragma once
#include "CmdTypes.h"

class Cmd_TerrainHeight : public ICommandX
{
public:

	Cmd_TerrainHeight(Terrain* terrain, const TerrainHFMap& newData);

	virtual ~Cmd_TerrainHeight();
	virtual ICommandXPtr Execute();

protected:

	Terrain* _terrain;
	TerrainHFMap _newData;
};
