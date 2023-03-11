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

class _CoreExport Property_Float : public Property<float, float>
{
public:

	typedef Property<float, float> Base;
	typedef Base::SetFunType SetFunType;
	typedef Base::GetFunType GetFunType;

	Property_Float(SetFunType setFun, GetFunType getFun);
	virtual ~Property_Float(){}

	virtual const String	GetTypeName() const { return "float"; }
	virtual void FromString(const String& val);
	virtual String ToString() const;

	bool IsRangeLimited() const { return _isRangeLimited; }
	void SetRangeLimited(bool lmt, float minRange = 0.0f, float maxRange = 1.0f)
	{
		_isRangeLimited = lmt;
		_rangeMin = minRange;
		_rangeMax = maxRange;
	}

	float GetMinRange() const { return _rangeMin; }
	float GetMaxRange() const { return _rangeMax; }

protected:

	bool	_isRangeLimited;
	float	_rangeMin;
	float	_rangeMax;
};

SmartPointer(Property_Float);
