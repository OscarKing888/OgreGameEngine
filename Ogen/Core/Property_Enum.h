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
#include "EnumDescriptorManager.h"

class _CoreExport IPropertyEnum
{
public:

	virtual void SetEnumTypeName(const String& enumTypeName) = 0;
	virtual const String& GetEnumTypeName() const = 0;

};

template<typename T>
class /*_CoreExport */Property_Enum : public Property<T, T>, public IPropertyEnum
{
public:

	typedef typename Property<T, T> Base;
	typedef typename Base::SetFunType SetFunType;
	typedef typename Base::GetFunType GetFunType;

	// 		Property_Enum(SetFunType setFun, GetFunType getFun);
	virtual ~Property_Enum(){}

	virtual void SetEnumTypeName(const String& enumTypeName) { _enumTypeName = enumTypeName; }
	virtual const String& GetEnumTypeName() const { return _enumTypeName; }

	virtual const String	GetTypeName() const { return "enum"; }
	// 		virtual void FromString(const String& val);
	// 		virtual String ToString() const;

	Property_Enum(SetFunType setFun, GetFunType getFun)
		: Base(setFun, getFun)
	{
	}

	void FromString(const String& val)
	{
		IEnumDescriptor* ed = EnumDescriptorManager::getSingleton().GetEnumDescriptor(_enumTypeName);
		assert(ed);
		int v = ed->FromString(val);
		T tval = static_cast<T>(v);
		SetValue(tval);
	}

	String ToString() const
	{
		int v = (int)GetValue();
		IEnumDescriptor* ed = EnumDescriptorManager::getSingleton().GetEnumDescriptor(_enumTypeName);
		assert(ed);
		return ed->ToString(v);
	}

protected:

	String _enumTypeName;
};
