//  -------------------------------------------------------------
// 	Copyright (c) 2007 XGames
// 	By   :	Oscar.Chen
// 	Date :	2008-08-24
//  Copyright (C) 2008 - All Rights Reserved
//  -------------------------------------------------------------

#include "stdafx.h"
#include "PropertyGrid.h"
#include "Resource.h"
#include "TypeUIEditorFactory.h"
#include "PropertyEditorBase.h"
#include "PropertyEditor_ColourValue.h"
#include "EditorApp.h"
#include "Cmd_SetProperty.h"

String PropertyEvents::PropertyChanged = "PropertyChanged";


IMPLEMENT_DYNAMIC(PropertyGridEx, CMFCPropertyGridCtrl)

IMPLEMENT_DYNAMIC(PropertyGrid, CDockablePane)

PropertyGrid::PropertyGrid()
: _object(0)
{

}

PropertyGrid::~PropertyGrid()
{
}


BEGIN_MESSAGE_MAP(PropertyGrid, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()	


void PropertyGrid::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	_label.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = _wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	_label.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 15, SWP_NOACTIVATE | SWP_NOZORDER);
	_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	_propGrid.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int PropertyGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE ;

	if (!_label.Create("ObjectNameLabel", dwViewStyle, rectDummy, this, 1))
	{
//		// TRACE0("未能创建显示物体名字标签 \n");
		return -1;      // 未能创建
	}
	// 		if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	// 		{
	// 			// TRACE0("Failed to create Properties Combo \n");
	// 			return -1;      // fail to create
	// 		}

	// 		m_wndObjectCombo.AddString(("Application"));
	// 		m_wndObjectCombo.AddString(("Properties Window"));
	// 		m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));
	// 		m_wndObjectCombo.SetCurSel(0);

	if (!_propGrid.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
//		// TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	_wndToolBar.CleanUpLockedImages();
	_wndToolBar.LoadBitmap(IDB_PROPERTIES_HC, 0, 0, TRUE /* Locked */);

	_wndToolBar.SetPaneStyle(_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	_wndToolBar.SetPaneStyle(_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	EnableDocking(CBRS_ALIGN_ANY);

	AdjustLayout();
	return 0;
}

void PropertyGrid::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void PropertyGrid::OnExpandAllProperties()
{
	_propGrid.ExpandAll();
}

void PropertyGrid::OnUpdateExpandAllProperties(CCmdUI* pCmdUI)
{
}

void PropertyGrid::OnSortProperties()
{
	_propGrid.SetAlphabeticMode(!_propGrid.IsAlphabeticMode());
}

void PropertyGrid::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(_propGrid.IsAlphabeticMode());
}

void PropertyGrid::OnProperties1()
{
	// TODO: Add your command handler code here
}

void PropertyGrid::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void PropertyGrid::OnProperties2()
{
	// TODO: Add your command handler code here
}

void PropertyGrid::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void PropertyGrid::InitPropList()
{
	SetPropListFont();

	_propGrid.EnableHeaderCtrl(FALSE);
	_propGrid.EnableDescriptionArea();
	_propGrid.SetVSDotNetLook();
	_propGrid.MarkModifiedProperties();
}

void PropertyGrid::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	_propGrid.SetFocus();
}

void PropertyGrid::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void PropertyGrid::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	_propGrid.SetFont(&m_fntPropList);
	_label.SetFont(&m_fntPropList);
}

void PropertyGrid::SetObject(IPropertyObject* obj)
{
	if(_object)
	{
		try
		{
			VisualObject* vo = dynamic_cast<VisualObject*>(_object);
			if(vo && vo->GetNode())
			{
				vo->GetNode()->showBoundingBox(false);
			}
		}
		catch(const std::__non_rtti_object&)
		{

		}
	}

	_label.SetWindowText("");
	_object = obj;
	_propGrid.SetObject(obj);
	if(_object)
	{
		VisualObject* vo = dynamic_cast<VisualObject*>(_object);
		if(vo && vo->GetNode())
		{
			vo->GetNode()->showBoundingBox(true);
			_propGrid.SetWindowText(vo->GetTypeName().c_str());
		}

		_label.SetWindowText(_object->GetDescription().c_str());
	}
}

