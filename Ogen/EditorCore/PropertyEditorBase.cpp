// PropertyEditorBase.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyEditorBase.h"
#include "PropertyGrid.h"
#include "EditorApp.h"

class PropertyEditorBaseFactory : public TTypeUIEditorFactor<PropertyEditorBase>
{
};

class PropertyEditor_StringFactory : public TTypeUIEditorFactor<PropertyEditor_String>
{
};


M_REG_TYPEUIEDITOR_FACTORY(_Base_, PropertyEditorBaseFactory);
M_REG_TYPEUIEDITOR_FACTORY(String, PropertyEditor_StringFactory);

// PropertyEditorBase

PropertyEditorBase::PropertyEditorBase(IPropertyPtr prop)
: CMFCPropertyGridProperty(_UIDisplayString(prop->GetDisplayName()).c_str(),
						   prop->ToString().c_str(),
						   _UIDisplayString(prop->GetDescription()).c_str())
, _prop(prop)
, _itemHeight(0)
{
	if(_prop->IsReadOnly())
	{
		//this->Enable(FALSE);
		this->AllowEdit(FALSE);
	}
}

PropertyEditorBase::PropertyEditorBase(const CString& groupName)
: CMFCPropertyGridProperty(groupName)
, _prop(0)
, _itemHeight(0)
{

}

PropertyEditorBase::~PropertyEditorBase()
{
}

CString PropertyEditorBase::FormatProperty()
{
	return _prop->ToString().c_str();
}

void PropertyEditorBase::UpdateDataToProperty()
{
	//_prop->FromString((LPCTSTR)CString(this->GetValue()));
	String str = (LPCTSTR)CString(this->GetValue());
	CommitEdit(str);
	//_prop->FromString(str);

}

void PropertyEditorBase::CommitEdit(const String& newValue)
{
	PropertyGridEx* grid = dynamic_cast<PropertyGridEx*>(this->m_pWndList);
	String propName = _UIKeyString(_prop->GetName());
	grid->OnPropertyChanged(propName, newValue);
}

 void PropertyEditorBase::UpdateDataFromProperty()
 {
 	this->SetValue(_prop->ToString().c_str());
 }

// PropertyEditorBase member functions

 void PropertyEditorBase::RemoveAllSubItems()
 {
	 while (!m_lstSubItems.IsEmpty())
	 {
		CMFCPropertyGridProperty* p = m_lstSubItems.RemoveHead();
		OGRE_DELETE p;	
	 }
 }

 //------------------------------------------------------------------------

 PropertyEditor_String::PropertyEditor_String(IPropertyPtr prop)
 : PropertyEditorBase(prop)
 {
	 Property_StringPtr p = Property_StringPtr::DynamicCast(prop);
	 if(p->HasSourceList())
	 {
		 AllowEdit(FALSE);
		String l = p->GetSourceProperty()->ToString();
		 StringVector v = StringUtil::split(l, "|");
		 for (size_t i = 0; i < v.size(); ++i)
		 {
			 AddOption(v[i].c_str());
		 }
	 }
 }

 PropertyEditor_String::~PropertyEditor_String()
 {

 }
