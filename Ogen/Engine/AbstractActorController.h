#pragma once
#include "EnginePrerequisites.h"
#include "IActorController.h"
#include "IState.h"

class _EngineExport AbstractActorController
	: public IActorController
	, public IStateListener
{
public:

	AbstractActorController();
	virtual ~AbstractActorController();

	virtual void SetActor(Actor* actor);
	virtual Actor* GetActor() const;

protected:

	virtual void OnStateInit(IStateManager* mgr, int active);
	virtual void OnPreStateChange(IStateManager* mgr, int fromStateID, int toStateID);
	virtual void OnPostStateChange(IStateManager* mgr, int fromStateID, int toStateID);

	Actor*		_actor;
};
