// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-03-04
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"
#include "RefObject.h"
#include <stack>

SmartPointer(ICommandX);

class _EngineExport ICommandX : public RefObject
{
public:

	ICommandX();
	virtual ~ICommandX();
	virtual ICommandXPtr Execute() = 0;
};

//--------------------------------------------------------------------------------------

typedef stack<ICommandXPtr>::type CommandStackContainer;

class _EngineExport CommandStack
{
public:

	CommandStack()
	{
	}

	CommandStack(const CommandStack& src)
		: _commandStack(src._commandStack)
	{
	}

	void Push(ICommandXPtr spCmd, bool bExecute = true)
	{
		if(bExecute)
		{
			_commandStack.push(spCmd->Execute());
		}
		else
		{
			_commandStack.push(spCmd);
		}
	}

	ICommandXPtr Top()
	{
		return _commandStack.top();
	}

	void Pop()
	{
		_commandStack.pop();
	}

	void Clear()
	{
		while(!_commandStack.empty())
		{
			_commandStack.pop();
		}
	}

	const CommandStack& operator =(const CommandStack& src)
	{
		if(&src != this)
		{
			_commandStack = src._commandStack;
		}
		return *this;
	}

	size_t Count() const
	{
		return _commandStack.size();
	}

	bool IsEmpty() const
	{
		return _commandStack.empty();
	}

protected:

	CommandStackContainer		_commandStack;
};

//--------------------------------------------------------------------------------------

class _EngineExport BatchCommand : public ICommandX
{
public:

	BatchCommand();
	virtual ~BatchCommand();

	BatchCommand(const CommandStack& commands)
		:_commands(commands)
	{
		_commands = commands;
	}

	ICommandXPtr Execute()
	{
		CommandStack cmds;
		while(_commands.Count() > 0)
		{
			ICommandXPtr cmd = _commands.Top();
			_commands.Pop();
			cmds.Push(cmd);
		}
		return OGRE_NEW BatchCommand(cmds); 
	}

protected:

	CommandStack _commands;
};

SmartPointer(BatchCommand);

//--------------------------------------------------------------------------------------

class _EngineExport ICommandXExecuteObserver
{
public:

	virtual void OnCommandExecuted() = 0;
};

typedef std::vector<ICommandXExecuteObserver*>		CommandExecuteObserverVector;

//--------------------------------------------------------------------------------------	

class _EngineExport CommandHistory: public RefObject
{
public:

	CommandHistory()
		: _isBatchMode(false)
	{

	}

	bool IsUndoEnable() const
	{
		return !_commandsUndo.IsEmpty();
	}

	bool IsRedoEnable() const
	{
		return !_commandsRedo.IsEmpty();
	}

	void Undo()
	{
		if(IsBatchMode())	// if the command hasn't been completed
		{
			EndBatch();
			Undo();
		}

		ICommandXPtr cmd = _commandsUndo.Top();
		_commandsRedo.Push(cmd);
		_commandsUndo.Pop();

		NotifyCommandExecuted();
	}

	void Redo()
	{
		if(IsBatchMode())	// if the command hasn't been completed
		{
			Undo();
		}
		ICommandXPtr cmd = _commandsRedo.Top();
		_commandsUndo.Push(cmd);
		_commandsRedo.Pop();

		NotifyCommandExecuted();
	}

	void Clear()
	{
		EndBatch();

		bool doNotify = !_commandsRedo.IsEmpty() || !_commandsUndo.IsEmpty();

		_commandsRedo.Clear();
		_commandsUndo.Clear();

		if(doNotify)
		{
			NotifyCommandExecuted();
		}
	}

	void AddCommand(ICommandXPtr cmd, bool execute = true)
	{
		if(_isBatchMode)
		{
			_commandsBatch.Push(cmd, execute);
		}
		else
		{
			_commandsUndo.Push(cmd, execute);
		}
		_commandsRedo.Clear();

		NotifyCommandExecuted();
	}

	bool IsBatchMode() const
	{
		return _isBatchMode;
	}

	void BeginBatch()
	{
		_isBatchMode = true;
	}

	void EndBatch()
	{
		if(!_commandsBatch.IsEmpty())
		{
			BatchCommandPtr cmd(OGRE_NEW BatchCommand(_commandsBatch));
			_commandsUndo.Push(cmd, false);				
			_commandsRedo.Clear();
			_commandsBatch.Clear();
			NotifyCommandExecuted();
		}
		_isBatchMode = false;
	}

