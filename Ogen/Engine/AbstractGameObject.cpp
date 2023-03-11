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
#include "AbstractGameObject.h"
#include "Property_String.h"
#include "IState.h"

AbstractGameObject::AbstractGameObject(void)
: _serializeable(true)
, _triggerMask((uint)~0)
// 		, _tile(0)
{
	_stateManager = OGRE_NEW StateManager();
}
//--------------------------------------------------------------------------------
AbstractGameObject::~AbstractGameObject(void)
{
	OGRE_DELETE _stateManager;
	_stateManager = 0;
}
//--------------------------------------------------------------------------------
void AbstractGameObject::BuildPropertyMap()
{
	// 		AddProperty("Name", OGRE_NEW Property_String(
	// 			std::tr1::bind(&AbstractGameObject::SetName, this, std::tr1::placeholders::_1),
	// 			std::tr1::bind(&AbstractGameObject::GetName, this)));

	//AddProp(Property_String, Name, AbstractGameObject);
	AddProp(Property_XGUID, GUID, AbstractGameObject, "GUID", "ObjectSystem");
	AddProp(Property_Bool, Serializeable, AbstractGameObject, "Is this objec serializeable flag", "ObjectSystem");
	AddProp(Property_IntHex, TriggerMask, AbstractGameObject, "TriggerMask for this object", "ObjectSystem");
}
//--------------------------------------------------------------------------------
/*template<class PropertyClassType, class SetFunType, class GetFunType>
IPropertyPtr AbstractGameObject::AddProperty(
const String& propName,
SetFunType setFun, GetFunType getFun,
size_t uFlag,
const String&	description,
const String& category)
{
IPropertyPtr prop = OGRE_NEW PropertyClassType(
std::tr1::bind(setFun, this, std::tr1::placeholders::_1),
std::tr1::bind(getFun, this));

AddProperty(propName, prop, uFlag, description, category);
return prop;
}*/

