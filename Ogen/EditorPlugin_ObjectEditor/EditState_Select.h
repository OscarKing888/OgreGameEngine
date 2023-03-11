#pragma once
#include "Axes.h"

extern int EditStateID_Select;

///
/// SPickResult
///
struct SPickResult : public SPickGameObjectResult
{
	EAxes			AxesPicked;
	Vector3			TerrainIntersectPoint;
	bool			IsPickedOnTerrain;

	SPickResult()
		: AxesPicked(EA_Null)
		, TerrainIntersectPoint(Vector3::ZERO)
		, IsPickedOnTerrain(false)
	{
	}

	const SPickResult& operator =(const SPickGameObjectResult& src)
	{
		(SPickGameObjectResult&)(*this) = src;
		return *this;
	}
};

///
/// EditState_Select
///
class EditState_Select
	: public AbstractEditState
	, public Singleton<EditState_Select>
	, public IEventLisitener	
{
public:

	EditState_Select(IStateManager* pMgr);
	virtual ~EditState_Select();

	virtual void CreateToolbar(CMFCRibbonCategory* mainCategory);

	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	void SetSelectObject(IGameObject* obj);
	IGameObject* GetSelecteObject() { return _selectedObject; }

	EAxes PickAxes(int x, int y);
	SPickResult Pick(int x, int y);

	bool IsObjectSelected() { return _selectedObject != 0; }

	void SetSubEditState(ESubEditState se);
	ESubEditState GetSubEditState() const { return _subEditState; }

	void UpdateEditAxes();
	void UpdateShowBoundingBox();
	Axes_Move* GetAxes_Move() { return _axesMove; }
	Axes_Rotate* GetAxes_Rotate() { return _axesRotate; }
	Axes_Scale* GetAxes_Scale() { return _axesScale; }

	virtual void		OnActive();
	virtual void		OnInActive();

	uint32 GetQueryFlag();
	EAxes GetPickAxes(MovableObject* obj);


protected:

	VisualObject*			_selectedObject;
	Axes_Move*				_axesMove;
	Axes_Rotate*			_axesRotate;
	Axes_Scale*				_axesScale;

	ESubEditState			_subEditState;
	EAxes					_selectedAxes;

	enum ECursorType
	{
		ECT_Normal = 0,
		ECT_Select,
		ECT_Move,
		ECT_MoveX,
		ECT_MoveY,
		ECT_MoveZ,
		ECT_Rotate,
		ECT_RotateX,
		ECT_RotateY,
		ECT_RotateZ,
		ECT_Scale
	};

	void SetCursor(ECursorType c);

	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast); 
};

///
/// EditState_SelectMsgHandler
///

class EditState_SelectMsgHandler
	: public CCmdTarget
	, public Singleton<EditState_SelectMsgHandler>
{
public:

	EditState_SelectMsgHandler(EditState_Select* state);
	virtual ~EditState_SelectMsgHandler();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnMove();
	afx_msg void OnRotate();
	afx_msg void OnScale();
	afx_msg void OnUpdateMoveUI(CCmdUI* ui);
	afx_msg void OnUpdateRotateUI(CCmdUI* ui);
	afx_msg void OnUpdateScaleUI(CCmdUI* ui);

protected:

	EditState_Select* _state;
};