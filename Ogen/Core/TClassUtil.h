// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-24
// *************************************************************************************

#pragma once
#include "CorePrerequisites.h"

static const String C_CPPClassNamePrefix = "class ";

_CoreExport String _getClassName(const String& classFullName, bool removeNamespace = true);

template<class T>
String getClassName(bool removeNamespace = true)
{
	const type_info* pTypeInfo = &typeid(T);
	const char* pszClassName = pTypeInfo->name();
	String str = pszClassName;
	return _getClassName(str, removeNamespace);
}

template<class T>
String getClassName(T* pClass, bool removeNamespace = true)
{
	const type_info* pTypeInfo = &typeid(*pClass);
	const char* pszClassName = pTypeInfo->name();
	String str = pszClassName;
	return _getClassName(str, removeNamespace);
}

template<class T>
static uint hashClassName()
{
	String className = getClassName<T>();
	size_t hashVal = stdext::hash_value(className.c_str());
	return (uint) hashVal;
}

