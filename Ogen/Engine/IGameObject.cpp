#include "stdafx.h"
#include "IGameObject.h"

String ITile::ConvertToTileFileName(long x, long z)
{
	return formatString("%d_%d.tile", x, z);
}
//--------------------------------------------------------------------------------
bool ITile::ConvertFromTileFileName(const String& fileName, long& x, long& z)
{
	int r = sscanf(fileName.c_str(), "%d_%d.tile", &x, &z);
	return (r == 2);
}