	void AddCommandExecuteObserver(ICommandXExecuteObserver* pObserver)
	{
		CommandExecuteObserverVector::iterator i = std::find(_cmdExecuteObservers.begin(), _cmdExecuteObservers.end(), pObserver);
		if(i == _cmdExecuteObservers.end())
		{
			_cmdExecuteObservers.push_back(pObserver);
		}
		else
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, "Observer already exist!", "CommandHistory::AddCommandExecuteObserver");
		}
	}

	void RemoveCommandExecuteObserver(ICommandXExecuteObserver* pObserver)
	{
		CommandExecuteObserverVector::iterator i = std::find(_cmdExecuteObservers.begin(), _cmdExecuteObservers.end(), pObserver);
		if(i != _cmdExecuteObservers.end())
		{
			_cmdExecuteObservers.erase(i);
		}
		else
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Observer not exist!", "CommandHistory::RemoveCommandExecuteObserver");
		}
	}

protected:

	void NotifyCommandExecuted()
	{
		for(CommandExecuteObserverVector::iterator i = _cmdExecuteObservers.begin(); i != _cmdExecuteObservers.end(); ++i)
		{
			(*i)->OnCommandExecuted();
		}
	}

	CommandExecuteObserverVector	_cmdExecuteObservers;

	bool				_isBatchMode;

	CommandStack		_commandsRedo;
	CommandStack		_commandsUndo;
	CommandStack		_commandsBatch;
};
//----------------------------------------------------------------------------------------------

class ObjectSelection;

class _EngineExport IObjectSelectionChangeObserver
{
public:

	virtual void OnSelectObject(IPropertyObject* obj) = 0;
	virtual void OnDeselectObject(IPropertyObject* obj) = 0;
	virtual void OnClearObjects() = 0;
};

typedef vector<IObjectSelectionChangeObserver*>::type ObjectSelectionChangeObserverList;

//--------------------------------------------------------------------------------------

class _EngineExport ObjectList : public std::vector<IPropertyObject*>
{
};

class _EngineExport ObjectSelection
{
public:

	const ObjectList& GetObjects()
	{
		return _selectedObjects;
	}

	void Add(IPropertyObject* obj, bool isMultiSelect = false)
	{
		if (!isMultiSelect)
		{
			Clear();
			_selectedObjects.push_back(obj);
			NotifySelect(obj);
		}
		else
		{
			ObjectList::iterator i = std::find(_selectedObjects.begin(), _selectedObjects.end(), obj);
			if (i != _selectedObjects.end())
			{
				_selectedObjects.erase(i);
				NotifyDeselect(obj);
			}
			else
			{
				_selectedObjects.push_back(obj);
				NotifySelect(obj);
			}
		}

		if(IsEmpty())
		{
			NotifyClear();
		}
	}

	void Clear()
	{
		ObjectList t = _selectedObjects;
		_selectedObjects.clear();
		for(ObjectList::iterator i = t.begin(); i != t.end(); ++i)
		{
			NotifyDeselect(*i);
		}

		NotifyClear();
	}

	void AddSelectionChangeObserver(IObjectSelectionChangeObserver* pObserver)
	{
		ObjectSelectionChangeObserverList::iterator i = std::find(_vSelectionObserver.begin(), _vSelectionObserver.end(), pObserver);
		if(i == _vSelectionObserver.end())
		{
			_vSelectionObserver.push_back(pObserver);
		}
		else
		{
			assert(false && "[ObjectSelection::AddSelectionChangeObserver] observer already exist!");
			//throw Util::throwf(_T("[ObjectSelection::AddSelectionChangeObserver] observer already exist!"));
		}
	}

	void RemoveSelectionChangeObserver(IObjectSelectionChangeObserver* pObserver)
	{
		ObjectSelectionChangeObserverList::iterator i = std::find(_vSelectionObserver.begin(), _vSelectionObserver.end(), pObserver);
		if(i != _vSelectionObserver.end())
		{
			_vSelectionObserver.erase(i);
		}
		else
		{
			assert(false && "[ObjectSelection::RemoveSelectionChangeObserver] observer not exist!");
			//throw Util::throwf(_T("[ObjectSelection::RemoveSelectionChangeObserver] observer not exist!"));
		}
	}

	bool IsEmpty()
	{
		return _selectedObjects.empty();
	}

	size_t Count()
	{
		return _selectedObjects.size();
	}

protected:

	void NotifySelect(IPropertyObject* obj)
	{
		for(ObjectSelectionChangeObserverList::iterator i = _vSelectionObserver.begin(); i != _vSelectionObserver.end(); ++i)
		{
			(*i)->OnSelectObject(obj);
		}
	}

	void NotifyDeselect(IPropertyObject* obj)
	{
		for(ObjectSelectionChangeObserverList::iterator i = _vSelectionObserver.begin(); i != _vSelectionObserver.end(); ++i)
		{
			(*i)->OnDeselectObject(obj);
		}
	}

	void NotifyClear()
	{
		for(ObjectSelectionChangeObserverList::iterator i = _vSelectionObserver.begin(); i != _vSelectionObserver.end(); ++i)
		{
			(*i)->OnClearObjects();
		}
	}

	ObjectSelectionChangeObserverList		_vSelectionObserver;
	ObjectList							_selectedObjects;

}; // class
