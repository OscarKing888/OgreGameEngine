#include "StdAfx.h"
#include "TimeBlock.h"

#pragma warning(push)
#pragma warning(disable : 4244)

//--------------------------------------------------------------------------------
// AbstractTimeblock
//--------------------------------------------------------------------------------
AbstractTimeblock::AbstractTimeblock()
: _startTime(0.0f)
, _length(1.0f)
, _isSelected(false)
, _bkColor(RGB(168, 209, 110))
, _borderColor(0)
, _selectBkColor(RGB(255, 255, 128))
, _parent(0)
{

}
//--------------------------------------------------------------------------------
AbstractTimeblock::~AbstractTimeblock()
{

}
//--------------------------------------------------------------------------------
COLORREF AbstractTimeblock::GetBackgroundColor() const
{
	return _bkColor;
}
//--------------------------------------------------------------------------------
void AbstractTimeblock::SetBackgroundColor( COLORREF clr )
{
	_bkColor = clr;
	NotifyChanged();
}
//--------------------------------------------------------------------------------
COLORREF AbstractTimeblock::GetBorderColor() const
{
	return _borderColor;
}
//--------------------------------------------------------------------------------
void AbstractTimeblock::SetBorderColor( COLORREF clr )
{
	_borderColor = clr;
	NotifyChanged();
}
//--------------------------------------------------------------------------------
COLORREF AbstractTimeblock::GetSelectBackgroundColor() const
{
	return _selectBkColor;
}
//--------------------------------------------------------------------------------
void AbstractTimeblock::SetSelectBackgroundColor( COLORREF clr )
{
	_selectBkColor = clr;
	NotifyChanged();
}
//--------------------------------------------------------------------------------
float AbstractTimeblock::GetStartTime() const
{
	return _startTime;
}
//--------------------------------------------------------------------------------
void AbstractTimeblock::SetStartTime( float val )
{
	_startTime = val;
	NotifyChanged();
}
//--------------------------------------------------------------------------------
float AbstractTimeblock::GetTimeLength() const
{
	return _length;
}
//--------------------------------------------------------------------------------
void AbstractTimeblock::SetTimeLength( float val )
{
	_length = val;
	NotifyChanged();
}
//--------------------------------------------------------------------------------
bool AbstractTimeblock::IsSelected() const
{
	return _isSelected;
}
//--------------------------------------------------------------------------------
void AbstractTimeblock::SetSelected( bool on )
{
	_isSelected = on;
}
//--------------------------------------------------------------------------------
IITimeblockContainer* AbstractTimeblock::GetParent() const
{
	return _parent;
}
//--------------------------------------------------------------------------------
void AbstractTimeblock::SetParent( IITimeblockContainer* parent )
{
	_parent = parent;
}
//--------------------------------------------------------------------------------
void AbstractTimeblock::NotifyChanged()
{
	if(_parent)
	{
		_parent->OnChildChange(this);
	}
}

const CString& AbstractTimeblock::GetCaption() const
{
	return _caption;
}

