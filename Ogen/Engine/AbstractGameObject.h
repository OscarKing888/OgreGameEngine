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
#include "IGameObject.h"
#include "PropertyObject.h"
#include "IGameObjectFactory.h"

class _EngineExport AbstractGameObject
	: virtual public IGameObject
	, virtual public PropertyObject
	, public OgeAlloc
{
public:

	//DeepClone(AbstractGameObject);

	AbstractGameObject(void);
	virtual ~AbstractGameObject(void);

	virtual const XGUID& GetGUID() const;
	virtual void SetGUID(const XGUID& guid);

	virtual void Destroy();
	//virtual void PrintProperties(std::ostream& os);

	virtual void SetSerializeable(bool on);
	virtual bool GetSerializeable() const;

	virtual unsigned int GetTriggerMask() const;
	virtual void SetTriggerMask(unsigned int mask);

	virtual void BuildPropertyMap();

	virtual void PreSave();
	virtual void PostSave();
	virtual void PreLoad();
	virtual void PostLoad();
	virtual void PostSceneLoad();

	virtual const PropertyMap& GetProperties() const;

	virtual String GetProperty(const String& propName) const;
	virtual void SetProperty(const String& propName, const String& propValue);

	virtual IPropertyPtr GetPropertyObject(const String& propName) const;

	virtual bool HasProperty(const String& propName) const;

	virtual const String& GetDescription() const;
	virtual void SetDescription(const String& name);

	virtual unsigned int GetDistributeFlag() const;
	virtual void AddDistributeFlag(EDistributeFlag flg);
	virtual void RemoveDistributeFlag(EDistributeFlag flg);
	virtual bool HasDistributeFlag(int flg) const;

	virtual Ogre::NameValuePairList GetValues() const;
	virtual void SetValues(const Ogre::NameValuePairList& values);

	virtual IStateManager* GetStateManager();
	virtual void ActiveState(const Ogre::String& stateID);
	virtual const Ogre::String& GetActiveStateID() const;
	virtual const Ogre::String& GetPreviousStateID() const;
	virtual void PushState(const Ogre::String& stateID);
	virtual void PopState();
	virtual Ogre::StringVector GetStateNameList() const;
	virtual Ogre::StringVector GetStateStack() const;

protected:

	XGUID			_guid;
	bool			_serializeable;
	unsigned int	_triggerMask;
	IStateManager*	_stateManager;
	//ITile*			_tile;

	virtual void OnAddToScene(SceneNode* rootNode);
	virtual void OnRemoveFromScene(SceneNode* rootNode);
	//virtual ITile* GetTile() const;
};