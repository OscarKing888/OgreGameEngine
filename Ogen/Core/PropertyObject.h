#pragma once

#include "IProperty.h"
#include "PropMacros.h"

class _CoreExport PropertyObject : virtual public IPropertyObject
{
public:

	PropertyObject(void);
	virtual ~PropertyObject(void);

	virtual void BuildPropertyMap();

	virtual const PropertyMap& GetProperties() const { return _propMap; }

	virtual String GetProperty(const String& propName) const;
	virtual void SetProperty(const String& propName, const String& propValue);

	virtual IPropertyPtr GetPropertyObject(const String& propName) const;

	virtual bool HasProperty(const String& propName) const;

	virtual const String& GetDescription() const { return _description; }
	virtual void SetDescription(const String& name) { _description = name; }

	virtual unsigned int GetDistributeFlag() const;
	virtual void AddDistributeFlag(EDistributeFlag flg);
	virtual void RemoveDistributeFlag(EDistributeFlag flg);
	virtual bool HasDistributeFlag(int flg) const;

	virtual Ogre::NameValuePairList GetValues() const;
	virtual void SetValues(const Ogre::NameValuePairList& values);

protected:

	void AddProperty(const String& propName, IPropertyPtr prop,
		size_t uFlag = ePropertyFlagDefault,
		const String&	description = "Object Description",
		const String& category = "Common");

	template<class ClassType, class PropertyClassType, class SetFunType, class GetFunType>
	IPropertyPtr TAddProperty(const String& propName, SetFunType setFun, GetFunType getFun,
		size_t uFlag = ePropertyFlagDefault,
		const String&	description = "Object Description",
		const String& category = "Common")
	{
		IPropertyPtr prop = OGRE_NEW PropertyClassType(
			std::tr1::bind(setFun, dynamic_cast<ClassType*>(this), std::tr1::placeholders::_1),
			std::tr1::bind(getFun, dynamic_cast<ClassType*>(this)));

		AddProperty(propName, prop, uFlag, description, category);
		return prop;
	}

	void RemoveProperty(const String& propName);

	PropertyMap		_propMap;

	String	_description;
	unsigned int	_distributeFlag;
};
