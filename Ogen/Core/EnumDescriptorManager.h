// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2006-11-23
// *************************************************************************************

#pragma once
#include "IEnumDescriptor.h"

typedef map<String, IEnumDescriptor*>::type EnumStringMap;

class _CoreExport EnumDescriptorManagerImp  
{
public:

	EnumDescriptorManagerImp();
	virtual ~EnumDescriptorManagerImp();

	void RegEnumDescriptor(const String& strName, IEnumDescriptor* enumDescription);
	IEnumDescriptor* GetEnumDescriptor(const String& strName);

protected:

	EnumStringMap					_mapEnumDesc;
};

class _CoreExport EnumDescriptorManager : public EnumDescriptorManagerImp, public SingletonMyers<EnumDescriptorManagerImp>
{
};

/**	一些映射用的辅助宏，参数中不要带namespace限定 */
#define M_AddEnumToDescriptor(e) _mapEnum2Str.insert(std::make_pair((int)e, (#e)))
#define M_AddEnumToDescriptorEx(e, str) _mapEnum2Str.insert(std::make_pair((int)e, (#str)))
#define M_AddEnumToDescriptorRaw(e, strVar) _mapEnum2Str.insert(std::make_pair((int)e, strVar))

/**
*	enum描述符的抽象实现，提供通用的功能，可继承自此类并添加新功能完成扩展
*	@par	在子类的构造函数中可用辅助宏初始化一个映射表，
*			再调用AbstractEnumDescriptor::Init()函数即可。
*/
class _CoreExport AbstractEnumDescriptor : public IEnumDescriptor
{
public:

	AbstractEnumDescriptor();
	virtual ~AbstractEnumDescriptor();
	virtual StringVector GetEnumValuesList();

	/**	通过查表方式返回相应string值 */
	String ToString(int enumVal);

	/**	通过查表方式返回相应int值 */
	int FromString(const String& strVal);

protected:

	typedef map<int, String>::type		Enum2StrMap;
	typedef map<String, int>::type		Str2EnumMap;

	Enum2StrMap	_mapEnum2Str;
	Str2EnumMap	_mapStr2Enum;
	StringVector	_enumList;

	/**	子类的构造函数中通过辅助宏初始化映射表后需要调用此函数完成剩下的映射工作 */
	void Init(const String& enumTypeName = "");
};

