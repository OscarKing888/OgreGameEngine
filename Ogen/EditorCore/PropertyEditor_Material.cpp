// PropertyEditor_Material.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyEditor_Material.h"

class PropertyEditor_MaterialFactory : public TTypeUIEditorFactor<PropertyEditor_Material>
{
};

M_REG_TYPEUIEDITOR_FACTORY(Material, PropertyEditor_MaterialFactory);

// PropertyEditor_Material

PropertyEditor_Material::PropertyEditor_Material(IPropertyPtr prop)
: PropertyEditorBase(prop)
{
	AllowEdit(FALSE);
	
	MaterialManager& mgr = MaterialManager::getSingleton();
	MaterialManager::ResourceMapIterator i = mgr.getResourceIterator();
	while (i.hasMoreElements())
	{		
		AddOption(i.peekNextValue()->getName().c_str());
		i.moveNext();
	}
}

PropertyEditor_Material::~PropertyEditor_Material()
{
}


//------------------------------------------------------------------------