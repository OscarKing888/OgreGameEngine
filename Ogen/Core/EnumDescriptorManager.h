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

/**	һЩӳ���õĸ����꣬�����в�Ҫ��namespace�޶� */
#define M_AddEnumToDescriptor(e) _mapEnum2Str.insert(std::make_pair((int)e, (#e)))
#define M_AddEnumToDescriptorEx(e, str) _mapEnum2Str.insert(std::make_pair((int)e, (#str)))
#define M_AddEnumToDescriptorRaw(e, strVar) _mapEnum2Str.insert(std::make_pair((int)e, strVar))

/**
*	enum�������ĳ���ʵ�֣��ṩͨ�õĹ��ܣ��ɼ̳��Դ��ಢ����¹��������չ
*	@par	������Ĺ��캯���п��ø������ʼ��һ��ӳ���
*			�ٵ���AbstractEnumDescriptor::Init()�������ɡ�
*/
class _CoreExport AbstractEnumDescriptor : public IEnumDescriptor
{
public:

	AbstractEnumDescriptor();
	virtual ~AbstractEnumDescriptor();
	virtual StringVector GetEnumValuesList();

	/**	ͨ�����ʽ������Ӧstringֵ */
	String ToString(int enumVal);

	/**	ͨ�����ʽ������Ӧintֵ */
	int FromString(const String& strVal);

protected:

	typedef map<int, String>::type		Enum2StrMap;
	typedef map<String, int>::type		Str2EnumMap;

	Enum2StrMap	_mapEnum2Str;
	Str2EnumMap	_mapStr2Enum;
	StringVector	_enumList;

	/**	����Ĺ��캯����ͨ���������ʼ��ӳ������Ҫ���ô˺������ʣ�µ�ӳ�乤�� */
	void Init(const String& enumTypeName = "");
};

