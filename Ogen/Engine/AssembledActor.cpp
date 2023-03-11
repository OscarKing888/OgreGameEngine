#include "stdafx.h"
#include "AssembledActor.h"
#if 0
class AssembledActorFactory : public TGameObjectFactory<AssembledActor>
{
public:

	AssembledActorFactory()
	{
		MovableObject::setDefaultVisibilityFlags(VisualObject::DefaultVisibilityFlag);
	}
};

M_REG_FACTORY(AssembledActor, AssembledActorFactory, Actor)
//--------------------------------------------------------------------------------
AssembledActor::AssembledActor()
{

}
//--------------------------------------------------------------------------------
AssembledActor::~AssembledActor()
{

}
//--------------------------------------------------------------------------------
void AssembledActor::AddModel(const String& meshFileName, bool linkToMainBody)
{
	String name = FormatEntityName(meshFileName);

	SceneManager* sm = GetSceneManager();

	Entity* ent = sm->createEntity(name, meshFileName);
	if(linkToMainBody)
	{
		Entity* mainBody = GetMainBody();
		SharedEntityPair pr;
		pr.first = ent;
		pr.second = false;
		if(mainBody)
		{
			ent->shareSkeletonInstanceWith(mainBody);
			//ent->getSkeleton()->addLinkedSkeletonAnimationSource(mainBody->getSkeleton()->getName());
			pr.second = true;
			_linkedEntityMap.insert(std::make_pair(meshFileName, pr));
		}
		else
		{
			_linkedEntityMap.insert(std::make_pair(meshFileName, pr));
		}
	}
	else
	{
		SharedEntityPair pr;
		pr.first = ent;
		pr.second = false;
		_unlinkedEntityMap.insert(std::make_pair(meshFileName, pr));
	}

	_node->attachObject(ent);
}
//--------------------------------------------------------------------------------
Entity* AssembledActor::GetMainBody() const
{
	return _entity;
}
//--------------------------------------------------------------------------------
void AssembledActor::Destroy()
{		
	DestroyLinkedEntities();
	Actor::Destroy();
}
//--------------------------------------------------------------------------------
void AssembledActor::DestroyLinkedEntities(bool clearAll)
{
	SceneManager* sm = GetSceneManager();
	EntityMap::iterator i = _linkedEntityMap.begin();
	for (; i != _linkedEntityMap.end(); ++i)
	{
		Entity* ent = i->second.first;
		if(ent)
		{
			sm ->destroyEntity(ent);
			i->second.first = 0;
		}
	}

	if(clearAll)
	{
		_linkedEntityMap.clear();
	}
}
//--------------------------------------------------------------------------------
void AssembledActor::PreDestroyEntity()
{
	Actor::PreDestroyEntity();
	DestroyLinkedEntities();
}
//--------------------------------------------------------------------------------
void AssembledActor::PostCreateEntity()
{
	Actor::PostCreateEntity();

	SceneManager* sm = GetSceneManager();
	EntityMap::iterator i = _linkedEntityMap.begin();
	for (; i != _linkedEntityMap.end(); ++i)
	{
		Entity* ent = i->second.first;
		if(!ent)
		{
			String name = FormatEntityName(i->first);

			Entity* ent = sm->createEntity(name, i->first);
			Entity* mainBody = GetMainBody();
			ent->shareSkeletonInstanceWith(mainBody);
			// 				ent->getSkeleton()->addLinkedSkeletonAnimationSource(mainBody->getSkeleton()->getName());
			i->second.first = ent;
			_node->attachObject(ent);
		}
		else
		{
			if(!i->second.second)
			{
				Entity* mainBody = GetMainBody();
				ent->shareSkeletonInstanceWith(mainBody);
			}
		}
	}

	i = _unlinkedEntityMap.begin();
	for (; i != _unlinkedEntityMap.end(); ++i)
	{
		Entity* ent = i->second.first;
		if(!ent)
		{
			String name = FormatEntityName(i->first);

			Entity* ent = sm->createEntity(name, i->first);
			i->second.first = ent;
			_node->attachObject(ent);
		}
	}
}
//--------------------------------------------------------------------------------
String AssembledActor::FormatEntityName(const String& meshName)
{
	String name = "[";
	name += meshName + "]-";
	name += GetGUID().toString();
	return name;
}
//--------------------------------------------------------------------------------
void AssembledActor::DestroyModel(const String& meshFileName)
{
	SceneManager* sm = GetSceneManager();

	EntityMap::iterator it = _linkedEntityMap.find(meshFileName);
	if(it != _linkedEntityMap.end())
	{
		Entity* ent = it->second.first;
		_node->detachObject(ent);
		sm->destroyEntity(ent);
		_linkedEntityMap.erase(it);
	}
	else
	{
		it = _unlinkedEntityMap.find(meshFileName);
		if(it != _unlinkedEntityMap.end())
		{
			Entity* ent = it->second.first;
			_node->detachObject(ent);
			sm->destroyEntity(ent);
			_unlinkedEntityMap.erase(it);
		}
	}
}
//--------------------------------------------------------------------------------

#endif