void AbstractGameObject::Destroy()
{
	//_propMap.clear();
}
//--------------------------------------------------------------------------------
// 	void AbstractGameObject::PrintProperties(std::ostream& os)
// 	{
// 		PropertyMap::iterator i = _propMap.begin();
// 		for (; i != _propMap.end(); ++i)
// 		{
// 			IPropertyPtr p = i->second;
// 			os << i->first << "=" << p->ToString() << "\n";
// 		}
// 	}
//--------------------------------------------------------------------------------
void AbstractGameObject::SetSerializeable(bool on)
{
	_serializeable = on;
}
//--------------------------------------------------------------------------------
bool AbstractGameObject::GetSerializeable() const
{
	return _serializeable;
}
//--------------------------------------------------------------------------------
unsigned int AbstractGameObject::GetTriggerMask() const
{
	return _triggerMask;
}
//--------------------------------------------------------------------------------
void AbstractGameObject::SetTriggerMask(unsigned int mask)
{
	_triggerMask = mask;
}
//--------------------------------------------------------------------------------
void AbstractGameObject::OnAddToScene(SceneNode* rootNode)
{
}
//--------------------------------------------------------------------------------
void AbstractGameObject::OnRemoveFromScene(SceneNode* rootNode)
{
}
//--------------------------------------------------------------------------------
void AbstractGameObject::PreSave()
{

}
//--------------------------------------------------------------------------------
void AbstractGameObject::PostSave()
{

}
//--------------------------------------------------------------------------------
void AbstractGameObject::PreLoad()
{

}
//--------------------------------------------------------------------------------
void AbstractGameObject::PostLoad()
{

}
//--------------------------------------------------------------------------------
const XGUID& AbstractGameObject::GetGUID() const
{
	return _guid;
}
//--------------------------------------------------------------------------------
void AbstractGameObject::SetGUID(const XGUID& guid)
{
	_guid = guid;
}
const PropertyMap& AbstractGameObject::GetProperties() const
{
	return PropertyObject::GetProperties();
}
//--------------------------------------------------------------------------------
String AbstractGameObject::GetProperty(const String& propName) const
{
	return PropertyObject::GetProperty(propName);
}
//--------------------------------------------------------------------------------
void AbstractGameObject::SetProperty(const String& propName, const String& propValue)
{
	return PropertyObject::SetProperty(propName, propValue);
}
//--------------------------------------------------------------------------------
IPropertyPtr AbstractGameObject::GetPropertyObject(const String& propName) const
{
	return PropertyObject::GetPropertyObject(propName);
}
//--------------------------------------------------------------------------------
bool AbstractGameObject::HasProperty(const String& propName) const
{
	return PropertyObject::HasProperty(propName);
}
//--------------------------------------------------------------------------------
const String& AbstractGameObject::GetDescription() const
{
	return PropertyObject::GetDescription();
}
//--------------------------------------------------------------------------------
void AbstractGameObject::SetDescription(const String& name)
{
	PropertyObject::SetDescription(name);
}
//--------------------------------------------------------------------------------
unsigned int AbstractGameObject::GetDistributeFlag() const
{
	return PropertyObject::GetDistributeFlag();
}
//--------------------------------------------------------------------------------
void AbstractGameObject::AddDistributeFlag(EDistributeFlag flg)
{
	PropertyObject::AddDistributeFlag(flg);
}
//--------------------------------------------------------------------------------
void AbstractGameObject::RemoveDistributeFlag(EDistributeFlag flg)
{
	PropertyObject::RemoveDistributeFlag(flg);
}
//--------------------------------------------------------------------------------
bool AbstractGameObject::HasDistributeFlag(int flg) const
{
	return PropertyObject::HasDistributeFlag(flg);
}
//--------------------------------------------------------------------------------
Ogre::NameValuePairList AbstractGameObject::GetValues() const
{
	return PropertyObject::GetValues();
}
//--------------------------------------------------------------------------------
void AbstractGameObject::SetValues( const Ogre::NameValuePairList& values )
{
	PropertyObject::SetValues(values);
}
//--------------------------------------------------------------------------------
IStateManager* AbstractGameObject::GetStateManager()
{
	return _stateManager;
}
//--------------------------------------------------------------------------------
void AbstractGameObject::ActiveState( const Ogre::String& stateID )
{
	_stateManager->ActiveState(stateID);
}
//--------------------------------------------------------------------------------
const Ogre::String& AbstractGameObject::GetActiveStateID() const
{
	return _stateManager->GetActiveStateID();
}
//--------------------------------------------------------------------------------
const Ogre::String& AbstractGameObject::GetPreviousStateID() const
{
	return _stateManager->GetPreviousStateID();
}
//--------------------------------------------------------------------------------
void AbstractGameObject::PushState( const Ogre::String& stateID )
{
	_stateManager->PushState(stateID);
}
//--------------------------------------------------------------------------------
void AbstractGameObject::PopState()
{
	_stateManager->PopState();
}
//--------------------------------------------------------------------------------
Ogre::StringVector AbstractGameObject::GetStateNameList() const
{
	return _stateManager->GetStateNameList();
}
//--------------------------------------------------------------------------------
Ogre::StringVector AbstractGameObject::GetStateStack() const
{
	return _stateManager->GetStateStack();
}
//--------------------------------------------------------------------------------
void AbstractGameObject::PostSceneLoad()
{

}
//--------------------------------------------------------------------------------
// 	ITile* AbstractGameObject::GetTile() const
// 	{
// 		return _tile;
// 	}
// 	void AbstractGameObject::Assign(const AbstractGameObject &src, XGUID newGUID)
// 	{
// 		_guid = newGUID;
// 		_description = src._description;
// 		_serializeable = src._serializeable;
// 		_triggerMask = src._triggerMask;
// 		_distributeFlag = src._distributeFlag;
// 	}
//----------------------------------------------------------------------------------------------
