#include "stdafx.h"
#include "Axes.h"

const float C_AxesUnits = 100.0f;
const uint32 C_AxesRenderQueue = RENDER_QUEUE_MAX;
const uint32 C_PickMask = 0xF0000000;

//------------------------------------------------------------------------
// Axes
//------------------------------------------------------------------------
Axes::Axes()
: _x(0)
, _y(0)
, _z(0)
{
	// 不要与Trigger碰撞检测
	_triggerMask = 0;
} 

void Axes::PreDestroyNode()
{
	VisualObject::PreDestroyNode();

	OGRE_DELETE _x;
	OGRE_DELETE _y;
	OGRE_DELETE _z;

	_x = _y = _z = 0;
}


EAxes Axes::GetSubAxes(MovableObject* obj)
{
	if(obj == _x)
	{
		return EA_X;
	}

	if(obj == _y)
	{
		return EA_Y;
	}

	if(obj == _z)
	{
		return EA_Z;
	}

	return EA_Null;
}

//------------------------------------------------------------------------
// Axes_Move
//------------------------------------------------------------------------
Axes_Move::Axes_Move()
: _xArrow(0)
, _yArrow(0)
, _zArrow(0)
, _xArrowNode(0)
, _yArrowNode(0)
, _zArrowNode(0)
{	
}

Axes_Move::~Axes_Move()
{
}

const String& Axes_Move::GetTypeName() const
{
	static const String Axes_Move_TypeName = "Axes_Move";
	return Axes_Move_TypeName;
}

void Axes_Move::PostCreateNode()
{
	Vector3List points;
	points.push_back(Vector3::ZERO);
	points.push_back(Vector3::UNIT_X * C_AxesUnits);
	_x = OGRE_NEW DynamicLines(points, GetGUID().toString() + "__Axes_Move_X__");
	_x->setLineColour(ColourValue::Red);
	_x->setMaterial("EditorCone_Red");
	_x->setUserAny(Any(AnyGameObject(this)));
	_x->setRenderQueueGroup(C_AxesRenderQueue);

	points[1] = Vector3::UNIT_Y * C_AxesUnits;
	_y = OGRE_NEW DynamicLines(points, GetGUID().toString() + "__Axes_Move_Y__");
	_y->setLineColour(ColourValue::Green);
	_y->setMaterial("EditorCone_Green");
	_y->setUserAny(Any(AnyGameObject(this)));
	_y->setRenderQueueGroup(C_AxesRenderQueue);

	points[1] = Vector3::UNIT_Z * C_AxesUnits;
	_z = OGRE_NEW DynamicLines(points, GetGUID().toString() + "__Axes_Move_Z__");
	_z->setLineColour(ColourValue::Blue);
	_z->setMaterial("EditorCone_Blue");
	_z->setUserAny(Any(AnyGameObject(this)));
	_z->setRenderQueueGroup(C_AxesRenderQueue);

	_node->attachObject(_x);
	_node->attachObject(_y);
	_node->attachObject(_z);

	_x->setQueryFlags(C_PickMask);
	_y->setQueryFlags(C_PickMask);
	_z->setQueryFlags(C_PickMask);

	_x->setCastShadows(false);
	_y->setCastShadows(false);
	_z->setCastShadows(false);

	CreateArrow();
}

void Axes_Move::CreateArrow()
{
	if(_node)
	{
		SceneManager* sm = _node->getCreator();
		
		_xArrowNode = _node->createChildSceneNode(GetGUID().toString() + ".XArrow",
			Vector3(C_AxesUnits, 0, 0), Quaternion(Radian(Degree(-90.0f)), Vector3::UNIT_Z));
		_xArrow = sm->createEntity(GetGUID().toString() + ".XArrow", "EditorCone.mesh");
		_xArrow->setMaterialName("EditorCone_Red");
		_xArrow->setUserAny(Any(AnyGameObject(this)));
		_xArrowNode->attachObject(_xArrow);

		_yArrowNode = _node->createChildSceneNode(GetGUID().toString() + ".YArrow",
			Vector3(0, C_AxesUnits, 0));
		_yArrow = sm->createEntity(GetGUID().toString() + ".YArrow", "EditorCone.mesh");
		_yArrow->setMaterialName("EditorCone_Green");
		_yArrow->setUserAny(Any(AnyGameObject(this)));
		_yArrowNode->attachObject(_yArrow);

		_zArrowNode = _node->createChildSceneNode(GetGUID().toString() + ".ZArrow",
			Vector3(0, 0, C_AxesUnits), Quaternion(Radian(Degree(90.0f)), Vector3::UNIT_X));
		_zArrow = sm->createEntity(GetGUID().toString() + ".ZArrow", "EditorCone.mesh");
		_zArrow->setMaterialName("EditorCone_Blue");
		_zArrow->setUserAny(Any(AnyGameObject(this)));
		_zArrowNode->attachObject(_zArrow);

		_xArrow->setQueryFlags(C_PickMask);
		_yArrow->setQueryFlags(C_PickMask);
		_zArrow->setQueryFlags(C_PickMask);
		
		_xArrow->setRenderQueueGroup(C_AxesRenderQueue);
		_yArrow->setRenderQueueGroup(C_AxesRenderQueue);
		_zArrow->setRenderQueueGroup(C_AxesRenderQueue);

		_xArrow->setCastShadows(false);
		_yArrow->setCastShadows(false);
		_zArrow->setCastShadows(false);
	}
}

