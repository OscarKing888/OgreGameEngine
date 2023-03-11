#include "StdAfx.h"
#include "PropertyObject.h"

PropertyObject::PropertyObject(void)
: _distributeFlag(eDF_All)
{
}
//--------------------------------------------------------------------------------
PropertyObject::~PropertyObject(void)
{
}
//--------------------------------------------------------------------------------
void PropertyObject::BuildPropertyMap()
{
}
//--------------------------------------------------------------------------------
void PropertyObject::AddProperty(
								 const String& propName,
								 IPropertyPtr prop,
								 size_t uFlag,
								 const String&	description,
								 const String& category)
{
	if(!HasProperty(propName))
	{
		_propMap.insert(std::make_pair(propName, prop));
		prop->SetName(propName);
		prop->SetPropertyCategory(category);
		prop->SetDescription(description);
		prop->SetFlag(uFlag);
		prop->SetDisplayName(propName);
	}
	else
	{
		assert(false && "Type value not found!");
		throw std::exception("[PropertyObject::AddProperty] Property already exist!");
		// 
		// 			FANTASY_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
		// 				formatString("Property[%s] already exist, class type[%s]",
		// 				propName, getClassName(this)), 
		// 				"PropertyObject::AddProperty");
	}
}
//--------------------------------------------------------------------------------
void PropertyObject::RemoveProperty(const String& propName)
{
	PropertyMap::iterator i = _propMap.find(propName);
	if(i != _propMap.end())
	{
		_propMap.erase(i);
	}
}
//--------------------------------------------------------------------------------
String PropertyObject::GetProperty(const String& propName) const
{
	IPropertyPtr p = GetPropertyObject(propName);
	if(!p)
	{
		//assert(false);
		//LogErrorIDf("Engine", "[PropertyObject::GetProperty] Property [%s] not found, class type[%s]",
		//	propName.c_str(), getClassName(this).c_str());
		return 0;
	}

	return p->ToString();
}
//--------------------------------------------------------------------------------
void PropertyObject::SetProperty(const String& propName, const String& propValue)
{
	IPropertyPtr p = GetPropertyObject(propName);
	if(!p)
	{
		//assert(false);
		//LogErrorIDf("Engine", "[PropertyObject::SetProperty] Property [%s] value[%s] not found, class type[%s]",
		//	propName.c_str(), propValue.c_str(), getClassName(this).c_str());
		return;
	}

	p->FromString(propValue);
}
//--------------------------------------------------------------------------------
IPropertyPtr PropertyObject::GetPropertyObject(const String& propName) const
{
	PropertyMap::const_iterator i = _propMap.find(propName);
	if(i != _propMap.end())
	{
		return i->second;
	}

	// 		LogWarningIDf("Engine", "[PropertyObject::GetPropertyObject] Property [%s] not found, class type[%s]",
	// 			propName.c_str(), getClassName(this).c_str());

	return 0;
}
//--------------------------------------------------------------------------------
bool PropertyObject::HasProperty(const String& propName) const
{
	PropertyMap::const_iterator i = _propMap.find(propName);
	if(i != _propMap.end())
	{
		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------
unsigned int PropertyObject::GetDistributeFlag() const
{
	return _distributeFlag;
}
//--------------------------------------------------------------------------------
void PropertyObject::AddDistributeFlag(EDistributeFlag flg)
{
	_distributeFlag |= flg;
}
//--------------------------------------------------------------------------------
void PropertyObject::RemoveDistributeFlag(EDistributeFlag flg)
{
	_distributeFlag &= ~flg;
}
//--------------------------------------------------------------------------------
bool PropertyObject::HasDistributeFlag(int flg) const
{
	return (_distributeFlag & flg) != 0;
}
//--------------------------------------------------------------------------------
Ogre::NameValuePairList PropertyObject::GetValues() const
{
	Ogre::NameValuePairList nvps;
	PropertyMap::const_iterator i = _propMap.begin();
	for (; i != _propMap.end(); ++i)
	{
		nvps.insert(std::make_pair(i->first,
			GetProperty(i->first)));
	}

	return nvps;
}
//--------------------------------------------------------------------------------
void PropertyObject::SetValues( const Ogre::NameValuePairList& values )
{
	Ogre::NameValuePairList::const_iterator i = values.begin();
	for (; i != values.end(); ++i)
	{
		SetProperty(i->first, i->second);
	}
}
//--------------------------------------------------------------------------------
