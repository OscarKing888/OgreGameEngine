#pragma once

class Memento_ShowHideObject : public IMemento
{
public:

	Memento_ShowHideObject(VisualObject* obj, bool show);
	virtual ~Memento_ShowHideObject();

	virtual void Set();
	virtual void Restore();

protected:

	VisualObject* _object;
	bool	_oldVisible;
	bool	_currentVisible;
};
