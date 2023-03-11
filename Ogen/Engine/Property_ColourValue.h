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



	class _EngineExport Property_ColourValue : public Property<ColourValue, ColourValue>
	{
	public:

		typedef Property<ColourValue, ColourValue> Base;
		typedef Base::SetFunType SetFunType;
		typedef Base::GetFunType GetFunType;

		Property_ColourValue(SetFunType setFun, GetFunType getFun);
		virtual ~Property_ColourValue(){}

		virtual const String	GetTypeName() const { return "ColourValue"; }
		virtual void FromString(const String& val);
		virtual String ToString() const;
	};
