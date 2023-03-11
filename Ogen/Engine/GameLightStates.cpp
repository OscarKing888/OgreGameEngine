#include "stdafx.h"
#include "GameLightStates.h"
#include "GameLight.h"

GameLightState_On::GameLightState_On(GameLight* owner)
: IState(owner->GetStateManager())
, _light(owner)
{
	_stateID = "On";
}
//--------------------------------------------------------------------------------
GameLightState_On::~GameLightState_On()
{
}
//--------------------------------------------------------------------------------
void GameLightState_On::OnActive()
{
	_light->SetOn(true);
}
//--------------------------------------------------------------------------------
GameLightState_Off::GameLightState_Off(GameLight* owner)
: IState(owner->GetStateManager())
, _light(owner)
{
	_stateID = "Off";
}
//--------------------------------------------------------------------------------
GameLightState_Off::~GameLightState_Off()
{
}
//--------------------------------------------------------------------------------
void GameLightState_Off::OnActive()
{
	_light->SetOn(false);
}
//--------------------------------------------------------------------------------