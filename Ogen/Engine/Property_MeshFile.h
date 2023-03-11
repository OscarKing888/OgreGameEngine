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



	class _EngineExport Property_MeshFile : public Property_String
	{
	public:

		Property_MeshFile(SetFunType setFun, GetFunType getFun);
		virtual ~Property_MeshFile(){}

		virtual const String	GetTypeName() const { return "MeshFile"; }
	};