void AbstractTimeblock::SetCaption( const CString& cap )
{
	_caption = cap;
	NotifyChanged();
}
//--------------------------------------------------------------------------------
void AbstractTimeblock::OnDraw( CDC& dc, const CRect& rc )
{
	DrawBackground(dc, rc);
	DrawText(dc, rc);
	DrawKeyFrame(dc, rc);
}
//--------------------------------------------------------------------------------
void AbstractTimeblock::DrawBackground( CDC& dc, const CRect& rc )
{
	CRect rcd = rc;
	CPen borderPen(PS_SOLID, 1, GetBorderColor());
	CBrush bgBrush(GetBackgroundColor());
	CBrush bgSelBrush(GetSelectBackgroundColor());

	CPen* oldPen = dc.SelectObject(&borderPen);		
	CBrush* oldBr = 0;

	if(IsSelected() || (this == _parent->GetSelectedBlock()))
	{
		oldBr = dc.SelectObject(&bgSelBrush);
	}
	else
	{
		oldBr = dc.SelectObject(&bgBrush);
	}

	dc.Rectangle(&rcd);

}
//--------------------------------------------------------------------------------
void AbstractTimeblock::DrawText( CDC& dc, const CRect& rc )
{
	CRect rcd = rc;
	rcd.left += 5;
	dc.DrawText(GetCaption(), &rcd, _parent->GetDrawTextFormat());
}
//--------------------------------------------------------------------------------
void AbstractTimeblock::DrawKeyFrame( CDC& dc, const CRect& rc )
{

}
//--------------------------------------------------------------------------------
// TimeblockContainer
//--------------------------------------------------------------------------------
TimeblockContainer::TimeblockContainer(void)
: _isDragingBlock(false)
, _dragFlag(DragBody)
, _selectedBlock(0)
, _hitBlock(0)
, _enableDragBlock(true)
, _drawTextFlag(DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS)
{
	_maxValidTime = 3.0f;
	_maxValidValue = 10.0f;
	_pixelsPerUnitValue = 50;
}
//--------------------------------------------------------------------------------
TimeblockContainer::~TimeblockContainer(void)
{
}
//--------------------------------------------------------------------------------
void TimeblockContainer::OnPostDrawGrid( CDC& dc, const CRect& rc )
{
	GridCtrl::OnPostDrawGrid(dc, rc);

	float px = _pixelsPerSecond * _zoomScale;
	float py = _pixelsPerUnitValue * _zoomScale;

	int id = dc.SaveDC();
	dc.SelectObject(&_textFont);
	dc.SetBkMode(TRANSPARENT);
	//dc.SetTextColor(RGB(255, 255, 255));

	for (size_t i = 0; i < _blocks.size(); ++i)
	{
		IITimeblockPtr blk = _blocks[i];
		CRect rcd;
		rcd.left = (_originX + blk->GetStartTime()) * px;
		rcd.right = rcd.left + px * blk->GetTimeLength();
		rcd.bottom = rc.Height() - (i + _originY) * py;
		rcd.top = rcd.bottom - py;
		rcd.DeflateRect(0, 2, 0, 2);

		if(!rcd.IntersectRect(&rc, &rcd))
		{
			continue;
		}
		
		blk->OnDraw(dc, rcd);

		/*dc.SelectObject(oldPen);
		dc.SelectObject(oldBr);*/
	}

	dc.RestoreDC(id);
}
//--------------------------------------------------------------------------------
void TimeblockContainer::OnMouseMove( const CPoint& point )
{
	GridCtrl::OnMouseMove(point);
	if(_isDragingBlock && _enableDragBlock)
	{
		CPoint d = point - _lastMouseDownPt;
		_lastMouseDownPt = point;

		float px = _pixelsPerSecond * _zoomScale;

		float dt = (float)d.x / px;
		switch(_dragFlag)
		{
		case DragBody:
			{
				OnDragBody(dt);

				break;
			}

		case DragHead:
			{
				OnDragHead(dt);

				break;
			}

		case DragTail:
			{
				OnDragTail(dt);

				break;
			}
		} // switch

	}
	else if(!_isMovingCurrentTime)
	{
		if(_hitBlock)
		{
			_hitBlock->SetSelected(false);
		}

		_hitBlock = Pick(point).PickedBlock;
		if(_hitBlock)
		{
			_hitBlock->SetSelected(true);
		}
	}
}
//--------------------------------------------------------------------------------
void TimeblockContainer::OnLButtonDown( const CPoint& point )
{
	GridCtrl::OnLButtonDown(point);

	if(!_isMovingCurrentTime)
	{
		SPickResult r = Pick(point);
		_selectedBlock = r.PickedBlock;
		_dragFlag = r.PickPosition;
		if(_selectedBlock)
		{
			_isDragingBlock = true;
			_lastMouseDownPt = point;

			if(GetListenerEx())
			{
				GetListenerEx()->OnBlockSelected(this, _selectedBlock);
			}
		}
		else
		{
			if(GetListenerEx())
			{
				GetListenerEx()->OnClearBlockSelected(this);
			}
		}
	}
}
//--------------------------------------------------------------------------------
void TimeblockContainer::OnLButtonUp( const CPoint& point )
{
	GridCtrl::OnLButtonUp(point);
	_isDragingBlock = false;
}
//--------------------------------------------------------------------------------
void TimeblockContainer::OnLButtonDblClk( const CPoint& point )
{
	GridCtrl::OnLButtonDblClk(point);
}
//--------------------------------------------------------------------------------
void TimeblockContainer::AddTimeblock( IITimeblockPtr block )
{
	_blocks.push_back(block);
	_maxValidValue = _blocks.size();

	CalcMaxTime();

	Redraw();
}
//--------------------------------------------------------------------------------
void TimeblockContainer::RemoveTimeblock( size_t idx )
{
	if(idx < _blocks.size())
	{
		_blocks.erase(_blocks.begin() + idx);
	}
}
//--------------------------------------------------------------------------------
int TimeblockContainer::GetTimeblockIndex( const IITimeblock* block ) const
{
	for (size_t i = 0; i < _blocks.size(); ++i)
	{
		if(block == _blocks[i].GetPointer())
		{
			return i;
		}
	}

	return -1;
}
//--------------------------------------------------------------------------------
TimeblockContainer::SPickResult TimeblockContainer::Pick( const CPoint& pt )
{
	CRect rc = GetDrawArea();

	SPickResult r;
	r.PickedBlock = 0;
	r.PickPosition = DragBody;

	float px = _pixelsPerSecond * _zoomScale;
	float py = _pixelsPerUnitValue * _zoomScale;

	for (size_t i = 0; i < _blocks.size(); ++i)
	{
		IITimeblockPtr blk = _blocks[i];
		CRect rcd;
		rcd.left = (_originX + blk->GetStartTime()) * px;
		rcd.right = rcd.left + px * blk->GetTimeLength();
		rcd.top = rc.Height() - (i + 1 + _originY) * py;
		rcd.bottom = rc.Height() - (i + _originY) * py;
		rcd.DeflateRect(0, 2, 0, 2);
		if(rcd.PtInRect(pt))
		{
			r.PickedBlock = blk.GetPointer();

			int dh = pt.x - rcd.left;
			int dt = rcd.right - pt.x;
			if(dh <= 8)
			{
				r.PickPosition = DragHead;
			}
			else if(dt <= 8)
			{
				r.PickPosition = DragTail;
			}

			break;
		}
	}

	return r;
}
//--------------------------------------------------------------------------------
void TimeblockContainer::OnChildChange( IITimeblock* block )
{
	if(GetListenerEx())
	{
		GetListenerEx()->OnBlockChange(this, block);
	}

	CalcMaxTime();
	Redraw();
}
//--------------------------------------------------------------------------------
ITimeBlockEventListener* TimeblockContainer::GetListenerEx() const
{
	return dynamic_cast<ITimeBlockEventListener*>(_listener);
}
//--------------------------------------------------------------------------------
void TimeblockContainer::CalcMaxTime()
{
	float t = 0;
	for (size_t i = 0; i < _blocks.size(); ++i)
	{
		IITimeblockPtr blk = _blocks[i];
		float endTime = blk->GetStartTime() + blk->GetTimeLength();
		t = std::max(endTime, t);
	}
	_maxValidTime = t;
}
//--------------------------------------------------------------------------------
IITimeblock* TimeblockContainer::GetTimeblock( size_t idx ) const
{
	if(idx < _blocks.size())
	{
		return _blocks[idx].GetPointer();
	}

	return 0;
}
//--------------------------------------------------------------------------------
void TimeblockContainer::RemoveAllTimeblock()
{
	_blocks.clear();
}
//--------------------------------------------------------------------------------
void TimeblockContainer::OnDragBody( float dt )
{
	float t = std::max(0.0f, _selectedBlock->GetStartTime() + dt);
	_selectedBlock->SetStartTime(t);
}
//--------------------------------------------------------------------------------
void TimeblockContainer::OnDragHead( float dt )
{
	float t = std::max(0.0f, _selectedBlock->GetStartTime() + dt);
	float l = _selectedBlock->GetTimeLength() - dt;
	l = std::max(0.1f, l);
	if((t > 0) && (l > 0.1))
	{
		_selectedBlock->SetStartTime(t);
		_selectedBlock->SetTimeLength(l);
	}
}
//--------------------------------------------------------------------------------
void TimeblockContainer::OnDragTail( float dt )
{
	float l = _selectedBlock->GetTimeLength() + dt;
	l = std::max(0.1f, l);
	_selectedBlock->SetTimeLength(l);
}

#pragma warning(pop)