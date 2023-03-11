#pragma once
#include "EditorCore/GridCtrl.h"

class TrackCtrl;

class IKeyframeVisualizer
{
public:

	virtual void OnDraw(TrackCtrl* ctrl, CDC& dc, const CRect& rc) = 0;
};

//--------------------------------------------------------------------------------
//TrackCtrl
//--------------------------------------------------------------------------------
class TrackCtrl : public GridCtrl
{
public:

	TrackCtrl(void);
	virtual ~TrackCtrl(void);

	IKeyframeVisualizer* GetVisualizer() const { return _visualizer; }
	void SetVisualizer(IKeyframeVisualizer* val) { _visualizer = val; Redraw(); }

protected:

	IKeyframeVisualizer* _visualizer;

	virtual void OnPostDrawGrid(CDC& dc, const CRect& rc);
};
