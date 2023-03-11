#include "StdAfx.h"
#include "CinemaTrackCtrl.h"
#include "CinemaEditorEvents.h"
#include "CinemaEditor.h"

//--------------------------------------------------------------------------------
CinemaTrackBlock::CinemaTrackBlock()
: _selectIdx(-1)
, _hitIdx(-1)
, _track(0)
{
}
//--------------------------------------------------------------------------------
CinemaTrackBlock::~CinemaTrackBlock()
{

}
//--------------------------------------------------------------------------------
ICinemaTrack* CinemaTrackBlock::GetTrack()
{
	return _track;
}
//--------------------------------------------------------------------------------
void CinemaTrackBlock::SetTrack( ICinemaTrack* track )
{
	_track = track;

	SetTimeLength(track->GetParent()->GetLength());
	SetCaption(track->ToString().c_str());
}
//--------------------------------------------------------------------------------
void CinemaTrackBlock::DrawKeyFrame( CDC& dc, const CRect& rc )
{
	CBrush redBrush(RGB(255, 0, 0));
	CBrush whiteBrush(RGB(255, 255, 255));

	CinemaTrackCtrl* trackCtrl = dynamic_cast<CinemaTrackCtrl*>(_parent);

	int idc = dc.SaveDC();

	size_t cnt = _track->GetKeyFrameCount();
	float yVal = (float)_track->GetParent()->GetTrackIndex(_track);
	for (size_t i = 0; i < cnt; ++i)
	{
		CinemaKeyFrame* ck = _track->GetKeyFrameByIndex(i);
		float xVal = GetStartTime() + ck->GetTime();
		if((i == GetHitIdx()) || (i == GetSelectIdx()))
		{
			dc.SelectObject(&redBrush);
		}
		else
		{
			dc.SelectObject(&whiteBrush);
		}

		CRect rc = trackCtrl->GetKeyframeRect(xVal, yVal);
		dc.Rectangle(&rc);
	}

	dc.RestoreDC(idc);
}

void CinemaTrackBlock::DrawText( CDC& dc, const CRect& rc )
{
	if(_track)
	{
		CRect rcd = rc;
		rcd.left += 5;
		dc.DrawText(_track->ToString().c_str(), &rcd, _parent->GetDrawTextFormat());
	}
	else
	{
		AbstractTimeblock::DrawText(dc, rc);
	}
}
//--------------------------------------------------------------------------------
//CinemaTrackCtrl
//--------------------------------------------------------------------------------
#define WM_ADDTRACK_BEGIN (WM_USER + 1)
#define WM_ADDTRACK_END (WM_USER + 200)

BEGIN_MESSAGE_MAP(CinemaTrackCtrl, TimeblockContainer)
	ON_WM_RBUTTONUP()
	ON_COMMAND_RANGE(WM_ADDTRACK_BEGIN, WM_ADDTRACK_END, OnAddTrack)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

