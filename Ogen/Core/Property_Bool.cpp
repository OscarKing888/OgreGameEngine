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
#include "Property_Bool.h"



	Property_Bool::Property_Bool(SetFunType setFun, GetFunType getFun)
		: Base(setFun, getFun)
	{
	}

	void Property_Bool::FromString(const String& val)
	{
		bool v = StringConverterBase::parseBool(val);
		SetValue(v);
	}

	String Property_Bool::ToString() const
	{
		bool v = GetValue();
		return StringConverterBase::toString(v);
	}