void Axes_Move::DestroyArrow()
{
	if(_xArrowNode)
	{
		SceneManager* sm = _node->getCreator();
		_node->removeAllChildren();

		_xArrowNode->detachAllObjects();
		_yArrowNode->detachAllObjects();
		_zArrowNode->detachAllObjects();

		sm->destroySceneNode(_xArrowNode);
		sm->destroySceneNode(_yArrowNode);
		sm->destroySceneNode(_zArrowNode);

		sm->destroyEntity(_xArrow);
		sm->destroyEntity(_yArrow);
		sm->destroyEntity(_zArrow);

		_xArrow = _yArrow = _zArrow = 0;
		_xArrowNode = _yArrowNode = _zArrowNode = 0;

		if(_xArrow)
		{
			OGRE_DELETE _xArrow;
			_xArrow = 0;
		}

		if(_yArrow)
		{
			OGRE_DELETE _yArrow;
			_yArrow = 0;
		}

		if(_zArrow)
		{
			OGRE_DELETE _zArrow;
			_zArrow = 0;
		}
	}
}


void Axes_Move::PreDestroyNode()
{
	Axes::PreDestroyNode();
	DestroyArrow();
}

void Axes_Move::SetShowBoundingBox(bool on)
{
	VisualObject::SetShowBoundingBox(on);
	if(_xArrowNode)
	{
		_xArrowNode->showBoundingBox(on);
		_yArrowNode->showBoundingBox(on);
		_zArrowNode->showBoundingBox(on);
	}
}

EAxes Axes_Move::GetSubAxes(MovableObject* obj)
{
	EAxes ax = Axes::GetSubAxes(obj);
	if(ax != EA_Null)
	{
		return ax;
	}

	if(obj == _xArrow)
	{
		return EA_X;
	}

	if(obj == _yArrow)
	{
		return EA_Y;
	}

	if(obj == _zArrow)
	{
		return EA_Z;
	}

	return EA_Null;
}

//------------------------------------------------------------------------
// Axes_Rotate
//------------------------------------------------------------------------
Axes_Rotate::Axes_Rotate()
{	
}

Axes_Rotate::~Axes_Rotate()
{
}

const String& Axes_Rotate::GetTypeName() const
{
	static const String Axes_Rotate_TypeName = "Axes_Rotate";
	return Axes_Rotate_TypeName;
}

