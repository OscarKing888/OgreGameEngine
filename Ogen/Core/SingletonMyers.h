// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-25
// *************************************************************************************

#pragma once

template<typename T, bool HasInit = false>
class SingletonMyers;

template<typename T>
class SingletonMyers<T, false>
{
public:

	static T& getSingleton()
	{
		static T inst;
		return inst;
	}
};

template<typename T>
class SingletonMyers<T, true>
{
	class SingletonInitHelper
	{
	public:

		explicit SingletonInitHelper(SingletonMyers* inst)
		{
			inst->initialize();
		}
	};

	virtual void initialize() {}

public:

	static T& getSingleton()
	{
		static T inst;
		static SingletonInitHelper initHelper((SingletonMyers*)&inst);

		return inst;
	}
};

//------------------------------------------------------------------------
#ifndef OGRE_VERSION
template <typename T> class Singleton
{
protected:

	static T* ms_Singleton;

public:
	Singleton( void )
	{
		assert( !ms_Singleton );
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
		int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
		ms_Singleton = (T*)((int)this + offset);
#else
		ms_Singleton = static_cast< T* >( this );
#endif
	}
	virtual ~Singleton( void )
	{  assert( ms_Singleton );  ms_Singleton = 0;  }
	static T& getSingleton( void )
	{	assert( ms_Singleton );  return ( *ms_Singleton ); }
	static T* getSingletonPtr( void )
	{ return ms_Singleton; }
};
#endif

//------------------------------------------------------------------------

template <typename T> class SingletonPhoenix
{
protected:

	static T* ms_Singleton;

public:
	SingletonPhoenix( void )
	{
		assert(!ms_Singleton);

#if defined( _MSC_VER ) && _MSC_VER < 1200	 
		int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
		ms_Singleton = (T*)((int)this + offset);
#else
		ms_Singleton = static_cast< T* >( this );
#endif
	}

	virtual ~SingletonPhoenix( void )
	{  assert( ms_Singleton );  ms_Singleton = 0;  }

	static T& getSingleton( void )
	{	assert( ms_Singleton );  return ( *ms_Singleton ); }

	static T* getSingletonPtr( void )
	{ return ms_Singleton; }

	// 		static void destroySingleton()
	// 		{
	// 			if(ms_Singleton)
	// 			{
	// 				OGRE_DELETE ms_Singleton;
	// 				ms_Singleton = 0;
	// 			}
	// 		}
};
