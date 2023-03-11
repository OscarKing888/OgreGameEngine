#pragma once
#include "ParticleEditorEvents.h"

class Action_AddEmitter : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};

//--------------------------------------------------------------------------------

class Action_RemoveEmitter : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};

//--------------------------------------------------------------------------------

class Action_AddAffector : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};

//--------------------------------------------------------------------------------

class Action_RemoveAffector : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};

//--------------------------------------------------------------------------------

class Action_ChangeParticleSysRenderer : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};

//--------------------------------------------------------------------------------

class Action_SelectEmitter : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};

//--------------------------------------------------------------------------------

class Action_SelectAffector : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};

//--------------------------------------------------------------------------------

class Action_SelectParticleSysProp : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};

//--------------------------------------------------------------------------------

class Action_SelectParticleSysRendererProp : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};


//--------------------------------------------------------------------------------

class Action_AddNewParticleSys : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};

//--------------------------------------------------------------------------------

class Action_EditParticleSystem : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};

//--------------------------------------------------------------------------------

class Action_Undo : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};

//--------------------------------------------------------------------------------

class Action_Redo : public IActionCommandX
{
public:

	virtual void Execute(const Any& contex);
};