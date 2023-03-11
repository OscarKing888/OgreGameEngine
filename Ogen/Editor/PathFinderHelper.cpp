#include "stdafx.h"
#include "PathFinderHelper.h"
#include "EditorGameApp.h"

//------------------------------------------------------------------------
// GridUintRenderable
//------------------------------------------------------------------------
GridUintRenderable::GridUintRenderable(float unitScale)
: _x(0)
, _y(0)
, mDirty(true)
, _unitScale(unitScale)
{
	initialize(RenderOperation::OT_TRIANGLE_LIST, false);
	setMaterial("FathFinder/Normal");
	setCastShadows(false);
	setQueryFlags(0);
}

GridUintRenderable::~GridUintRenderable()
{

}

void GridUintRenderable::setPosition(int x, int y)
{
	_x = x;
	_y = y;
	mDirty = true;
}

void GridUintRenderable::createVertexDeclaration()
{
	VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
	decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);
}

void GridUintRenderable::fillHardwareBuffers()
{
	int sz = 6;

	prepareHardwareBuffers(sz, 0);

	if (!sz)
	{ 
		mBox.setExtents(Vector3::ZERO,Vector3::ZERO);
		return;
	}

	HardwareVertexBufferSharedPtr vbuf =
		mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);

	float d = _unitScale;
	float x = (float)_x * d;
	float z = (float)_y * d;
	float y = 5.0f;

	/*
	*	*---* z + d
	*	|  /|
	*	| /	|
	*	*---*  z
	*	x   x + d
	*/

	float vertex_pos[18] =
	{
		x,		y,		z,
		x,		y,		z + d,
		x + d,	y,		z + d, //
		x,		y,		z,
		x + d,	y,		z + d,
		x + d,	y,		z
	};

	EditorGame& game = EditorGame::getSingleton();

	Real *prPos = static_cast<Real*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
	{
		for(int i = 0; i < sz; i++)
		{
			float pos_x =  vertex_pos[i * 3 + 0];
			float pos_y =  vertex_pos[i * 3 + 1];
			float pos_z =  vertex_pos[i * 3 + 2];
			
			pos_y += game.GetTerrainHeight(pos_x, pos_z);

			*prPos++ = pos_x;
			*prPos++ = pos_y;
			*prPos++ = pos_z;
		}
	}
	vbuf->unlock();

	mBox.setExtents(Vector3(x, y, z), Vector3(x + d, y + 1.0f, z + d));
	mDirty = false;
}

void GridUintRenderable::update()
{
	if (mDirty)
	{
		fillHardwareBuffers();
	}
}

bool GridUintRenderable::preRender(SceneManager* sm, RenderSystem* rsys)
{
	update();
	return true;
}
