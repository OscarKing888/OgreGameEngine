#pragma once

class Cmd_ScaleObject : public ICommandX
{
public:

	Cmd_ScaleObject(VisualObject* obj, const Vector3& newScale);
	virtual ~Cmd_ScaleObject();
	virtual ICommandXPtr Execute();

protected:

	VisualObject* _object;
	Vector3 _newScale;
};
