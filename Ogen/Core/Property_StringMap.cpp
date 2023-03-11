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
#include "Property_StringMap.h"





	const String C_StringMapPairDelimer(":");
	const String C_StringMapValueDelimer(";");

	Property_StringMap::Property_StringMap(SetFunType setFun, GetFunType getFun)
		: Base(setFun, getFun)
	{
	}

	void Property_StringMap::FromString(const String& val)
	{
		StringMap m;
		StringVector matPairs = StringUtil::split(val, C_StringMapValueDelimer);
		
		for (size_t i = 0; i < matPairs.size(); ++i)
		{
			StringVector paras = StringUtil::split(matPairs[i], C_StringMapPairDelimer);
			if(paras.size() != 2)
			{
				assert(false && "[Property_StringMap::FromString] error value pair!");
				//LogErrorIDf("Property", "[Property_StringMap::FromString] error value pair[%s]", matPairs[i].c_str());
			}
			else
			{
				m.insert(std::make_pair(paras[0], paras[1]));
			}
		}

		SetValue(m);
	}

	String Property_StringMap::ToString() const
	{
		StringMap v = GetValue();		
		return toString(v);
	}

	String Property_StringMap::toString(const StringMap& m)
	{
		String val;
		for (StringMap::const_iterator i = m.begin(); i != m.end(); ++i)
		{
			String str = formatString("%s:%s;", i->first.c_str(), i->second.c_str());
			val += str;
		}

		return val;
	}
