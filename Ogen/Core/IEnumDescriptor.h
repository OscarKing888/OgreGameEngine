// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2005-11-23
// *************************************************************************************

#pragma once
#include "CorePrerequisites.h"


	
	/**
	*	enum描述器接口，用于将enum数据类型转换为string类型
	*/
	class _CoreExport IEnumDescriptor
	{
	public:

		/**	取得所有的可用的enum值的字符串类型表 */
		virtual StringVector	GetEnumValuesList() = 0;

		/**	将一个enum值转换为字符串，enum值都可转换为int */
		virtual String		ToString(int) = 0;

		/**	将一个字符串值解析为enum值 */
		virtual int					FromString(const String&) = 0;
	};
