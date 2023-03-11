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



	class _EngineExport Property_Quaternion : public Property<Quaternion, Quaternion>
	{
	public:

		typedef Property<Quaternion, Quaternion> Base;
		typedef Base::SetFunType SetFunType;
		typedef Base::GetFunType GetFunType;

		Property_Quaternion(SetFunType setFun, GetFunType getFun);
		virtual ~Property_Quaternion(){}

		virtual const String	GetTypeName() const { return "Quaternion"; }
		virtual void FromString(const String& val);
		virtual String ToString() const;
	};
