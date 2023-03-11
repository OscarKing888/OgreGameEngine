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
#include "Property_XGUID.h"



	Property_XGUID::Property_XGUID(SetFunType setFun, GetFunType getFun)
		: Base(setFun, getFun)
	{
	}

	void Property_XGUID::FromString(const String& val)
	{
		XGUID v;
		v.fromString(val);
		SetValue(v);
	}

	String Property_XGUID::ToString() const
	{
		XGUID v = GetValue();
		return v.toString();
	}
	