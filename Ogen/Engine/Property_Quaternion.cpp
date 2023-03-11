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
#include "Property_Quaternion.h"



	Property_Quaternion::Property_Quaternion(SetFunType setFun, GetFunType getFun)
		: Base(setFun, getFun)
	{
	}

	void Property_Quaternion::FromString(const String& val)
	{
		Quaternion v = StringConverter::parseQuaternion(val);
		SetValue(v);
	}

	String Property_Quaternion::ToString() const
	{
		Quaternion v = GetValue();
		return StringConverter::toString(v);
	}
