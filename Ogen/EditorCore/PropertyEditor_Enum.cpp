// PropertyEditor_Enum.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyEditor_Enum.h"

class PropertyEditor_EnumFactory : public TTypeUIEditorFactor<PropertyEditor_Enum>
{
};


M_REG_TYPEUIEDITOR_FACTORY(enum, PropertyEditor_EnumFactory);

// PropertyEditor_Enum

PropertyEditor_Enum::PropertyEditor_Enum(IPropertyPtr prop)
: PropertyEditorBase(prop)
{
	AllowEdit(FALSE);
	IPropertyEnum* enumProp = dynamic_cast<IPropertyEnum*>(prop.GetPointer());
	
	String enumType = enumProp->GetEnumTypeName();
	IEnumDescriptor* eds = EnumDescriptorManager::getSingleton().GetEnumDescriptor(enumType);
	StringVector valLst = eds->GetEnumValuesList();
	for(size_t i = 0; i < valLst.size(); ++i)
	{
		AddOption(valLst[i].c_str());
	}
}

PropertyEditor_Enum::~PropertyEditor_Enum()
{
}


// PropertyEditor_Enum member functions
