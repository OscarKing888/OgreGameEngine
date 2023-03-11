#pragma once

class TimeLinePanel;

class TimeLinePanelController
	: public IEventLisitener
	, public OgeAlloc
	, public ITimeBlockEventListener
{
public:

	TimeLinePanelController(TimeLinePanel* panel);
	virtual ~TimeLinePanelController();

protected:

	TimeLinePanel* _ui;

	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast);

	virtual void OnBlockChange(TimeblockContainer* c, IITimeblock* block);
	virtual void OnBlockSelected(TimeblockContainer* c, IITimeblock* block);
	virtual void OnClearBlockSelected(TimeblockContainer* c);
	virtual void OnEditKeyFrame(TimeblockContainer* c, IITimeblock* block, int keyFrameIdx);

	virtual void OnSetCurrentTime(GridCtrl* ctrl, float t);
	virtual void OnZoomChange(GridCtrl* ctrl, float scale);
	virtual void OnOriginChange(GridCtrl* ctrl, float xOff, float yOff);
};