#include "StdAfx.h"
#include "AbstractEffectElement.h"
#include "SpecialEffectsManager.h"

AbstractEffectElement::AbstractEffectElement(const String& name, ISpecialEffects* parent)
: _parent(parent)
, _timeElapsed(0.0f)
, _startTime(0.0f)
, _lifeTime(1.0f)
, _isStarted(false)
, _isEnd(false)
, _isManualLifeTime(false)
, _positionOffset(Vector3::ZERO)
, _orientationOffset(Quaternion::IDENTITY)
, _attach(true)
, _node(0)
{
	_name = name;
}

AbstractEffectElement::~AbstractEffectElement(void)
{
	OnDestroy();
}

void AbstractEffectElement::BuildPropertyMap()
{
	PropertyObject::BuildPropertyMap();
	AddProp(Property_Bool, Attach, AbstractEffectElement, "Attach", "EffectElement");
	AddProp(Property_String, AttachPointName, AbstractEffectElement, "AttachPointName", "EffectElement");
	AddProp(Property_Float, LifeTime, AbstractEffectElement, "LifeTime", "EffectElement");
	AddProp(Property_Float, StartTime, AbstractEffectElement, "StartTime", "EffectElement");
	AddProp(Property_Bool, ManualLifeTime, AbstractEffectElement, "ManualLifeTime", "EffectElement");
	AddProp(Property_Vector3, PositionOffset, AbstractEffectElement, "PositionOffset", "EffectElement");
	AddProp(Property_Quaternion, OrientationOffset, AbstractEffectElement, "OrientationOffset", "EffectElement");

	RemoveProperty("TriggerMask");
}

const Vector3&  AbstractEffectElement::GetPositionOffset() const
{
	return _positionOffset;
}

void AbstractEffectElement::SetPositionOffset(const Vector3& pos)
{
	_positionOffset = pos;
}

const Vector3&  AbstractEffectElement::GetScaleOffset() const
{
	return _scaleOffset;
}

void AbstractEffectElement::SetScaleOffset(const Vector3& s)
{
	_scaleOffset = s;
}

const Quaternion& AbstractEffectElement::GetOrientationOffset() const
{
	return _orientationOffset;
}

void AbstractEffectElement::SetOrientationOffset(const Quaternion& orient)
{
	_orientationOffset = orient;
}


ISpecialEffects* AbstractEffectElement::GetParent() const
{
	return _parent;
}

void AbstractEffectElement::Update(float deltaTime)
{
	_timeElapsed += deltaTime;
	if((!_isStarted) && (_timeElapsed >= _startTime))
	{
		_isStarted = true;
		OnStart();
	}

	if((!_isManualLifeTime) && (!_isEnd) && (_timeElapsed >= _lifeTime + _startTime))
	{
		_isEnd = true;
		OnEnd();
	}

	if(_isStarted && !_isEnd)
	{
		OnUpdateController(deltaTime);
	}
}

void AbstractEffectElement::OnStart()
{
}

void AbstractEffectElement::OnEnd()
{

}

void AbstractEffectElement::OnUpdateController(float deltaTime)
{
}

bool AbstractEffectElement::GetManualLifeTime() const
{
	return _isManualLifeTime;
}

bool AbstractEffectElement::IsManualLifeTime() const
{
	return _isManualLifeTime;
}

void AbstractEffectElement::SetManualLifeTime(bool on)
{
	_isManualLifeTime = on;
}

float AbstractEffectElement::GetLifeTime() const
{
	return _lifeTime;
}

void AbstractEffectElement::SetLifeTime(float val)
{
	_lifeTime = val;
}

float AbstractEffectElement::GetStartTime() const
{
	return _startTime;
}

void AbstractEffectElement::SetStartTime(float val)
{
	_startTime = val;
}

void AbstractEffectElement::Reset()
{
	if(_isStarted)
	{
		OnEnd();
	}

	_timeElapsed = 0.0f;
	_isEnd = false;
	_isStarted = false;
}

void AbstractEffectElement::Cancel()
{
	_isEnd = true;
	_isStarted = true;

	if(_isStarted)
	{
		OnEnd();
	}
}

SceneManager* AbstractEffectElement::GetSceneManager()
{
	return GameApp::getSingleton().GetGame()->GetVisualScene();
}

void AbstractEffectElement::OnDestroy()
{

}

AnimableValuePtr AbstractEffectElement::CreateAnimableValue(const String& valueName)
{
	AnimableValuePtr val;
	return val;
}

bool AbstractEffectElement::HasAnimableValue(const String& valueName) const
{
	return false;
}

StringVector AbstractEffectElement::GetAnimableValueList() const
{
	StringVector lst;
	return lst;
}

const Vector3& AbstractEffectElement::GetPosition() const
{
	Node* node = GetNode();
	if(node)
	{
		return node->getPosition();
	}
	return Vector3::ZERO;
}

void AbstractEffectElement::SetPosition(const Vector3& pos)
{
	Node* node = GetNode();
	if(node)
	{
		node->setPosition(pos);
	}
}

const Vector3& AbstractEffectElement::GetWorldPosition() const
{
	Node* node = GetNode();
	if(node)
	{
		return node->_getDerivedPosition();
	}
	return Vector3::ZERO;
}

const Vector3& AbstractEffectElement::GetScale() const
{
	Node* node = GetNode();
	if(node)
	{
		return node->getScale();
	}

	return Vector3::UNIT_SCALE;
}

void AbstractEffectElement::SetScale(const Vector3& scale)
{
	Node* node = GetNode();
	if(node)
	{
		node->setScale(scale);
	}
}

const Vector3& AbstractEffectElement::GetWorldScale() const
{
	Node* node = GetNode();
	if(node)
	{
		return node->_getDerivedScale();
	}

	return Vector3::UNIT_SCALE;
}

const Quaternion& AbstractEffectElement::GetOrientation() const
{
	Node* node = GetNode();
	if(node)
	{
		return node->getOrientation();
	}

	return Quaternion::IDENTITY;
}

void AbstractEffectElement::SetOrientation(const Quaternion& orient)
{
	Node* node = GetNode();
	if(node)
	{
		node->setOrientation(orient);
	}
}

const Quaternion& AbstractEffectElement::GetWorldOrientation() const
{
	Node* node = GetNode();
	if(node)
	{
		return node->_getDerivedOrientation();
	}
	return Quaternion::IDENTITY;
}

Node* AbstractEffectElement::GetNode() const
{
	return _node;
}

bool AbstractEffectElement::IsEnd() const
{
	return _isStarted && _isEnd;
}

void AbstractEffectElement::SetAttach(bool on)
{
	_attach = on;
}
