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
#include "Property_String.h"

Property_String::Property_String(SetFunType setFun, GetFunType getFun)
	: Base(setFun, getFun)
	, _sourceListProperty(0)
{
}

void Property_String::FromString(const String& val)
{
	SetValue(val);
}

String Property_String::ToString() const
{
	return GetValue();
}

void Property_String::SetSourceProperty(IProperty* prop)
{
	_sourceListProperty = prop;
}
