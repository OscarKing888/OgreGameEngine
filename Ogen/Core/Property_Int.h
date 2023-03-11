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

class _CoreExport Property_Int : public Property<int, int>
{
public:

	typedef Property<int, int> Base;
	typedef Base::SetFunType SetFunType;
	typedef Base::GetFunType GetFunType;

	Property_Int(SetFunType setFun, GetFunType getFun);
	virtual ~Property_Int(){}

	virtual const String	GetTypeName() const { return "int"; }
	virtual void FromString(const String& val);
	virtual String ToString() const;
};

//--------------------------------------------------------------------------------

class _CoreExport Property_IntHex : public Property<int, int>
{
public:

	typedef Property<int, int> Base;
	typedef Base::SetFunType SetFunType;
	typedef Base::GetFunType GetFunType;

	Property_IntHex(SetFunType setFun, GetFunType getFun);
	virtual ~Property_IntHex(){}

	virtual const String	GetTypeName() const { return "hex"; }
	virtual void FromString(const String& val);
	virtual String ToString() const;
};
