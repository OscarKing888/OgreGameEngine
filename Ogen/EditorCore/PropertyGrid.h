//  -------------------------------------------------------------
// 	Copyright (c) 2007 XGames
// 	By   :	Oscar.Chen
// 	Date :	2008-08-24
//  Copyright (C) 2008 - All Rights Reserved
//  -------------------------------------------------------------

#pragma once
#include "EditorCorePrerequisites.h"

struct _EditorCoreExport PropertyEvents
{
	static String PropertyChanged;
};

class _EditorCoreExport PropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};
//--------------------------------------------------------------------------------
class _EditorCoreExport PropertyGridEx : public CMFCPropertyGridCtrl
{
public:

	DECLARE_DYNAMIC(PropertyGridEx)

	PropertyGridEx();
	virtual ~PropertyGridEx();

	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;

	int GetBoldEditLeftMargin() const { return m_nBoldEditLeftMargin; }
	int GetEditLeftMargin() const { return m_nEditLeftMargin; }

	virtual void UpdateColor(COLORREF color);
	virtual void CloseColorPopup();
	void SetObject(IPropertyObject* obj);
	IPropertyObject* GetObject() const { return _object; }

	void RemoveAllItem();

	void UpdateProperyEditor(const String& propName);

	virtual void OnPropertyChanged(const String& propName, const String& newValue);

	void SetPropertyChangeUndo(bool on) { _changePropUndo = on; }
	bool GetPropertyChangeUndo() const { return _changePropUndo; }

protected:

	IPropertyObject* _object;
	typedef vector<CMFCPropertyGridProperty*>::type EditorList;
	EditorList		_propEditorList;
	bool			_changePropUndo;

	virtual void AdjustLayout();
	void ReposPropertiesEx();
};
//--------------------------------------------------------------------------------
class _EditorCoreExport PropertyGrid : public CDockablePane
{
	DECLARE_DYNAMIC(PropertyGrid)

public:

	PropertyGrid();
	virtual ~PropertyGrid();
	void AdjustLayout();

	void SetObject(IPropertyObject* obj);
	
	IPropertyObject* GetObject() const { return _object; }	
	IGameObject* GetGameObject() const { return dynamic_cast<IGameObject*>(_object); }

	void UpdateProperyEditor(const String& propName){ _propGrid.UpdateProperyEditor(propName); }

	void SetPropertyChangeUndo(bool on) { _propGrid.SetPropertyChangeUndo(on); }
	bool GetPropertyChangeUndo() const { return _propGrid.GetPropertyChangeUndo(); }

protected:

	CFont m_fntPropList;
	CStatic _label;
	PropertiesToolBar _wndToolBar;
	PropertyGridEx	_propGrid;
	IPropertyObject* _object;


	void InitPropList();
	void SetPropListFont();

	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

};
//--------------------------------------------------------------------------------
class _EditorCoreExport ObjectPropertiesWnd
	: public PropertyGrid
	, public Singleton<ObjectPropertiesWnd>
	, public IEventLisitener
{
public:

	ObjectPropertiesWnd();
	virtual ~ObjectPropertiesWnd();

	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast);
};
