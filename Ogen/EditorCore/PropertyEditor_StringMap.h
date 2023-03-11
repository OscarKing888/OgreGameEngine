#pragma once
#include "PropertyEditorBase.h"

class PropertyEditor_StringMap : public PropertyEditorBase
{
public:

	PropertyEditor_StringMap(IPropertyPtr prop);
	virtual ~PropertyEditor_StringMap();
	
	virtual void UpdateDataFromProperty();
	virtual void UpdateDataToProperty();
	virtual void OnChildValueChanged(CMFCPropertyGridProperty* child, const String& name, const String& value);

	virtual void InitSubItems();
};