void Axes_Rotate::PostCreateNode()
{
	const float C_Radius = C_AxesUnits;
	const int C_Segments = 40;
	const float C_DeltaAng = Math::TWO_PI / (float)C_Segments;

	// build yaw circle on x-z
	{
		Vector3List points;
		for (int i = 0; i < C_Segments; ++i)
		{
			float da = C_DeltaAng * (float)i;
			float x = C_Radius * cosf(da);
			float y = C_Radius * sinf(da);
			points.push_back(Vector3(x, 0.0f, y));
		}

		float x = C_Radius * cosf(0.0f);
		float y = C_Radius * sinf(0.0f);
		points.push_back(Vector3(x, 0.0f, y));

		_x = OGRE_NEW DynamicLines(points, GetGUID().toString() + "__Axes_Rotate_X__");
		_x->setLineColour(ColourValue::Red);
		_x->setMaterial("EditorCone_Red");
		_x->setUserAny(Any(AnyGameObject(this)));
		_x->setPoint(points);
	}

	// build pitch circle on y-z
	{
		Vector3List points;
		for (int i = 0; i < C_Segments; ++i)
		{
			float da = C_DeltaAng * (float)i;
			float x = C_Radius * cosf(da);
			float y = C_Radius * sinf(da);
			points.push_back(Vector3(0.0f, y, x));
		}

		float x = C_Radius * cosf(0.0f);
		float y = C_Radius * sinf(0.0f);
		points.push_back(Vector3(0.0f, y, x));

		_y = OGRE_NEW DynamicLines(points, GetGUID().toString() + "__Axes_Rotate_Y__");
		_y->setLineColour(ColourValue::Green);
		_y->setMaterial("EditorCone_Green");
		_y->setUserAny(Any(AnyGameObject(this)));
		_y->setPoint(points);
	}

	// build roll circle on x-y
	{
		Vector3List points;
		for (int i = 0; i < C_Segments; ++i)
		{
			float da = C_DeltaAng * (float)i;
			float x = C_Radius * cosf(da);
			float y = C_Radius * sinf(da);
			points.push_back(Vector3(x, y, 0.0f));
		}

		float x = C_Radius * cosf(0.0f);
		float y = C_Radius * sinf(0.0f);
		points.push_back(Vector3(x, y, 0.0f));

		_z = OGRE_NEW DynamicLines(points, GetGUID().toString() + "__Axes_Rotate_Z__");
		_z->setLineColour(ColourValue::Blue);
		_z->setMaterial("EditorCone_Blue");
		_z->setUserAny(Any(AnyGameObject(this)));
		_z->setPoint(points);
	}

	_node->attachObject(_x);
	_node->attachObject(_y);
	_node->attachObject(_z);

	_x->setQueryFlags(C_PickMask);
	_y->setQueryFlags(C_PickMask);
	_z->setQueryFlags(C_PickMask);

	_x->setRenderQueueGroup(C_AxesRenderQueue);
	_y->setRenderQueueGroup(C_AxesRenderQueue);
	_z->setRenderQueueGroup(C_AxesRenderQueue);

	_x->setCastShadows(false);
	_y->setCastShadows(false);
	_z->setCastShadows(false);
}

//------------------------------------------------------------------------
// Axes_Scale
//------------------------------------------------------------------------
Axes_Scale::Axes_Scale()
: _xCubeNode(0)
, _yCubeNode(0)
, _zCubeNode(0)
, _xCube(0)
, _yCube(0)
, _zCube(0)
{	
}

Axes_Scale::~Axes_Scale()
{
}

const String& Axes_Scale::GetTypeName() const
{
	static const String Axes_Scale_TypeName = "Axes_Scale";
	return Axes_Scale_TypeName;
}

