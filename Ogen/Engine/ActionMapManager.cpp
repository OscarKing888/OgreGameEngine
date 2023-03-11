#include "stdafx.h"
#include "ActionMapManager.h"

template<> ActionMapManager* Singleton<ActionMapManager>::ms_Singleton = 0;

//--------------------------------------------------------------------------------
// ActionCommand
//--------------------------------------------------------------------------------
ActionCommand::ActionCommand(const String& name, const String& description)
: _name(name)
, _description(description)
{
}
//--------------------------------------------------------------------------------
ActionCommand::~ActionCommand()
{
}
//--------------------------------------------------------------------------------
String ActionCommand::GetName() const
{
	return _name;
}
//--------------------------------------------------------------------------------
String ActionCommand::GetDescription() const
{
	return _description;
}
//--------------------------------------------------------------------------------
// ActionMapManager
//--------------------------------------------------------------------------------
ActionMapManager::ActionMapManager()
{
}
//--------------------------------------------------------------------------------
ActionMapManager::~ActionMapManager()
{
}
//--------------------------------------------------------------------------------
void ActionMapManager::SetActionMap(OIS::KeyCode kc, ActionCommandPtr cmd)
{
	_actionMap[kc] = cmd;
}
//--------------------------------------------------------------------------------
void ActionMapManager::RemoveActionMap(OIS::KeyCode kc)
{
	ActionMap::iterator it = _actionMap.find(kc);
	if(it != _actionMap.end())
	{
		_actionMap.erase(it);
	}
}
//--------------------------------------------------------------------------------
ActionCommandPtr ActionMapManager::GetAction(OIS::KeyCode kc)
{
	ActionMap::iterator it = _actionMap.find(kc);
	if(it != _actionMap.end())
	{
		return it->second;
	}

	return 0;
}
//--------------------------------------------------------------------------------
bool ActionMapManager::HandleKeyPressed(OIS::KeyCode kc)
{
	ActionMap::iterator it = _actionMap.find(kc);
	if(it != _actionMap.end())
	{
		it->second->Execute();
		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------
bool ActionMapManager::HandleKeyReleased(OIS::KeyCode kc)
{
	ActionMap::iterator it = _actionMap.find(kc);
	if(it != _actionMap.end())
	{
		it->second->Execute();
		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------
const ActionMap& ActionMapManager::GetActionMap() const
{
	return _actionMap;
}
//--------------------------------------------------------------------------------