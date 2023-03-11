#pragma once
#include "EnginePrerequisites.h"

///
/// The event listener interface.
///
class _EngineExport IEventLisitener
{
public:

	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast) = 0;
};

//--------------------------------------------------------------------------------
///
/// Event manager for dispatch events, manage listeners.
/// This is immediately dispatching event system.
///
class _EngineExport EventsManager : public Singleton<EventsManager>, public OgeAlloc
{
public:

	void CreateEvent(const String& eventName, const Any& eventContex);

	void AddListener(IEventLisitener* l);
	void RemoveListener(IEventLisitener* l);

	void RemoveAll();

	typedef Ogre::list<IEventLisitener*>::type EventListenerList;

protected:

	EventListenerList _listeners;
	EventListenerList _newListeners;

	void AddAndRemoveListeners();
};
