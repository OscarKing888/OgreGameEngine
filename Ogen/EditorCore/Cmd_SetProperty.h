#pragma once
#include "EditorCorePrerequisites.h"

class Cmd_SetProperty : public ICommandX
{
public:

	Cmd_SetProperty(IPropertyObject* gameObject, const String& propName, const String& newVal);
	virtual ~Cmd_SetProperty();
	virtual ICommandXPtr Execute();

protected:

	IPropertyObject* _gameObject;
	String	_propName;
	String	_propValue;
};