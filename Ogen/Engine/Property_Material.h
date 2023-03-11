// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************

#pragma once
#include "Property.h"



	class _EngineExport Property_Material : public Property_String
	{
	public:

		Property_Material(SetFunType setFun, GetFunType getFun);
		virtual ~Property_Material(){}

		virtual const String	GetTypeName() const { return "Material"; }
	};

