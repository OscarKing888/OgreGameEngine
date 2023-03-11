// PropertyEditor_ColourValue.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyEditor_ColourValue.h"
#include "PropertyGrid.h"

class PropertyEditor_ColourValueFactory : public TTypeUIEditorFactor<PropertyEditor_ColourValue>
{
};

M_REG_TYPEUIEDITOR_FACTORY(ColourValue, PropertyEditor_ColourValueFactory);

// PropertyEditor_ColourValue member functions

IMPLEMENT_DYNAMIC(PropertyEditor_ColourValue, PropertyEditorBase)

PropertyEditor_ColourValue::PropertyEditor_ColourValue(IPropertyPtr prop)
	: PropertyEditorBase(prop)
{
	Property_ColourValue* p = dynamic_cast<Property_ColourValue*>(prop.GetPointer());
	ColourValue clr = p->GetValue();

	COLORREF color = clr.getAsABGR();
	m_Color = color;
	m_ColorOrig = color;

	CMFCColorBarEx::InitColors(0, m_Colors);

	m_varValue = (LONG) m_Color;
	m_varValueOrig = (LONG) m_Color;

	m_dwFlags = 0x0001/*AFX_PROP_HAS_LIST*/;

	m_pPopup = NULL;
	m_bStdColorDlg = FALSE;
	m_ColorAutomatic = RGB(0, 0, 0);
	m_nColumnsNumber = 5;

	EnableOtherButton("ÆäËü...");
	EnableAutomaticButton("Ä¬ÈÏ", ::GetSysColor(COLOR_3DFACE));
}

PropertyEditor_ColourValue::~PropertyEditor_ColourValue()
{
}

void PropertyEditor_ColourValue::UpdateDataToProperty()
{	
	COLORREF clr = m_Color;
	ColourValue clrVal;
	clrVal.setAsABGR(clr);
	String str = StringConverter::toString(clrVal);
	//_prop->FromString(str);
	CommitEdit(str);
}

void PropertyEditor_ColourValue::OnDrawValue(CDC* pDC, CRect rect)
{
	CRect rectColor = rect;

	rect.left += rect.Height();
	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);

	rectColor.right = rectColor.left + rectColor.Height();
	rectColor.DeflateRect(1, 1);
	rectColor.top++;
	rectColor.left++;

	//COLORREF brClr = m_Color == (COLORREF)-1 ? m_ColorAutomatic : m_Color;
	COLORREF color = m_Color & 0x00ffffff;
	CBrush br(color);
	pDC->FillRect(rectColor, &br);
	pDC->Draw3dRect(rectColor, 0, 0);
}

void PropertyEditor_ColourValue::OnClickButton(CPoint /*point*/)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	m_bButtonIsDown = TRUE;
	Redraw();

	CList<COLORREF,COLORREF> lstDocColors;

	m_pPopup = new ColorPopupMenu(NULL, m_Colors, m_Color, NULL, NULL, NULL, lstDocColors, m_nColumnsNumber, m_ColorAutomatic);
	m_pPopup->SetPropList(m_pWndList);

	if (!m_strOtherColor.IsEmpty()) // Other color button
	{
		m_pPopup->m_wndColorBar.EnableOtherButton(m_strOtherColor, !m_bStdColorDlg);
	}

	if (!m_strAutoColor.IsEmpty()) // Automatic color button
	{
		m_pPopup->m_wndColorBar.EnableAutomaticButton(m_strAutoColor, m_ColorAutomatic);
	}

	CPoint pt(m_pWndList->GetListRect().left + m_pWndList->GetPropertyColumnWidth() + 1, m_rectButton.bottom + 1);
	m_pWndList->ClientToScreen(&pt);

	if (!m_pPopup->Create(m_pWndList, pt.x, pt.y, NULL, FALSE))
	{
		ASSERT(FALSE);
		m_pPopup = NULL;
	}
	else
	{
		m_pPopup->GetMenuBar()->SetFocus();
	}
}

