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
#include "Property_StringVector.h"



	Property_StringVector::Property_StringVector(SetFunType setFun, GetFunType getFun)
		: Base(setFun, getFun)
	{
	}

	void Property_StringVector::FromString(const String& val)
	{
		StringVector v = StringUtil::split(val, "|");
		SetValue(v);
	}

	String Property_StringVector::ToString() const
	{
		StringVector v = GetValue();
		String val;
		for (size_t i = 0; i < v.size(); ++i)
		{
			val += v[i];
			if(i < v.size() - 1)
			{
				val += "|";
			}
		}

		return val;
	}
