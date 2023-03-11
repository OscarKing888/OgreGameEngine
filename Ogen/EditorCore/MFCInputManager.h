#pragma once
#include "EditorCorePrerequisites.h"

//--------------------------------------------------------------------------------
///
/// MFC mouse event listener
///
class _EditorCoreExport IMFCMouseListener
{
public:
	virtual BOOL OnLButtonUp(UINT nFlags, CPoint point){return FALSE;}
	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point){return FALSE;}
	virtual BOOL OnMButtonDown(UINT nFlags, CPoint point){return FALSE;}
	virtual BOOL OnMButtonUp(UINT nFlags, CPoint point){return FALSE;}
	virtual BOOL OnMouseMove(UINT nFlags, CPoint point){return FALSE;}
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){return FALSE;}
	virtual BOOL OnRButtonDown(UINT nFlags, CPoint point){return FALSE;}
	virtual BOOL OnRButtonUp(UINT nFlags, CPoint point){return FALSE;}
};

//--------------------------------------------------------------------------------
///
/// MFC key event listener
///
class _EditorCoreExport IMFCKeyListener
{
public:

	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){return FALSE;}
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags){return FALSE;}
};

//--------------------------------------------------------------------------------
///
/// MFC input events manager
///
class _EditorCoreExport MFCInputManager
	: public SingletonMyers<MFCInputManager>
{
public:

	MFCInputManager();

	void AddMouseListener(IMFCMouseListener* ml);
	void RemoveMouseListener(IMFCMouseListener* ml);

	void AddKeyListener(IMFCKeyListener* kl);
	void RemoveKeyListener(IMFCKeyListener* kl);

	bool IsKeyDown(int k) const;
	bool IsShiftDown() const;
	bool IsCtrlDown() const;

	BOOL OnLButtonUp(UINT nFlags, CPoint point);
	BOOL OnLButtonDown(UINT nFlags, CPoint point);
	BOOL OnMButtonDown(UINT nFlags, CPoint point);
	BOOL OnMButtonUp(UINT nFlags, CPoint point);
	BOOL OnMouseMove(UINT nFlags, CPoint point);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	BOOL OnRButtonDown(UINT nFlags, CPoint point);
	BOOL OnRButtonUp(UINT nFlags, CPoint point);

	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:

	typedef vector<IMFCMouseListener*>::type MouseListenerList;
	typedef vector<IMFCKeyListener*>::type KeyListenerList;

	MouseListenerList	_mouseListeners;
	MouseListenerList	_addMouseListeners;
	KeyListenerList		_keyListeners;
	KeyListenerList		_addKeyListeners;

	void ClearMouseListeners();
	void ClearKeyListeners();
	void UpdateListenersModify();
};