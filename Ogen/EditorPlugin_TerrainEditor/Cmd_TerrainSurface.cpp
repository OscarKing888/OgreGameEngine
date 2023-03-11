#include "stdafx.h"
#include "Cmd_TerrainSurface.h"

//--------------------------------------------------------------------------------
// Cmd_AddSurfaceTexture
//--------------------------------------------------------------------------------
Cmd_AddSurfaceTexture::Cmd_AddSurfaceTexture(Terrain* terrain, const StringVector& texName)
: _terrain(terrain)
, _textureName(texName)
{
}
//--------------------------------------------------------------------------------
Cmd_AddSurfaceTexture::~Cmd_AddSurfaceTexture()
{

}
//--------------------------------------------------------------------------------
ICommandXPtr Cmd_AddSurfaceTexture::Execute()
{
	_terrain->addLayer(0.0f, &_textureName);

	_terrain->update();

	return new Cmd_RemoveSurfaceTexture(_terrain, _textureName
		, _terrain->getLayerCount() - 1);
}
//--------------------------------------------------------------------------------
// Cmd_RemoveSurfaceTexture
//--------------------------------------------------------------------------------
Cmd_RemoveSurfaceTexture::Cmd_RemoveSurfaceTexture(Terrain* terrain,
												   const StringVector& texName, uint8 idx)
: _terrain(terrain)
, _textureName(texName)
, _layerIdx(idx)
{
}
//--------------------------------------------------------------------------------
Cmd_RemoveSurfaceTexture::~Cmd_RemoveSurfaceTexture()
{

}
//--------------------------------------------------------------------------------
ICommandXPtr Cmd_RemoveSurfaceTexture::Execute()
{
	_terrain->removeLayer(_layerIdx);
	_terrain->update();

	return new Cmd_AddSurfaceTexture(_terrain, _textureName);
}
//--------------------------------------------------------------------------------
// Cmd_PaintLayer
//--------------------------------------------------------------------------------
Cmd_PaintLayer::Cmd_PaintLayer(TerrainLayerBlendMap* layer,
const BlendValueMap& newData)
 : _layer(layer)
 , _newData(newData)
{
}

Cmd_PaintLayer::~Cmd_PaintLayer()
{

}

ICommandXPtr Cmd_PaintLayer::Execute()
{

	BlendValueMap oldDat;
	BlendValueMap::iterator x = _newData.begin();
	for (; x != _newData.end(); ++x)
	{
		size_t tx = x->first;

		BlendValue::iterator y = x->second.begin();
		for (; y != x->second.end(); ++y)
		{
			size_t ty = y->first;

			float oldVal = _layer->getBlendValue(tx, ty);
			oldDat[tx][ty] = oldVal;
			_layer->setBlendValue(tx, ty, y->second);
		}
	}


	_layer->update();

	return new Cmd_PaintLayer(_layer, oldDat);
}