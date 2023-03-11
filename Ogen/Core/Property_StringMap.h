// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************

#pragma once
#include "Property.h"

typedef map<String, String>::type StringMap;

class _CoreExport Property_StringMap : public Property<StringMap, StringMap>
{
public:

	typedef Property<StringMap, StringMap> Base;
	typedef Base::SetFunType SetFunType;
	typedef Base::GetFunType GetFunType;

	Property_StringMap(SetFunType setFun, GetFunType getFun);
	virtual ~Property_StringMap(){}

	virtual const String	GetTypeName() const { return "StringMap"; }
	virtual void FromString(const String& val);
	virtual String ToString() const;

	static String toString(const StringMap& m);
};

SmartPointer(Property_StringMap);

