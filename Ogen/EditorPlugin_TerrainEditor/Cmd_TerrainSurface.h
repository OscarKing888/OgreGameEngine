#pragma once
#include "CmdTypes.h"

///
/// Add a surface texture to terrain
///
class Cmd_AddSurfaceTexture : public ICommandX
{
public:

	Cmd_AddSurfaceTexture(Terrain* terrain, const StringVector& texName);
	virtual ~Cmd_AddSurfaceTexture();
	virtual ICommandXPtr Execute();

protected:

	Terrain* _terrain;
	StringVector _textureName;
};

///
/// Remove a surface texture from terrain
///
class Cmd_RemoveSurfaceTexture : public ICommandX
{
public:

	Cmd_RemoveSurfaceTexture(Terrain* terrain, const StringVector& texName, uint8 idx);
	virtual ~Cmd_RemoveSurfaceTexture();
	virtual ICommandXPtr Execute();

protected:

	Terrain* _terrain;
	uint8 _layerIdx;
	StringVector _textureName;
};

///
/// Paint a layer with new values
///

class Cmd_PaintLayer : public ICommandX
{
public:

	Cmd_PaintLayer(TerrainLayerBlendMap* layer, const BlendValueMap& newData);

	virtual ~Cmd_PaintLayer();
	virtual ICommandXPtr Execute();

protected:

	TerrainLayerBlendMap* _layer;
	BlendValueMap _newData;
};

