#include "stdafx.h"
#include "PropertyEditor_TerrainBrush.h"
#include "Property_TerrainBrush.h"
#include "TerrainBrush.h"

IMPLEMENT_DYNAMIC(PropertyEditor_TerrainBrush, PropertyEditorBase)

PropertyEditor_TerrainBrush::PropertyEditor_TerrainBrush(IPropertyPtr prop)
: PropertyEditorBase(prop)
{
	_itemHeight = 64;

	AllowEdit(FALSE);
	TerrainBrushManager& mgr = TerrainBrushManager::getSingleton();
	for (size_t i = 0; i < mgr.GetBrushCount(); ++i)
	{
		AddOption(mgr.GetBrush(i)->GetName().c_str());
	}
}

PropertyEditor_TerrainBrush::~PropertyEditor_TerrainBrush()
{
}

void PropertyEditor_TerrainBrush::OnDrawName(CDC* pDC, CRect rect)
{


}

void PropertyEditor_TerrainBrush::OnDrawValue(CDC* pDC, CRect rect)
{
	CRect rc = rect;

	//rc.left = 0;
	//pDC->SelectClipRgn(NULL);
	//rect.left += rect.Height();
	//CMFCPropertyGridProperty::OnDrawValue(pDC, rect);
	Property_TerrainBrushPtr brush = TGetProperty<Property_TerrainBrushPtr>();
	HBITMAP hbmp = brush->GetValue()->GetThumbImage();

	BITMAP bmpObj;
	::GetObject(hbmp, sizeof(BITMAP), &bmpObj);
	CDC bmpDC;
	bmpDC.CreateCompatibleDC(pDC);
	bmpDC.SelectObject(hbmp);

// 	CSize sz = bmp->GetBitmapDimension();
	pDC->StretchBlt(rc.left, rc.top, _itemHeight, _itemHeight, &bmpDC, 0, 0, bmpObj.bmWidth, bmpObj.bmHeight, SRCCOPY);

	rc.right = rc.left + rc.Height();
	rc.DeflateRect(1, 1);
	rc.top++;
	rc.left++;

	//COLORREF brClr = m_Color == (COLORREF)-1 ? m_ColorAutomatic : m_Color;
// 	COLORREF color = /*m_Color & */0x00ffffff;
// // 	CBrush br(color);
// // 	pDC->FillRect(rc, &br);
//  	pDC->Draw3dRect(rc, 0, 0);
}

void PropertyEditor_TerrainBrush::AdjustInPlaceEditRect(CRect& rectEdit, CRect& rectSpin)
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

void PropertyEditor_TerrainBrush::ResetOriginalValue()
{
	CMFCPropertyGridProperty::ResetOriginalValue();
}
