#include "stdafx.h"
#include "Events.h"

template<> EventsManager* Singleton<EventsManager>::ms_Singleton = 0;

void EventsManager::CreateEvent(const String& eventName, const Any& eventContex)
{
	AddAndRemoveListeners();

	EventListenerList ls = _listeners;
	bool stopCast = false;
	for (EventListenerList::iterator i = ls.begin(); i != ls.end(); ++i)
	{
		IEventLisitener* l = *i;
		if(l)
		{
			l->OnEvents(eventName, eventContex, stopCast);
			if(stopCast)
			{
				return;
			}
		}
	}
}
//--------------------------------------------------------------------------------
void EventsManager::AddListener(IEventLisitener* l)
{
#ifdef _DEBUG
	EventListenerList::iterator it = std::find(_listeners.begin(), _listeners.end(), l);
	if(it != _listeners.end())
	{
		assert(false && "[EventsManager::AddListener] listener already exist!");
		return;
	}
	
	it = std::find(_newListeners.begin(), _newListeners.end(), l);
	if(it != _newListeners.end())
	{
		assert(false && "[EventsManager::AddListener] listener already exist!");
		return;
	}
#endif

	_newListeners.push_back(l);
}
//--------------------------------------------------------------------------------
void EventsManager::RemoveListener(IEventLisitener* l)
{
	EventListenerList::iterator it = std::find(_listeners.begin(), _listeners.end(), l);
	if(it != _listeners.end())
	{
		*it = 0;
	}

	it = std::find(_newListeners.begin(), _newListeners.end(), l);
	if(it != _newListeners.end())
	{
		*it = 0;
	}
}
//--------------------------------------------------------------------------------
void EventsManager::RemoveAll()
{
	_listeners.clear();
	_newListeners.clear();
}
//--------------------------------------------------------------------------------
void EventsManager::AddAndRemoveListeners()
{
	// first do add
	_listeners.insert(_listeners.end(), _newListeners.begin(), _newListeners.end());
	_newListeners.clear();

	// and then removed NULL
	for (EventListenerList::iterator i = _listeners.begin(); i != _listeners.end();)
	{
		IEventLisitener* l = *i;
		if(l == 0)
		{
			i = _listeners.erase(i);
		}
		else
		{
			++i;
		}
	}
}
