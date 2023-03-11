// PropertyEditor_MeshFile.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyEditor_MeshFile.h"

class PropertyEditor_MeshFileFactory : public TTypeUIEditorFactor<PropertyEditor_MeshFile>
{
};

M_REG_TYPEUIEDITOR_FACTORY(MeshFile, PropertyEditor_MeshFileFactory);

// PropertyEditor_MeshFile

PropertyEditor_MeshFile::PropertyEditor_MeshFile(IPropertyPtr prop)
: PropertyEditorBase(prop)
{
	AllowEdit(FALSE);
	
	StringVectorPtr files = ResourceGroupManager::getSingleton().listResourceNames(
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	for (size_t i = 0; i < files->size(); ++i)
	{
		String name = (*files)[i];
		if(name.find(".mesh") !=String::npos)
		{
			if(name.substr(name.length() - 5, 5) == ".mesh")
			{
				AddOption(name.c_str());
			}
		}
	}
}

PropertyEditor_MeshFile::~PropertyEditor_MeshFile()
{
}


//------------------------------------------------------------------------
