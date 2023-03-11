#pragma once

#include "EditorCore/TimeBlock.h"

class TrackCtrl;
class CinemaTrackCtrl;

class ViewSyncMediator
	: public ITimeBlockEventListener
	, public IEventLisitener
{
public:
	ViewSyncMediator(void);
	~ViewSyncMediator(void);

	void SetTrackView(TrackCtrl* v1);
	void SetCinemaView(CinemaTrackCtrl* v2);

	TrackCtrl* GetTrackView();
	CinemaTrackCtrl* GetCinemaView();

	void OnTrackViewZoom(float scale);
	void OnCinemaViewZoom(float scale);

	void OnTrackViewOffset(float x);
	void OnCinemaViewOffset(float x);

	void OnTrackViewCurrentTimeChange(float t);
	void OnCinemaViewCurrentTimeChange(float t);

protected:

	TrackCtrl* _trackView;
	CinemaTrackCtrl* _cinemaView;

	virtual void OnSetCurrentTime(GridCtrl* ctrl, float t);
	virtual void OnZoomChange(GridCtrl* ctrl, float scale);
	virtual void OnOriginChange(GridCtrl* ctrl, float xOff, float yOff);

	virtual void OnBlockChange(TimeblockContainer* c, IITimeblock* block);
	virtual void OnBlockSelected(TimeblockContainer* c, IITimeblock* block);
	virtual void OnClearBlockSelected(TimeblockContainer* c);
	virtual void OnEditKeyFrame(TimeblockContainer* c, IITimeblock* block, int keyFrameIdx);

	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast); 

	void _RefreshTracks( Cinema* c );
	void _AddTrack( ICinemaTrack* track );
};
