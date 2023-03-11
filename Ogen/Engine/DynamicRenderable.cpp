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
#include "DynamicRenderable.h"


DynamicRenderable::DynamicRenderable()
: mWireBoundingBox(0)
, mShowBoundingBox(false)
, mUseParentTransform(true)
{
}

DynamicRenderable::DynamicRenderable(const String& name)
: mWireBoundingBox(0)
, mShowBoundingBox(false)
, mUseParentTransform(true)
{
	mName = name;
}

DynamicRenderable::~DynamicRenderable()
{
	if(mWireBoundingBox)
	{
		OGRE_DELETE mWireBoundingBox;
	}

	OGRE_DELETE mRenderOp.vertexData;
	OGRE_DELETE mRenderOp.indexData;
}

void DynamicRenderable::initialize(RenderOperation::OperationType operationType,
								   bool useIndices)
{
	// Initialize render operation
	mRenderOp.operationType = operationType;
	mRenderOp.useIndexes = useIndices;
	mRenderOp.vertexData = OGRE_NEW VertexData;
	if (mRenderOp.useIndexes)
		mRenderOp.indexData = OGRE_NEW IndexData;

	// Reset buffer capacities
	mVertexBufferCapacity = 0;
	mIndexBufferCapacity = 0;

	// Create vertex declaration
	createVertexDeclaration();
}

void DynamicRenderable::prepareHardwareBuffers(size_t vertexCount, 
											   size_t indexCount)
{
	// Prepare vertex buffer
	size_t newVertCapacity = mVertexBufferCapacity;
	if ((vertexCount > mVertexBufferCapacity) ||
		(!mVertexBufferCapacity))
	{
		// vertexCount exceeds current capacity!
		// It is necessary to reallocate the buffer.

		// Check if this is the first call
		if (!newVertCapacity)
			newVertCapacity = 1;

		// Make capacity the next power of two
		while (newVertCapacity < vertexCount)
			newVertCapacity <<= 1;
	}
	else if (vertexCount < mVertexBufferCapacity>>1) {
		// Make capacity the previous power of two
		while (vertexCount < newVertCapacity>>1)
			newVertCapacity >>= 1;
	}
	if (newVertCapacity != mVertexBufferCapacity) 
	{
		mVertexBufferCapacity = newVertCapacity;

		size_t vertexStride = 0;
		size_t c = mRenderOp.vertexData->vertexDeclaration->getElementCount();
		for (size_t i = 0; i < c; ++i)
		{
			vertexStride += mRenderOp.vertexData->vertexDeclaration->getVertexSize(i);
		}

		// Create OGRE_NEW vertex buffer
		HardwareVertexBufferSharedPtr vbuf =
			HardwareBufferManager::getSingleton().createVertexBuffer(
			vertexStride,
			mVertexBufferCapacity,
			HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?

		// Bind buffer
		mRenderOp.vertexData->vertexBufferBinding->setBinding(0, vbuf);
	}
	// Update vertex count in the render operation
	mRenderOp.vertexData->vertexCount = vertexCount;

	if (mRenderOp.useIndexes)
	{
		OgreAssert(indexCount <= std::numeric_limits<unsigned short>::max(), "indexCount exceeds 16 bit");

		size_t newIndexCapacity = mIndexBufferCapacity;
		// Prepare index buffer
		if ((indexCount > newIndexCapacity) ||
			(!newIndexCapacity))
		{
			// indexCount exceeds current capacity!
			// It is necessary to reallocate the buffer.

			// Check if this is the first call
			if (!newIndexCapacity)
				newIndexCapacity = 1;

			// Make capacity the next power of two
			while (newIndexCapacity < indexCount)
				newIndexCapacity <<= 1;

		}
		else if (indexCount < newIndexCapacity>>1) 
		{
			// Make capacity the previous power of two
			while (indexCount < newIndexCapacity>>1)
				newIndexCapacity >>= 1;
		}

		if (newIndexCapacity != mIndexBufferCapacity)
		{
			mIndexBufferCapacity = newIndexCapacity;
			// Create OGRE_NEW index buffer
			mRenderOp.indexData->indexBuffer =
				HardwareBufferManager::getSingleton().createIndexBuffer(
				HardwareIndexBuffer::IT_16BIT,
				mIndexBufferCapacity,
				HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?
		}

		// Update index count in the render operation
		mRenderOp.indexData->indexCount = indexCount;
		mRenderOp.srcRenderable = this;
	}
}

Real DynamicRenderable::getBoundingRadius(void) const
{
	return Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
}

Real DynamicRenderable::getSquaredViewDepth(const Camera* cam) const
{
	Vector3 vMin, vMax, vMid, vDist;
	vMin = mBox.getMinimum();
	vMax = mBox.getMaximum();
	vMid = ((vMin - vMax) * 0.5) + vMin;
	vDist = cam->getDerivedPosition() - vMid;

	return vDist.squaredLength();
}

void DynamicRenderable::_addBoundingBoxToQueue(RenderQueue* queue)
{
	// Create a WireBoundingBox if needed.
	if (mWireBoundingBox == NULL)
	{
		mWireBoundingBox = OGRE_NEW WireBoundingBox();
	}

	if(mParentNode)
	{
		Vector3 worldPos = mParentNode->_getDerivedPosition();
		Vector3 worldScale = mParentNode->_getDerivedScale();
		Quaternion worldOrient = mParentNode->_getDerivedOrientation();
		
		Matrix4 mat;
		mat.makeTransform(worldPos, worldScale, worldOrient);

		AxisAlignedBox box = mBox;
		box.transform(mat);

		mWireBoundingBox->setupBoundingBox(box);
	}
	else
	{
		mWireBoundingBox->setupBoundingBox(mBox);
	}

	queue->addRenderable(mWireBoundingBox);
}

void DynamicRenderable::_updateRenderQueue(RenderQueue* queue)
{
	SimpleRenderable::_updateRenderQueue(queue);

	if (mShowBoundingBox || (mManager && mManager->getShowBoundingBoxes())) 
	{ 
		_addBoundingBoxToQueue(queue);
	}
}

void DynamicRenderable::getWorldTransforms( Matrix4* xform ) const
{
	if(mUseParentTransform)
	{
		*xform = m_matWorldTransform * mParentNode->_getFullTransform();
	}
	else
	{
		*xform = m_matWorldTransform;
	}
}