#pragma once
#include "PropertyEditorBase.h"

class ColorPopupMenu : public CMFCColorPopupMenu
{
public:

	friend class PropertyEditor_ColourValue;

	ColorPopupMenu(const CArray<COLORREF, COLORREF>& colors, COLORREF color, LPCTSTR lpszAutoColor, LPCTSTR lpszOtherColor, LPCTSTR lpszDocColors, CList<COLORREF,COLORREF>& lstDocColors,
		int nColumns, int nHorzDockRows, int nVertDockColumns, COLORREF colorAutomatic, UINT uiCommandID, BOOL bStdColorDlg = FALSE)
		: CMFCColorPopupMenu(colors, color, lpszAutoColor, lpszOtherColor, lpszDocColors, lstDocColors,
		nColumns, nHorzDockRows, nVertDockColumns, colorAutomatic, uiCommandID, bStdColorDlg)
	{
	}

	ColorPopupMenu(CMFCColorButton* pParentBtn, const CArray<COLORREF, COLORREF>& colors, COLORREF color, LPCTSTR lpszAutoColor, LPCTSTR lpszOtherColor,
		LPCTSTR lpszDocColors, CList<COLORREF,COLORREF>& lstDocColors, int nColumns, COLORREF colorAutomatic)
		: CMFCColorPopupMenu(pParentBtn, colors, color, lpszAutoColor, lpszOtherColor,
		lpszDocColors, lstDocColors, nColumns, colorAutomatic)
	{
	}

	ColorPopupMenu(CMFCRibbonColorButton* pParentBtn, const CArray<COLORREF, COLORREF>& colors, COLORREF color, LPCTSTR lpszAutoColor, LPCTSTR lpszOtherColor,
		LPCTSTR lpszDocColors, CList<COLORREF,COLORREF>& lstDocColors, int nColumns, COLORREF colorAutomatic, UINT nID)
		: CMFCColorPopupMenu(pParentBtn, colors, color, lpszAutoColor, lpszOtherColor,
		lpszDocColors, lstDocColors, nColumns, colorAutomatic, nID)
	{
	}

	virtual ~ColorPopupMenu()
	{

	}
};

class CMFCColorBarEx : public CMFCColorBar
{
public:

	friend class PropertyEditor_ColourValue;
};

class PropertyEditor_ColourValue : public PropertyEditorBase
{
	friend class CMFCPropertyGridCtrl;
	friend class PropertyGridEx;

	DECLARE_DYNAMIC(PropertyEditor_ColourValue)

	// Construction
public:
	PropertyEditor_ColourValue(IPropertyPtr prop);
	virtual ~PropertyEditor_ColourValue();

	// Overrides
public:
	virtual void OnDrawValue(CDC* pDC, CRect rect);
	virtual void OnClickButton(CPoint point);
	virtual BOOL OnEdit(LPPOINT lptClick);
	virtual BOOL OnUpdateValue();
	//virtual CString FormatProperty();
	virtual void UpdateDataToProperty();

protected:
	virtual BOOL OnKillFocus(CWnd* pNewWnd) { return pNewWnd->GetSafeHwnd() != m_pPopup->GetSafeHwnd(); }
	virtual BOOL OnEditKillFocus() { return m_pPopup == NULL; }
	virtual BOOL IsValueChanged() const { return m_Color != m_ColorOrig; }

	virtual void AdjustInPlaceEditRect(CRect& rectEdit, CRect& rectSpin);
	virtual void ResetOriginalValue();

	// Attributes
public:
	COLORREF GetColor() const { return m_Color; }
	void SetColor(COLORREF color);

	// Color popup attributes:
	void EnableAutomaticButton(LPCTSTR lpszLabel, COLORREF colorAutomatic, BOOL bEnable = TRUE);
	void EnableOtherButton(LPCTSTR lpszLabel, BOOL bAltColorDlg = TRUE, BOOL bEnable = TRUE);
	void SetColumnsNumber(int nColumnsNumber);

	// Attributes
protected:
	COLORREF     m_Color;          // Color value
	COLORREF     m_ColorOrig;      // Original color value
	COLORREF     m_ColorAutomatic; // Automatic (default) color value
	CString      m_strAutoColor;   // Atomatic color label
	CString      m_strOtherColor;  // Alternative color label
	ColorPopupMenu* m_pPopup;

	CArray<COLORREF, COLORREF> m_Colors;

	int  m_nColumnsNumber; // Number of columns in dropped-down colors list
	BOOL m_bStdColorDlg;   // Use standard Windows color dialog
};

