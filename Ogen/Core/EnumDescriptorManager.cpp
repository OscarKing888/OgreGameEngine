// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2006-11-23
// *************************************************************************************

#include "stdafx.h"
#include "EnumDescriptorManager.h"
#include "TClassUtil.h"


	
	AbstractEnumDescriptor::AbstractEnumDescriptor()
	{
	}

	AbstractEnumDescriptor::~AbstractEnumDescriptor()
	{

	}

	StringVector AbstractEnumDescriptor::GetEnumValuesList()
	{
		return _enumList;
	}

	String AbstractEnumDescriptor::ToString(int enumVal)
	{
		Enum2StrMap::iterator i = _mapEnum2Str.find(enumVal);
		if(i != _mapEnum2Str.end())
		{
			return i->second;
		}

		assert(false && "Type value not found!");
		throw std::exception("[AbstractEnumDescriptor::ToString] Type value not found!");

		//FANTASY_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
		//	formatString("Type value not found! Val:[%d]", enumVal),
		//	"AbstractEnumDescriptor::ToString");
	}

	int AbstractEnumDescriptor::FromString(const String& strVal)
	{
		Str2EnumMap::iterator i = _mapStr2Enum.find(strVal);
		if(i != _mapStr2Enum.end())
		{
			return i->second;
		}

		assert(false && "Type value not found!");
		throw std::exception("[AbstractEnumDescriptor::FromString] Type value not found!");

// 		FANTASY_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
// 			formatString("Type value not found! Val:[%s]", strVal.c_str()),
// 			"AbstractEnumDescriptor::FromString");
	}

	void AbstractEnumDescriptor::Init(const String& enumTypeName)
	{
		assert(!_mapEnum2Str.empty());
		for(Enum2StrMap::iterator i = _mapEnum2Str.begin(); i != _mapEnum2Str.end(); ++i) 
		{
			_mapStr2Enum.insert(std::make_pair(i->second, i->first));
			_enumList.push_back(i->second);
		}

		if(enumTypeName.empty())
		{
			EnumDescriptorManager::getSingleton().RegEnumDescriptor(
				getClassName(this), this);
		}
		else
		{
			EnumDescriptorManager::getSingleton().RegEnumDescriptor(enumTypeName, this);
		}
	}

	//------------------------------------------------------------------------

	EnumDescriptorManagerImp::EnumDescriptorManagerImp()
	{
		
	}
	
	EnumDescriptorManagerImp::~EnumDescriptorManagerImp()
	{
		
	}
	
	void EnumDescriptorManagerImp::RegEnumDescriptor(const String& strName, IEnumDescriptor* enumDescription)
	{
		_mapEnumDesc.insert(std::make_pair(strName, enumDescription));
	}
	
	IEnumDescriptor* EnumDescriptorManagerImp::GetEnumDescriptor(const String& strName)
	{
		EnumStringMap::iterator it  =_mapEnumDesc.find(strName);
		if(it != _mapEnumDesc.end())
		{
			return it->second;
		}
		
		//LogErrorIDf("Engine", "[GetEnumDescriptor] [%s] not found!", strName.c_str());

		return 0;
	}
