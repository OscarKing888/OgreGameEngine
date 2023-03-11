#include "StdAfx.h"
#include "ViewSyncMediator.h"
#include "CinemaTrackCtrl.h"
#include "TrackCtrl.h"
#include "CinemaEditor.h"
#include "CinemaEditorEvents.h"

ViewSyncMediator::ViewSyncMediator(void)
: _trackView(0)
, _cinemaView(0)
{
	EventsManager::getSingleton().AddListener(this);
}
//--------------------------------------------------------------------------------
ViewSyncMediator::~ViewSyncMediator(void)
{
	EventsManager::getSingleton().RemoveListener(this);
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::SetTrackView( TrackCtrl* v1 )
{
	_trackView = v1;
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::SetCinemaView( CinemaTrackCtrl* v2 )
{
	_cinemaView = v2;
}
//--------------------------------------------------------------------------------
TrackCtrl* ViewSyncMediator::GetTrackView()
{
	return _trackView;
}
//--------------------------------------------------------------------------------
CinemaTrackCtrl* ViewSyncMediator::GetCinemaView()
{
	return _cinemaView;
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnTrackViewZoom( float scale )
{
	_cinemaView->SetZoomScale(scale);
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnCinemaViewZoom( float scale )
{
	_trackView->SetZoomScale(scale);
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnTrackViewOffset( float x )
{
	_cinemaView->SetOriginX(x);
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnCinemaViewOffset( float x )
{
	_trackView->SetOriginX(x);
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnTrackViewCurrentTimeChange( float t )
{
	_cinemaView->SetCurrentTime(t);
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnCinemaViewCurrentTimeChange( float t )
{
	_trackView->SetCurrentTime(t);
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnSetCurrentTime( GridCtrl* ctrl, float t )
{
	if(ctrl == _trackView)
	{
		OnTrackViewCurrentTimeChange(t);
	}
	else
	{
		OnCinemaViewCurrentTimeChange(t);
	}

	CinemaEditor::getSingleton().SetCurrentTime(t);
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnZoomChange( GridCtrl* ctrl, float scale )
{
	if(ctrl == _trackView)
	{
		OnTrackViewZoom(scale);
	}
	else
	{
		OnCinemaViewZoom(scale);
	}
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnOriginChange( GridCtrl* ctrl, float xOff, float yOff )
{
	if(ctrl == _trackView)
	{
		OnTrackViewOffset(xOff);
	}
	else
	{
		OnCinemaViewOffset(xOff);
	}
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnBlockChange( TimeblockContainer* c, IITimeblock* block )
{
	if(c == _cinemaView)
	{
		CinemaTrackBlock* blk = dynamic_cast<CinemaTrackBlock*>(block);
	}
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnBlockSelected( TimeblockContainer* c, IITimeblock* block )
{
	CinemaTrackBlock* track = dynamic_cast<CinemaTrackBlock*>(block);
	if(track)
	{
		CinemaEditor::getSingleton().SetTrack(track->GetTrack());

		EventsManager::getSingleton().CreateEvent(
			CinemaEditorEvents::ShowCinemaPropObject, Any((IPropertyObject*)track->GetTrack()));
	}
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnClearBlockSelected( TimeblockContainer* c )
{
	CinemaEditor::getSingleton().SetTrack(0);
	EventsManager::getSingleton().CreateEvent(
		CinemaEditorEvents::ClearCinemaPropObject, Any());
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(eventName == CinemaEditorEvents::UpdateUI_CinemaCtrl)
	{
		Cinema* c = any_cast<Cinema*>(eventContex);
		_RefreshTracks(c);
	}
	else if(eventName == CinemaEditorEvents::UpdateUI_AddTrack)
	{
		Cinema* c = any_cast<Cinema*>(eventContex);
		ICinemaTrack* track = c->GetTrack(c->GetTrackCount() - 1);

		_AddTrack(track);

		_cinemaView->Redraw();
	}
	else if(eventName == CinemaEditorEvents::UpdateUI_RemoveTrack)
	{
		Cinema* c = any_cast<Cinema*>(eventContex);
		CinemaEditor::getSingleton().SetTrack(0);
		_RefreshTracks(c);
	}
	else if(eventName == "CinemaTimeChange")
	{
		Cinema* c = any_cast<Cinema*>(eventContex);
		OnTrackViewCurrentTimeChange(c->GetTimePosition());
		OnCinemaViewCurrentTimeChange(c->GetTimePosition());
	}
	else if(CinemaEditorEvents::UpdateUI_CreateKeyFrame == eventName)
	{
		size_t cnt = _cinemaView->GetTimeblockCount();
		ICinemaTrack* track = any_cast<ICinemaTrack*>(eventContex);
		for (size_t i = 0; i < cnt; ++i)
		{
			CinemaTrackBlock* blk = dynamic_cast<CinemaTrackBlock*>(
				_cinemaView->GetTimeblock(i));
			if(blk->GetTrack() == track)
			{
				_cinemaView->Redraw();
				return;
			}
		}
	}
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::_AddTrack( ICinemaTrack* track )
{
	CinemaTrackBlock* atb = OGRE_NEW CinemaTrackBlock();
	atb->SetParent(_cinemaView);

	String cap = track->ToString();
	//atb->SetCaption(cap.c_str());

	atb->SetStartTime(0);
	atb->SetTrack(track);
	_cinemaView->AddTimeblock(atb);
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::_RefreshTracks( Cinema* c )
{
	_cinemaView->RemoveAllTimeblock();

	for (size_t i = 0; i < c->GetTrackCount(); ++i)
	{
		ICinemaTrack* track = c->GetTrack(i);
		_AddTrack(track);
	}

	_cinemaView->Redraw();
}
//--------------------------------------------------------------------------------
void ViewSyncMediator::OnEditKeyFrame( TimeblockContainer* c, IITimeblock* block, int keyFrameIdx )
{
	if(c == _cinemaView)
	{
		CinemaTrackBlock* blk = dynamic_cast<CinemaTrackBlock*>(block);
		ICinemaTrack* track = blk->GetTrack();
		ITrackEditor* editor = CinemaEditor::getSingleton().GetTrackEditor(track);
		//Cinema* c = CinemaEditor::getSingleton().GetCinema();
		CinemaKeyFrame* kf = track->GetKeyFrameByIndex(keyFrameIdx);
		editor->EditKeyFrameWithDlg(track, kf);
	}
}
//--------------------------------------------------------------------------------
