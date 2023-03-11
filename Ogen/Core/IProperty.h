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
#include "CorePrerequisites.h"
#include "RefObject.h"

enum _CoreExport EPropertyFlag
{
	ePropertyFlagReadonly			= 1 << 0,		// property is readonly
	ePropertyFlagSerializable		= 1 << 1,		// property can be saved into to file
	ePropertyFlagEditable			= 1 << 2,		// property can be seen in editor

	ePropertyFlagDefault			= ePropertyFlagSerializable | ePropertyFlagEditable
};

class _CoreExport IProperty : public RefObject
{
public:

	virtual ~IProperty(void);

	virtual const String	GetTypeName() const = 0;

	virtual const String&	GetName() const = 0;
	virtual  void SetName(const String& strName) = 0;

	virtual const String&	GetDisplayName() const = 0;
	virtual void SetDisplayName(const String& strName) = 0;

	virtual const String&	GetPropertyCategory() const = 0;
	virtual void SetPropertyCategory(const String& strCategory) = 0;

	virtual const String&	GetDescription() const = 0;
	virtual void SetDescription(const String& strDescription) = 0;

	virtual bool IsSerializeable() const = 0;
	virtual void SetSerializeable(bool on) = 0;

	virtual bool IsReadOnly() const = 0;
	virtual void SetReadOnly(bool on) = 0;

	virtual size_t GetFlag() const = 0;
	virtual void SetFlag(size_t uFlag) = 0;

	virtual bool IsVisibleToEditor() const = 0;
	virtual void SetVisibleToEditor(bool on) = 0;

	virtual void FromString(const String& val) = 0;
	virtual String ToString() const = 0;

	// property priority used for serializing
	virtual int GetSerializingPriority() const = 0;
	virtual void SetSerializingPriority(int priority) = 0;

	virtual int GetDistributeFlag() const = 0;
	virtual void SetDistributeFlag(int flag) = 0;

	virtual float GetVisibleRange() const = 0;
	virtual void SetVisibleRange(float range) = 0;
};

SmartPointer(IProperty);

typedef map<String, IPropertyPtr>::type PropertyMap;

//----------------------------------------------------------------------------------------------
///
/// Distribute flag for objects/properties
///
enum _CoreExport EDistributeFlag
{
	eDF_Client = 1,
	eDF_Server = 2,
	eDF_Tools = 4,
	eDF_ClientTools = eDF_Client | eDF_Tools,
	eDF_ServerTools = eDF_Server | eDF_Tools,
	eDF_All = eDF_Client | eDF_Server | eDF_Tools
};

//----------------------------------------------------------------------------------------------

class _CoreExport IPropertyObject
{
public:

	virtual void BuildPropertyMap() = 0;

	virtual const PropertyMap& GetProperties() const = 0;

	virtual String GetProperty(const String& propName) const = 0;
	virtual void SetProperty(const String& propName, const String& propValue) = 0;

	virtual IPropertyPtr GetPropertyObject(const String& propName) const = 0;

	virtual bool HasProperty(const String& propName) const = 0;

	virtual const String& GetDescription() const = 0;
	virtual void SetDescription(const String& name) = 0;

	virtual unsigned int GetDistributeFlag() const = 0;
	virtual void AddDistributeFlag(EDistributeFlag flg) = 0;
	virtual void RemoveDistributeFlag(EDistributeFlag flg) = 0;
	virtual bool HasDistributeFlag(int flg) const = 0;

	virtual Ogre::NameValuePairList GetValues() const = 0;
	virtual void SetValues(const Ogre::NameValuePairList& values) = 0;
};