void Axes_Scale::PostCreateNode()
{
	Axes::PostCreateNode();

	if(_node)
	{
		SceneManager* sm = _node->getCreator();

		_xCube = OGRE_NEW ColourCube(GetGUID().toString() + "__Axes_Scale_X__");
		_xCube->setUserAny(Any(AnyGameObject(this)));
		_xCube->setColour(ColourValue::Red);
		_xCube->setMaterial("EditorCone_Red");
		_xCubeNode = sm->createSceneNode(GetGUID().toString() + ".XNode");
		_xCubeNode->setPosition(C_AxesUnits, 0, 0);
		_xCubeNode->attachObject(_xCube);

		_yCube = OGRE_NEW ColourCube(GetGUID().toString() + "__Axes_Scale_Y__");
		_yCube->setUserAny(Any(AnyGameObject(this)));
		_yCube->setColour(ColourValue::Green);
		_yCube->setMaterial("EditorCone_Green");
		_yCubeNode = sm->createSceneNode(GetGUID().toString() + ".YNode");
		_yCubeNode->setPosition(0, C_AxesUnits, 0);
		_yCubeNode->attachObject(_yCube);

		_zCube = OGRE_NEW ColourCube(GetGUID().toString() + "__Axes_Scale_Z__");
		_zCube->setUserAny(Any(AnyGameObject(this)));
		_zCube->setColour(ColourValue::Blue);
		_zCube->setMaterial("EditorCone_Blue");
		_zCubeNode = sm->createSceneNode(GetGUID().toString() + ".ZNode");
		_zCubeNode->setPosition(0, 0, C_AxesUnits);
		_zCubeNode->attachObject(_zCube);

		// 绽放为轴向1/10大小
		float scale = C_AxesUnits * 0.1f;
		_xCube->setCubeSize(scale);
		_yCube->setCubeSize(scale);	
		_zCube->setCubeSize(scale);

		_node->addChild(_xCubeNode);
		_node->addChild(_yCubeNode);
		_node->addChild(_zCubeNode);

		_xCube->setQueryFlags(C_PickMask);
		_yCube->setQueryFlags(C_PickMask);
		_zCube->setQueryFlags(C_PickMask);

		_xCube->setRenderQueueGroup(C_AxesRenderQueue);
		_yCube->setRenderQueueGroup(C_AxesRenderQueue);
		_zCube->setRenderQueueGroup(C_AxesRenderQueue);

		_xCube->setCastShadows(false);
		_yCube->setCastShadows(false);
		_zCube->setCastShadows(false);

		Vector3List points;
		points.push_back(Vector3::ZERO);
		points.push_back(Vector3::UNIT_X * C_AxesUnits);
		_x = OGRE_NEW DynamicLines(points, GetGUID().toString() + "__Axes_Scale_X__");
		_x->setLineColour(ColourValue::Red);
		_x->setMaterial("EditorCone_Red");
		_x->setUserAny(Any(AnyGameObject(this)));
		_x->setRenderQueueGroup(C_AxesRenderQueue);

		points[1] = Vector3::UNIT_Y * C_AxesUnits;
		_y = OGRE_NEW DynamicLines(points, GetGUID().toString() + "__Axes_Scale_Y__");
		_y->setLineColour(ColourValue::Green);
		_y->setMaterial("EditorCone_Green");
		_y->setUserAny(Any(AnyGameObject(this)));
		_y->setRenderQueueGroup(C_AxesRenderQueue);

		points[1] = Vector3::UNIT_Z * C_AxesUnits;
		_z = OGRE_NEW DynamicLines(points, GetGUID().toString() + "__Axes_Scale_Z__");
		_z->setLineColour(ColourValue::Blue);
		_z->setMaterial("EditorCone_Blue");
		_z->setUserAny(Any(AnyGameObject(this)));
		_z->setRenderQueueGroup(C_AxesRenderQueue);

		_node->attachObject(_x);
		_node->attachObject(_y);
		_node->attachObject(_z);

		_x->setQueryFlags(C_PickMask);
		_y->setQueryFlags(C_PickMask);
		_z->setQueryFlags(C_PickMask);

		_x->setRenderQueueGroup(C_AxesRenderQueue);
		_y->setRenderQueueGroup(C_AxesRenderQueue);
		_z->setRenderQueueGroup(C_AxesRenderQueue);

		_x->setCastShadows(false);
		_y->setCastShadows(false);
		_z->setCastShadows(false);
	}
}

void Axes_Scale::PreDestroyNode()
{
	Axes::PreDestroyNode();
	if(_node)
	{
		_node->removeChild(_xCubeNode);
		_node->removeChild(_yCubeNode);
		_node->removeChild(_zCubeNode);

		_xCubeNode->detachAllObjects();
		_yCubeNode->detachAllObjects();
		_zCubeNode->detachAllObjects();

		SceneManager* sm = _node->getCreator();

		sm->destroySceneNode(_xCubeNode);
		sm->destroySceneNode(_yCubeNode);
		sm->destroySceneNode(_zCubeNode);

		if(_xCube)
		{
			OGRE_DELETE _xCube;
			_xCube = 0;
		}

		if(_yCube)
		{
			OGRE_DELETE _yCube;
			_yCube = 0;
		}

		if(_zCube)
		{
			OGRE_DELETE _zCube;
			_zCube = 0;
		}
	}
}

EAxes Axes_Scale::GetSubAxes(MovableObject* obj)
{
	EAxes ax = Axes::GetSubAxes(obj);
	if(ax != EA_Null)
	{
		return ax;
	}

	if(obj == _xCube)
	{
		return EA_X;
	}

	if(obj == _yCube)
	{
		return EA_Y;
	}

	if(obj == _zCube)
	{
		return EA_Z;
	}

	return EA_Null;
}

void Axes_Scale::SetShowBoundingBox(bool on)
{
	VisualObject::SetShowBoundingBox(on);
	if(_xCubeNode)
	{
		_xCubeNode->showBoundingBox(on);
		_yCubeNode->showBoundingBox(on);
		_zCubeNode->showBoundingBox(on);
	}
}

//------------------------------------------------------------------------
// ColourCube
//------------------------------------------------------------------------

ColourCube::ColourCube(const String& name)
: mSize(100.0f)
, mColour(ColourValue::White)
, mDirty(true)
{
	mName = name;
	_init();
}

ColourCube::~ColourCube()
{

}

void ColourCube::_init()
{
	initialize(RenderOperation::OT_TRIANGLE_LIST, true);
	setMaterial("BaseVertexColor");
	mDirty = true;
	setCastShadows(false);
	update();
}

