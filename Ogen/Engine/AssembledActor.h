#pragma once
#include "Actor.h"
#if 0
class _EngineExport AssembledActor : public Actor
{
public:

	AssembledActor();
	virtual ~AssembledActor();

	void AddModel(const String& meshFileName, bool linkToMainBody = true);
	void DestroyModel(const String& meshFileName);

	Entity* GetMainBody() const;

	void DestroyLinkedEntities(bool clearAll = true);

protected:

	String FormatEntityName(const String& meshName);

	virtual void Destroy();

	virtual void PreDestroyEntity();
	virtual void PostCreateEntity();

	typedef std::pair<Entity*, bool> SharedEntityPair;
	typedef map<String, SharedEntityPair>::type EntityMap;
	EntityMap	_linkedEntityMap;
	EntityMap	_unlinkedEntityMap;
};
#endif