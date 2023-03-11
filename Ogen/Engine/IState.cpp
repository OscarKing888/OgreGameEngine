// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-27
// *************************************************************************************


#include "stdafx.h"
#include "IState.h"
#include <typeinfo>
#include "GameApp.h"

using namespace std;
//--------------------------------------------------------------------------------
IState::IState(IStateManager* mgr)
: _stateMgr(mgr)
, _commandID(0)
{
}
//--------------------------------------------------------------------------------
IState::~IState()
{
	_stateMgr = 0;
}
//--------------------------------------------------------------------------------
void IState::ActiveState(const Ogre::String& stateID)
{
	assert(_stateMgr);
	_stateMgr->ActiveState(stateID);
}
//--------------------------------------------------------------------------------
const Ogre::String& IState::GetID() const
{
	return _stateID;
}
//--------------------------------------------------------------------------------
int IState::GetCommandID() const
{
	return _commandID;
}
//--------------------------------------------------------------------------------
StateManager::StateManager()
: _activeState(0)
{
	GameApp::getSingleton().AddUpdateable(this);
}
//--------------------------------------------------------------------------------
StateManager::~StateManager()
{
	GameApp::getSingleton().RemoveUpdateable(this);
}
//--------------------------------------------------------------------------------
struct FFindGameState
{
	Ogre::String	TargetID;
	bool operator()(IStatePtr pState)
	{
		NoCaseEqual compare;
		return compare(pState->GetID(), TargetID);
	}
};
//--------------------------------------------------------------------------------
IStatePtr StateManager::GetState(const Ogre::String& id) const
{
	FFindGameState f;
	f.TargetID = id;
	StateList::const_iterator i = std::find_if(_stateList.begin(), _stateList.end(), f);
	if(i != _stateList.end())
	{
		return *i;
	}

	return 0;
}
//--------------------------------------------------------------------------------
struct FFindGameStateByCommandID
{
	int	TargetID;
	bool operator()(IStatePtr pState)
	{
		return pState->GetCommandID() == TargetID;
	}
};
//--------------------------------------------------------------------------------
IStatePtr StateManager::GetState( int id ) const
{
	FFindGameStateByCommandID f;
	f.TargetID = id;
	StateList::const_iterator i = std::find_if(_stateList.begin(), _stateList.end(), f);
	if(i != _stateList.end())
	{
		return *i;
	}

	return 0;
}
//--------------------------------------------------------------------------------
void StateManager::AddState(IStatePtr pState)
{	
	if(!GetState(pState->GetID()))
	{
		pState->SetStateManager(this);
		_stateList.push_back(pState);
	}
	else
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
			formatString("State already exist![%s]",
			typeid(*pState).name()).c_str(), "StateManager::AddState");
	}
}
//--------------------------------------------------------------------------------
void StateManager::RemoveState(const Ogre::String& stateID)
{
	FFindGameState f;
	f.TargetID = stateID;
	StateList::iterator i = find_if(_stateList.begin(), _stateList.end(), f);
	if(i != _stateList.end())
	{
		_stateList.erase(i);
	}
	else
	{
		LogErrorIDf("Engine", ("[StateManager::RemoveState] State not found:[%d]"), stateID);
	}
}
//--------------------------------------------------------------------------------
void StateManager::ActiveState(const Ogre::String& stateID)
{
	IStatePtr state = GetState(stateID);
	assert(state);
	if(_activeState == state)
	{
		return;
	}

	Ogre::String fromID;

	if(_activeState)
	{
		fromID = _activeState->GetID();
		NotifyPreStateChange(fromID, stateID);

		_lastActiveState = fromID;
		_activeState->OnInActive();
	}
	else
	{
		NotifyOnStateInit(stateID);
	}

	_activeState = state;

#if !defined(_PRODUCT_)
	const type_info* pTypeInfo = &typeid(*_activeState);
	const char* pszClassName = pTypeInfo->name();
	String str = pszClassName;
	size_t i = str.find_last_of(("::"));
	if(i != String::npos)
	{
		str = str.substr(i + 1);
	}
	else
	{
		const String classStr = ("class ");
		str = str.substr(classStr.length());
	}

	LogConsoleIDf(("EngineState"), ("+++[StateManager::ActiveState][%s]"), str.c_str());
#endif

	if(_activeState != 0)
	{
		_activeState->OnActive();

		NotifyPostStateChange(fromID, stateID);
		return;
	}

	OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
		formatString("State not found![%d]", stateID).c_str(), "StateManager::ActiveState");
}
//--------------------------------------------------------------------------------
void StateManager::ActiveState( int commandID )
{
	IStatePtr state = GetState(commandID);
	if(state)
	{
		ActiveState(state->GetID());
	}
}
//--------------------------------------------------------------------------------
const Ogre::String&	 StateManager::GetActiveStateID() const
{
	return	_activeState->GetID();
}
//--------------------------------------------------------------------------------
int StateManager::GetActiveStateCommandID() const
{
	return	_activeState->GetCommandID();
}
//--------------------------------------------------------------------------------
IStatePtr StateManager::GetActiveState() const
{
	return _activeState;
}
//--------------------------------------------------------------------------------
void StateManager::Update(float fTime)
{
	//assert(_activeState);
	if(_activeState)
	{
		_activeState->Update(fTime);
	}
}
//--------------------------------------------------------------------------------
void StateManager::RemoveAllStates()
{
	if(_activeState)
	{
		_activeState = 0;
	}

	_stateList.clear();
}
//--------------------------------------------------------------------------------
bool StateManager::HasState(const Ogre::String& stateID) const
{
	return GetState(stateID) != 0;
}
//--------------------------------------------------------------------------------
const Ogre::String& StateManager::GetPreviousStateID() const
{
	return _lastActiveState;
}
//--------------------------------------------------------------------------------
void StateManager::RevertToPreviousState()
{
	ActiveState(_lastActiveState);
}
//--------------------------------------------------------------------------------
void StateManager::AddListener(IStateListener* l)
{
#ifdef _DEBUG
	StateListenerList::iterator i = std::find(_listener.begin(), _listener.end(), l);
	assert(i == _listener.end());
#endif
	_listener.push_back(l);
}
//--------------------------------------------------------------------------------
void StateManager::RemoveListener(IStateListener* l)
{
	StateListenerList::iterator i = std::find(_listener.begin(), _listener.end(), l);
	assert(i != _listener.end());

	if(i != _listener.end())
	{
		_listener.erase(i);
	}
}
//--------------------------------------------------------------------------------
void StateManager::NotifyPreStateChange(const Ogre::String& from, const Ogre::String& to)
{
	StateListenerList::iterator i = _listener.begin();
	for (; i != _listener.end(); ++i)
	{
		IStateListener* l = *i;
		l->OnPreStateChange(this, from, to);
	}
}
//--------------------------------------------------------------------------------
void StateManager::NotifyPostStateChange(const Ogre::String& from, const Ogre::String& to)
{
	StateListenerList::iterator i = _listener.begin();
	for (; i != _listener.end(); ++i)
	{
		IStateListener* l = *i;
		l->OnPostStateChange(this, from, to);
	}
}
//--------------------------------------------------------------------------------
void StateManager::NotifyOnStateInit(const Ogre::String& active)
{
	StateListenerList::iterator i = _listener.begin();
	for (; i != _listener.end(); ++i)
	{
		IStateListener* l = *i;
		l->OnStateInit(this, active);
	}
}
//--------------------------------------------------------------------------------
void StateManager::PushState( const Ogre::String& stateID )
{
	if(_activeState)
	{
		_stateStack.push_back(_activeState->GetID());
	}

	ActiveState(stateID);
}
//--------------------------------------------------------------------------------
void StateManager::PopState()
{
	if(!_stateStack.empty())
	{
		ActiveState(_stateStack.back());
		_stateStack.pop_back();
	}
}
//--------------------------------------------------------------------------------
Ogre::StringVector StateManager::GetStateNameList() const
{
	Ogre::StringVector lst;
	StateList::const_iterator it = _stateList.begin();
	for (; it != _stateList.end(); ++it)
	{
		lst.push_back((*it)->GetID());
	}

	return lst;
}
//--------------------------------------------------------------------------------
Ogre::StringVector StateManager::GetStateStack() const
{
	return _stateStack;
}