//------------------------------------------------------------------------------
// PropertyGridEx
//------------------------------------------------------------------------------

PropertyGridEx::PropertyGridEx()
: _object(0)
, _changePropUndo(true)
{

}

PropertyGridEx::~PropertyGridEx()
{

}

void PropertyGridEx::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	PropertyEditorBase* prop = dynamic_cast<PropertyEditorBase*>(pProp);
	assert(prop);
	prop->UpdateDataToProperty();
	Root::getSingleton().renderOneFrame();
// 	IPropertyPtr propPtr = prop->GetProperty();
// 	String propName = propPtr->GetName();
// 	String val = (LPCTSTR)CString(prop->GetValue());
// 	CommandHistory& cmdH = EditorGame::getSingleton().GetCommandHistory();
// 	cmdH.AddCommand(OGRE_NEW Cmd_SetProperty(_object, propName, val));
}

void PropertyGridEx::OnPropertyChanged( const String& propName, const String& newValue )
{
	if(_changePropUndo)
	{
		CommandHistory& cmdH = EditorApp::getSingleton().GetCommandHistory();
		cmdH.AddCommand(OGRE_NEW Cmd_SetProperty(GetObject(), propName, newValue));
	}
	else
	{
		GetObject()->SetProperty(propName, newValue);
	}
}

void PropertyGridEx::UpdateColor(COLORREF color)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pSel);

	PropertyEditor_ColourValue* pColorProp = dynamic_cast<PropertyEditor_ColourValue*>(m_pSel);
	if (pColorProp == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	BOOL bChanged = color != pColorProp->GetColor();
	pColorProp->SetColor(color);

	if (bChanged)
	{
		OnPropertyChanged(pColorProp);
	}

	pColorProp->OnUpdateValue();
}

void PropertyGridEx::CloseColorPopup()
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pSel);

	PropertyEditor_ColourValue* pColorProp = dynamic_cast<PropertyEditor_ColourValue*>(m_pSel);
	if (pColorProp == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	pColorProp->m_pPopup = NULL;

	pColorProp->m_bButtonIsDown = FALSE;
	pColorProp->Redraw();

	if (pColorProp->m_pWndInPlace != NULL)
	{
		pColorProp->m_pWndInPlace->SetFocus();
	}
}

bool sortPropertyByCategory(IPropertyPtr lhs, IPropertyPtr rhs)
{
	return (lhs)->GetPropertyCategory() < (rhs)->GetPropertyCategory();
}

