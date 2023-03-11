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

class _EngineExport WireBox : public SimpleRenderable
{
public:

	WireBox();
	~WireBox();

	void SetSize(const Vector3& sz);
	const Vector3& GetSize() const;
	AxisAlignedBox GetBoundingBox() const;

	Real getBoundingRadius(void) const { return mRadius; }

	Real getSquaredViewDepth(const Camera* cam) const;

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

	Real mRadius;
	Vector3	_size;

	void getWorldTransforms( Matrix4* xform ) const;
	void setupBoundingBoxVertices(const AxisAlignedBox& aab);
	void setupBoundingBox(const AxisAlignedBox& aabb);
};
