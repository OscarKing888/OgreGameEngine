// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-10-25
// *************************************************************************************

#include "stdafx.h"
#include "WireBox.h"

WireBox::WireBox()
: mRadius(0.0f)
{
	mRenderOp.vertexData = OGRE_NEW VertexData();

	mRenderOp.indexData = 0;
	mRenderOp.vertexData->vertexCount = 24; 
	mRenderOp.vertexData->vertexStart = 0; 
	mRenderOp.operationType = RenderOperation::OT_LINE_LIST; 
	mRenderOp.useIndexes = false; 

	VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
	VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

	decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);

	HardwareVertexBufferSharedPtr vbuf = 
		HardwareBufferManager::getSingleton().createVertexBuffer(
		decl->getVertexSize(0),
		mRenderOp.vertexData->vertexCount,
		HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	// Bind buffer
	bind->setBinding(0, vbuf);

	// set basic white material
	this->setMaterial("BaseWhiteNoLighting");
}

WireBox::~WireBox() 
{
	OGRE_DELETE mRenderOp.vertexData;
}

void WireBox::SetSize(const Vector3& sz)
{
	_size = sz;
	setupBoundingBox(GetBoundingBox());
}

const Vector3& WireBox::GetSize() const
{
	return _size;
}

void WireBox::setupBoundingBox(const AxisAlignedBox& aabb) 
{
	// init the vertices to the aabb
	setupBoundingBoxVertices(aabb);

	// setup the bounding box of this SimpleRenderable
	setBoundingBox(aabb);
}

//-----------------------------------------------------------------------
void WireBox::setupBoundingBoxVertices(const AxisAlignedBox& /*aab*/)
{
	Vector3 half = _size * 0.5f;
	Vector3 vmax(half.x, half.y, half.z);
	Vector3 vmin(-half.x, -half.y, -half.z);

	Real sqLen = std::max(vmax.squaredLength(), vmin.squaredLength());
	mRadius = Math::Sqrt(sqLen);

	Real maxx = vmax.x;
	Real maxy = vmax.y;
	Real maxz = vmax.z;

	Real minx = vmin.x;
	Real miny = vmin.y;
	Real minz = vmin.z;

	// fill in the Vertex buffer: 12 lines with 2 endpoints each make up a box
	HardwareVertexBufferSharedPtr vbuf =
		mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);     

	float* pPos = static_cast<float*>(
		vbuf->lock(HardwareBuffer::HBL_DISCARD));

	// line 0
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = minz;
	// line 1
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = maxz;
	// line 2
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = minz;
	// line 3
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = minz;
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	// line 4
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = minz;
	// line 5
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = maxz;
	// line 6
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = minz;
	// line 7
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	// line 8
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = maxz;
	// line 9
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	// line 10
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = maxz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	// line 11
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = maxz;
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = maxz;
	vbuf->unlock();
}

//-----------------------------------------------------------------------
Real WireBox::getSquaredViewDepth(const Camera* cam) const
{
	Vector3 min, max, mid, dist;
	min = mBox.getMinimum();
	max = mBox.getMaximum();
	mid = ((max - min) * 0.5) + min;
	dist = cam->getDerivedPosition() - mid;


	return dist.squaredLength();
}

AxisAlignedBox WireBox::GetBoundingBox() const
{
	AxisAlignedBox aabb;
	Vector3 parentPos = Vector3::ZERO;
	if(this->getParentNode())
	{
		parentPos = getParentNode()->_getDerivedPosition();
	}

	aabb.setMinimum(parentPos.x - _size.x * 0.5f,
		parentPos.y - _size.y * 0.5f,
		parentPos.z - _size.z * 0.5f);

	aabb.setMaximum(parentPos.x + _size.x * 0.5f,
		parentPos.y + _size.y * 0.5f,
		parentPos.z + _size.z * 0.5f);

	return aabb;		
}

void WireBox::getWorldTransforms( Matrix4* xform ) const
{
#if 0
	return SimpleRenderable::getWorldTransforms(xform);
#else
	// 只需要其位置变化，SCALE/ORIENTATION不要
	Matrix4 nodeTransform;
	nodeTransform.makeTransform(
		mParentNode->_getDerivedPosition(),
		//Vector3::ZERO,
		Vector3(1, 1, 1),
		Quaternion::IDENTITY);
	*xform = m_matWorldTransform * nodeTransform;
#endif
}

