// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************

#include "Stdafx.h"
#include "Property_TerrainBrush.h"

Property_TerrainBrush::Property_TerrainBrush(SetFunType setFun, GetFunType getFun)
: Base(setFun, getFun)
{
}

void Property_TerrainBrush::FromString(const String& val)
{
	ITerrainBrush* br = TerrainBrushManager::getSingleton().GetBrush(val);
	SetValue(br);
}

String Property_TerrainBrush::ToString() const
{
	ITerrainBrush* br = GetValue();
	return br->GetName();
}