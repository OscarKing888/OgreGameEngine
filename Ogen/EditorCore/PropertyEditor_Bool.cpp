// PropertyEditor_Bool.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyEditor_Bool.h"
#include "PropertyGrid.h"

class PropertyEditor_BoolFactory : public TTypeUIEditorFactor<PropertyEditor_Bool>
{
};

M_REG_TYPEUIEDITOR_FACTORY(bool, PropertyEditor_BoolFactory);

typedef TSmartPointer<Property_Bool> Property_BoolPtr;

class CCheckButton : public CButton
{
	DECLARE_DYNAMIC(CCheckButton)
	DECLARE_MESSAGE_MAP();
public:

	CCheckButton()
		: _brush(RGB(255, 255, 255))
	{

	}

protected:

	CBrush _brush;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC)
	{
		return TRUE;
	}


	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
	{
		return _brush;
	}
};

IMPLEMENT_DYNAMIC(CCheckButton, CButton)

BEGIN_MESSAGE_MAP(CCheckButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


#define M_USE_CHECKBUTTON 0

PropertyEditor_Bool::PropertyEditor_Bool(IPropertyPtr prop)
: PropertyEditorBase(prop)
//, _brush(RGB(255, 255, 255))
{
	AllowEdit(FALSE);

#if !M_USE_CHECKBUTTON
	AddOption(StringConverter::toString(true).c_str());
	AddOption(StringConverter::toString(false).c_str());
#endif
}

void PropertyEditor_Bool::OnInitEditUI()
{
#if M_USE_CHECKBUTTON
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	Property_BoolPtr prop = TGetProperty<Property_BoolPtr>();

	m_pWndInPlace = NULL;

	CCheckButton* pWndEdit = OGRE_NEW CCheckButton();
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX;

	if (!m_bEnabled)
	{
		dwStyle |= ES_READONLY;
	}

	CRect rectEdit = m_Rect;
	PropertyGridEx* gridEx = dynamic_cast<PropertyGridEx*>(m_pWndList);
	//int nMargin = m_pWndList->IsMarkModifiedProperties() && m_bIsModified ? gridEx->GetBoldEditLeftMargin() :gridEx->GetEditLeftMargin();

	rectEdit.left = m_pWndList->GetListRect().left + m_pWndList->GetPropertyColumnWidth();// + m_Rect.Height() + AFX_TEXT_MARGIN - nMargin + 1;

	pWndEdit->Create("", dwStyle, rectEdit, m_pWndList, AFX_PROPLIST_ID_INPLACE);
	m_pWndInPlace = pWndEdit;
	m_pWndInPlace->SetFocus();

	pWndEdit->SetCheck(prop->GetValue() ? BST_CHECKED : BST_UNCHECKED);
#else
	PropertyEditorBase::OnInitEditUI();
#endif
}

void PropertyEditor_Bool::OnPosSizeChanged(CRect rectOld)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

#if M_USE_CHECKBUTTON
	if(m_pWndInPlace)
	{
		CRect rectEdit = m_Rect;
		PropertyGridEx* gridEx = dynamic_cast<PropertyGridEx*>(m_pWndList);
		rectEdit.left = m_pWndList->GetListRect().left + m_pWndList->GetPropertyColumnWidth();
		//rectEdit.right = rectEdit.left + 14;
		m_pWndInPlace->MoveWindow(rectEdit, FALSE);
	}
#else
	PropertyEditorBase::OnPosSizeChanged(rectOld);
#endif
}

PropertyEditor_Bool::~PropertyEditor_Bool()
{
}

BOOL PropertyEditor_Bool::OnEdit(LPPOINT lptClick)
{	
#if M_USE_CHECKBUTTON
	m_bInPlaceEdit = TRUE;
	return TRUE;
#else
	return PropertyEditorBase::OnEdit(lptClick);
#endif
}

BOOL PropertyEditor_Bool::OnUpdateValue()
{
	ASSERT_VALID(this);

#if M_USE_CHECKBUTTON
	if (m_pWndInPlace == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	CButton* pWndEdit = static_cast<CButton*>(m_pWndInPlace);
	SetValue(COleVariant(StringConverter::toString(pWndEdit->GetCheck() == BST_CHECKED).c_str()));
	m_pWndList->OnPropertyChanged(this);
#else
	PropertyEditorBase::OnUpdateValue();
#endif


	return TRUE;
}

// HBRUSH PropertyEditor_Bool::OnCtlColor(CDC* pDC, UINT nCtlColor)

// 	if(nCtlColor == CTLCOLOR_BTN)
// 	{
// 		return _brush;
// 	}
// 
// 	return PropertyEditorBase::OnCtlColor(pDC, nCtlColor);
// }

void PropertyEditor_Bool::OnDrawValue(CDC* pDC, CRect rect)
{
#if M_USE_CHECKBUTTON

#else
	PropertyEditorBase::OnDrawValue(pDC, rect);
#endif
}