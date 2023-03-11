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
#include "Property_ColourValue.h"



	Property_ColourValue::Property_ColourValue(SetFunType setFun, GetFunType getFun)
		: Base(setFun, getFun)
	{
	}

	void Property_ColourValue::FromString(const String& val)
	{
		ColourValue v = StringConverter::parseColourValue(val);
		SetValue(v);
	}

	String Property_ColourValue::ToString() const
	{
		ColourValue v = GetValue();
		return StringConverter::toString(v);
	}
