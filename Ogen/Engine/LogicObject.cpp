#include "StdAfx.h"
#include "LogicObject.h"

template<> LogicObjectManager* Singleton<LogicObjectManager>::ms_Singleton = 0;

LogicObject::LogicObject(void)
{
	LogicObjectManager::getSingleton().AddObject(this);
}
//--------------------------------------------------------------------------------
LogicObject::~LogicObject(void)
{
	LogicObjectManager::getSingleton().RemoveObject(this);
}
//--------------------------------------------------------------------------------
// LogicObjectManager
//--------------------------------------------------------------------------------
LogicObjectManager::LogicObjectManager()
{

}
//--------------------------------------------------------------------------------
LogicObjectManager::~LogicObjectManager()
{

}
//--------------------------------------------------------------------------------
void LogicObjectManager::AddObject( LogicObject* obj )
{
	_lst.push_back(obj);
}
//--------------------------------------------------------------------------------
void LogicObjectManager::RemoveObject( LogicObject* obj )
{
	LogicObjectList::iterator it =
		std::find(_lst.begin(), _lst.end(), obj);

	if(it != _lst.end())
	{
		_lst.erase(it);
	}
	else
	{
		assert(false && "[LogicObjectManager::RemoveObject]LogicObject not found!");
	}
}
//--------------------------------------------------------------------------------
const LogicObjectList& LogicObjectManager::GetLoagicObjectList() const
{
	return _lst;
}