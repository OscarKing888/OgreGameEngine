#pragma once
#include "SFXEditorEvents.h"

class Action_NewSFX : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};
//--------------------------------------------------------------------------------
class Action_AddElement : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};
//--------------------------------------------------------------------------------
class Action_ShowSFXObjectProp : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};