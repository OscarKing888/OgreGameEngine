// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-07-18
// *************************************************************************************

#include "stdafx.h"
#include "IGameObjectFactory.h"



IGameObject* AbstractGameObjectFactory::Create(const XGUID& guid, const NameValuePairList* para)
{
	IGameObject* obj = CreateImp(guid, para);
	obj->SetGUID(guid);
	obj->BuildPropertyMap();

	if(para != 0)
	{
		NameValuePairList::const_iterator i = para->begin();
		for (; i != para->end(); ++i)
		{
			if(obj->HasProperty(i->first))
			{
				obj->SetProperty(i->first, i->second);
			}
			else
			{
				LogErrorIDf("Engine", "[AbstractGameObjectFactory::Create]Unknown para:[%s]-[%s]",
					i->first.c_str(), i->second.c_str());
			}
		}
	}

	return obj;
}

void AbstractGameObjectFactory::InitClassCategory(const String& name)
{
	assert(_classCategory.empty());
	_classCategory = name;
}

const String& AbstractGameObjectFactory::GetClassCategory() const
{
	return _classCategory;
}

