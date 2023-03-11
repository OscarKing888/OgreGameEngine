// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2009-09-04
// *************************************************************************************

#include "StdAfx.h"
#include "ObjectSerializer.h"

namespace Fantasy
{
	//------------------------------------------------------------------------
	ObjectSerializer::ObjectSerializer(void)
		: _currentVersion("N/A")
	{
	}
	//------------------------------------------------------------------------
	ObjectSerializer::~ObjectSerializer(void)
	{
	}
	//------------------------------------------------------------------------
	bool ObjectSerializer::canReadVersion(const String& version) const
	{
		return (getSerializer(version) != 0);
	}
	//------------------------------------------------------------------------
	IObjectSerializer* ObjectSerializer::getSerializer(const String& version) const
	{
		SerializerVersionMap::const_iterator it = _serializerMap.find(version);
		if(it != _serializerMap.end())
		{
			return it->second;
		}

		return 0;
	}
	//------------------------------------------------------------------------
	void ObjectSerializer::addSerializer(const String& version, IObjectSerializer* ser)
	{
		if(getSerializer(version) != 0)
		{
			//TODO:
			//OGRE_EXCEPT()
		}
		else
		{
			_serializerMap.insert(std::make_pair(version, ser));
		}
	}
	//------------------------------------------------------------------------
	const String& ObjectSerializer::getVersion() const
	{
		return _currentVersion;
	}
	//------------------------------------------------------------------------
	bool ObjectSerializer::save(OutStream& os)
	{
		IObjectSerializer* s = getSerializer(_currentVersion);
		if(s)
		{
			return s->save(os);
		}
		else
		{
			// TODO:
		}

		return false;
	}
	//------------------------------------------------------------------------
	bool ObjectSerializer::load(InStream& is)
	{
		IObjectSerializer* s = getSerializer(_currentVersion);
		if(s)
		{
			return s->load(is);
		}
		else
		{
			// TODO:
		}

		return false;
	}
	//------------------------------------------------------------------------
}