void ColourCube::update()
{
	if (mDirty) fillHardwareBuffers();
}

void ColourCube::createVertexDeclaration()
{
	size_t offset = 0;
	VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
	decl->addElement(0, offset, VET_FLOAT3, VES_POSITION);
	offset += VertexElement::getTypeSize(VET_FLOAT3);
	
	decl->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
	offset += VertexElement::getTypeSize(VET_FLOAT3);

	decl->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
	offset += VertexElement::getTypeSize(VET_FLOAT2);

	decl->addElement(0, offset, VET_COLOUR, VES_DIFFUSE);
}

void ColourCube::fillHardwareBuffers()
{
	const int NUM_VERTICES = 4 * 6; // 4 vertices per side * 6 sides
	const int NUM_ENTRIES_PER_VERTEX = 8;
	const int NUM_VERTEX_ENTRIES = NUM_VERTICES * NUM_ENTRIES_PER_VERTEX;
	const int NUM_INDICES = 3 * 2 * 6; // 3 indices per face * 2 faces per side * 6 sides

	const Real CUBE_SIZE = mSize;
	const Real CUBE_HALF_SIZE = CUBE_SIZE / 2.0;

	prepareHardwareBuffers(NUM_VERTEX_ENTRIES, NUM_INDICES);

	HardwareVertexBufferSharedPtr vbuf =
		mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);

	struct  CustomVertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
		RGBA clr;
	};
	
	RGBA clrT = 0;
	Root::getSingleton().convertColourValue(mColour, &clrT);

	CustomVertex vertices[NUM_VERTEX_ENTRIES] = {
		// front side
		-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,	// pos
		0,0,1,	// normal
		0,1,	// texcoord
		clrT,	// diffuse
		CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
		0,0,1,
		1,1,
		clrT,
		CUBE_HALF_SIZE,  CUBE_HALF_SIZE, CUBE_HALF_SIZE,
		0,0,1,
		1,0,
		clrT,
		-CUBE_HALF_SIZE,  CUBE_HALF_SIZE, CUBE_HALF_SIZE ,
		0,0,1,
		0,0,
		clrT,

		// back side
		CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		0,0,-1,
		0,1,
		clrT,
		-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		0,0,-1,
		1,1,
		clrT,
		-CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		0,0,-1,
		1,0,
		clrT,
		CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		0,0,-1,
		0,0,
		clrT,

		// left side
		-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		-1,0,0,
		0,1,
		clrT,
		-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
		-1,0,0,
		1,1,
		clrT,
		-CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE,
		-1,0,0,
		1,0,
		clrT,
		-CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		-1,0,0,
		0,0, 
		clrT,

		// right side
		CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
		1,0,0,
		0,1,
		clrT,
		CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		1,0,0,
		1,1,
		clrT,
		CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		1,0,0,
		1,0,
		clrT,
		CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE,
		1,0,0,
		0,0,
		clrT,

		// up side
		-CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE,
		0,1,0,
		0,1,
		clrT,
		CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE,
		0,1,0,
		1,1,
		clrT,
		CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		0,1,0,
		1,0,
		clrT,
		-CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		0,1,0,
		0,0,
		clrT,

		// down side
		-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		0,-1,0,
		0,1,
		clrT,
		CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		0,-1,0,
		1,1,
		clrT,
		CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
		0,-1,0,
		1,0,
		clrT,
		-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
		0,-1,0,
		0,0, 
		clrT
	};

	vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);

	HardwareIndexBufferSharedPtr ibuf = mRenderOp.indexData->indexBuffer;

	unsigned short faces[NUM_INDICES] = {
		// front
		0,1,2,
		0,2,3,

		// back
		4,5,6,
		4,6,7,

		// left
		8,9,10,
		8,10,11,

		// right
		12,13,14,
		12,14,15,

		// up
		16,17,18,
		16,18,19,

		// down
		20,21,22,
		20,22,23
	};

	ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);

	mBox = AxisAlignedBox(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
		CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE);//.setExtents(vaabMin, vaabMax);

	mDirty = false;
}

void ColourCube::setColour(const ColourValue& clr)
{
	mColour = clr;
	fillHardwareBuffers();
}

const ColourValue& ColourCube::getColour() const
{
	return mColour;
}

void ColourCube::setCubeSize(float sz)
{
	mSize = sz;
	fillHardwareBuffers();
}

float ColourCube::getCubeSize() const
{
	return mSize;
}

