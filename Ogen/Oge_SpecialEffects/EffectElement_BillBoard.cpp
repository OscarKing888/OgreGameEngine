#include "StdAfx.h"
#include "EffectElement_Billboard.h"
#include "SpecialEffectsManager.h"


EffectElement_Billboard::EffectElement_Billboard(const String& name, ISpecialEffects* parent)
: AbstractEffectElement(name, parent)
, _billboard(0)
, _poolSize(1)
, _color(ColourValue::White)
, _width(1.0f)
, _height(1.0f)
{
}

EffectElement_Billboard::~EffectElement_Billboard(void)
{
}

const String& EffectElement_Billboard::GetTypeName() const
{
	static const String C_TypeName = "Billboard";
	return C_TypeName;
}

void EffectElement_Billboard::SetName(const String& name)
{
	OnDestroyBillboard();
	_name = name;
	OnCreateBillboard();
}

void EffectElement_Billboard::OnStart()
{
	if(_billboard)
	{
		_billboard->setVisible(true);
	}
}

void EffectElement_Billboard::OnEnd()
{
	if(_billboard)
	{
		_billboard->setVisible(false);
	}
}

void EffectElement_Billboard::BuildPropertyMap()
{
	AbstractEffectElement::BuildPropertyMap();
	AddProp(Property_Int, PoolSize, EffectElement_Billboard, "PoolSize", "Billboard");
	AddProp(Property_String, MaterialName, EffectElement_Billboard, "MaterialName", "Billboard");
	AddProp(Property_ColourValue, Colour, EffectElement_Billboard, "Colour", "Billboard");
	AddProp(Property_Float, Width, EffectElement_Billboard, "Width", "Billboard");
	AddProp(Property_Float, Height, EffectElement_Billboard, "Height", "Billboard");
}

void EffectElement_Billboard::OnCreateBillboard()
{
	SceneManager* sm = GetSceneManager();
	if(_name.empty())
	{
		return;
	}

	_billboard = sm->createBillboardSet(_parent->GetName() + "/" + GetName(), _poolSize);
	_billboard->setQueryFlags(0);
	_billboard->setVisible(false);
	_billboard->createBillboard(Vector3::ZERO, _color);
	UpdateMaterial();


	if(!_node)
	{
		String nodeName = _name;
		_node = _parent->GetNode()->createChildSceneNode(nodeName);
	}

	if(_billboard)
	{
		_node->attachObject(_billboard);
	}
}

void EffectElement_Billboard::OnDestroyBillboard()
{
	if(_billboard)
	{	
		SceneManager* sm = GetSceneManager();
		sm->destroyBillboardSet(_billboard);
		_billboard = 0;
	}
}

void EffectElement_Billboard::OnDestroy()
{
	OnDestroyBillboard();
}

Node* EffectElement_Billboard::GetNode() const
{
	if(_billboard)
	{
		return _billboard->getParentNode();
	}

	return 0;
}

void EffectElement_Billboard::SetMaterialName(const String& matName)
{
	_materialName = matName;
	UpdateMaterial();
}

void EffectElement_Billboard::UpdateMaterial()
{
	if(_billboard && (!_materialName.empty()))
	{
		if(MaterialManager::getSingleton().resourceExists(_materialName))
		{
			_billboard->setMaterialName(_materialName);
		}
	}
}

void EffectElement_Billboard::SetColour(const ColourValue& val)
{
	_color = val;
	OnDestroyBillboard();
	OnCreateBillboard();
}

void EffectElement_Billboard::SetWidth(float val)
{
	_width = val;
}

void EffectElement_Billboard::SetHeight(float val)
{
	_height = val;
	int cnt = _billboard->getNumBillboards();
	if(_billboard && (cnt > 0))
	{
		for (int i = 0; i < cnt; ++i)
		{
			_billboard->getBillboard(i)->setDimensions(_width, _height);
		}
	}
}

void EffectElement_Billboard::SetPoolSize(int val)
{
	OnDestroyBillboard();
	OnCreateBillboard();
}
