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
#include "DynamicLines.h"





DynamicLines::DynamicLines(const String& name, OperationType opType)
{
	if(!name.empty())
	{
		mName = name;
	}

	_init(opType);
}

DynamicLines::DynamicLines(const Vector3List& pointList, const String& name, OperationType opType)
{
	if(!name.empty())
	{
		mName = name;
	}

	_init(opType);
	for (size_t i = 0; i < pointList.size(); ++i)
	{
		addPoint(pointList[i]);
	}
	update();
}

void DynamicLines::_init(OperationType opType)
{
	initialize(opType,false);
	setMaterial("BaseVertexColor");
	mDirty = true;
	setCastShadows(false);
}

DynamicLines::~DynamicLines()
{
}

void DynamicLines::setOperationType(OperationType opType)
{
	mRenderOp.operationType = opType;
}

RenderOperation::OperationType DynamicLines::getOperationType() const
{
	return mRenderOp.operationType;
}

void DynamicLines::addPoint(const Vector3 &p)
{
	mPoints.push_back(p);
	mDirty = true;
}
void DynamicLines::addPoint(Real x, Real y, Real z)
{
	mPoints.push_back(Vector3(x,y,z));
	mDirty = true;
}
const Vector3& DynamicLines::getPoint(unsigned short index) const
{
	assert(index < mPoints.size() && "Point index is out of bounds!!");
	return mPoints[index];
}
unsigned short DynamicLines::getNumPoints(void) const
{
	return (unsigned short)mPoints.size();
}
void DynamicLines::setPoint(unsigned short index, const Vector3 &value)
{
	assert(index < mPoints.size() && "Point index is out of bounds!!");

	mPoints[index] = value;
	mDirty = true;
}

void DynamicLines::setPoint(const Vector3List& pointList)
{
	clear();
	mPoints = pointList;
	update();
}

void DynamicLines::clear()
{
	mPoints.clear();
	mDirty = true;
}

void DynamicLines::update()
{
	if (mDirty) fillHardwareBuffers();
}

void DynamicLines::createVertexDeclaration()
{
	VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
	decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);
	size_t offset = VertexElement::getTypeSize(VET_FLOAT3);
	decl->addElement(0, offset, VET_COLOUR, VES_DIFFUSE);
}

void DynamicLines::fillHardwareBuffers()
{
	size_t sz = mPoints.size();

	prepareHardwareBuffers(sz, 0);

	if (!sz) { 
		mBox.setExtents(Vector3::ZERO,Vector3::ZERO);
		mDirty=false;
		return;
	}

	Vector3 vaabMin = mPoints[0];
	Vector3 vaabMax = mPoints[0];

	HardwareVertexBufferSharedPtr vbuf =
		mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);

	struct  CustomVertex
	{
		float x, y, z;
		RGBA clr;
	};

	CustomVertex *prPos = static_cast<CustomVertex*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
	{
		for(size_t i = 0; i < sz; i++)
		{
			prPos->x = mPoints[i].x;
			prPos->y = mPoints[i].y;
			prPos->z = mPoints[i].z;			
			Root::getSingleton().convertColourValue(mColour, &prPos->clr);
			prPos++;

			if(mPoints[i].x < vaabMin.x)
				vaabMin.x = mPoints[i].x;
			if(mPoints[i].y < vaabMin.y)
				vaabMin.y = mPoints[i].y;
			if(mPoints[i].z < vaabMin.z)
				vaabMin.z = mPoints[i].z;

			if(mPoints[i].x > vaabMax.x)
				vaabMax.x = mPoints[i].x;
			if(mPoints[i].y > vaabMax.y)
				vaabMax.y = mPoints[i].y;
			if(mPoints[i].z > vaabMax.z)
				vaabMax.z = mPoints[i].z;
		}
	}
	vbuf->unlock();

	mBox.setExtents(vaabMin, vaabMax);

	mDirty = false;
}

void DynamicLines::setLineColour(const ColourValue& clr)
{
	mColour = clr;
	fillHardwareBuffers();
}

const ColourValue& DynamicLines::getLineColour() const
{
	return mColour;
}

