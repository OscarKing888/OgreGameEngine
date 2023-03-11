#pragma once

class Cmd_RotateObject : public ICommandX
{
public:

	Cmd_RotateObject(VisualObject* obj, const Quaternion& newRot);
	virtual ~Cmd_RotateObject();
	virtual ICommandXPtr Execute();

protected:

	VisualObject* _object;
	Quaternion _newRot;
};
