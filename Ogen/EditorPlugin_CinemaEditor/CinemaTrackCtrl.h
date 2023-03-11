#pragma once
#include "EditorCore/TimeBlock.h"

typedef std::vector<float> FloatVector;


class CinemaTrackBlock : public AbstractTimeblock
{
public:

	CinemaTrackBlock();
	virtual ~CinemaTrackBlock();

	int GetSelectIdx() const { return _selectIdx; }
	void SetSelectIdx(int val) { _selectIdx = val; }

	int GetHitIdx() const { return _hitIdx; }
	void SetHitIdx(int val) { _hitIdx = val; }

	ICinemaTrack* GetTrack();
	void SetTrack(ICinemaTrack* track);

protected:

	ICinemaTrack* _track;

	int _selectIdx;
	int _hitIdx;

	virtual void DrawText(CDC& dc, const CRect& rc);
	virtual void DrawKeyFrame( CDC& dc, const CRect& rc );
};

//--------------------------------------------------------------------------------

class CinemaTrackCtrl :	public TimeblockContainer
{
public:

	CinemaTrackCtrl(void);
	virtual ~CinemaTrackCtrl(void);

	virtual void CalcMaxTime();

	CRect GetKeyframeRect( float xVal, float yVal );

protected:

	StringVector _trackTypeList;
	bool _dragKeyFrame;
	int _dragKeyFrameIndex;

	virtual void OnPostDrawGrid(CDC& dc, const CRect& rc);

	virtual void OnMouseMove(const CPoint& point);
	virtual void OnLButtonDown(const CPoint& point);
	virtual void OnLButtonUp(const CPoint& point);

	int HitKeyFrame( const CPoint& point );

	DECLARE_MESSAGE_MAP()

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAddTrack(UINT id);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
