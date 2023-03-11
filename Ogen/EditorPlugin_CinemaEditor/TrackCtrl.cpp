#include "StdAfx.h"
#include "TrackCtrl.h"

TrackCtrl::TrackCtrl(void)
: _visualizer(0)
{
}
//--------------------------------------------------------------------------------
TrackCtrl::~TrackCtrl(void)
{
}
//--------------------------------------------------------------------------------
void TrackCtrl::OnPostDrawGrid( CDC& dc, const CRect& rc )
{
	GridCtrl::OnPostDrawGrid(dc, rc);

	if(_visualizer)
	{
		_visualizer->OnDraw(this, dc, rc);
	}
}
