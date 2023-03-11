#pragma once

class Cmd_MoveObject : public ICommandX
{
public:

	Cmd_MoveObject(VisualObject* obj, const Vector3& newPos,
		ITile* newTile, ITile* oldTile);
	virtual ~Cmd_MoveObject();
	virtual ICommandXPtr Execute();

protected:

	VisualObject* _object;
	Vector3 _newPosition;
	ITile* _newTile;
	ITile* _oldTile;
};
