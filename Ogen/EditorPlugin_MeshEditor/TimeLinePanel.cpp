#include "stdafx.h"
#include "TimeLinePanel.h"
#include "MeshEditorFrm.h"
#include "EditorCore/MemDC.h"
#include "Resource.h"

#define BTN_WIDTH 32
#define BTN_HEIGHT 32
#define KEYFRAME_DRAW_WIDTH 4

//----------------------------------------------------------------------------------------------
//BEGIN_MESSAGE_MAP(TimeSliderCtrl, CSliderCtrl)
	//ON_NOTIFY_REFLECT(TVN_SELCHANGED, &TimeSliderCtrl::OnNMClk)
//END_MESSAGE_MAP()
//----------------------------------------------------------------------------------------------
// TimeSliderCtrl::TimeSliderCtrl()

// 
// }
// //----------------------------------------------------------------------------------------------
// TimeSliderCtrl::~TimeSliderCtrl()

// 
// }
// //----------------------------------------------------------------------------------------------
// void TimeSliderCtrl::OnNMClk(NMHDR *pNMHDR, LRESULT *pResult)
	
// // 	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
// // 	*pResult = 0;
// // 	HTREEITEM item = pNMTreeView->itemNew.hItem;
// // 	HTREEITEM itemParent = GetParentItem(item);
// // 
// // 	TimeLinePanel* p = dynamic_cast<TimeLinePanel*>(GetParent());
// // 	p->OnItemClick(item, itemParent);
// }
//----------------------------------------------------------------------------------------------
// TimeLinePanel
//----------------------------------------------------------------------------------------------
TimeLinePanel::TimeLinePanel()
: _totalTime(1.0f)
, _currentTime(0.0f)
, _currentCursorClr(RGB(0, 0, 0))
, _frameEventClr(RGB(0, 128, 255))
, _selectedFrameEventClr(RGB(255, 0, 0))
, _moveingCursorX(0)
, _isMouseLDown(false)
, _selectedFrameEvent(0)
, _meshData(0)
{
}
//----------------------------------------------------------------------------------------------
TimeLinePanel::~TimeLinePanel()
{
}
//----------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(TimeLinePanel, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_BTN_TIMELINE_PLAY, &TimeLinePanel::OnPlay)
	ON_COMMAND(ID_BTN_TIMELINE_PAUSE, &TimeLinePanel::OnPause)
	ON_COMMAND(ID_BTN_TIMELINE_STOP, &TimeLinePanel::OnStop)
	ON_COMMAND(ID_BTN_TIMELINE_TOBEGIN, &TimeLinePanel::OnToBegin)
	ON_COMMAND(ID_BTN_TIMELINE_TOEND, &TimeLinePanel::OnToEnd)
END_MESSAGE_MAP()
//----------------------------------------------------------------------------------------------
int TimeLinePanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
//	const DWORD dwViewStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TBS_HORZ;

// 	if (!_slider.Create(dwViewStyle, rectDummy, this, 2))
// 	{
// 		// TRACE0("未能创建Tree控件\n");
// 		return -1;      // 未能创建
// 	}

