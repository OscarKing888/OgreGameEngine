#include "stdafx.h"
#include "Cmd_MoveObject.h"

Cmd_MoveObject::Cmd_MoveObject(VisualObject* obj, const Vector3& newPos,
							   ITile* newTile, ITile* oldTile)
								 : _object(obj)
								 , _newPosition(newPos)
								 , _newTile(newTile)
								 , _oldTile(oldTile)
{
}

Cmd_MoveObject::~Cmd_MoveObject()
{

}

ICommandXPtr Cmd_MoveObject::Execute()
{
	Vector3 oldPos = _object->GetWorldPosition();

	if(_newTile != _oldTile)
	{
		_oldTile->RemoveGameObject(_object);
		_newTile->AddGameObject(_object);
	}

	_object->GetNode()->_setDerivedPosition(_newPosition);

	EventsManager::getSingleton().CreateEvent(
		PropertyEvents::PropertyChanged, Any(String("Position")));

	EventsManager::getSingleton().CreateEvent("UpdateGizmo", Any(_newPosition));
	return OGRE_NEW Cmd_MoveObject(_object, oldPos, _oldTile, _newTile);
}