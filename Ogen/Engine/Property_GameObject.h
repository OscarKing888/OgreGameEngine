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
#include "IGameObject.h"



	class _EngineExport Property_GameObject : public Property<IGameObject*, IGameObject*>
	{
	public:

		typedef Property<IGameObject*, IGameObject*> Base;
		typedef Base::SetFunType SetFunType;
		typedef Base::GetFunType GetFunType;

		Property_GameObject(SetFunType setFun, GetFunType getFun);
		virtual ~Property_GameObject(){}

		virtual const String	GetTypeName() const { return "GameObject"; }
		virtual void FromString(const String& val);
		virtual String ToString() const;
	};