CinemaTrackCtrl::CinemaTrackCtrl(void)
: _dragKeyFrame(false)
, _dragKeyFrameIndex(-1)
{
	_enableDragBlock = false;
	_drawTextFlag = DT_SINGLELINE | DT_LEFT | DT_TOP | DT_END_ELLIPSIS;
}
//--------------------------------------------------------------------------------
CinemaTrackCtrl::~CinemaTrackCtrl(void)
{
}
//--------------------------------------------------------------------------------
void CinemaTrackCtrl::OnPostDrawGrid( CDC& dc, const CRect& rc )
{
	TimeblockContainer::OnPostDrawGrid(dc, rc);	
}
//--------------------------------------------------------------------------------
void CinemaTrackCtrl::OnMouseMove( const CPoint& point )
{
	TimeblockContainer::OnMouseMove(point);

	if(_dragKeyFrame && (_dragKeyFrameIndex != -1))
	{
		CinemaTrackBlock* blk = dynamic_cast<CinemaTrackBlock*>(_selectedBlock);
		ICinemaTrack* track = blk->GetTrack();
		float t = this->GetMouseTime(point);
		CinemaKeyFrame* kf = track->GetKeyFrameByIndex(_dragKeyFrameIndex);
		kf->SetTime(t);
		track->GetParent()->_OnKeyFrameListChanged();
	}
	else
	{
		for (size_t i = 0; i < _blocks.size(); ++i)
		{
			CinemaTrackBlock* blk = dynamic_cast<CinemaTrackBlock*>(_blocks[i].GetPointer());
			blk->SetHitIdx(-1);
		}

		HitKeyFrame(point);
	}
}
//--------------------------------------------------------------------------------
CRect CinemaTrackCtrl::GetKeyframeRect( float xVal, float yVal )
{
	int x = 0, y = 0;
	ConvertToPixelCoord(xVal, yVal + 0.5f, x, y); // y+0.5居中显示
	CRect rc(x - 4, y - 4, x + 4, y + 4);
	return rc;
}
//--------------------------------------------------------------------------------
void CinemaTrackCtrl::OnLButtonDown( const CPoint& point )
{
	TimeblockContainer::OnLButtonDown(point);

	for (size_t i = 0; i < _blocks.size(); ++i)
	{
		CinemaTrackBlock* blk = dynamic_cast<CinemaTrackBlock*>(_blocks[i].GetPointer());
		blk->SetSelectIdx(-1);
	}

	_dragKeyFrameIndex = HitKeyFrame(point);
	if(_dragKeyFrameIndex != -1)
	{
		_dragKeyFrame = true;
	}

	return;
}
//--------------------------------------------------------------------------------
void CinemaTrackCtrl::OnLButtonUp( const CPoint& point )
{
	TimeblockContainer::OnLButtonUp(point);
	_dragKeyFrame = false;
	_dragKeyFrameIndex = -1;
}
//--------------------------------------------------------------------------------
void CinemaTrackCtrl::OnRButtonUp( UINT nFlags, CPoint point )
{
	TimeblockContainer::OnRButtonUp(nFlags, point);

	_trackTypeList.clear();

	CMenu menu;
	menu.CreatePopupMenu();
	CinemaEditor& editor = CinemaEditor::getSingleton();
	if(editor.GetCinema())
	{
		const CinemaManager::FactoryMap& trackFactories = CinemaManager::getSingleton().GetAllTrackFactory();
		UINT i = 0;
		for (CinemaManager::FactoryMap::const_iterator it = trackFactories.begin();
			it != trackFactories.end(); ++it)
		{
			_trackTypeList.push_back(it->first);
			UINT cmdID = WM_ADDTRACK_BEGIN + i;
			UINT flag = MF_STRING;
			ITrackEditor* te = editor.GetTrackEditor(it->first);
			if(!te->CanCreateKeyFrame(EditorApp::getSingleton().GetSelectedObject()))
			{
				flag |= MF_DISABLED | MF_GRAYED;
			}

			menu.AppendMenu(flag, cmdID, it->first.c_str());

			++i;
		}
	}
	else
	{
		menu.AppendMenu(MF_STRING, WM_ADDTRACK_BEGIN,
			_UIDisplayString("IDS_EDIT_CINEMA_FIRST").c_str());
		menu.EnableMenuItem(WM_ADDTRACK_BEGIN, MF_BYCOMMAND | MF_GRAYED);
	}
//	menu.EnableMenuItem(WM_ADDTRACK_BEGIN, MF_BYCOMMAND | MF_GRAYED);
	
	ClientToScreen(&point);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
}
//--------------------------------------------------------------------------------
void CinemaTrackCtrl::CalcMaxTime()
{
	TimeblockContainer::CalcMaxTime();
	_minValidValue = 0.0f;
	_maxValidValue = (float)_blocks.size();
}
//--------------------------------------------------------------------------------
void CinemaTrackCtrl::OnAddTrack( UINT id )
{
	EventsManager::getSingleton().CreateEvent(
		CinemaEditorEvents::AddTrack, Any(_trackTypeList[id - WM_ADDTRACK_BEGIN]));

	Redraw();
}
//--------------------------------------------------------------------------------
void CinemaTrackCtrl::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	int idx = HitKeyFrame(point);
	if(-1 != idx)
	{
		GetListenerEx()->OnEditKeyFrame(this, _selectedBlock, idx);
	}
}
//--------------------------------------------------------------------------------
int CinemaTrackCtrl::HitKeyFrame( const CPoint& point )
{
	int trackIdx = (int)GetMouseValue(point);
	// 只在有效TRACK索引范围检查
	if((trackIdx >= 0) && (trackIdx < (int)_blocks.size()))
	{
		float x = GetMouseTime(point);
		CinemaTrackBlock* blk = dynamic_cast<CinemaTrackBlock*>(_blocks[trackIdx].GetPointer());
		// 只在有效TRACK时间范围检查
		if((x >= 0.0f) && (x <= blk->GetStartTime() + blk->GetTimeLength()))
		{
			ICinemaTrack* track = blk->GetTrack();			
			for (size_t c = 0; c < track->GetKeyFrameCount(); ++c)
			{
				float t = track->GetKeyFrameByIndex(c)->GetTime();
				float xVal = t + blk->GetStartTime();
				CRect rc = GetKeyframeRect(xVal, (float)trackIdx);
				rc.InflateRect(2, 2, 2, 2);
				if(rc.PtInRect(point))
				{
					blk->SetSelectIdx(c);
					return c;
				}
			}
		}
	}

	return -1;
}
//--------------------------------------------------------------------------------