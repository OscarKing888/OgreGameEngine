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
#include "EnginePrerequisites.h"
#include "IGameObject.h"

class _EngineExport IGameObjectFactory
{
public:

	virtual IGameObject* Create(const XGUID& objID, const NameValuePairList* para = 0) = 0;
	virtual void Destroy(IGameObject* obj) = 0;
	virtual bool IsShownInEditorClassList() const = 0;
	virtual const String& GetClassCategory() const = 0;
	virtual void InitClassCategory(const String& name) = 0;
};

class _EngineExport AbstractGameObjectFactory : public IGameObjectFactory
{
public:

	template<class T>
	struct SDestroy
	{
		SDestroy(IGameObject* obj)
		{
			obj->Destroy();
			OGRE_DELETE dynamic_cast<T*>(obj);
		}
	};

	virtual bool IsShownInEditorClassList() const
	{
		return true;
	}

	virtual IGameObject* Create(const XGUID& objID, const NameValuePairList* para = 0);

	virtual void InitClassCategory(const String& name);
	virtual const String& GetClassCategory() const;

protected:

	String _classCategory;

	virtual IGameObject* CreateImp(const XGUID& objID, const NameValuePairList* para = 0) = 0;
};

template<class T>
class TGameObjectFactory : public AbstractGameObjectFactory
{
public:

	virtual void Destroy(IGameObject* obj)
	{
		SDestroy<T> d(obj);
	}

	virtual IGameObject* CreateImp(const XGUID& objID, const NameValuePairList* para = 0)
	{
		IGameObject* obj = OGRE_NEW T();
		return obj;
	}
};

