// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-24
// *************************************************************************************

#include "stdafx.h"
#include "TClassUtil.h"



	String _getClassName(const String& classFullName, bool removeNamespace)
	{
		String t;

		if(removeNamespace)
		{
			size_t i = classFullName.find_last_of("::");
			if(i != String::npos)
			{
				t = classFullName.substr(i + 1);
				return t;
			}
		}

		t = classFullName.substr(C_CPPClassNamePrefix.length());

		return t;
	}

