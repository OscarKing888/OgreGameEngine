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
#include "Property_Vector3.h"

//--------------------------------------------------------------------------------
// Vector2
//--------------------------------------------------------------------------------
Property_Vector2::Property_Vector2(SetFunType setFun, GetFunType getFun)
: Base(setFun, getFun)
{
}
//--------------------------------------------------------------------------------
void Property_Vector2::FromString(const String& val)
{
	Vector2 v = StringConverter::parseVector2(val);
	SetValue(v);
}
//--------------------------------------------------------------------------------
String Property_Vector2::ToString() const
{
	Vector2 v = GetValue();
	return StringConverter::toString(v);
}
//--------------------------------------------------------------------------------
// Vector3
//--------------------------------------------------------------------------------
Property_Vector3::Property_Vector3(SetFunType setFun, GetFunType getFun)
: Base(setFun, getFun)
{
}
//--------------------------------------------------------------------------------
void Property_Vector3::FromString(const String& val)
{
	Vector3 v = StringConverter::parseVector3(val);
	SetValue(v);
}
//--------------------------------------------------------------------------------
String Property_Vector3::ToString() const
{
	Vector3 v = GetValue();
	return StringConverter::toString(v);
}
//--------------------------------------------------------------------------------
// Vector4
//--------------------------------------------------------------------------------
Property_Vector4::Property_Vector4(SetFunType setFun, GetFunType getFun)
: Base(setFun, getFun)
{
}
//--------------------------------------------------------------------------------
void Property_Vector4::FromString(const String& val)
{
	Vector4 v = StringConverter::parseVector4(val);
	SetValue(v);
}
//--------------------------------------------------------------------------------
String Property_Vector4::ToString() const
{
	Vector4 v = GetValue();
	return StringConverter::toString(v);
}
//--------------------------------------------------------------------------------
// Vector3List
//--------------------------------------------------------------------------------
Property_Vector3List::Property_Vector3List(SetFunType setFun, GetFunType getFun)
: Base(setFun, getFun)
{
}
//--------------------------------------------------------------------------------
void Property_Vector3List::FromString(const String& val)
{
	StringVector v = StringUtil::split(val, "|");
	Vector3List vl;
	for (size_t i = 0; i < v.size(); ++i)
	{
		Vector3 v3 = StringConverter::parseVector3(v[i]);
		vl.push_back(v3);
	}

	SetValue(vl);
}
//--------------------------------------------------------------------------------
String Property_Vector3List::ToString() const
{
	Vector3List v = GetValue();
	String val;
	for (size_t i = 0; i < v.size(); ++i)
	{
		val += StringConverter::toString(v[i]);
		if(i < v.size() - 1)
		{
			val += "|";
		}
	}

	return val;
}
//--------------------------------------------------------------------------------
