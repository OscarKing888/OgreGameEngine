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
#include "TerrainBrush.h"

class Property_TerrainBrush : public Property<ITerrainBrush* , ITerrainBrush*>
{
public:

	typedef Property<ITerrainBrush*, ITerrainBrush*> Base;
	typedef Base::SetFunType SetFunType;
	typedef Base::GetFunType GetFunType;

	Property_TerrainBrush(SetFunType setFun, GetFunType getFun);
	virtual ~Property_TerrainBrush(){}

	virtual const String	GetTypeName() const { return "TerrainBrush"; }
	virtual void FromString(const String& val);
	virtual String ToString() const;
};

SmartPointer(Property_TerrainBrush);