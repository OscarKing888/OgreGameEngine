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
#include "Property_GameObject.h"



	Property_GameObject::Property_GameObject(SetFunType setFun, GetFunType getFun)
		: Base(setFun, getFun)
	{
	}

	void Property_GameObject::FromString(const String& val)
	{
		XGUID guid;
		guid.fromString(val);
		IGameObject* obj = GameApp::getSingleton().GetGame()->GetGameObject(guid);
		SetValue(obj);
	}

	String Property_GameObject::ToString() const
	{
		IGameObject* obj = GetValue();
		if(obj)
		{
			return obj->GetGUID().toString();
		}

		return "";
	}
