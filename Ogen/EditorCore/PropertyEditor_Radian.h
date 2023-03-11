#pragma once
#include "PropertyEditorBase.h"

class PropertyEditor_Radian : public PropertyEditorBase
{
public:

	PropertyEditor_Radian(IPropertyPtr prop);
	virtual ~PropertyEditor_Radian();

	virtual BOOL OnEdit(LPPOINT lptClick);
	virtual BOOL OnUpdateValue();
};
