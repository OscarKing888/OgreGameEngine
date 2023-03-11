#pragma once
#include "PropertyEditorBase.h"

class PropertyEditor_Bool : public PropertyEditorBase
{

public:

	PropertyEditor_Bool(IPropertyPtr prop);
	virtual ~PropertyEditor_Bool();

protected:

	virtual void OnInitEditUI();
	virtual BOOL OnEdit(LPPOINT lptClick);
	virtual BOOL OnUpdateValue();
	virtual void OnPosSizeChanged(CRect rectOld);
	virtual void OnDrawValue(CDC* pDC, CRect rect);
	//virtual HBRUSH OnCtlColor(CDC* pDC, UINT nCtlColor);
};
