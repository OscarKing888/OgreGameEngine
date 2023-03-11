#include "StdAfx.h"
#include "EffectElement_RibbonTrail.h"
#include "SpecialEffectsManager.h"


EffectElement_RibbonTrail::EffectElement_RibbonTrail(const String& name, ISpecialEffects* parent)
: AbstractEffectElement(name, parent)
, _trail(0)
, _trailLength(500.0f)
, _numChains(1)
, _maxElements(80)
, _useLocalPosition(false)
, _faceToCamera(false)
, _elementOrientationOffset(Vector3::ZERO)
, _elementPositionOffsetOnWidthExtent(0.0f)
, _elementScaleOffset(Vector3::UNIT_SCALE)
{
}

EffectElement_RibbonTrail::~EffectElement_RibbonTrail(void)
{
}

const String& EffectElement_RibbonTrail::GetTypeName() const
{
	static const String C_TypeName = "RibbonTrail";
	return C_TypeName;
}

void EffectElement_RibbonTrail::SetName(const String& name)
{
	//OnDestroyEntity();
	_name = name;
}

void EffectElement_RibbonTrail::OnStart()
{
	OnCreateEntity();
	if(_trail)
	{
		_trail->setVisible(true);
	}
}

void EffectElement_RibbonTrail::OnEnd()
{
	if(_trail)
	{
		_trail->setVisible(false);
	}

	OnDestroyEntity();
}

void EffectElement_RibbonTrail::BuildPropertyMap()
{
	AbstractEffectElement::BuildPropertyMap();
	AddProp(Property_String, MaterialName, EffectElement_RibbonTrail, "MaterialName", "RibbonTrail");
	AddProp(Property_StringVector, TargetNodeName, EffectElement_RibbonTrail, "TargetNodeName", "RibbonTrail");

	AddProp(Property_Float, TrailLength, EffectElement_RibbonTrail, "TrailLength", "RibbonTrail");
	AddProp(Property_Int, NumberOfChains, EffectElement_RibbonTrail, "NumberOfChains", "RibbonTrail");
	AddProp(Property_Int, MaxChainElements, EffectElement_RibbonTrail, "MaxChainElements", "RibbonTrail");

	AddProp(Property_StringMap, InitialWidths, EffectElement_RibbonTrail, "InitialWidths", "RibbonTrail");
	AddProp(Property_StringMap, WidthChanges, EffectElement_RibbonTrail, "WidthChanges", "RibbonTrail");
	AddProp(Property_StringMap, ColourChanges, EffectElement_RibbonTrail, "ColourChanges", "RibbonTrail");

	AddProp(Property_Bool, FaceToCamera, EffectElement_RibbonTrail, "FaceToCamera", "RibbonTrail");
	AddProp(Property_Bool, UseLocalPosition, EffectElement_RibbonTrail, "UseLocalPosition", "RibbonTrail");

	AddProp(Property_Float, ElementPositionOffsetOnWidthExtent, EffectElement_RibbonTrail, "ElementPositionOffsetOnWidthExtent", "RibbonTrail");
	AddProp(Property_Vector3, ElementScaleOffset, EffectElement_RibbonTrail, "ElementScaleOffset", "RibbonTrail");
	AddProp(Property_Vector3, ElementOrientationOffset, EffectElement_RibbonTrail, "ElementOrientationOffset", "RibbonTrail");
}

void EffectElement_RibbonTrail::OnCreateEntity()
{
	SceneManager* sm = GetSceneManager();
	if(_name.empty())
	{
		return;
	}

	_trail = sm->createRibbonTrail(_name);
	_trail->setFaceToCamera(_faceToCamera);
	_trail->setUseLocalPosition(_useLocalPosition);
	_trail->setMaterialName(_materialName);
	_trail->setTrailLength(_trailLength);
	_trail->setNumberOfChains(_numChains);
	_trail->setMaxChainElements(_maxElements);
	_trail->setElementPositionOffsetOnWidthExtent(_elementPositionOffsetOnWidthExtent);

	_trail->setElementOrientationOffset(MathUtil::MakeQuaternion(
		Degree(_elementOrientationOffset.y),
		Degree(_elementOrientationOffset.x),
		Degree(_elementOrientationOffset.z)));

	_trail->setElementScaleOffset(_elementScaleOffset);

	_parent->GetNode()->getCreator()->getRootSceneNode()->attachObject(_trail);

	WidthMap::iterator i = _initWidthMap.begin();
	for (; i != _initWidthMap.end(); ++i)
	{
		_trail->setInitialWidth(i->first, i->second);
	}

	i = _widthChangeMap.begin();
	for (; i != _widthChangeMap.end(); ++i)
	{
		_trail->setWidthChange(i->first, i->second);
	}

	ColourMap::iterator j = _colourChangeMap.begin();
	for (; j != _colourChangeMap.end(); ++j)
	{
		_trail->setColourChange(j->first, j->second);
	}

	_trail->setQueryFlags(0);
	_trail->setVisible(false);


// 	if(!_node)
// 	{
// 		String nodeName = _name;
// 		_node = _parent->GetNode()->createChildSceneNode(nodeName);
// 	}
// 
// 	if(_trail)
// 	{
// 		_node->attachObject(_trail);
// 	}
}

