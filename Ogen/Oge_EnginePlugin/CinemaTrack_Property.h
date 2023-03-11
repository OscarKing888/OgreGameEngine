#pragma once
#include "ICinemaTrack.h"

class _OgeEnginePluginExport CinemaKeyFrame_Property : public CinemaKeyFrame
{
public:

	friend class CinemaTrack_Property;

	CinemaKeyFrame_Property(ICinemaTrack* parent, float timePos);
	virtual ~CinemaKeyFrame_Property();

	const Ogre::NameValuePairList& GetProperties() const { return _properties; }
	void SetProperties(const Ogre::NameValuePairList& val) { _properties = val; }

	virtual String ToString() const;
	virtual void FromString(const String& val);

protected:

	NameValuePairList _properties;
	bool _actived;
};
//-----------------------------------------------------------------------------
class _OgeEnginePluginExport CinemaTrack_Property
	: public AbstractCinemaTrack
{
public:

	CinemaTrack_Property(Cinema* parent, IGameObject* object);
	virtual ~CinemaTrack_Property(void);

	virtual const String& GetTypeName() const;

	virtual void SetTimePosition(float timePos);

	virtual void BuildPropertyMap();

	virtual void AutoUpdateKeyFrame(CinemaKeyFrame* kf);

protected:

	virtual CinemaKeyFrame* CreateKeyFrameImpl(float timePos);

	void ResetAllKF();
};
//-----------------------------------------------------------------------------
class _OgeEnginePluginExport CinemaTrack_PropertyFactory : public TCinemaTrackFactory<CinemaTrack_Property>
{
public:

	static const String FactoryName;

	virtual const String& GetTypeName() const
	{	
		return FactoryName;
	}
};