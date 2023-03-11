#include "stdafx.h"
#include "SpecialEffects.h"
#include "SpecialEffectsManager.h"

SpecialEffects::SpecialEffects(const String& name)
: _node(0)
, _visible(true)
, _position(Vector3::ZERO)
, _scale(Vector3::UNIT_SCALE)
, _orientation(Quaternion::IDENTITY)
, _autoLifeTimeManage(true)
{
	_name = name;
	BuildPropertyMap();
}
//--------------------------------------------------------------------------------
SpecialEffects::~SpecialEffects()
{
	Cancel();

	EffectElementList::iterator i = _elements.begin();
	for (; i != _elements.end(); ++i)
	{
		IEffectsElement* elem = *i;
		SpecialEffectsManager::getSingleton().DestroyEffectsElement(elem);
	}
}
//--------------------------------------------------------------------------------
void SpecialEffects::SetName(const String& name)
{
	OnDestroyNode();
	_name = name;
	OnCreateNode();
}
//--------------------------------------------------------------------------------
const String& SpecialEffects::GetTypeName() const
{
	static const String C_TypeName = "SpecialEffects";
	return C_TypeName;
}
//--------------------------------------------------------------------------------
void SpecialEffects::BuildPropertyMap()
{
	PropertyObject::BuildPropertyMap();

	AddPropEx1(Property_Vector3, Position, SpecialEffects, const Vector3&, "Position", "SpecialEffects");
	AddPropEx1(Property_Vector3, Scale, SpecialEffects, const Vector3&, "Scale", "SpecialEffects");
	AddPropEx1(Property_Quaternion, Orientation, SpecialEffects, const Quaternion&, "Orientation", "SpecialEffects");
	AddProp(Property_Bool, Visible, SpecialEffects, "Visible", "SpecialEffects");

	RemoveProperty("TriggerMask");
}
//--------------------------------------------------------------------------------
void SpecialEffects::Update(float deltaTime)
{
	EffectElementList::iterator i = _elements.begin();
	for (; i != _elements.end(); ++i)
	{
		IEffectsElement* e = *i;
		if(!e->IsEnd())
		{
			e->Update(deltaTime);
		}
	}
}
//--------------------------------------------------------------------------------
ISpecialEffects* SpecialEffects::Clone(const String& newName)
{
	ISpecialEffects* effe = OGRE_NEW SpecialEffects(newName);
	effe->SetName(newName);
	// 		effe->SetPosition(_position);
	effe->SetScale(_scale);
	// 		effe->SetOrientation(_orientation);
	effe->SetVisible(_visible);

	// clone elements
	EffectElementList::iterator i = _elements.begin();
	for (; i != _elements.end(); ++i)
	{
		IEffectsElement* elem = *i;
		String elemType = elem->GetTypeName();
		String newElemName = elem->GetName() + "/" + newName;
		IEffectsElement* newElem = SpecialEffectsManager::getSingleton().CreateEffectsElement(
			elemType, newElemName, effe);

		newElem->SetName(newElemName);

		// clone element properties
		const PropertyMap& propMap = elem->GetProperties();
		for (PropertyMap::const_iterator it = propMap.begin(); it != propMap.end(); ++it)
		{
			if(it->first != "Name")
			{
				newElem->SetProperty(it->first, it->second->ToString());
			}
		}

		effe->AddElement(newElem);

	}

	return effe;
}
//--------------------------------------------------------------------------------
size_t SpecialEffects::GetElementsCount() const
{
	return _elements.size();
}
//--------------------------------------------------------------------------------
IEffectsElement* SpecialEffects::GetElementsByIndex(size_t idx) const
{
	assert(idx < _elements.size());
	return _elements[idx];
}
//--------------------------------------------------------------------------------
IEffectsElement* SpecialEffects::GetElementsByName(const String& name) const
{
	EffectElementList::const_iterator i = _elements.begin();
	for (; i != _elements.end(); ++i)
	{
		IEffectsElement* elem = *i;
		if(elem->GetName() == name)
		{
			return elem;
		}
	}

	assert(false);
	return 0;
}
//--------------------------------------------------------------------------------
EffectElementList SpecialEffects::GetElementsByType(const String& name) const
{
	EffectElementList typedElementList;
	typedElementList.clear();
	EffectElementList::const_iterator i = _elements.begin();
	for (; i != _elements.end(); ++i)
	{
		IEffectsElement* elem = *i;
		if(elem->GetTypeName() == name)
		{
			typedElementList.push_back(elem);
		}
	}
	return typedElementList;
}
//--------------------------------------------------------------------------------
int SpecialEffects::AddElement(IEffectsElement* element)
{
	element->Reset();
	_elements.push_back(element);
	return (int)_elements.size();
}
//--------------------------------------------------------------------------------
void SpecialEffects::RemoveElement(const String& name)
{
	EffectElementList::iterator i = _elements.begin();
	for (; i != _elements.end(); ++i)
	{
		IEffectsElement* elem = *i;
		if(elem->GetName() == name)
		{
			SpecialEffectsManager::getSingleton().DestroyEffectsElement(elem);
			_elements.erase(i);
			return;
		}
	}
}
//--------------------------------------------------------------------------------
void SpecialEffects::RemoveElement( size_t idx )
{
	if(idx < _elements.size())
	{
		IEffectsElement* elem = _elements[idx];
		SpecialEffectsManager::getSingleton().
			DestroyEffectsElement(elem);

		_elements.erase(_elements.begin() + idx);
	}
}
//--------------------------------------------------------------------------------
void SpecialEffects::Cancel()
{
	EffectElementList::iterator i = _elements.begin();
	for (; i != _elements.end(); ++i)
	{
		IEffectsElement* elem = *i;
		elem->Cancel();
	}
}
//--------------------------------------------------------------------------------
void SpecialEffects::Reset()
{
	EffectElementList::iterator i = _elements.begin();
	for (; i != _elements.end(); ++i)
	{
		IEffectsElement* elem = *i;
		elem->Reset();
	}
}
//--------------------------------------------------------------------------------
void SpecialEffects::PlaySkeletonAnimation(const String& aniName, bool loopAnimation)
{

}
//--------------------------------------------------------------------------------
void SpecialEffects::StopSkeletonAnimation(const String& aniName)
{

}
//--------------------------------------------------------------------------------
void SpecialEffects::AttachEffectObject(MovableObject* obj, const String& attatchPointName,
										const Vector3& offsetPos, const Quaternion& offsetOrientation)
{
	AttachMap::iterator i = _attachMap.find(obj);
	if(i != _attachMap.end())
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
			"Object already exist [" + obj->getName() + "]", 
			"SpecialEffects::AttachEffectObject");
	}

	String nodeName = GetName() + "/" + attatchPointName + "/" + obj->getName();
	SceneNode* rootNode = _node->getCreator()->getRootSceneNode();
	SceneNode* node = rootNode->createChildSceneNode(nodeName, offsetPos, offsetOrientation);
	node->attachObject(obj);
	node->setOrientation(GetOrientation());
	node->setPosition(GetPosition());
	_attachMap.insert(std::make_pair(obj, node));
}
//--------------------------------------------------------------------------------
void SpecialEffects::DetachEffectObject(MovableObject* object)
{
	SceneNode* node = object->getParentSceneNode();
	if(node)
	{
		node->detachObject(object);

		AttachMap::iterator i = _attachMap.find(object);
		if(i != _attachMap.end())
		{
			assert(node == i->second);
			_attachMap.erase(i);
			SceneNode* rootNode = _node->getCreator()->getRootSceneNode();
			rootNode->removeAndDestroyChild(node->getName());
		}
		else
		{
			//assert(false && "[SpecialEffects::DetachEffectObject] object not attached!");
		}
	}
}
//--------------------------------------------------------------------------------
void SpecialEffects::OnCreateNode()
{
	if(!_name.empty())
	{
		SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
		_node = sm->getRootSceneNode()->createChildSceneNode(GetName());
		_node->setPosition(_position);
		_node->setScale(_scale);
		_node->setOrientation(_orientation);
		_node->setVisible(_visible);
	}
}
//--------------------------------------------------------------------------------
void SpecialEffects::OnDestroyNode()
{
	if(_node)
	{
		Reset();
		SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
		sm->destroySceneNode(_node);
		_node = 0;
	}
}
//--------------------------------------------------------------------------------
const Vector3& SpecialEffects::GetPosition() const
{
	assert(_node != 0);
	return _node->getPosition();
}
//--------------------------------------------------------------------------------
void SpecialEffects::SetPosition(const Vector3& pos)
{
	_position = pos;
	assert(_node != 0);
	_node->setPosition(pos);
}
//--------------------------------------------------------------------------------
const Vector3& SpecialEffects::GetScale() const
{
	assert(_node != 0);
	return _node->getScale();
}
//--------------------------------------------------------------------------------
void SpecialEffects::SetScale(const Vector3& scale)
{
	_scale = scale;
	assert(_node != 0);
	_node->setScale(scale);
}
//--------------------------------------------------------------------------------
const Quaternion& SpecialEffects::GetOrientation() const
{
	assert(_node != 0);
	return _node->getOrientation();
}
//--------------------------------------------------------------------------------
const Quaternion& SpecialEffects::GetTargetWorldOrientation() const
{
	assert(_node != 0);
	return _node->_getDerivedOrientation();
}
//--------------------------------------------------------------------------------
void SpecialEffects::SetOrientation(const Quaternion& orient)
{
	_orientation = orient;
	assert(_node != 0);
	_node->setOrientation(orient);
}
//--------------------------------------------------------------------------------
void SpecialEffects::SetOrientation(float yawDegree, float pitchDegree, float rollDegree)
{
	Matrix3 mat;
	// yaw	pitch	roll
	// y	x		z
	// up	right	dir
	mat.FromEulerAnglesYXZ(Radian(Degree(yawDegree)),
		Radian(Degree(pitchDegree)),
		Radian(Degree(rollDegree)));

	SetOrientation(Quaternion(mat));
}
//--------------------------------------------------------------------------------
void SpecialEffects::SetVisible(bool on)
{
	_visible = on;
	_node->setVisible(_visible);
}
//--------------------------------------------------------------------------------
bool SpecialEffects::GetVisible() const
{
	return _visible;
}
//--------------------------------------------------------------------------------
const Vector3& SpecialEffects::GetTargetWorldPosition() const
{
	assert(_node != 0);
	return _node->_getDerivedPosition();
}
//--------------------------------------------------------------------------------
void SpecialEffects::SetDirection(const Vector3& dir)
{
	assert(_node != 0);
	_node->setDirection(dir);
}
//--------------------------------------------------------------------------------
bool SpecialEffects::IsEnd() const
{
	if(_autoLifeTimeManage)
	{
		return IsAllElementEnd();
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------------------
bool SpecialEffects::IsAllElementEnd() const
{
	EffectElementList::const_iterator i = _elements.begin();
	for (; i != _elements.end(); ++i)
	{
		IEffectsElement* elem = *i;
		if(!elem->IsEnd())
		{
			return false;
		}
	}
	return true;
}
//--------------------------------------------------------------------------------
String SpecialEffects::GetEffectMeshFile() const
{
	return "";
}
//--------------------------------------------------------------------------------
void SpecialEffects::ShowWeapons(bool /*show*/)
{

}
//--------------------------------------------------------------------------------
void SpecialEffects::HittedProcess()
{

}
//--------------------------------------------------------------------------------
Node* SpecialEffects::GetEffectNode(const String& nodeName)
{
	return _node;
}
//--------------------------------------------------------------------------------
float SpecialEffects::GetMinStartTime() const
{
	float t = std::numeric_limits<float>::max();
	EffectElementList::const_iterator i = _elements.begin();
	for (; i != _elements.end(); ++i)
	{
		IEffectsElement* elem = *i;
		t = std::min(elem->GetStartTime(),  t);
	}

	return t;
}
//--------------------------------------------------------------------------------
float SpecialEffects::GetMaxLifeTime() const
{
	float t = 0.0f;
	EffectElementList::const_iterator i = _elements.begin();
	for (; i != _elements.end(); ++i)
	{
		IEffectsElement* elem = *i;
		t = std::max(elem->GetLifeTime(),  t);
	}

	return t;
}
//--------------------------------------------------------------------------------
void SpecialEffects::OnEffectsDestroy(ISpecialEffects* effects)
{

}
//--------------------------------------------------------------------------------
