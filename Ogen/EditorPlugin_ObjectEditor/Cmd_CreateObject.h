#pragma once

//------------------------------------------------------------------------
class Cmd_AddObject : public ICommandX
{
public:

	Cmd_AddObject(IGameObject* obj, ITile* tile);

	virtual ~Cmd_AddObject();
	virtual ICommandXPtr Execute();

protected:

	ITile* _tile;
	IGameObject* _object;
};

//------------------------------------------------------------------------

class Cmd_RemoveObject : public ICommandX
{
public:

	Cmd_RemoveObject(IGameObject* obj, ITile* tile);

	virtual ~Cmd_RemoveObject();
	virtual ICommandXPtr Execute();

protected:

	ITile* _tile;
	IGameObject* _object;
};