void PropertyGridEx::SetObject(IPropertyObject* obj)
{
	RemoveAllItem();

	_object = obj;

	if(_object)
	{
		const PropertyMap& propMap = obj->GetProperties();
		typedef vector<IPropertyPtr>::type PropertyList;
		PropertyList propList;
		for(PropertyMap::const_iterator i = propMap.begin(); i != propMap.end(); ++i)
		{
			if(i->second->IsVisibleToEditor())
			{
				propList.push_back(i->second);
			}
		}

		std::sort(propList.begin(), propList.end(), sortPropertyByCategory);

#if 1
		String lastCategory;
		//CMFCPropertyGridProperty* categoryGroup = 0;
		PropertyEditorBase* categoryGroup = 0;
		for(PropertyList::iterator it = propList.begin(); it != propList.end(); ++it)
		{
			IPropertyPtr prop = *it;
			
			String cate = prop->GetPropertyCategory();
			if((cate != lastCategory) || (categoryGroup == 0))
			{
// 				if(categoryGroup) // expand last group
// 				{
// 					categoryGroup->Expand(TRUE);
// 				}

				//categoryGroup = OGRE_NEW CMFCPropertyGridProperty(cate.c_str());
				categoryGroup = new PropertyEditorBase(cate.c_str());

				AddProperty(categoryGroup, FALSE, FALSE);
				lastCategory = cate;
			}

			PropertyEditorBase* propUI =
				TypeUIEditorFactoryManager::getSingleton().createTypeUIEditor(prop);
			//AddProperty(propUI);
			categoryGroup->AddSubItem(propUI);
			propUI->InitSubItems();
			propUI->OnInitEditUI();			
			_propEditorList.push_back(propUI);
		}

// 		if(categoryGroup) // expand final group
// 		{
// 			categoryGroup->Expand(TRUE);
// 		}

		this->ExpandAll(TRUE);
#else
		for(PropertyMap::const_iterator i = propMap.begin(); i != propMap.end(); ++i)
		{
			const IPropertyPtr prop = i->second;
			if(prop->IsVisibleToEditor())
			{
				PropertyEditorBase* propUI =
					TypeUIEditorFactoryManager::getSingleton().createTypeUIEditor(prop);
				AddProperty(propUI);
				propUI->OnInitEditUI();
				_propEditorList.push_back(propUI);
			}
		}
#endif
	}
}

void PropertyGridEx::RemoveAllItem()
{
	RemoveAll();
	_propEditorList.clear();
	Invalidate(TRUE);
	UpdateWindow();
}

void PropertyGridEx::UpdateProperyEditor(const String& propName)
{
	for (size_t i = 0; i < _propEditorList.size(); ++i)
	{
		CMFCPropertyGridProperty* propUI = _propEditorList[i];
		PropertyEditorBase* prop = dynamic_cast<PropertyEditorBase*>(propUI);
		if(prop->GetProperty()->GetName() == propName)
		{
			prop->UpdateDataFromProperty();
			prop->Redraw();
			return;
		}
	}
}

