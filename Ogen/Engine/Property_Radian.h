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



	class _EngineExport Property_Radian : public Property<Radian, Radian>
	{
	public:

		typedef Property<Radian, Radian> Base;
		typedef Base::SetFunType SetFunType;
		typedef Base::GetFunType GetFunType;

		Property_Radian(SetFunType setFun, GetFunType getFun);
		virtual ~Property_Radian(){}

		virtual const String	GetTypeName() const { return "Radian"; }
		virtual void FromString(const String& val);
		virtual String ToString() const;

		bool IsRangeLimited() const { return _isRangeLimited; }
		void SetRangeLimited(bool lmt, Radian minRange = Radian(0.0f), Radian maxRange = Radian(Math::TWO_PI))
		{
			_isRangeLimited = lmt;
			_rangeMin = minRange;
			_rangeMax = maxRange;
		}

		Radian GetMinRange() const { return _rangeMin; }
		Radian GetMaxRange() const { return _rangeMax; }

	protected:

		bool	_isRangeLimited;
		Radian	_rangeMin;
		Radian	_rangeMax;
	};

	SmartPointer(Property_Radian);
