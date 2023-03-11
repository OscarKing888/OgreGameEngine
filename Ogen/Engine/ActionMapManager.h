#pragma once
#include "EnginePrerequisites.h"
#include "ICommandX.h"

///
/// Action command for invoke functions
///
class _EngineExport ActionCommand : public RefObject
{
public:
	ActionCommand(const String& name, const String& description);
	virtual ~ActionCommand();
	/// Return the command name
	virtual String GetName() const;
	/// Return the command description
	virtual String GetDescription() const;
	/// Execute to call function
	virtual void Execute() = 0;

protected:

	String _name;
	String _description;
};

SmartPointer(ActionCommand);

typedef map<OIS::KeyCode, ActionCommandPtr>::type ActionMap;

///
/// Action command and keycode map manager
///
class _EngineExport ActionMapManager : public Singleton<ActionMapManager>, public OgeAlloc
{
public:

	ActionMapManager();
	virtual ~ActionMapManager();

	/// Set a command with keycode
	void SetActionMap(OIS::KeyCode kc, ActionCommandPtr cmd);
	/// Remove a action command with specified keycode
	void RemoveActionMap(OIS::KeyCode kc);
	/// Get action command from keycode
	ActionCommandPtr GetAction(OIS::KeyCode kc);
	/// Check key press events, return true if handled
	bool HandleKeyPressed(OIS::KeyCode kc);
	/// Check key release events, return true if handled
	bool HandleKeyReleased(OIS::KeyCode kc);
	/// Return the action map
	const ActionMap& GetActionMap() const;

protected:
	
	ActionMap _actionMap;
};