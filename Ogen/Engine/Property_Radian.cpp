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
#include "Property_Radian.h"



	Property_Radian::Property_Radian(SetFunType setFun, GetFunType getFun)
		: Base(setFun, getFun)
		, _rangeMin(0.0f)
		, _rangeMax(Math::TWO_PI)
		, _isRangeLimited(false)
	{
	}

	void Property_Radian::FromString(const String& val)
	{
		float v = 0.0f;
		if(val.find("d") != String::npos)
		{
			String s = val.substr(0, val.length() - 1);
			v = StringConverter::parseReal(val);
			SetValue(Radian(Degree(v)));
		}
		else
		{
			v = StringConverter::parseReal(val);
			SetValue(Radian(v));
		}
	}

	String Property_Radian::ToString() const
	{
		Radian v = GetValue();
		return StringConverter::toString(v.valueRadians());
	}
