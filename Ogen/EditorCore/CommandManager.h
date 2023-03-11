#pragma once
#include "EditorCorePrerequisites.h"

SmartPointer(IActionCommandX);

class _EditorCoreExport IActionCommandX : public RefObject
{
public:

	IActionCommandX();
	virtual ~IActionCommandX();
	virtual void Execute(const Any& contex) = 0;
};

//--------------------------------------------------------------------------------

typedef map<String, IActionCommandXPtr>::type CommandMap;

class _EditorCoreExport ActionCommandManager
{
public:

	ActionCommandManager();
	virtual ~ActionCommandManager();

	void RegisterCommand(const String& name, IActionCommandXPtr cmd);
	IActionCommandXPtr GetCommand(const String& name);

	const CommandMap& GetAllCommands() const;
	
	bool Execute(const String& name, const Any& contex);

protected:

	CommandMap _commands;
};