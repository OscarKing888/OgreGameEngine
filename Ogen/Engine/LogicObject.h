#pragma once
#include "AbstractGameObject.h"

class _EngineExport LogicObject :
	public AbstractGameObject
{
public:

	LogicObject(void);
	virtual ~LogicObject(void);
};

//--------------------------------------------------------------------------------
typedef list<LogicObject*>::type LogicObjectList;
///
/// Class to track logic objects
///
class _EngineExport LogicObjectManager
	: public Singleton<LogicObjectManager>
	, public OgeAlloc
{
public:

	LogicObjectManager();
	virtual ~LogicObjectManager();

	void AddObject(LogicObject* obj);
	void RemoveObject(LogicObject* obj);

	const LogicObjectList& GetLoagicObjectList() const;

protected:

	LogicObjectList _lst;
};