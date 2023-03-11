// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-10-25
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"
#include <OgreWireBoundingBox.h>



class _EngineExport DynamicRenderable : public SimpleRenderable
{
public:
	/// Constructor
	DynamicRenderable();
	DynamicRenderable(const String& name);
	/// Virtual destructor
	virtual ~DynamicRenderable();

	/** Initializes the dynamic renderable.
	@remarks
	This function should only be called once. It initializes the
	render operation, and calls the abstract function
	createVertexDeclaration().
	@param operationType The type of render operation to perform.
	@param useIndices Specifies whether to use indices to determine the
	vertices to use as input. */
	void initialize(RenderOperation::OperationType operationType,
		bool useIndices);

	/// Implementation of SimpleRenderable
	virtual Real getBoundingRadius(void) const;
	/// Implementation of SimpleRenderable
	virtual Real getSquaredViewDepth(const Camera* cam) const;

	virtual void _updateRenderQueue(RenderQueue* queue);

	virtual void showBoundingBox(bool bShow) { mShowBoundingBox = bShow; }

	virtual void getWorldTransforms( Matrix4* xform ) const;

	void setUseParentNodeTransform(bool on) { mUseParentTransform = on; }
	bool isUseParentNodeTransform() const { return mUseParentTransform; }

	/** Sets any kind of user value on this object.
	@remarks
	This method allows you to associate any user value you like with 
	this Renderable. This can be a pointer back to one of your own
	classes for instance.
	*/
	virtual void setUserAny(const Any& anything) { MovableObject::setUserAny(anything); }

	/** Retrieves the custom user value associated with this object.
	*/
	virtual const Any& getUserAny(void) const { return MovableObject::getUserAny(); }

protected:
	/// Maximum capacity of the currently allocated vertex buffer.
	size_t mVertexBufferCapacity;
	/// Maximum capacity of the currently allocated index buffer.
	size_t mIndexBufferCapacity;

	/// Pointer to a Wire Bounding Box for this Node
	WireBoundingBox *mWireBoundingBox;
	/// Flag that determines if the bounding box of the node should be displayed
	bool mShowBoundingBox;

	bool mUseParentTransform;

	void _addBoundingBoxToQueue(RenderQueue* queue);

	/** Creates the vertex declaration.
	@remarks
	Override and set mRenderOp.vertexData->vertexDeclaration here.
	mRenderOp.vertexData will be created for you before this method
	is called. */
	virtual void createVertexDeclaration() = 0;

	/** Prepares the hardware buffers for the requested vertex and index counts.
	@remarks
	This function must be called before locking the buffers in
	fillHardwareBuffers(). It guarantees that the hardware buffers
	are large enough to hold at least the requested number of
	vertices and indices (if using indices). The buffers are
	possibly reallocated to achieve this.
	@par
	The vertex and index count in the render operation are set to
	the values of vertexCount and indexCount respectively.
	@param vertexCount The number of vertices the buffer must hold.

	@param indexCount The number of indices the buffer must hold. This
	parameter is ignored if not using indices. */
	void prepareHardwareBuffers(size_t vertexCount, size_t indexCount);

	/** Fills the hardware vertex and index buffers with data.
	@remarks
	This function must call prepareHardwareBuffers() before locking
	the buffers to ensure the they are large enough for the data to
	be written. Afterwards the vertex and index buffers (if using
	indices) can be locked, and data can be written to them. */
	virtual void fillHardwareBuffers() = 0;
};