// 	EnableDocking(CBRS_ALIGN_ANY);

	// image list

	CBitmap bmp;
	if (!bmp.LoadBitmap(IDR_MESH_INFO_ICON))
	{
		TRACE(("无法加载位图: %x\n"), IDR_MESH_INFO_ICON);
		ASSERT(FALSE);
		return -1;
	}
	
	const DWORD btnStyle = WS_BORDER | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | BS_OWNERDRAW | WS_TABSTOP;
	
	_playBtn.Create("Play", btnStyle, CRect(0, 0, 100, 100), this, ID_BTN_TIMELINE_PLAY);
	_playBtn.LoadBitmaps(IDB_BTN_PLAY);

	_stopBtn.Create("Stop", btnStyle, CRect(0, 0, 100, 100), this, ID_BTN_TIMELINE_STOP);
	_stopBtn.LoadBitmaps(IDB_BTN_STOP);

	_pauseBtn.Create("Pause", btnStyle, CRect(0, 0, 100, 100), this, ID_BTN_TIMELINE_PAUSE);
	_pauseBtn.LoadBitmaps(IDB_BTN_PAUSE);

	_toBeginBtn.Create("ToBegin", btnStyle, CRect(0, 0, 100, 100), this, ID_BTN_TIMELINE_TOBEGIN);
	_toBeginBtn.LoadBitmaps(IDB_BTN_TOBEGIN);

	_toEndBtn.Create("ToEnd", btnStyle, CRect(0, 0, 100, 100), this, ID_BTN_TIMELINE_TOEND);
	_toEndBtn.LoadBitmaps(IDB_BTN_TOEND);
	
	/*_tooltip.Create(this);
	_tooltip.AddTool(this, "Test tool tip");
	_tooltip.Activate(TRUE);*/

	_popupMenu.LoadMenu(IDR_MODELEDITOR);

	this->SetCaptionStyle(TRUE, TRUE);
	this->SetWindowText("TimeLine");
	return 0;
}
//----------------------------------------------------------------------------------------------
BOOL TimeLinePanel::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
//----------------------------------------------------------------------------------------------
void TimeLinePanel::OnPaint()
{
	CPaintDC paintDC(this);
	MemDC dc(&paintDC);
	CRect rc;
	GetClientRect(&rc);

	int w = rc.Width();
	int h = rc.Height() - BTN_HEIGHT;

	// draw text info
	CRect rcTxt;
	rcTxt.left = 0;
	rcTxt.top = h + 1;
	rcTxt.right = 400;
	CString str;
	str.Format("%.3f/%.3f     ", _currentTime, _totalTime);

	if(_selectedFrameEvent)
	{
		str += _selectedFrameEvent->EventName.c_str();
	}

	dc.DrawText(str, &rcTxt, DT_SINGLELINE | DT_LEFT | DT_TOP);

	// current time cursor
	int x = (_currentTime / _totalTime) * w;
	CPen pen(PS_SOLID, KEYFRAME_DRAW_WIDTH, _currentCursorClr);
	CPen* oldPen = dc.SelectObject(&pen);
	dc.MoveTo(x, 0);
	dc.LineTo(x, h);

	// moving cursor
	CPen penMoving(PS_DOT, 1, RGB(128, 128, 128));
	dc.SelectObject(&penMoving);
	dc.MoveTo(_moveingCursorX, 0);
	dc.LineTo(_moveingCursorX, h);

	// frame events
	if(_meshData)
	{
		const String& playingAnim = MeshEditorFrm::getSingleton().GetPlayingAnimation();
		if(!playingAnim.empty())
		{
			CPen penFrameEvent(PS_SOLID, KEYFRAME_DRAW_WIDTH, _frameEventClr);
			dc.SelectObject(&penFrameEvent);
			const MeshData::FrameEventList& frameEventList =
				_meshData->GetAnimationFrameEventList(playingAnim);

			MeshData::FrameEventList::const_iterator it = frameEventList.begin();
			for (; it != frameEventList.end(); ++it)
			{
				const SFrameEvent& frmEvt = *it;
				int x = (frmEvt.AnimationTime / _totalTime) * w;
				dc.MoveTo(x, 0);
				dc.LineTo(x, h);
			}
		}
	}

	// selected frame event
	if(_selectedFrameEvent)
	{
		CPen penSelFrameEvent(PS_SOLID, KEYFRAME_DRAW_WIDTH, _selectedFrameEventClr);
		dc.SelectObject(&penSelFrameEvent);

		int x = (_selectedFrameEvent->AnimationTime / _totalTime) * w;
		dc.MoveTo(x, 0);
		dc.LineTo(x, h);
	}

	dc.SelectObject(oldPen);
}
//----------------------------------------------------------------------------------------------
void TimeLinePanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	//_slider.MoveWindow(rectClient);
	
	_playBtn.MoveWindow(cx - BTN_WIDTH * 5 - 5, cy - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT);
	_pauseBtn.MoveWindow(cx - BTN_WIDTH * 4 - 5, cy - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT);
	_stopBtn.MoveWindow(cx - BTN_WIDTH * 3 - 5, cy - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT);
	_toBeginBtn.MoveWindow(cx - BTN_WIDTH * 2 - 5, cy - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT);
	_toEndBtn.MoveWindow(cx - BTN_WIDTH * 1 - 5, cy - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT);	
}
//----------------------------------------------------------------------------------------------
void TimeLinePanel::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	//_slider.SetFocus();
}
//----------------------------------------------------------------------------------------------
BOOL TimeLinePanel::OnBeforeShowPaneMenu(CMenu& m)
{
	//CMenu* subM = _popupMenu.GetSubMenu(0);
	//m.Attach(subM->GetSafeHmenu());
	if(MeshEditorFrm::getSingleton().HasAnimationPalying())
	{
		m.AppendMenu(MF_SEPARATOR);
		m.AppendMenu(MF_BYCOMMAND | MF_STRING, ID_MESHEDIT_ADDFRAMEEVENT, "Add FrameEvent(&A)...");
		m.AppendMenu(MF_BYCOMMAND | MF_STRING, ID_MESHEDIT_DELETEFRAMEEVENT, "Delete FrameEvent(&D)");
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------
BOOL TimeLinePanel::OnAfterShowPaneMenu(int nMenuResult)
{
	switch(nMenuResult)
	{
	case ID_MESHEDIT_ADDFRAMEEVENT:
		MeshEditorFrm::getSingleton().OnAddFrameEvent();
		break;

	case ID_MESHEDIT_DELETEFRAMEEVENT:
		MeshEditorFrm::getSingleton().OnDelFrameEvent();
		break;
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------
void TimeLinePanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);
	_currentTime = _totalTime * (float)point.x / (float)rc.Width();
	MeshEditorFrm::getSingleton().PauseAnimation();
	MeshEditorFrm::getSingleton().SetPlayingAnimationTimePos(_currentTime);
	//CDockablePane::OnLButtonDown()

	_isMouseLDown = true;

	SetCapture();
}
//----------------------------------------------------------------------------------------------
void TimeLinePanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	_isMouseLDown = false;
	ReleaseCapture();
}
//----------------------------------------------------------------------------------------------
void TimeLinePanel::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);
	if(point.x >= 0 && point.x <= rc.Width())
	{
		_moveingCursorX = point.x;
		if(_isMouseLDown)
		{
			_currentTime = _totalTime * (float)_moveingCursorX / (float)rc.Width();
			MeshEditorFrm::getSingleton().SetPlayingAnimationTimePos(_currentTime);
		}
		else
		{
			if(_meshData)
			{
				HitTestFrameEvent(point);
			}
		}

		if(_meshData)
		{
			if(_isMouseLDown && _selectedFrameEvent)
			{
				const String& playingAnim = MeshEditorFrm::getSingleton().GetPlayingAnimation();
				SFrameEvent* frmEvt = _meshData->GetFrameEvent(playingAnim, _selectedFrameEvent->EventName);
				frmEvt->AnimationTime = _currentTime;
			}
		}
	}
	//_tooltip.ShowWindow(TRUE);
}
//----------------------------------------------------------------------------------------------
BOOL TimeLinePanel::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message== WM_LBUTTONDOWN ||
		pMsg->message== WM_LBUTTONUP ||
		pMsg->message== WM_MOUSEMOVE)
	{
		//_tooltip.RelayEvent(pMsg);
	}

	return CDockablePane::PreTranslateMessage(pMsg);
}
//----------------------------------------------------------------------------------------------
void TimeLinePanel::OnPlay()
{
	MeshEditorFrm::getSingleton().PlayAnimation();
}
//----------------------------------------------------------------------------------------------
void TimeLinePanel::OnPause()
{
	MeshEditorFrm::getSingleton().PauseAnimation();
}
//----------------------------------------------------------------------------------------------
void TimeLinePanel::OnStop()
{
	MeshEditorFrm::getSingleton().StopPlayAnimation();
	_currentTime = 0.0f;
}
//----------------------------------------------------------------------------------------------
void TimeLinePanel::OnToBegin()
{
	_currentTime = 0.0f;
	MeshEditorFrm::getSingleton().SetPlayingAnimationTimePos(_currentTime);
	MeshEditorFrm::getSingleton().PauseAnimation();
}
//----------------------------------------------------------------------------------------------
void TimeLinePanel::OnToEnd()
{
	_currentTime = _totalTime;
	MeshEditorFrm::getSingleton().SetPlayingAnimationTimePos(_currentTime);
	MeshEditorFrm::getSingleton().PauseAnimation();
}
//----------------------------------------------------------------------------------------------
void TimeLinePanel::HitTestFrameEvent(const CPoint& mousePt)
{
	CRect rc;
	GetClientRect(&rc);

	int w = rc.Width();
// 	int h = rc.Height() - BTN_HEIGHT;

	const String& playingAnim = MeshEditorFrm::getSingleton().GetPlayingAnimation();
	if(!playingAnim.empty())
	{
		const MeshData::FrameEventList& frameEventList = _meshData->GetAnimationFrameEventList(playingAnim);
		MeshData::FrameEventList::const_iterator it = frameEventList.begin();
		for (; it != frameEventList.end(); ++it)
		{
			const SFrameEvent& frm = *it;
			int x = (frm.AnimationTime / _totalTime) * w;
			if(std::abs(x - mousePt.x) <= KEYFRAME_DRAW_WIDTH)
			{
				_selectedFrameEvent = &frm;
				return;
			}
		}
	}

	_selectedFrameEvent = 0;
}
//----------------------------------------------------------------------------------------------
