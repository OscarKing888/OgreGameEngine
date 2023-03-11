#include "stdafx.h"
#include "CommandManager.h"


IActionCommandX::IActionCommandX()
{

}

IActionCommandX::~IActionCommandX()
{

}

//--------------------------------------------------------------------------------

ActionCommandManager::ActionCommandManager()
{

}

ActionCommandManager::~ActionCommandManager()
{

}

void ActionCommandManager::RegisterCommand( const String& name, IActionCommandXPtr cmd )
{
	CommandMap::iterator it = _commands.find(name);
	if(it != _commands.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
			formatString("Command[%s] already exist!", name.c_str()), __FUNCTION__);
	}
	else
	{
		_commands.insert(std::make_pair(name, cmd));
	}
}

IActionCommandXPtr ActionCommandManager::GetCommand( const String& name )
{
	CommandMap::iterator it = _commands.find(name);
	if(it != _commands.end())
	{
		return it->second;
	}
	else
	{
		LogErrorIDf("Editor", "[%s]Command[%s] not found!", __FUNCTION__, name.c_str());
		return 0;
	}
}

const CommandMap& ActionCommandManager::GetAllCommands() const
{
	return _commands;
}

bool ActionCommandManager::Execute( const String& name, const Any& contex )
{
	IActionCommandXPtr cmd = GetCommand(name);
	if(cmd)
	{
		cmd->Execute(contex);
		return true;
	}

	return false;
}