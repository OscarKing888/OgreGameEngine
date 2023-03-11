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
	*	enum�������ӿڣ����ڽ�enum��������ת��Ϊstring����
	*/
	class _CoreExport IEnumDescriptor
	{
	public:

		/**	ȡ�����еĿ��õ�enumֵ���ַ������ͱ� */
		virtual StringVector	GetEnumValuesList() = 0;

		/**	��һ��enumֵת��Ϊ�ַ�����enumֵ����ת��Ϊint */
		virtual String		ToString(int) = 0;

		/**	��һ���ַ���ֵ����Ϊenumֵ */
		virtual int					FromString(const String&) = 0;
	};
