#include "stdafx.h"
#include "ISound.h"

template<> SoundManager* Singleton<SoundManager>::ms_Singleton = 0;