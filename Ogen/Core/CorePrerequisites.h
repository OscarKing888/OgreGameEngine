#pragma once
#include "CoreConfig.h"
//----------------------------------------------------------------------------------------------
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>

// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

extern "C"
{
#   include <sys/types.h>
#   include <sys/stat.h>
}
//----------------------------------------------------------------------------------------------
#include "CoreExport.h"
#include <stack>

#pragma warning(push)
#pragma warning(disable : 4244)
#include <Ogre.h>
#pragma warning(pop)

using namespace Ogre;

namespace Ogre
{ 
	template <typename T, typename A = STLAllocator<T, GeneralAllocPolicy> > 
	struct stack 
	{ 
#if OGRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
		typedef typename std::stack<T, typename Ogre::deque<T, A>::type > type;    
#else
		typedef typename std::stack<T> type;    
#endif
	}; 

}

#ifdef _DEBUG
#	pragma comment(lib, "OgreMain_d.lib")
#else
#	pragma comment(lib, "OgreMain.lib")
#endif

//------------------------------------------------------------------------
// Data types define
//------------------------------------------------------------------------

#ifndef byte
	typedef unsigned char byte;
#endif

typedef vector<byte>::type ByteBuffer;

#ifdef _DEBUG
	
	struct _checkTypeSize
	{
		_checkTypeSize()
		{
			assert(sizeof(byte) == 1);
			assert(sizeof(int16) == 2);
			assert(sizeof(int32) == 4);
			assert(sizeof(uint64) == 8);
		}
	};

	static _checkTypeSize g_checkTypeSize;

#endif


#ifndef OGRE_VERSION
	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef unsigned int uint;
	typedef unsigned long ulong;

	typedef std::string _StringBase;
	typedef _StringBase String;
#endif

//------------------------------------------------------------------------
// Multi-threading macros
//------------------------------------------------------------------------

#if OGE_MULTI_THREADING
#	pragma warning(push)
#	pragma warning(disable : 4244)
#	include <boost/function.hpp>
#	include <boost/thread.hpp>
#	pragma warning(pop)
#endif

#if OGE_MULTI_THREADING
#	define OGE_AUTO_MUTEX_NAME _autoMutex
#	define OGE_AUTO_MUTEX_RAW boost::recursive_mutex OGE_AUTO_MUTEX_NAME;
#	define OGE_AUTO_MUTEX mutable OGE_AUTO_MUTEX_RAW
#	define OGE_AUTO_SCOPED_LOCK boost::recursive_mutex::scoped_lock scopedLock(OGE_AUTO_MUTEX_NAME);
#	define OGE_AUTO_STATIC_SCOPED_LOCK static OGE_AUTO_MUTEX_RAW; OGE_AUTO_SCOPED_LOCK;
#else
#	define OGE_AUTO_MUTEX
#	define OGE_AUTO_SCOPED_LOCK
#	define OGE_AUTO_STATIC_SCOPED_LOCK
#endif

//----------------------------------------------------------------------------------------------
// Memory macros
//----------------------------------------------------------------------------------------------

#if _DEBUG

	/// Allocate a block of memory for a primitive type, and indicate the category of usage
#	define OGE_ALLOC_T(T, count) static_cast<T*>(::Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))
	/// Free the memory allocated with OGRE_MALLOC or OGRE_ALLOC_T. Category is required to be restated to ensure the matching policy is used
#	define OGE_FREE(ptr) ::Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL>::deallocateBytes((void*)ptr)

	/// Allocate space for one primitive type, external type or non-virtual type with constructor parameters
#	define OGE_NEW_T(T) new (::Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL>::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
	/// Allocate a block of memory for 'count' primitive types - do not use for classes that inherit from AllocatedObject
#	define OGE_NEW_ARRAY_T(T, count) ::Ogre::constructN(static_cast<T*>(::Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count) 
	/// Free the memory allocated with OGRE_NEW_T. Category is required to be restated to ensure the matching policy is used
#	define OGE_DELETE_T(ptr, T) if(ptr){(ptr)->~T(); ::Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL>::deallocateBytes((void*)ptr);}
	/// Free the memory allocated with OGRE_NEW_ARRAY_T. Category is required to be restated to ensure the matching policy is used, count and type to call destructor
#	define OGE_DELETE_ARRAY_T(ptr, T, count) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL>::deallocateBytes((void*)ptr);}

#else // !_DEBUG

	/// Allocate a block of memory for a primitive type, and indicate the category of usage
#	define OGE_ALLOC_T(T, count) static_cast<T*>(::Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL>::allocateBytes(sizeof(T)*(count)))
	/// Free the memory allocated with OGRE_MALLOC or OGRE_ALLOC_T. Category is required to be restated to ensure the matching policy is used
#	define OGE_FREE(ptr) ::Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL>::deallocateBytes((void*)ptr)

	/// Allocate space for one primitive type, external type or non-virtual type with constructor parameters
#	define OGE_NEW_T(T) new (::Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL>::allocateBytes(sizeof(T))) T
	/// Allocate a block of memory for 'count' primitive types - do not use for classes that inherit from AllocatedObject
#	define OGE_NEW_ARRAY_T(T, count) ::Ogre::constructN(static_cast<T*>(::Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL>::allocateBytes(sizeof(T)*(count))), count) 
	/// Free the memory allocated with OGRE_NEW_T. Category is required to be restated to ensure the matching policy is used
#	define OGE_DELETE_T(ptr, T) if(ptr){(ptr)->~T(); ::Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL>::deallocateBytes((void*)ptr);}
	/// Free the memory allocated with OGRE_NEW_ARRAY_T. Category is required to be restated to ensure the matching policy is used, count and type to call destructor
#	define OGE_DELETE_ARRAY_T(ptr, T, count) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL>::deallocateBytes((void*)ptr);}

#endif

typedef Ogre::GeneralAllocatedObject OgeAlloc;



#include "TString.h"
#include "DebugUtil.h"
#include "ByteStream.h"
#include "SingletonMyers.h"

