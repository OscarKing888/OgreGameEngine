// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-24
// *************************************************************************************

#include "StdAfx.h"
#include "RefObject.h"



	RefObject::RefObject(void)
		: _refCount(0)
	{
	}

	RefObject::~RefObject(void)
	{
	}

	String RefObject::ToString() const
	{
		const type_info* pTypeInfo = &typeid(*this);
		const char* pszClassName = pTypeInfo->name();
		String str = formatString(("[ %30s ] ToString() function not override!"), pszClassName);
		return str;
	}