void EffectElement_RibbonTrail::OnDestroyEntity()
{
	if(_trail)
	{	
		SceneManager* sm = GetSceneManager();
		sm->destroyRibbonTrail(_trail);
		_trail = 0;
	}
}


 void EffectElement_RibbonTrail::SetInitialWidth(size_t chainIndex, float width)
 {
	if(_trail)
	{
		_parent->GetNode()->getCreator()->getRootSceneNode()->detachObject(_trail);
		_trail->setInitialWidth(chainIndex, width);
	}

	_initWidthMap[chainIndex] = width;
 }

 float EffectElement_RibbonTrail::GetInitialWidth(size_t chainIndex) const
 {
	 WidthMap::const_iterator it = _initWidthMap.find(chainIndex);
	 if(it != _initWidthMap.end())
	 {
		 return it->second;
	 }

	 if(_trail)
	 {
		 return _trail->getInitialWidth(chainIndex);
	 }


	 return 0.0f;
 }
 
 void EffectElement_RibbonTrail::SetWidthChange(size_t chainIndex, float widthDeltaPerSecond)
 {
	 if(_trail)
	 {
		 _trail->setWidthChange(chainIndex, widthDeltaPerSecond);
	 }

	 _widthChangeMap[chainIndex] = widthDeltaPerSecond;
 }

 float EffectElement_RibbonTrail::GetWidthChange(size_t chainIndex) const
 {
	 WidthMap::const_iterator it = _widthChangeMap.find(chainIndex);
	 if(it != _widthChangeMap.end())
	 {
		 return it->second;
	 }

	 if(_trail)
	 {
		 return _trail->getWidthChange(chainIndex);
	 }

	 return 0.0f;
 }

 void EffectElement_RibbonTrail::SetColourChange(size_t chainIndex, const ColourValue& clr)
 {
	 if(_trail)
	 {
		 _trail->setColourChange(chainIndex, clr);
	 }

	 _colourChangeMap[chainIndex] = clr;
 }

 const ColourValue& EffectElement_RibbonTrail::GetColourChange(size_t chainIndex) const
 {
	 ColourMap::const_iterator it = _colourChangeMap.find(chainIndex);
	 if(it != _colourChangeMap.end())
	 {
		 return it->second;
	 }

	 if(_trail)
	 {
		 return _trail->getColourChange(chainIndex);
	 }

	 return ColourValue::Black;
 }

 void EffectElement_RibbonTrail::SetInitialWidths(const StringMap& idxAndVal)
 {
	 for (StringMap::const_iterator i = idxAndVal.begin(); i != idxAndVal.end(); ++i)
	 {
		 size_t idx = StringConverter::parseInt(i->first);
		 float val = StringConverter::parseReal(i->second);
		 SetInitialWidth(idx, val);
	 }
 }

 StringMap EffectElement_RibbonTrail::GetInitialWidths() const
 {
	 StringMap mats;
	 if(!_initWidthMap.empty()) 
	 {
		 WidthMap::const_iterator i = _initWidthMap.begin();
		 for (; i != _initWidthMap.end(); ++i)
		 {
			mats.insert(std::make_pair(StringConverter::toString(i->first), StringConverter::toString(i->second)));
		 }
	 }
	 else if(_trail)
	 {
		 size_t c = _trail->getNumberOfChains();
		 for (size_t i = 0; i < c; ++i)
		 {			
			 mats.insert(std::make_pair(StringConverter::toString(i), StringConverter::toString(_trail->getInitialWidth(i))));
		 }
	 }

	 return mats;
 }

 void EffectElement_RibbonTrail::SetWidthChanges(const StringMap& idxAndVal)
 {
	 for (StringMap::const_iterator i = idxAndVal.begin(); i != idxAndVal.end(); ++i)
	 {
		 size_t idx = StringConverter::parseInt(i->first);
		 float val = StringConverter::parseReal(i->second);
		 SetWidthChange(idx, val);
	 }
 }

 StringMap EffectElement_RibbonTrail::GetWidthChanges() const
 {
	 StringMap mats;
	 if(!_widthChangeMap.empty()) 
	 {
		 WidthMap::const_iterator i = _widthChangeMap.begin();
		 for (; i != _widthChangeMap.end(); ++i)
		 {
			 mats.insert(std::make_pair(StringConverter::toString(i->first), StringConverter::toString(i->second)));
		 }
	 }
	 else if(_trail)
	 {
		 size_t c = _trail->getNumberOfChains();
		 for (size_t i = 0; i < c; ++i)
		 {			
			 mats.insert(std::make_pair(StringConverter::toString(i), StringConverter::toString(_trail->getWidthChange(i))));
		 }
	 }

	 return mats;
 }

 void EffectElement_RibbonTrail::SetColourChanges(const StringMap& idxAndVal)
 {
	 for (StringMap::const_iterator i = idxAndVal.begin(); i != idxAndVal.end(); ++i)
	 {
		 size_t idx = StringConverter::parseInt(i->first);
		 ColourValue val = StringConverter::parseColourValue(i->second);
		 SetColourChange(idx, val);
	 }
 }

 StringMap EffectElement_RibbonTrail::GetColourChanges() const
 {
	 StringMap mats;
	 if(!_colourChangeMap.empty()) 
	 {
		 ColourMap::const_iterator i = _colourChangeMap.begin();
		 for (; i != _colourChangeMap.end(); ++i)
		 {
			 mats.insert(std::make_pair(StringConverter::toString(i->first), StringConverter::toString(i->second)));
		 }
	 }
	 else if(_trail)
	 {
		 size_t c = _trail->getNumberOfChains();
		 for (size_t i = 0; i < c; ++i)
		 {			
			 mats.insert(std::make_pair(StringConverter::toString(i), StringConverter::toString(_trail->getColourChange(i))));
		 }
	 }

	 return mats;
}



