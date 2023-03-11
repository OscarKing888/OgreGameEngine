#pragma once
#include "IState.h"

class GameLight;

//--------------------------------------------------------------------------------
// GameLightState_On
//--------------------------------------------------------------------------------
class GameLightState_On : public IState
{
public:

	GameLightState_On(GameLight* owner);
	virtual ~GameLightState_On();
	virtual void OnActive();

protected:

	GameLight* _light;
};

//--------------------------------------------------------------------------------
// GameLightState_Off
//--------------------------------------------------------------------------------
class GameLightState_Off : public IState
{
public:

	GameLightState_Off(GameLight* owner);
	virtual ~GameLightState_Off();
	virtual void OnActive();

protected:

	GameLight* _light;
};