BOOL PropertyEditor_ColourValue::OnEdit(LPPOINT /*lptClick*/)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	m_pWndInPlace = NULL;

	CRect rectEdit;
	CRect rectSpin;

	AdjustInPlaceEditRect(rectEdit, rectSpin);

	CMFCMaskedEdit* pWndEdit = new CMFCMaskedEdit;
	DWORD dwStyle = WS_VISIBLE | WS_CHILD;

	if (!m_bEnabled)
	{
		dwStyle |= ES_READONLY;
	}

	pWndEdit->SetValidChars(". 01234567890");

	pWndEdit->Create(dwStyle, rectEdit, m_pWndList, AFX_PROPLIST_ID_INPLACE);
	m_pWndInPlace = pWndEdit;

	CString str = FormatProperty();
	m_pWndInPlace->SetWindowText(str);

	CString strT;
	m_pWndInPlace->GetWindowText(strT);

	m_pWndInPlace->SetFont(IsModified() && m_pWndList->IsMarkModifiedProperties() ? &m_pWndList->GetBoldFont() : m_pWndList->GetFont());
	m_pWndInPlace->SetFocus();

	m_bInPlaceEdit = TRUE;
	return TRUE;
}

void PropertyEditor_ColourValue::AdjustInPlaceEditRect(CRect& rectEdit, CRect& rectSpin)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	rectSpin.SetRectEmpty();

	rectEdit = m_Rect;
	rectEdit.DeflateRect(0, 2);

	PropertyGridEx* gridEx = dynamic_cast<PropertyGridEx*>(m_pWndList);
	int nMargin = m_pWndList->IsMarkModifiedProperties() && m_bIsModified ? gridEx->GetBoldEditLeftMargin() :gridEx->GetEditLeftMargin();

	rectEdit.left = m_pWndList->GetListRect().left + m_pWndList->GetPropertyColumnWidth() + m_Rect.Height() + AFX_TEXT_MARGIN - nMargin + 1;

	AdjustButtonRect();
	rectEdit.right = m_rectButton.left;
}

void PropertyEditor_ColourValue::ResetOriginalValue()
{
	CMFCPropertyGridProperty::ResetOriginalValue();
	m_Color = m_ColorOrig;
}

// CString PropertyEditor_ColourValue::FormatProperty()

// 	ASSERT_VALID(this);
// 
// 	CString str;
// 	//str.Format(("%02x%02x%02x"), GetRValue(m_Color), GetGValue(m_Color), GetBValue(m_Color));
// 
// 	ColourValue clr;
// 	clr.setAsABGR(m_Color);
// 	str = StringConverter::toString(clr).c_str();
// 	return str;
// }

void PropertyEditor_ColourValue::SetColor(COLORREF color)
{
	ASSERT_VALID(this);

	m_Color = color | 0xff000000;
	m_varValue = (LONG) (color | 0xff000000);

	if (::IsWindow(m_pWndList->GetSafeHwnd()))
	{
		CRect rect = m_Rect;
		rect.DeflateRect(0, 1);

		m_pWndList->InvalidateRect(rect);
		m_pWndList->UpdateWindow();
	}

	if (m_pWndInPlace != NULL)
	{
		ASSERT_VALID(m_pWndInPlace);
		m_pWndInPlace->SetWindowText(FormatProperty());
	}
}

void PropertyEditor_ColourValue::SetColumnsNumber(int nColumnsNumber)
{
	ASSERT_VALID(this);
	ASSERT(nColumnsNumber > 0);

	m_nColumnsNumber = nColumnsNumber;
}

void PropertyEditor_ColourValue::EnableAutomaticButton(LPCTSTR lpszLabel, COLORREF colorAutomatic, BOOL bEnable)
{
	ASSERT_VALID(this);

	m_ColorAutomatic = colorAutomatic;
	m_strAutoColor = (!bEnable || lpszLabel == NULL) ? "" : lpszLabel;
}

void PropertyEditor_ColourValue::EnableOtherButton(LPCTSTR lpszLabel, BOOL bAltColorDlg, BOOL bEnable)
{
	ASSERT_VALID(this);

	m_bStdColorDlg = !bAltColorDlg;
	m_strOtherColor = (!bEnable || lpszLabel == NULL) ? "" : lpszLabel;
}

BOOL PropertyEditor_ColourValue::OnUpdateValue()
{
	ASSERT_VALID(this);

	if (m_pWndInPlace == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	CString strText;
	m_pWndInPlace->GetWindowText(strText);

	//COLORREF colorCurr = m_Color;

	//int nR = 0, nG = 0, nB = 0;
	//_stscanf_s(strText, ("%2x%2x%2x"), &nR, &nG, &nB);
	//m_Color = RGB(nR, nG, nB);

	//ColourValue clr = StringConverter::parseColourValue((LPCTSTR)strText);
	//m_Color = clr.getAsABGR();

	//if (colorCurr != m_Color)
	{
		m_pWndList->OnPropertyChanged(this);
	}

	return TRUE;
}