void EffectElement_RibbonTrail::OnDestroy()
{
	OnDestroyEntity();
}

void EffectElement_RibbonTrail::Update(float deltaTime)
{
	AbstractEffectElement::Update(deltaTime);	
}

void EffectElement_RibbonTrail::Reset()
{
	AbstractEffectElement::Reset();	
}

Node* EffectElement_RibbonTrail::GetNode() const
{
	if(_trail)
	{
		return _trail->getParentNode();
	}
	return 0;
}

void EffectElement_RibbonTrail::SetMaterialName(const String& matName)
{
	_materialName = matName;
	if(_trail)
	{
		_trail->setMaterialName(matName);
	}
}

void EffectElement_RibbonTrail::SetTrailLength(float len)
{
	_trailLength = len;
	if(_trail)
	{
		_trail->setTrailLength(len);
	}
}

void EffectElement_RibbonTrail::SetMaxChainElements(size_t maxElements)
{
	_maxElements = maxElements;
	if(_trail)
	{
		_trail->setMaxChainElements(maxElements);
	}
}

void EffectElement_RibbonTrail::SetNumberOfChains(size_t numChains)
{
	_numChains = numChains;
	if(_trail)
	{
		_trail->setNumberOfChains(numChains);
	}
}

void EffectElement_RibbonTrail::SetUseLocalPosition(bool on)
{
	_useLocalPosition = on;
	if(_trail)
	{
		_trail->setUseLocalPosition(on);
	}
}

void EffectElement_RibbonTrail::SetFaceToCamera(bool on)
{
	_faceToCamera = on;
	if(_trail)
	{
		_trail->setFaceToCamera(on);
	}
}

void EffectElement_RibbonTrail::SetElementPositionOffsetOnWidthExtent(float val)
{
	_elementPositionOffsetOnWidthExtent = val;
	if(_trail)
	{
		_trail->setElementPositionOffsetOnWidthExtent(val);
	}
}

void EffectElement_RibbonTrail::SetElementOrientationOffset(const Vector3& val)
{
	_elementOrientationOffset = val;
	if(_trail)
	{
		_trail->setElementOrientationOffset(
			MathUtil::MakeQuaternion(Degree(val.y), Degree(val.x), Degree(val.z)));
	}
}

void EffectElement_RibbonTrail::SetElementScaleOffset(const Vector3& val)
{
	_elementScaleOffset = val;
	if(_trail)
	{
		_trail->setElementScaleOffset(val);
	}
}

