#include "stdafx.h"
#include "AbstractActorController.h"
#include "Actor.h"

AbstractActorController::AbstractActorController()
: _actor(0)
{

}

AbstractActorController::~AbstractActorController()
{

}

void AbstractActorController::SetActor(Actor* actor)
{
	if(_actor)
	{
		//IStateManager& sm = _actor->GetStateManager();
		//sm.RemoveListener(this);
	}

	_actor = actor;	

	if(_actor)
	{
		//IStateManager& sm = _actor->GetStateManager();
		//sm.AddListener(this);
	}
}

Actor* AbstractActorController::GetActor() const
{
	return _actor;
}

void AbstractActorController::OnStateInit(IStateManager* mgr, int active)
{

}

void AbstractActorController::OnPreStateChange(IStateManager* mgr, int fromStateID, int toStateID)
{

}

void AbstractActorController::OnPostStateChange(IStateManager* mgr, int fromStateID, int toStateID)
{

}
