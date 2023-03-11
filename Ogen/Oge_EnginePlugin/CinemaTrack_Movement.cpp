#include "StdAfx.h"
#include "CinemaTrack_Movement.h"
#include "Cinema.h"
#include "Engine/VisualObject.h"
#include "AnimationEnum.h"

const String CinemaTrack_MovementFactory::FactoryName = "Movement";

//-----------------------------------------------------------------------------
// CinemaKeyFrame_Transform
//-----------------------------------------------------------------------------
CinemaKeyFrame_Transform::CinemaKeyFrame_Transform( ICinemaTrack* parent, float timePos )
: CinemaKeyFrame(parent, timePos)
, _position(Vector3::ZERO)
, _rotation(Quaternion::IDENTITY)
, _scale(Vector3::UNIT_SCALE)
{
}
//-----------------------------------------------------------------------------
CinemaKeyFrame_Transform::~CinemaKeyFrame_Transform()
{
}
//-----------------------------------------------------------------------------
void CinemaKeyFrame_Transform::SetTranslate( const Vector3& trans )
{
	_position = trans;
	if (_parent)
	{
		_parent->_OnKeyFrameDataChanged();
	}
}
//-----------------------------------------------------------------------------
const Vector3& CinemaKeyFrame_Transform::GetTranslate( void ) const
{
	return _position;
}
//-----------------------------------------------------------------------------
void CinemaKeyFrame_Transform::SetScale( const Vector3& scale )
{
	_scale = scale;
	if (_parent)
	{
		_parent->_OnKeyFrameDataChanged();
	}
}
//-----------------------------------------------------------------------------
const Vector3& CinemaKeyFrame_Transform::GetScale( void ) const
{
	return _scale;
}
//-----------------------------------------------------------------------------
void CinemaKeyFrame_Transform::SetOrientation( const Quaternion& rot )
{
	_rotation = rot;
	if (_parent)
	{
		_parent->_OnKeyFrameDataChanged();
	}
}
//-----------------------------------------------------------------------------
const Quaternion& CinemaKeyFrame_Transform::GetRotation( void ) const
{
	return _rotation;
}
//-----------------------------------------------------------------------------
String CinemaKeyFrame_Transform::ToString() const
{
	return formatString("%s|%s|%s",
		StringConverter::toString(_position).c_str(),
		StringConverter::toString(_scale).c_str(),
		StringConverter::toString(_rotation).c_str());
}
//-----------------------------------------------------------------------------
void CinemaKeyFrame_Transform::FromString( const String& val )
{
	StringVector sv = StringUtil::split(val, "|");
	if(sv.size() == 3)
	{
		_position = StringConverter::parseVector3(sv[0]);
		_scale = StringConverter::parseVector3(sv[1]);
		_rotation = StringConverter::parseQuaternion(sv[2]);
	}
}
//-----------------------------------------------------------------------------
// CinemaTrack_Movement
//-----------------------------------------------------------------------------
CinemaTrack_Movement::CinemaTrack_Movement(Cinema* parent, IGameObject* object)
: AbstractCinemaTrack(parent, object)
, _useShortestRotationPath(true)
, _splineBuildNeeded(false)
, _rotationInterpolationMode(Animation::RIM_LINEAR)
, _interpolationMode(Animation::IM_LINEAR)
, _vo(0)
, _sSplines(0)
{
	_needLoad = true;
}
//-----------------------------------------------------------------------------
CinemaTrack_Movement::~CinemaTrack_Movement(void)
{
	if(_sSplines)
	{
		OGRE_DELETE_T(_sSplines, Splines, MEMCATEGORY_ANIMATION);
	}
}
//-----------------------------------------------------------------------------
const String& CinemaTrack_Movement::GetTypeName() const
{
	return CinemaTrack_MovementFactory::FactoryName;
}
//-----------------------------------------------------------------------------
CinemaKeyFrame* CinemaTrack_Movement::CreateKeyFrameImpl( float timePos )
{
	return OGRE_NEW CinemaKeyFrame_Transform(this, timePos);
}
//-----------------------------------------------------------------------------
void CinemaTrack_Movement::SetTimePosition( float timePos )
{
	// Nothing to do if no keyframes or zero weight or no node
	if (_keyframes.empty())
		return;

	Real scl = 1.0f;
	TimeIndex timeIndex(timePos);

	CinemaKeyFrame_Transform kf(0, timeIndex.getTimePos());
	GetInterpolatedKeyFrame(timeIndex, &kf);

	// add to existing. Weights are not relative, but treated as absolute multipliers for the animation
	Vector3 translate = kf.GetTranslate();// * weight * scl;
	//Vector3 curPos = _vo->GetPosition();
	//translate += curPos;

	// interpolate between no-rotation and full rotation, to point 'weight', so 0 = no rotate, 1 = full
	Quaternion rotate;
	Animation::RotationInterpolationMode rim =
		GetRotationInterpolationMode();
	if (rim == Animation::RIM_LINEAR)
	{
		rotate = Quaternion::nlerp(1.0f/*weight*/, Quaternion::IDENTITY, kf.GetRotation(), _useShortestRotationPath);
	}
	else //if (rim == Animation::RIM_SPHERICAL)
	{
		rotate = Quaternion::Slerp(1.0f/*weight*/, Quaternion::IDENTITY, kf.GetRotation(), _useShortestRotationPath);
	}
	Vector3 scale = kf.GetScale();
	// Not sure how to modify scale for cumulative anims... leave it alone
	//scale = ((Vector3::UNIT_SCALE - kf.getScale()) * weight) + Vector3::UNIT_SCALE;
	if (scl != 1.0f && scale != Vector3::UNIT_SCALE)
	{
		scale = Vector3::UNIT_SCALE + (scale - Vector3::UNIT_SCALE) * scl;
	}

	OnTransform(translate, scale, rotate);
}
//-----------------------------------------------------------------------------
void CinemaTrack_Movement::GetInterpolatedKeyFrame(const TimeIndex& timeIndex, CinemaKeyFrame* kf) const
{
// 	if (mListener)
// 	{
// 		if (mListener->GetInterpolatedKeyFrame(this, timeIndex, kf))
// 			return;
// 	}

	CinemaKeyFrame_Transform* kret = static_cast<CinemaKeyFrame_Transform*>(kf);

	// Keyframe pointers
	CinemaKeyFrame *kBase1, *kBase2;
	CinemaKeyFrame_Transform *k1, *k2;
	unsigned short firstKeyIndex;

	Real t = this->GetKeyFramesAtTime(timeIndex, &kBase1, &kBase2, &firstKeyIndex);
	k1 = static_cast<CinemaKeyFrame_Transform*>(kBase1);
	k2 = static_cast<CinemaKeyFrame_Transform*>(kBase2);

	if (t == 0.0)
	{
		// Just use k1
		kret->SetOrientation(k1->GetRotation());
		kret->SetTranslate(k1->GetTranslate());
		kret->SetScale(k1->GetScale());
	}
	else
	{
		// Interpolate by t
		Animation::InterpolationMode im = GetInterpolationMode();
		Animation::RotationInterpolationMode rim =
			GetRotationInterpolationMode();
		Vector3 base;
		switch(im)
		{
		case Animation::IM_LINEAR:
			// Interpolate linearly
			// Rotation
			// Interpolate to nearest rotation if _useShortestRotationPath set
			if (rim == Animation::RIM_LINEAR)
			{
				kret->SetOrientation( Quaternion::nlerp(t, k1->GetRotation(),
					k2->GetRotation(), _useShortestRotationPath) );
			}
			else //if (rim == Animation::RIM_SPHERICAL)
			{
				kret->SetOrientation( Quaternion::Slerp(t, k1->GetRotation(),
					k2->GetRotation(), _useShortestRotationPath) );
			}

			// Translation
			base = k1->GetTranslate();
			kret->SetTranslate( base + ((k2->GetTranslate() - base) * t) );

			// Scale
			base = k1->GetScale();
			kret->SetScale( base + ((k2->GetScale() - base) * t) );
			break;

		case Animation::IM_SPLINE:
			// Spline interpolation

			// Build splines if required
			if (_splineBuildNeeded)
			{
				_BuildInterpolationSplines();
			}

			// Rotation, take _useShortestRotationPath into account
			kret->SetOrientation( _sSplines->rotationSpline.interpolate(firstKeyIndex, t,
				_useShortestRotationPath) );

			// Translation
			kret->SetTranslate( _sSplines->positionSpline.interpolate(firstKeyIndex, t) );

			// Scale
			kret->SetScale( _sSplines->scaleSpline.interpolate(firstKeyIndex, t) );

			break;
		}

	}
}
//-----------------------------------------------------------------------------
void CinemaTrack_Movement::_BuildInterpolationSplines(void) const
{
	// Allocate splines if not exists
	if (!_sSplines)
	{
		_sSplines = OGRE_NEW_T(Splines, MEMCATEGORY_ANIMATION);
	}

	// Cache to register for optimisation
	Splines* splines = _sSplines;

	// Don't calc automatically, do it on request at the end
	splines->positionSpline.setAutoCalculate(false);
	splines->rotationSpline.setAutoCalculate(false);
	splines->scaleSpline.setAutoCalculate(false);

	splines->positionSpline.clear();
	splines->rotationSpline.clear();
	splines->scaleSpline.clear();

	CinemaKeyFrameList::const_iterator i, iend;
	iend = _keyframes.end(); // precall to avoid overhead
	for (i = _keyframes.begin(); i != iend; ++i)
	{
		CinemaKeyFrame_Transform* kf = static_cast<CinemaKeyFrame_Transform*>(*i);
		splines->positionSpline.addPoint(kf->GetTranslate());
		splines->rotationSpline.addPoint(kf->GetRotation());
		splines->scaleSpline.addPoint(kf->GetScale());
	}

	splines->positionSpline.recalcTangents();
	splines->rotationSpline.recalcTangents();
	splines->scaleSpline.recalcTangents();


	_splineBuildNeeded = false;
}
//--------------------------------------------------------------------------------
void CinemaTrack_Movement::_OnKeyFrameDataChanged()
{
	_splineBuildNeeded = true;
}
//--------------------------------------------------------------------------------
void CinemaTrack_Movement::BuildPropertyMap()
{
	AbstractCinemaTrack::BuildPropertyMap();

	AddEnumProp(AnimationRotationInterpolationModeConverter, Animation::RotationInterpolationMode,
		RotationInterpolationMode, CinemaTrack_Movement, "RotationInterpolationMode", "Transform");

	AddEnumProp(AnimationInterpolationModeConverter, Animation::InterpolationMode,
		InterpolationMode, CinemaTrack_Movement, "InterpolationMode", "Transform");
}
//--------------------------------------------------------------------------------
void CinemaTrack_Movement::AutoUpdateKeyFrame( CinemaKeyFrame* kf )
{
	CinemaKeyFrame_Transform* tkf = static_cast<CinemaKeyFrame_Transform*>(kf);
	VisualObject* vo = dynamic_cast<VisualObject*>(_target);
	if(vo)
	{
		tkf->SetTranslate(vo->GetPosition());
		tkf->SetOrientation(vo->GetOrientation());
		tkf->SetScale(vo->GetScale());
	}
	else
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
			"Target object is not VisualObject", __FUNCTION__);
	}
}
//--------------------------------------------------------------------------------
void CinemaTrack_Movement::OnTransform( const Vector3& pos, const Vector3& scale, const Quaternion& rot )
{
	if(!_vo)
	{
		_vo = dynamic_cast<VisualObject*>(_target);
		if(!_vo)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Cinema Movement Track must use VisualObject:" + _target->GetTypeName(),
				__FUNCTION__);
		}
	}

	_vo->SetPosition(pos);
	_vo->SetOrientation(rot);
	_vo->SetScale(scale);
}