#pragma once
#include "EditorCorePrerequisites.h"
#include "TypeUIEditorFactory.h"

class _EditorCoreExport PropertyEditorBase
	: public CMFCPropertyGridProperty
{
public:

	friend class PropertyGridEx;

	PropertyEditorBase(IPropertyPtr prop);
	PropertyEditorBase(const CString& groupName);
	virtual ~PropertyEditorBase();

	// 重载格式化函数，以显示我们的Property提供的格式化字符串
	virtual CString FormatProperty();

	// 更新数据函数
	virtual void UpdateDataFromProperty();
	virtual void UpdateDataToProperty();

	IPropertyPtr GetProperty() const { return _prop; }

	// 提交编辑改变的属性值，以使能够Undo
	void CommitEdit(const String& newValue);

	// 对于可展开类型的数据可重载此函数进行相应处理
	virtual void OnChildValueChanged(CMFCPropertyGridProperty* child, const String& name, const String& value){}

	void RemoveAllSubItems();

	template<class T>
	T TGetProperty()
	{
		return T::DynamicCast(_prop);
	}

	virtual void OnInitEditUI() { }

	virtual void InitSubItems() {}

	void SetItemHeight(int h = 32) { _itemHeight = h; }
	int GetItemHeight() const { return _itemHeight; }


protected:

	IPropertyPtr	_prop;
	int				_itemHeight;
};

//------------------------------------------------------------------------

class _EditorCoreExport ISubItem
{
public:

	virtual const String& getName() const = 0;
	virtual PropertyEditorBase* GetParent() = 0;
};

template<class T = CMFCPropertyGridProperty>
class PropertyEditorSubItem
	: public T
	, public ISubItem
{
public:

	PropertyEditorSubItem(PropertyEditorBase* parent, const String& name, const String& value, const String& displayName)
		: CMFCPropertyGridProperty(displayName.c_str(), value.c_str())
		, _parent(parent)
		, _name(name)
	{
	}

	virtual ~PropertyEditorSubItem()
	{
	}

	const String& getName() const { return _name; }
	PropertyEditorBase* GetParent() { return _parent; }

protected:

	PropertyEditorBase*	_parent;
	String				_name;

	virtual BOOL OnUpdateValue()
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

		if (strText != CString(GetValue()))
		{
			SetValue(strText);
			_parent->OnChildValueChanged(this, _name, (LPCTSTR)strText);
		}

		return TRUE;
	}
};

typedef PropertyEditorSubItem<CMFCPropertyGridProperty> PropertyEditorSubItem_String;

class PropertyEditor_String : public PropertyEditorBase
{
public:

	PropertyEditor_String(IPropertyPtr prop);
	virtual ~PropertyEditor_String();

};