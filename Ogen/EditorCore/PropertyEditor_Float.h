#pragma once
#include "PropertyEditorBase.h"

class _EditorCoreExport ISliderValueListener
{
public:

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) = 0;
};

//--------------------------------------------------------------------------------

class PropertyEditor_Float
	: public PropertyEditorBase
	, public ISliderValueListener
{
public:

	PropertyEditor_Float(IPropertyPtr prop);
	virtual ~PropertyEditor_Float();

	virtual BOOL OnEdit(LPPOINT lptClick);
	virtual BOOL OnUpdateValue();

protected:

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

class _EditorCoreExport SliderCtrlEx : public CSliderCtrl
{
	DECLARE_DYNAMIC(SliderCtrlEx)

public:
	SliderCtrlEx();
	virtual ~SliderCtrlEx();

	ISliderValueListener* getListener() const { return _listener; }
	void setListener(ISliderValueListener* l) { _listener = l; }
	
protected:
	DECLARE_MESSAGE_MAP()
	ISliderValueListener* _listener;

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

