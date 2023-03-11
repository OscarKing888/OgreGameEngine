#include "stdafx.h"
#include "MFCInputManager.h"

MFCInputManager::MFCInputManager()
{
}

void MFCInputManager::ClearMouseListeners()
{
	MouseListenerList::iterator i = _mouseListeners.begin();
	while (i != _mouseListeners.end())
	{
		if((*i) == 0)
		{
			i = _mouseListeners.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void MFCInputManager::ClearKeyListeners()
{
	KeyListenerList::iterator i = _keyListeners.begin();
	while (i != _keyListeners.end())
	{
		if((*i) == 0)
		{
			i = _keyListeners.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void MFCInputManager::AddMouseListener(IMFCMouseListener* ml)
{
	MouseListenerList::iterator i = std::find(_mouseListeners.begin(), _mouseListeners.end(), ml);
	if(i == _mouseListeners.end())
	{
		i = std::find(_addMouseListeners.begin(), _addMouseListeners.end(), ml);
		if(i == _addMouseListeners.end())
		{
			_addMouseListeners.push_back(ml);
			return;
		}
	}

	assert(false);
}

void MFCInputManager::RemoveMouseListener(IMFCMouseListener* ml)
{
	MouseListenerList::iterator i = std::find(_mouseListeners.begin(), _mouseListeners.end(), ml);
	if(i != _mouseListeners.end())
	{
		//_mouseListeners.erase(i);
		*i = 0;
	}

	i = std::find(_addMouseListeners.begin(), _addMouseListeners.end(), ml);
	if(i != _addMouseListeners.end())
	{
		*i = 0;
	}
}

void MFCInputManager::AddKeyListener(IMFCKeyListener* kl)
{
	KeyListenerList::iterator i = std::find(_keyListeners.begin(), _keyListeners.end(), kl);
	if(i == _keyListeners.end())
	{
		i = std::find(_addKeyListeners.begin(), _addKeyListeners.end(), kl);
		if(i == _addKeyListeners.end())
		{
			_addKeyListeners.push_back(kl);
			return;
		}
	}

	assert(false);
}

void MFCInputManager::RemoveKeyListener(IMFCKeyListener* kl)
{
	KeyListenerList::iterator i = std::find(_keyListeners.begin(), _keyListeners.end(), kl);
	if(i != _keyListeners.end())
	{
		//_keyListeners.erase(i);
		*i = 0;
	}

	i = std::find(_addKeyListeners.begin(), _addKeyListeners.end(), kl);
	if(i != _addKeyListeners.end())
	{
		*i = 0;
	}
}

bool MFCInputManager::IsKeyDown(int k) const
{
	return (GetKeyState(k) & 0x8000) != 0;
}

bool MFCInputManager::IsShiftDown() const
{
	return IsKeyDown(VK_LSHIFT);
}

bool MFCInputManager::IsCtrlDown() const
{
	return IsKeyDown(VK_LCONTROL);
}

void MFCInputManager::UpdateListenersModify()
{
	{
		MouseListenerList::iterator i = _addMouseListeners.begin();
		for (; i != _addMouseListeners.end(); ++i)
		{
			if(*i)
			{
				_mouseListeners.push_back(*i);
			}
		}
	}

	_addMouseListeners.clear();

	{
		KeyListenerList::iterator i = _addKeyListeners.begin();
		for (; i != _addKeyListeners.end(); ++i)
		{
			if(*i)
			{
				_keyListeners.push_back(*i);
			}
		}
	}

	_addKeyListeners.clear();

	ClearMouseListeners();
	ClearKeyListeners();
}

BOOL MFCInputManager::OnLButtonUp(UINT nFlags, CPoint point)
{
	UpdateListenersModify();

	MouseListenerList::iterator i = _mouseListeners.begin();
	for (; i != _mouseListeners.end(); ++i)
	{
		if((*i) && (*i)->OnLButtonUp(nFlags, point))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL MFCInputManager::OnLButtonDown(UINT nFlags, CPoint point)
{
	UpdateListenersModify();

	MouseListenerList::iterator i = _mouseListeners.begin();
	for (; i != _mouseListeners.end(); ++i)
	{
		if((*i) && (*i)->OnLButtonDown(nFlags, point))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL MFCInputManager::OnMButtonDown(UINT nFlags, CPoint point)
{
	UpdateListenersModify();

	MouseListenerList::iterator i = _mouseListeners.begin();
	for (; i != _mouseListeners.end(); ++i)
	{
		if((*i) && (*i)->OnMButtonDown(nFlags, point))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL MFCInputManager::OnMButtonUp(UINT nFlags, CPoint point)
{
	UpdateListenersModify();

	MouseListenerList::iterator i = _mouseListeners.begin();
	for (; i != _mouseListeners.end(); ++i)
	{
		if((*i) && (*i)->OnMButtonUp(nFlags, point))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL MFCInputManager::OnMouseMove(UINT nFlags, CPoint point)
{
	UpdateListenersModify();

	MouseListenerList::iterator i = _mouseListeners.begin();
	for (; i != _mouseListeners.end(); ++i)
	{
		if((*i) && (*i)->OnMouseMove(nFlags, point))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL MFCInputManager::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UpdateListenersModify();

	MouseListenerList::iterator i = _mouseListeners.begin();
	for (; i != _mouseListeners.end(); ++i)
	{
		if((*i) && (*i)->OnMouseWheel(nFlags, zDelta, pt))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL MFCInputManager::OnRButtonDown(UINT nFlags, CPoint point)
{
	UpdateListenersModify();

	MouseListenerList::iterator i = _mouseListeners.begin();
	for (; i != _mouseListeners.end(); ++i)
	{
		if((*i) && (*i)->OnRButtonDown(nFlags, point))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL MFCInputManager::OnRButtonUp(UINT nFlags, CPoint point)
{
	UpdateListenersModify();

	MouseListenerList::iterator i = _mouseListeners.begin();
	for (; i != _mouseListeners.end(); ++i)
	{
		if((*i) && (*i)->OnRButtonUp(nFlags, point))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL MFCInputManager::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UpdateListenersModify();

	KeyListenerList::iterator i = _keyListeners.begin();
	for (; i != _keyListeners.end(); ++i)
	{
		if((*i) && (*i)->OnKeyDown(nChar, nRepCnt, nFlags))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL MFCInputManager::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UpdateListenersModify();

	KeyListenerList::iterator i = _keyListeners.begin();
	for (; i != _keyListeners.end(); ++i)
	{
		if((*i) && (*i)->OnKeyUp(nChar, nRepCnt, nFlags))
		{
			return TRUE;
		}
	}
	return FALSE;
}