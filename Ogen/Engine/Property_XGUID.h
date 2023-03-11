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



	class _EngineExport Property_XGUID : public Property<XGUID, XGUID>
	{
	public:

		typedef Property<XGUID, XGUID> Base;
		typedef Base::SetFunType SetFunType;
		typedef Base::GetFunType GetFunType;

		Property_XGUID(SetFunType setFun, GetFunType getFun);
		virtual ~Property_XGUID(){}

		virtual const String	GetTypeName() const { return "XGUID"; }
		virtual void FromString(const String& val);
		virtual String ToString() const;
	};

	SmartPointer(Property_XGUID);
