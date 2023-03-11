#pragma once
#include "CinemaTrack_Movement.h"

//-----------------------------------------------------------------------------
class _OgeEnginePluginExport CinemaTrack_Camera : public CinemaTrack_Movement
{
public:

	CinemaTrack_Camera(Cinema* parent, IGameObject* object);
	virtual ~CinemaTrack_Camera(void);

	virtual const String& GetTypeName() const;

	virtual void BuildPropertyMap();

	virtual void AutoUpdateKeyFrame(CinemaKeyFrame* kf);

	virtual String ToString() const;
protected:

	virtual void OnTransform( const Vector3& pos, const Vector3& scale, const Quaternion& rot );
};
//-----------------------------------------------------------------------------
class _OgeEnginePluginExport CinemaTrack_CameraFactory : public TCinemaTrackFactory<CinemaTrack_Camera>
{
public:

	static const String FactoryName;

	virtual const String& GetTypeName() const
	{	
		return FactoryName;
	}
};