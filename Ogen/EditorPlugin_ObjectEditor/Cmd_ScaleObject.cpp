#include "stdafx.h"
#include "Cmd_ScaleObject.h"

Cmd_ScaleObject::Cmd_ScaleObject(VisualObject* obj, const Vector3& newScale)
								 : _object(obj)
								 , _newScale(newScale)
{
}

Cmd_ScaleObject::~Cmd_ScaleObject()
{

}

ICommandXPtr Cmd_ScaleObject::Execute()
{
	Vector3 oldScale = _object->GetScale();
	_object->SetScale(_newScale);
	EventsManager::getSingleton().CreateEvent(
		PropertyEvents::PropertyChanged, Any(String("Scale")));
	return OGRE_NEW Cmd_ScaleObject(_object, oldScale);
}