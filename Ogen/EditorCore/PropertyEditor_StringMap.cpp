// PropertyEditor_StringMap.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyEditor_StringMap.h"

class PropertyEditor_StringMapFactory : public TTypeUIEditorFactor<PropertyEditor_StringMap>
{
};

M_REG_TYPEUIEDITOR_FACTORY(StringMap, PropertyEditor_StringMapFactory);

// PropertyEditor_StringMap

PropertyEditor_StringMap::PropertyEditor_StringMap(IPropertyPtr prop)
: PropertyEditorBase(prop)
{
	m_bGroup = true;	
}

PropertyEditor_StringMap::~PropertyEditor_StringMap()
{
}

void PropertyEditor_StringMap::InitSubItems()
{
	Property_StringMapPtr p = Property_StringMapPtr::DynamicCast(_prop);	
	StringMap m = p->GetValue();
	for (StringMap::iterator i = m.begin(); i != m.end(); ++i)
	{
		String name = i->first;
		String value = i->second;
		CMFCPropertyGridProperty* subItem = new PropertyEditorSubItem_String(this, name, value, _prop->GetDisplayName());
		AddSubItem(subItem);
	}
}


void PropertyEditor_StringMap::UpdateDataFromProperty()
{
	RemoveAllSubItems();
	InitSubItems();
	PropertyEditorBase::UpdateDataFromProperty();
}

void PropertyEditor_StringMap::UpdateDataToProperty()
{
	StringMap m;
	for (int i = 0; i < GetSubItemsCount(); ++i)
	{
		CMFCPropertyGridProperty* subItem = GetSubItem(i);
		ISubItem* sub = dynamic_cast<ISubItem*>(subItem);
		assert(sub);
		String name = sub->getName();
		String val = (LPCTSTR)CString(subItem->GetValue());
		m.insert(std::make_pair(name, val));
	}

	this->SetValue(Property_StringMap::toString(m).c_str());

	PropertyEditorBase::UpdateDataToProperty();
}

void PropertyEditor_StringMap::OnChildValueChanged(CMFCPropertyGridProperty* child, const String& name, const String& value)
{
	UpdateDataToProperty();
}


// PropertyEditor_StringMap member functions
