#pragma once
#include "ICinemaTrack.h"

class VisualObject;

class _OgeEnginePluginExport CinemaKeyFrame_Transform : public CinemaKeyFrame
{
public:

	CinemaKeyFrame_Transform(ICinemaTrack* parent, float timePos);
	virtual ~CinemaKeyFrame_Transform();

	void SetTranslate(const Vector3& trans);
	const Vector3& GetTranslate(void) const;
	
	void SetScale(const Vector3& scale);
	const Vector3& GetScale(void) const;
	
	void SetOrientation(const Quaternion& rot);
	const Quaternion& GetRotation(void) const;

	virtual String ToString() const;
	virtual void FromString(const String& val);

protected:

	Vector3 _position;
	Quaternion _rotation;
	Vector3 _scale;
};
//-----------------------------------------------------------------------------
class _OgeEnginePluginExport CinemaTrack_Movement : public AbstractCinemaTrack
{
public:

	CinemaTrack_Movement(Cinema* parent, IGameObject* object);
	virtual ~CinemaTrack_Movement(void);

	virtual const String& GetTypeName() const;

	virtual void SetTimePosition(float timePos);

	void GetInterpolatedKeyFrame(const TimeIndex& timeIndex, CinemaKeyFrame* kf) const;
	
	Animation::RotationInterpolationMode GetRotationInterpolationMode() const { return _rotationInterpolationMode; }
	void SetRotationInterpolationMode(Animation::RotationInterpolationMode val) { _rotationInterpolationMode = val; }

	Animation::InterpolationMode GetInterpolationMode() const { return _interpolationMode; }
	void SetInterpolationMode(Animation::InterpolationMode val) { _interpolationMode = val; }

	bool GetUseShortestRotationPath() const { return _useShortestRotationPath; }
	void SetUseShortestRotationPath(bool val) { _useShortestRotationPath = val; }

	virtual void _OnKeyFrameDataChanged();
	virtual void BuildPropertyMap();

	virtual void AutoUpdateKeyFrame(CinemaKeyFrame* kf);

protected:

	virtual CinemaKeyFrame* CreateKeyFrameImpl(float timePos);

	Animation::RotationInterpolationMode _rotationInterpolationMode;
	Animation::InterpolationMode _interpolationMode;
	mutable bool _useShortestRotationPath;

	struct Splines
	{
		SimpleSpline positionSpline;
		SimpleSpline scaleSpline;
		RotationalSpline rotationSpline;
	};

	VisualObject* _vo;

	mutable Splines* _sSplines;
	mutable bool _splineBuildNeeded;

	virtual void _BuildInterpolationSplines(void) const;
	virtual void OnTransform(const Vector3& pos, const Vector3& scale, const Quaternion& rot);
};
//-----------------------------------------------------------------------------
class _OgeEnginePluginExport CinemaTrack_MovementFactory : public TCinemaTrackFactory<CinemaTrack_Movement>
{
public:

	static const String FactoryName;

	virtual const String& GetTypeName() const
	{	
		return FactoryName;
	}
};