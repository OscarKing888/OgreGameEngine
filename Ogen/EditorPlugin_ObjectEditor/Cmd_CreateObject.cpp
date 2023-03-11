#include "stdafx.h"
#include "Cmd_CreateObject.h"

Cmd_AddObject::Cmd_AddObject(IGameObject* obj, ITile* tile)
: _object(obj)
, _tile(tile)
{
}

Cmd_AddObject::~Cmd_AddObject()
{

}

ICommandXPtr Cmd_AddObject::Execute()
{
	_tile->AddGameObject(_object);
	_tile->SetModified(true);
	return OGRE_NEW Cmd_RemoveObject(_object, _tile);
}

//------------------------------------------------------------------------

Cmd_RemoveObject::Cmd_RemoveObject(IGameObject* obj, ITile* tile)
: _object(obj)
, _tile(tile)
{
}

Cmd_RemoveObject::~Cmd_RemoveObject()
{

}

ICommandXPtr Cmd_RemoveObject::Execute()
{
	_tile->RemoveGameObject(_object);
	_tile->SetModified(true);
	return OGRE_NEW Cmd_AddObject(_object, _tile);
}