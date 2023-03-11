#include <stdafx.h>
#include "Memento_ShowHideObject.h"

Memento_ShowHideObject::Memento_ShowHideObject(VisualObject* obj, bool show)
: _object(obj)
, _currentVisible(show)
{
	_oldVisible = obj->GetVisible();
	Set();
}

Memento_ShowHideObject::~Memento_ShowHideObject()
{
	Restore();
}

void Memento_ShowHideObject::Set()
{
	_object->SetVisible(_currentVisible);
}

void Memento_ShowHideObject::Restore()
{
	_object->SetVisible(_oldVisible);
}

