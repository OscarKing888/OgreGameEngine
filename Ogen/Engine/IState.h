// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-27
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"
#include "IUpdateAble.h"
#include "RefObject.h"

class IStateManager;

class _EngineExport IState : virtual public RefObject
{
public:

	explicit IState(IStateManager* mgr);

	virtual ~IState();

	virtual const Ogre::String& GetID() const;
	virtual int GetCommandID() const;

	virtual void		OnActive(){}
	virtual void		OnInActive(){}
	virtual void		Update(float /*deltaTime*/){}

	IStateManager* GetStateManager() const
	{
		return _stateMgr;
	}

	void SetStateManager(IStateManager* stateManager)
	{
		_stateMgr = stateManager;
	}

	void ActiveState(const Ogre::String& stateID);

protected:

	Ogre::String _stateID;
	int _commandID;
	IStateManager*	_stateMgr;
};

SmartPointer(IState);

//--------------------------------------------------------------------------------------

typedef Ogre::vector<IStatePtr>::type StateList;

class _EngineExport IStateListener
{
public:

	virtual void OnStateInit(IStateManager* mgr, const Ogre::String& active) = 0;
	virtual void OnPreStateChange(IStateManager* mgr, const Ogre::String& fromStateID, const Ogre::String& toStateID) = 0;
	virtual void OnPostStateChange(IStateManager* mgr, const Ogre::String& fromStateID, const Ogre::String& toStateID) = 0;
};

class _EngineExport IStateManager
{
public:

	virtual ~IStateManager(){}

	virtual void	AddState(IStatePtr pState) = 0;
	virtual void	RemoveState(const Ogre::String& stateID) = 0;
	virtual void	RemoveAllStates() = 0;
	virtual bool	HasState(const Ogre::String& stateID) const = 0;

	virtual void	ActiveState(const Ogre::String& stateID) = 0;
	virtual void	ActiveState(int stateID) = 0;
	virtual const Ogre::String&		GetActiveStateID() const = 0;
	virtual int GetActiveStateCommandID() const = 0;

	virtual IStatePtr GetActiveState() const = 0;
	virtual IStatePtr GetState(const Ogre::String& id) const = 0;
	virtual IStatePtr GetState(int id) const = 0;
	virtual const Ogre::String& GetPreviousStateID() const = 0;
	virtual void	RevertToPreviousState() = 0;

	virtual void AddListener(IStateListener* l) = 0;
	virtual void RemoveListener(IStateListener* l) = 0;

	virtual void PushState(const Ogre::String& stateID) = 0;
	virtual void PopState() = 0;

	virtual Ogre::StringVector GetStateNameList() const = 0;
	virtual Ogre::StringVector GetStateStack() const = 0;
};

class _EngineExport StateManager :
	public IStateManager,
	public IUpdateAble,
	public OgeAlloc
{
public:

	StateManager();
	virtual ~StateManager();

	virtual void	AddState(IStatePtr pState);
	virtual void	RemoveState(const Ogre::String& stateID);
	virtual void	RemoveAllStates();
	virtual bool	HasState(const Ogre::String& stateID) const;

	//virtual void    DelayActiveState(int stateID, float time){}
	virtual void	ActiveState(const Ogre::String& stateID);
	virtual void	ActiveState(int commandID);
	virtual const Ogre::String&		GetActiveStateID() const;
	virtual int GetActiveStateCommandID() const;
	virtual IStatePtr GetActiveState() const;
	virtual IStatePtr GetState(const Ogre::String& id) const;
	virtual IStatePtr GetState(int id) const;
	virtual const Ogre::String& GetPreviousStateID() const;
	virtual void	RevertToPreviousState();

	virtual void Update(float deltaTime);

	virtual void AddListener(IStateListener* l);
	virtual void RemoveListener(IStateListener* l);

	virtual void PushState(const Ogre::String& stateID);
	virtual void PopState();

	virtual Ogre::StringVector GetStateNameList() const;
	virtual Ogre::StringVector GetStateStack() const;

protected:

	typedef Ogre::list<IStateListener*>::type StateListenerList;
	StateListenerList	_listener;

	void NotifyPreStateChange(const Ogre::String& from, const Ogre::String& to);
	void NotifyPostStateChange(const Ogre::String& from, const Ogre::String& to);
	void NotifyOnStateInit(const Ogre::String& active);

	IStatePtr		_activeState;
	StateList		_stateList;
	Ogre::String	_lastActiveState;

	Ogre::StringVector _stateStack;
};

