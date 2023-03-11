#pragma once

extern const uint32 C_AxesRenderQueue;
extern const uint32 C_PickMask;

enum EAxes
{
	EA_Null,
	EA_X,
	EA_Y,
	EA_Z,
	EA_XZ,
	EA_XY,
	EA_YZ
};


enum ESubEditState
{
	ESES_Move = 0,
	ESES_Rotate = 1,
	ESES_Scale = 2
};


class Axes
	: public VisualObject

{
public:

	Axes();

	virtual EAxes GetSubAxes(MovableObject* obj);

protected:

	DynamicLines*	_x;
	DynamicLines*	_y;
	DynamicLines*	_z;

	virtual void PreDestroyNode();
};

//------------------------------------------------------------------------

class Axes_Move : public Axes
{
public:

	Axes_Move();
	virtual ~Axes_Move();

	virtual const String& GetTypeName() const;
	virtual void SetShowBoundingBox(bool on);
	virtual EAxes GetSubAxes(MovableObject* obj);

protected:

	virtual void PostCreateNode();
	virtual void PreDestroyNode();
	virtual void CreateArrow();
	virtual void DestroyArrow();

	SceneNode*	_xArrowNode;
	SceneNode*	_yArrowNode;
	SceneNode*	_zArrowNode;

	Entity*	_xArrow;
	Entity*	_yArrow;
	Entity*	_zArrow;
};

//------------------------------------------------------------------------

class Axes_Rotate : public Axes
{
public:

	Axes_Rotate();
	virtual ~Axes_Rotate();

	virtual const String& GetTypeName() const;

protected:

	virtual void PostCreateNode();	
};

//------------------------------------------------------------------------

class ColourCube : public DynamicRenderable
{
public:

	ColourCube(const String& name);
	virtual ~ColourCube();

	void update();
	void setColour(const ColourValue& clr);
	const ColourValue& getColour() const;

	void setCubeSize(float sz);
	float getCubeSize() const;

protected:
	/// Implementation DynamicRenderable, creates a simple vertex-only decl
	virtual void createVertexDeclaration();
	/// Implementation DynamicRenderable, pushes point list out to hardware memory
	virtual void fillHardwareBuffers();

	void _init();

private:
	bool mDirty;
	float mSize;
	ColourValue mColour;
};

//------------------------------------------------------------------------
class Axes_Scale : public Axes
{
public:

	Axes_Scale();
	virtual ~Axes_Scale();

	virtual const String& GetTypeName() const;

	virtual EAxes GetSubAxes(MovableObject* obj);
	virtual void SetShowBoundingBox(bool on);

protected:

	virtual void PostCreateNode();
	virtual void PreDestroyNode();

	virtual void CreateArrow(){}
	virtual void DestroyArrow(){}

	SceneNode*	_xCubeNode;
	SceneNode*	_yCubeNode;
	SceneNode*	_zCubeNode;

	ColourCube*		_xCube;
	ColourCube*		_yCube;
	ColourCube*		_zCube;
};


//------------------------------------------------------------------------