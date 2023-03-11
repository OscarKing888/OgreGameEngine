#include "stdafx.h"
#include <OgreCodec.h>


void InitQueryMask()
{
	int offset = 0;
	GameObjectFactoryManager::getSingleton().SetQueryFlag<StaticModel>(1 << offset);
// 	GameObjectFactoryManager::getSingleton().SetQueryFlag<WaypointPath>(1 << offset);
// 	++offset;

	GameObjectFactoryManager::getSingleton().SetQueryFlag<AnimationModel>(1 << offset);
	++offset;

	GameObjectFactoryManager::getSingleton().SetQueryFlag<Light>(1 << offset);
	++offset;

	GameObjectFactoryManager::getSingleton().SetQueryFlag<GameCamera>(1 << offset);
	++offset;

	GameObjectFactoryManager::getSingleton().SetQueryFlag<Trigger>(1 << offset);
	++offset;

	GameObjectFactoryManager::getSingleton().SetQueryFlag<ParticleSys>(1 << offset);
	++offset;

	GameObjectFactoryManager::getSingleton().SetQueryFlag<Actor>(1 << offset);
	GameObjectFactoryManager::getSingleton().SetQueryFlag<Teleport>(1 << offset);
	++offset;

// 	GameObjectFactoryManager::getSingleton().SetQueryFlag<MonsterSpawnPoint>(1 << offset);
// 	++offset;

// 	GameObjectFactoryManager::getSingleton().SetQueryFlag<Axes_Move>(1 << offset);
// 	++offset;
// 
// 	GameObjectFactoryManager::getSingleton().SetQueryFlag<Axes_Rotate>(1 << offset);
// 	++offset;
// 
// 	GameObjectFactoryManager::getSingleton().SetQueryFlag<Axes_Scale>(1 << offset);
// 	++offset;
}

