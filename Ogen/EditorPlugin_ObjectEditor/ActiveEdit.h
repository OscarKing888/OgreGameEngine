#pragma once
#include "Axes.h"

//------------------------------------------------------------------------
// 动态编辑执行环境参数，可以根据不同的动态编辑需要增加数据成员，但要注意初始化。
// 未用到的参数可不初始化，但必须在构造时赋初值，以便于Debug。
//------------------------------------------------------------------------
struct SActiveEditExecuteContex
{
	int MousePositionX;
	int MousePositionY;

	int MouseMoveDeltaOnX;
	int MouseMoveDeltaOnY;
	int MouseWheelDelta;

	bool PickOnTerrainOK;
	bool PickManipulatorOK;

	Vector3 PickedPosition;
	Vector3 BrushDownPoint;
	IGameObject* PickedObject;

	ESubEditState SubEditState;
	EAxes ManipulateFlag;

	Camera*	CurrentCamera;

	SActiveEditExecuteContex()
	{
		MouseMoveDeltaOnX = 0;
		MouseMoveDeltaOnY = 0;
		MouseWheelDelta = 0;
		PickedPosition = Vector3::ZERO;
		BrushDownPoint = Vector3::ZERO;
		PickedObject = 0;
		SubEditState = ESES_Move;
		ManipulateFlag = EA_Null;
		CurrentCamera = 0;
		PickOnTerrainOK = false;
		PickManipulatorOK = false;
		MousePositionX = 0;
		MousePositionY = 0;
	}

	friend std::ostream& operator<<(std::ostream& o, const SActiveEditExecuteContex& r)
	{ (void)r; return o; }
};

//------------------------------------------------------------------------

class ActiveEdit_MoveObject : public AbstractActiveEdit
{
public:

	ActiveEdit_MoveObject(VisualObject* object);
	virtual ~ActiveEdit_MoveObject();

	virtual ICommandXPtr GetCommand();
	virtual void Execute(const Any& c);

protected:

	VisualObject*	_visualObject;
	Vector3			_startPosition;
	Quaternion		_startOrientation;
	ITile*			_startTile;
};

//------------------------------------------------------------------------

class ActiveEdit_ScaleObject : public AbstractActiveEdit
{
public:

	ActiveEdit_ScaleObject(VisualObject* object)
		: _visualObject(object)
	{
		_startScale = object->GetScale();
	}

	virtual ICommandXPtr GetCommand();
	virtual void Execute(const Any& c);

protected:

	VisualObject*	_visualObject;
	Vector3			_startScale;

};

//------------------------------------------------------------------------

class ActiveEdit_RotateObject : public AbstractActiveEdit
{
public:

	ActiveEdit_RotateObject(VisualObject* object)
		: _visualObject(object)
	{
		_startOrientation = object->GetOrientation();
	}

	virtual ICommandXPtr GetCommand();
	virtual void Execute(const Any& c);

protected:

	VisualObject*	_visualObject;
	Quaternion			_startOrientation;
};
