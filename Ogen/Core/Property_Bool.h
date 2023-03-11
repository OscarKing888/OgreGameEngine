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

class _CoreExport Property_Bool : public Property<bool, bool>
{
public:

	typedef Property<bool, bool> Base;
	typedef Base::SetFunType SetFunType;
	typedef Base::GetFunType GetFunType;

	Property_Bool(SetFunType setFun, GetFunType getFun);
	virtual ~Property_Bool(){}

	virtual const String	GetTypeName() const { return "bool"; }
	virtual void FromString(const String& val);
	virtual String ToString() const;
};
