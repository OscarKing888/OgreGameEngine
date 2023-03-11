#pragma once

///
/// General property add function
///
#define AddProp(propertyClassName, name, className, description, category)	\
	PropertyObject::TAddProperty<className, propertyClassName>(#name,		\
		&className::Set##name, &className::Get##name, ePropertyFlagDefault, description, category)

///
/// General property add function
///
#define AddEditorProp(propertyClassName, name, className, description, category)	\
	PropertyObject::TAddProperty<className, propertyClassName>(#name,		\
	&className::Set##name, &className::Get##name, ePropertyFlagEditable, description, category)

///
/// Add read only property
///
#define AddReadonlyProp(propertyClassName, name, className, description, category)			\
	PropertyObject::TAddProperty<className, propertyClassName>(								\
		#name, &className::Set##name, &className::Get##name,								\
		ePropertyFlagReadonly | ePropertyFlagEditable, description, category)

///
/// For overload functions that get/set parameters type are differents
///
#define AddPropEx2(propertyClassName, name, className, setArgType, getArgType, description, category)	\
{																										\
	void (className::*pSetFun)(setArgType) = &className::Set##name;										\
	getArgType (className::*pGetFun)(void) const = &className::Get##name;								\
	IPropertyPtr prop = OGRE_NEW propertyClassName(												\
	std::tr1::bind(pSetFun, static_cast<className*>(this), std::tr1::placeholders::_1),					\
	std::tr1::bind(pGetFun, static_cast<className*>(this)));											\
	PropertyObject::AddProperty(#name, prop, ePropertyFlagDefault, description, category);		\
}

///
/// For overload functions that get/set has the same parameters type
///
#define AddPropEx1(propertyClassName, name, className, argType, description, category) \
	AddPropEx2(propertyClassName, name, className, argType, argType, description, category)

///
/// Add enum property
///
#define AddEnumProp(enumTypeConverterClass, enumType, name, className, description, category)	\
{																								\
	Property_Enum<enumType>* prop =													\
	dynamic_cast< Property_Enum<enumType>* >(											\
	PropertyObject::TAddProperty<className, Property_Enum<enumType> >					\
	(#name, &className::Set##name,																\
	&className::Get##name, ePropertyFlagDefault, description, category).GetPointer());	\
	prop->SetEnumTypeName(#enumTypeConverterClass);												\
 }

///
/// Add float property with min/max range
///
#define AddRangeFloatProp(name, minRange, maxRange, className, description, category)	\
{																						\
	Property_FloatPtr prop = Property_FloatPtr::DynamicCast(			\
		PropertyObject::TAddProperty<className, Property_Float>(#name,			\
		&className::Set##name, &className::Get##name,									\
		ePropertyFlagDefault, description, category));							\
	prop->SetRangeLimited(true, minRange, maxRange);									\
}


///
/// Add radian property with min/max range
///
#define AddRangeRadianProp(name, minRange, maxRange, className, description, category)			\
{																								\
	Property_RadianPtr prop = Property_RadianPtr::DynamicCast(				\
		PropertyObject::TAddProperty<className, Property_Radian>(#name,				\
		&className::Set##name, &className::Get##name,											\
		ePropertyFlagDefault, description, category));									\
	prop->SetRangeLimited(true, minRange, maxRange);											\
}

///
/// Add visible range property
///
#define AddVisibleRangeProp(propertyClassName, name, className, visibleRange, description, category)	\
{																										\
	IPropertyPtr prop = PropertyObject::TAddProperty<className, propertyClassName>(			\
		#name, &className::Set##name, &className::Get##name,											\
		ePropertyFlagDefault, description, category);											\
	prop->SetVisibleRange(visibleRange);																\
}

///
/// Add string property with list options
///
#define AddStringPropWithSourceList(name, sourcePropName, className, description, category) \
{																							\
	Property_StringPtr prop = Property_StringPtr::DynamicCast(			\
		PropertyObject::TAddProperty<className, Property_String>(					\
		#name, &className::Set##name, &className::Get##name,								\
		ePropertyFlagDefault, description, category));								\
	prop->SetSourceProperty(GetPropertyObject(#sourcePropName).GetPointer());				\
}
