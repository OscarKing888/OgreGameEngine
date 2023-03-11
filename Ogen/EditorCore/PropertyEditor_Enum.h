#pragma once
#include "PropertyEditorBase.h"

class PropertyEditor_Enum : public PropertyEditorBase
{
public:

	PropertyEditor_Enum(IPropertyPtr prop);
	virtual ~PropertyEditor_Enum();
};
