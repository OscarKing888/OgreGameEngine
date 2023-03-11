#include "stdafx.h"
#include "Cmd_SetProperty.h"

Cmd_SetProperty::Cmd_SetProperty(IPropertyObject* gameObject,
								 const String& propName, const String& newVal)
								 : _gameObject(gameObject)
								 , _propName(propName)
								 , _propValue(newVal)
{

}

Cmd_SetProperty::~Cmd_SetProperty()
{

}

ICommandXPtr Cmd_SetProperty::Execute()
{
	String oldVal = _gameObject->GetProperty(_propName);
	_gameObject->SetProperty(_propName, _propValue);
	return OGRE_NEW Cmd_SetProperty(_gameObject, _propName, oldVal);
}
