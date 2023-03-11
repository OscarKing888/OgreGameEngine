#pragma once
#include "GridCtrl.h"

class IITimeblock;
class TimeblockContainer;

class _EditorCoreExport IITimeblockContainer
{
public:

	virtual void OnChildChange(IITimeblock* block) = 0;
	virtual IITimeblock* GetSelectedBlock() = 0;
	virtual UINT GetDrawTextFormat() = 0;
};
//--------------------------------------------------------------------------------
class _EditorCoreExport ITimeBlockEventListener : public IGridCtrlEventListener
{
public:

	virtual void OnBlockChange(TimeblockContainer* c, IITimeblock* block) = 0;
	virtual void OnBlockSelected(TimeblockContainer* c, IITimeblock* block) = 0;
	virtual void OnClearBlockSelected(TimeblockContainer* c) = 0;
	virtual void OnEditKeyFrame(TimeblockContainer* c, IITimeblock* block, int keyFrameIdx) = 0;
};
//--------------------------------------------------------------------------------
class _EditorCoreExport IITimeblock : public RefObject
{
public:

	virtual IITimeblockContainer* GetParent() const = 0;
	virtual void SetParent(IITimeblockContainer* parent) = 0;

	virtual COLORREF GetBackgroundColor() const = 0;
	virtual void SetBackgroundColor(COLORREF clr) = 0;

	virtual COLORREF GetBorderColor() const = 0;
	virtual void SetBorderColor(COLORREF clr) = 0;

	virtual COLORREF GetSelectBackgroundColor() const = 0;
	virtual void SetSelectBackgroundColor(COLORREF clr) = 0;

	virtual float GetStartTime() const = 0;
	virtual void SetStartTime(float val) = 0;

	virtual float GetTimeLength() const = 0;
	virtual void SetTimeLength(float val) = 0;

	virtual bool IsSelected() const = 0;
	virtual void SetSelected(bool on) = 0;

	virtual const CString& GetCaption() const = 0;
	virtual void SetCaption(const CString& cap) = 0;

	virtual void OnDraw(CDC& dc, const CRect& rc) = 0;
};

SmartPointer(IITimeblock);

//--------------------------------------------------------------------------------
class _EditorCoreExport AbstractTimeblock : public IITimeblock
{
public:

	AbstractTimeblock();
	virtual ~AbstractTimeblock();
	
	virtual IITimeblockContainer* GetParent() const;
	virtual void SetParent(IITimeblockContainer* parent);

	virtual COLORREF GetBackgroundColor() const;
	virtual void SetBackgroundColor(COLORREF clr);

	virtual COLORREF GetBorderColor() const;
	virtual void SetBorderColor(COLORREF clr);

	virtual COLORREF GetSelectBackgroundColor() const;
	virtual void SetSelectBackgroundColor(COLORREF clr);

	virtual float GetStartTime() const;
	virtual void SetStartTime(float val);

	virtual float GetTimeLength() const;
	virtual void SetTimeLength(float val);

	virtual bool IsSelected() const;
	virtual void SetSelected(bool on);

	virtual const CString& GetCaption() const;
	virtual void SetCaption(const CString& cap);

	virtual void OnDraw(CDC& dc, const CRect& rc);

protected:

	IITimeblockContainer* _parent;
	float _startTime;
	float _length;
	bool _isSelected;
	CString _caption;

	COLORREF _bkColor;
	COLORREF _borderColor;
	COLORREF _selectBkColor;

	void NotifyChanged();

	virtual void DrawBackground(CDC& dc, const CRect& rc);
	virtual void DrawText(CDC& dc, const CRect& rc);
	virtual void DrawKeyFrame(CDC& dc, const CRect& rc);
};

//--------------------------------------------------------------------------------

typedef Ogre::vector<IITimeblockPtr>::type TimeblockList;

class _EditorCoreExport TimeblockContainer : public GridCtrl, public IITimeblockContainer
{
public:

	TimeblockContainer(void);
	virtual ~TimeblockContainer(void);

	size_t GetTimeblockCount() const { return _blocks.size(); }
	IITimeblock* GetTimeblock(size_t idx) const;

	void AddTimeblock(IITimeblockPtr block);
	void RemoveTimeblock(size_t idx);
	void RemoveAllTimeblock();

	virtual void CalcMaxTime();
	int GetTimeblockIndex(const IITimeblock* block) const;

	enum EDragFlag
	{
		DragBody,
		DragHead,
		DragTail
	};

	struct SPickResult
	{
		EDragFlag PickPosition;
		IITimeblock* PickedBlock;
	};

	SPickResult Pick(const CPoint& pt);

	ITimeBlockEventListener* GetListenerEx() const;

	bool GetEnableDragBlock() const { return _enableDragBlock; }
	void SetEnableDragBlock(bool val) { _enableDragBlock = val; }

	UINT GetDrawTextFlag() const { return _drawTextFlag; }
	void SetDrawTextFlag(UINT val) { _drawTextFlag = val; }

	virtual IITimeblock* GetSelectedBlock() { return _selectedBlock; }
	virtual UINT GetDrawTextFormat() { return _drawTextFlag; }

protected:

	TimeblockList _blocks;
	IITimeblock* _selectedBlock;
	IITimeblock* _hitBlock;
	
	EDragFlag _dragFlag;
	bool _isDragingBlock;

	bool _enableDragBlock;

	UINT _drawTextFlag;

	virtual void OnPostDrawGrid(CDC& dc, const CRect& rc);
	virtual void OnMouseMove(const CPoint& point);

	virtual void OnDragTail( float dt );
	virtual void OnDragHead( float dt );
	virtual void OnDragBody( float dt );

	virtual void OnLButtonDown(const CPoint& point);
	virtual void OnLButtonUp(const CPoint& point);
	virtual void OnLButtonDblClk(const CPoint& point);

	virtual void OnChildChange(IITimeblock* block);
};
