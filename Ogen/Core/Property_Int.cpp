// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************

#include "Stdafx.h"
#include "Property_Int.h"

///
/// Property_Int
///
Property_Int::Property_Int(SetFunType setFun, GetFunType getFun)
: Base(setFun, getFun)
{
}

void Property_Int::FromString(const String& val)
{
	int v = StringConverterBase::parseInt(val);
	SetValue(v);
}

String Property_Int::ToString() const
{
	int v = GetValue();
	return StringConverterBase::toString(v);
}
//--------------------------------------------------------------------------------
///
/// Property_IntHex
///
Property_IntHex::Property_IntHex(SetFunType setFun, GetFunType getFun)
: Base(setFun, getFun)
{
}

void Property_IntHex::FromString(const String& val)
{
	int v = 0;
	sscanf(val.c_str(), "%X", &v);
	SetValue(v);
}

String Property_IntHex::ToString() const
{
	int v = GetValue();
	return formatString("%X", v);
}
