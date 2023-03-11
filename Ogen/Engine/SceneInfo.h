#pragma once
#include "EnginePrerequisites.h"
#include "LogicObject.h"

//------------------------------------------------------------------------
// SceneInfo for server
//------------------------------------------------------------------------

class _EngineExport SceneInfo
	: public LogicObject
	, public Singleton<SceneInfo>
{
public:

	SceneInfo(void);
	virtual ~SceneInfo(void);

	virtual const String& GetTypeName() const;

	void SetMapWidth(int w){}
	int GetMapWidth() const;

	void SetMapHeight(int h){}
	int GetMapHeight() const;

protected:

	virtual void BuildPropertyMap();
};