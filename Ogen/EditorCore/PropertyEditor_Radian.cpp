// PropertyEditor_Radian.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyEditor_Radian.h"
#include "PropertyGrid.h"

class PropertyEditor_RadianFactory : public TTypeUIEditorFactor<PropertyEditor_Radian>
{
};


M_REG_TYPEUIEDITOR_FACTORY(Radian, PropertyEditor_RadianFactory);

// PropertyEditor_Radian

PropertyEditor_Radian::PropertyEditor_Radian(IPropertyPtr prop)
: PropertyEditorBase(prop)
{
}

PropertyEditor_Radian::~PropertyEditor_Radian()
{
}

BOOL PropertyEditor_Radian::OnEdit(LPPOINT lptClick)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	Property_RadianPtr prop = TGetProperty<Property_RadianPtr>();

	if(prop->IsRangeLimited())
	{
		m_pWndInPlace = NULL;
		CSliderCtrl* pWndEdit = new CSliderCtrl();
		DWORD dwStyle = WS_VISIBLE | WS_CHILD;

		if (!m_bEnabled)
		{
			dwStyle |= ES_READONLY;
		}
		
		CRect rectEdit = m_Rect;
		PropertyGridEx* gridEx = dynamic_cast<PropertyGridEx*>(m_pWndList);
		int nMargin = m_pWndList->IsMarkModifiedProperties() && m_bIsModified ? gridEx->GetBoldEditLeftMargin() :gridEx->GetEditLeftMargin();

		rectEdit.left = m_pWndList->GetListRect().left + m_pWndList->GetPropertyColumnWidth() + m_Rect.Height() + AFX_TEXT_MARGIN - nMargin + 1;

		pWndEdit->Create(dwStyle, rectEdit, m_pWndList, AFX_PROPLIST_ID_INPLACE);
		m_pWndInPlace = pWndEdit;
		m_pWndInPlace->SetFocus();
		
		float minVal = prop->GetMinRange().valueRadians() * 100.0f;
		float maxVal = prop->GetMaxRange().valueRadians() * 100.0f;
		float val = prop->GetValue().valueRadians() * 100.0f;
		
		pWndEdit->SetRange(minVal, maxVal);
		pWndEdit->SetPos(val);
		pWndEdit->SetLineSize((maxVal - minVal) / 100.0f);
		pWndEdit->SetPageSize((maxVal - minVal) / 10.0f);

		m_bInPlaceEdit = TRUE;
	}
	else
	{
		PropertyEditorBase::OnEdit(lptClick);
	}

	return TRUE;
}

BOOL PropertyEditor_Radian::OnUpdateValue()
{
	ASSERT_VALID(this);

	if (m_pWndInPlace == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	Property_RadianPtr prop = TGetProperty<Property_RadianPtr>();

	if(prop->IsRangeLimited())
	{
		CSliderCtrl* pWndEdit = static_cast<CSliderCtrl*>(m_pWndInPlace);
		SetValue(COleVariant(StringConverter::toString((float)pWndEdit->GetPos() / 100.0f).c_str()));
	}
	else
	{
		CString val;
		m_pWndInPlace->GetWindowText(val);
		SetValue(COleVariant(val));
	}

	m_pWndList->OnPropertyChanged(this);

	return TRUE;
}
