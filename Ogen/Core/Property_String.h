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

class _CoreExport Property_String : public Property<String, String>
{
public:

	typedef Property<String, String> Base;
	typedef Base::SetFunType SetFunType;
	typedef Base::GetFunType GetFunType;

	Property_String(SetFunType setFun, GetFunType getFun);
	virtual ~Property_String(){}

	virtual const String	GetTypeName() const { return "String"; }
	virtual void FromString(const String& val);
	virtual String ToString() const;

	void SetSourceProperty(IProperty* prop);
	IProperty* GetSourceProperty() { return _sourceListProperty; }
	bool HasSourceList() const { return _sourceListProperty != 0; }

protected:

	IProperty*	_sourceListProperty;
};

SmartPointer(Property_String);
