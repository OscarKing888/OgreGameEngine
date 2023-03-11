// PropertyEditor_Float.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyEditor_Float.h"
#include "PropertyGrid.h"

class PropertyEditor_FloatFactory
	: public TTypeUIEditorFactor<PropertyEditor_Float>
{
};

M_REG_TYPEUIEDITOR_FACTORY(float, PropertyEditor_FloatFactory);

//--------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(SliderCtrlEx, CSliderCtrl)

SliderCtrlEx::SliderCtrlEx()
: _listener(0)
{

}

SliderCtrlEx::~SliderCtrlEx()
{
}


BEGIN_MESSAGE_MAP(SliderCtrlEx, CSliderCtrl)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()



// SliderCtrlEx message handlers



void SliderCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CSliderCtrl::OnHScroll(nSBCode, nPos, pScrollBar);

	if(_listener)
	{
		_listener->OnHScroll(nSBCode, nPos, pScrollBar);
	}
}

//--------------------------------------------------------------------------------

// PropertyEditor_Float

PropertyEditor_Float::PropertyEditor_Float(IPropertyPtr prop)
: PropertyEditorBase(prop)
{
}

PropertyEditor_Float::~PropertyEditor_Float()
{
}

BOOL PropertyEditor_Float::OnEdit(LPPOINT lptClick)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	Property_FloatPtr prop = TGetProperty<Property_FloatPtr>();

	if(prop->IsRangeLimited())
	{
		m_pWndInPlace = NULL;
		SliderCtrlEx* pWndEdit = new SliderCtrlEx();
		pWndEdit->setListener(this);
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
		
		float minVal = prop->GetMinRange() * 100.0f;
		float maxVal = prop->GetMaxRange() * 100.0f;
		float val = prop->GetValue() * 100.0f;
		
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

BOOL PropertyEditor_Float::OnUpdateValue()
{
	ASSERT_VALID(this);

	if (m_pWndInPlace == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	Property_FloatPtr prop = TGetProperty<Property_FloatPtr>();

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

void PropertyEditor_Float::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnUpdateValue();
}
