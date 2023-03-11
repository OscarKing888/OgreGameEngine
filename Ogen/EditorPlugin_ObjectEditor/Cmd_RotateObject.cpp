#include "stdafx.h"
#include "Cmd_RotateObject.h"

Cmd_RotateObject::Cmd_RotateObject(VisualObject* obj, const Quaternion& newRot)
								 : _object(obj)
								 , _newRot(newRot)
{
}

Cmd_RotateObject::~Cmd_RotateObject()
{

}

ICommandXPtr Cmd_RotateObject::Execute()
{
	Quaternion oldRot = _object->GetOrientation();
	_object->SetOrientation(_newRot);
	EventsManager::getSingleton().CreateEvent(
		PropertyEvents::PropertyChanged, Any(String("Orientation")));

	return OGRE_NEW Cmd_RotateObject(_object, oldRot);
}
