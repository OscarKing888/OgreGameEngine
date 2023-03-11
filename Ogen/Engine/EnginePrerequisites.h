// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-02-02
// *************************************************************************************

#pragma once
#pragma warning(push)

// warning C4512: 'OIS::MouseEvent' : assignment operator could not be generated
#pragma warning(disable : 4512)

// warning C4245: 'initializing' : conversion from 'int' to 'const UTFString::size_type', signed/unsigned mismatch
#pragma warning(disable : 4245)

// warning C4100: 'xxx' : unreferenced formal parameter
#pragma warning(disable : 4100)

// warning C4127: conditional expression is constant
#pragma warning(disable : 4127)

#include <OIS/OIS.h>

#pragma warning(pop)

#include <Core/Core.h>
#include "EngineConfig.h"
#include <OgreTerrainGroup.h>

//------------------------------------------------------------------------
// Profile macros
//------------------------------------------------------------------------

#if !defined(_PRODUCT_)
#	define _ENABLE_PROFILER_ 0
#else
#	define _ENABLE_PROFILER_ 1
#endif

#define _PROFILE_FREQUENCY_ 10

#if _ENABLE_PROFILER_
#	include <OgreProfiler.h>
#   define OgeProfile(a)			Profile __OgreProfileInstance__( (a) )
#   define OgeProfileBegin(a)		Profiler::getSingleton().beginProfile( (a) )
#   define OgeProfileEnd(a)			Profiler::getSingleton().endProfile( (a) )
#else
#   define OgeProfile(a)		
#   define OgeProfileBegin(a)	
#   define OgeProfileEnd(a)	
#endif


#include "EngineExport.h"
#include "CommonTypes.h"
#include "PlatformAPI.h"
#include "UnitScale.h"
#include "Log.h"
#include "File.h"
#include "MathUtil.h"
#include "ClassUtil.h"

#include "Property_Quaternion.h"
#include "Property_Vector3.h"
#include "Property_ColourValue.h"
#include "Property_Radian.h"
#include "Property_GameObject.h"
#include "Property_XGUID.h"
#include "Property_Material.h"
#include "Property_MeshFile.h"


#include "IGame.h"
#include "GameApp.h"
#include "GameObjectFactoryManager.h"
#include "AbstractGameObject.h"