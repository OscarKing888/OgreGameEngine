#include "stdafx.h"
#include "TimeLinePanelController.h"
#include "TimeLinePanel.h"
#include "AbstractSFXTimeblock.h"
#include "SFXEditorEvents.h"
#include "SFXEditor.h"

TimeLinePanelController::TimeLinePanelController( TimeLinePanel* panel )
: _ui(panel)
{
	EventsManager::getSingleton().AddListener(this);
	_ui->GetContainer().SetListener(this);
}
//--------------------------------------------------------------------------------
TimeLinePanelController::~TimeLinePanelController()
{
	EventsManager::getSingleton().RemoveListener(this);
}
//--------------------------------------------------------------------------------
void TimeLinePanelController::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(eventName == SFXEditorEvents::UpdateUI_AddElement)
	{
		AbstractSFXTimeblockPtr blk = any_cast<AbstractSFXTimeblockPtr>(eventContex);
		_ui->GetContainer().AddTimeblock(blk);
	}
	else if(eventName == SFXEditorEvents::UpdateUI_RemoveElement)
	{
		int idx = any_cast<int>(eventContex);
		_ui->GetContainer().RemoveTimeblock(idx);
	}
	else if(eventName == SFXEditorEvents::UpdateUI_RemoveAllElement)
	{
		_ui->GetContainer().RemoveAllTimeblock();
	}
}
//--------------------------------------------------------------------------------
void TimeLinePanelController::OnBlockChange( TimeblockContainer* c, IITimeblock* block )
{

}
//--------------------------------------------------------------------------------
void TimeLinePanelController::OnBlockSelected( TimeblockContainer* c, IITimeblock* block )
{
	AbstractSFXTimeblock* sfxblk = dynamic_cast<AbstractSFXTimeblock*>(block);
	if(sfxblk)
	{
		EventsManager::getSingleton().CreateEvent(
			SFXEditorEvents::ShowSFXObjectProp, Any((IPropertyObject*)sfxblk->GetElement()));
	}
}
//--------------------------------------------------------------------------------
void TimeLinePanelController::OnClearBlockSelected(TimeblockContainer* c)
{
	EventsManager::getSingleton().CreateEvent(
		SFXEditorEvents::ShowSFXObjectProp, Any((IPropertyObject*)
		SFXEditor::getSingleton().GetCurrentSFX()));
}
//--------------------------------------------------------------------------------
void TimeLinePanelController::OnSetCurrentTime(GridCtrl* ctrl, float t )
{
	SFXEditor::getSingleton().StepUpdate(t);
}
//--------------------------------------------------------------------------------
void TimeLinePanelController::OnZoomChange( GridCtrl* ctrl, float scale )
{

}
//--------------------------------------------------------------------------------
void TimeLinePanelController::OnOriginChange( GridCtrl* ctrl, float xOff, float yOff )
{

}
//--------------------------------------------------------------------------------
void TimeLinePanelController::OnEditKeyFrame( TimeblockContainer* c, IITimeblock* block, int keyFrameIdx )
{

}
//--------------------------------------------------------------------------------