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

class _EngineExport Property_Vector2 : public Property<Vector2, Vector2>
{
public:

	typedef Property<Vector2, Vector2> Base;
	typedef Base::SetFunType SetFunType;
	typedef Base::GetFunType GetFunType;

	Property_Vector2(SetFunType setFun, GetFunType getFun);
	virtual ~Property_Vector2(){}

	virtual const String	GetTypeName() const { return "Vector2"; }
	virtual void FromString(const String& val);
	virtual String ToString() const;
};
//--------------------------------------------------------------------------------
class _EngineExport Property_Vector3 : public Property<Vector3, Vector3>
{
public:

	typedef Property<Vector3, Vector3> Base;
	typedef Base::SetFunType SetFunType;
	typedef Base::GetFunType GetFunType;

	Property_Vector3(SetFunType setFun, GetFunType getFun);
	virtual ~Property_Vector3(){}

	virtual const String	GetTypeName() const { return "Vector3"; }
	virtual void FromString(const String& val);
	virtual String ToString() const;
};
//--------------------------------------------------------------------------------
class _EngineExport Property_Vector4 : public Property<Vector4, Vector4>
{
public:

	typedef Property<Vector4, Vector4> Base;
	typedef Base::SetFunType SetFunType;
	typedef Base::GetFunType GetFunType;

	Property_Vector4(SetFunType setFun, GetFunType getFun);
	virtual ~Property_Vector4(){}

	virtual const String	GetTypeName() const { return "Vector4"; }
	virtual void FromString(const String& val);
	virtual String ToString() const;
};
//--------------------------------------------------------------------------------
class _EngineExport Property_Vector3List : public Property<Vector3List, Vector3List>
{
public:

	typedef Property<Vector3List, Vector3List> Base;
	typedef Base::SetFunType SetFunType;
	typedef Base::GetFunType GetFunType;

	Property_Vector3List(SetFunType setFun, GetFunType getFun);
	virtual ~Property_Vector3List(){}

	virtual const String	GetTypeName() const { return "Vector3List"; }
	virtual void FromString(const String& val);
	virtual String ToString() const;
};

