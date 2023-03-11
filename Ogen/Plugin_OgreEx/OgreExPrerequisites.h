#pragma once

#include "OgrePrerequisites.h"
#include "OgreRoot.h"
#include "OgreParticleAffector.h"
#include "OgreParticleSystemManager.h"
#include "OgreStringInterface.h"
#include "OgreParticleAffectorFactory.h"
#include "OgreParticleEmitterFactory.h"
#include "OgreColourValue.h"
#include "OgreVector3.h"
#include "OgreParticle.h"
#include "OgreStringConverter.h"

#ifdef PLUGIN_PARTICLEEX_EXPORTS
#	define _OgreExExport __declspec(dllexport)
#else
#	define _OgreExExport __declspec(dllimport)
#endif

