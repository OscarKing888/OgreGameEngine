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

class _CoreExport Property_StringVector : public Property<StringVector, StringVector>
{
public:

	typedef Property<StringVector, StringVector> Base;
	typedef Base::SetFunType SetFunType;
	typedef Base::GetFunType GetFunType;

	Property_StringVector(SetFunType setFun, GetFunType getFun);
	virtual ~Property_StringVector(){}

	virtual const String	GetTypeName() const { return "StringVector"; }
	virtual void FromString(const String& val);
	virtual String ToString() const;
};