void PropertyGridEx::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CClientDC dc(this);
	HFONT hfontOld = SetCurrFont(&dc);

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	m_nRowHeight = tm.tmHeight + 4;

	m_nHeaderHeight = 0;

	CRect rectClient;
	GetClientRect(rectClient);

	if (m_bHeaderCtrl)
	{
		m_nHeaderHeight = m_nRowHeight + 4;

		GetHeaderCtrl().SendMessage(WM_SETFONT, (WPARAM)(m_hFont != NULL ? m_hFont : ::GetStockObject(DEFAULT_GUI_FONT)));
		GetHeaderCtrl().SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nHeaderHeight, SWP_NOZORDER | SWP_NOACTIVATE);

		HDITEM hdItem;
		hdItem.mask = HDI_WIDTH ;
		hdItem.cxy = m_nLeftColumnWidth + 2;

		GetHeaderCtrl().SetItem(0, &hdItem);

		hdItem.cxy = rectClient.Width() + 10;
		GetHeaderCtrl().SetItem(1, &hdItem);

		GetHeaderCtrl().ShowWindow(SW_SHOWNOACTIVATE);
	}
	else
	{
		GetHeaderCtrl().ShowWindow(SW_HIDE);
	}

	::SelectObject(dc.GetSafeHdc(), hfontOld);

	m_rectList = rectClient;
	m_rectList.top += m_nHeaderHeight;

	if (m_bDescriptionArea && m_nDescrHeight != -1 && rectClient.Height() > 0)
	{
		m_nDescrHeight = std::max(m_nDescrHeight, m_nRowHeight);
		m_nDescrHeight = std::min(m_nDescrHeight, rectClient.Height() - m_nRowHeight);
		m_rectList.bottom -= m_nDescrHeight;
	}

	int cxScroll = ::GetSystemMetrics(SM_CXHSCROLL);
	SetScrollSizes();

	if (m_nVertScrollTotal > 0)
	{
		m_rectList.right -= cxScroll;
		m_wndScrollVert.SetWindowPos(NULL, m_rectList.right, m_rectList.top, cxScroll, m_rectList.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else
	{
		m_wndScrollVert.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	ReposPropertiesEx();

	if (m_pSel != NULL && ((PropertyEditorBase*)m_pSel)->HasButton())
	{
		ASSERT_VALID(m_pSel);
		m_pSel->AdjustButtonRect();
	}

	RedrawWindow();
}

void PropertyGridEx::ReposPropertiesEx()
{
	ASSERT_VALID(this);
	m_lstTerminalProps.RemoveAll();

	int oldRowHeight = m_nRowHeight;

	if (m_ToolTip.GetSafeHwnd() != NULL)
	{
		while (m_nTooltipsCount > 0)
		{
			m_ToolTip.DelTool(this, m_nTooltipsCount);
			m_nTooltipsCount--;
		}
	}

	int y = m_rectList.top - m_nRowHeight * m_nVertScrollOffset - 1;

	if (!m_bAlphabeticMode)
	{
		for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
		{
			CMFCPropertyGridProperty* pProp = m_lstProps.GetNext(pos);
			ASSERT_VALID(pProp);

			PropertyEditorBase* baseProp = dynamic_cast<PropertyEditorBase*>(pProp);
			if(baseProp)
			{
				int tmpHeight = oldRowHeight;

				for (POSITION pos = baseProp->m_lstSubItems.GetHeadPosition(); pos != NULL;)
				{
					CMFCPropertyGridProperty* pProp2 = baseProp->m_lstSubItems.GetNext(pos);
					PropertyEditorBase* baseProp2 = dynamic_cast<PropertyEditorBase*>(pProp2);
					if(baseProp2)
					{
						tmpHeight = (baseProp2->GetItemHeight() > tmpHeight ? baseProp2->GetItemHeight() : tmpHeight);
					}
				}

				m_nRowHeight = tmpHeight;

				baseProp->Reposition(y);
			}
		}

		m_nRowHeight = oldRowHeight;

		return;
	}

	POSITION pos = NULL;

	// Get sorted list of terminal properties:
	for (pos = m_lstProps.GetHeadPosition(); pos != NULL;)
	{
		CMFCPropertyGridProperty* pProp = m_lstProps.GetNext(pos);
		PropertyEditorBase* baseProp = dynamic_cast<PropertyEditorBase*>(pProp);

		if(baseProp)
		{
			baseProp->AddTerminalProp(m_lstTerminalProps);
		}
	}

	for (pos = m_lstTerminalProps.GetHeadPosition(); pos != NULL;)
	{
		CMFCPropertyGridProperty* pProp = m_lstTerminalProps.GetNext(pos);
		PropertyEditorBase* baseProp = dynamic_cast<PropertyEditorBase*>(pProp);

		if(baseProp)
		{
			m_nRowHeight = (baseProp->GetItemHeight() > 0 ? baseProp->GetItemHeight() : oldRowHeight);
			baseProp->Reposition(y);
		}
	}

	m_nRowHeight = oldRowHeight;
}

//------------------------------------------------------------------------
// ObjectPropertiesWnd
//------------------------------------------------------------------------
template<> ObjectPropertiesWnd* Singleton<ObjectPropertiesWnd>::ms_Singleton = 0;

ObjectPropertiesWnd::ObjectPropertiesWnd()
{
}
//--------------------------------------------------------------------------------
ObjectPropertiesWnd::~ObjectPropertiesWnd()
{
	EventsManager::getSingleton().RemoveListener(this);
}
//--------------------------------------------------------------------------------
void ObjectPropertiesWnd::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(eventName == PropertyEvents::PropertyChanged)
	{
		String propName = any_cast<String>(eventContex);
		UpdateProperyEditor(propName);
		stopBroadCast = true;
	}
	if(eventName == "ShowObjectProp")
	{
		IPropertyObject* obj = any_cast<IPropertyObject*>(eventContex);
		this->SetObject(obj);
	}
}
