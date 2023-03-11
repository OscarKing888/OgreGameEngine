#pragma once
#include "ICinemaTrack.h"

class GameLight;

class _OgeEnginePluginExport CinemaTrack_LightColour
	: public AbstractCinemaTrack
{
public:

	CinemaTrack_LightColour(Cinema* parent, IGameObject* object);
	virtual ~CinemaTrack_LightColour(void);

	virtual const String& GetTypeName() const;

	virtual void SetTimePosition(float timePos);

	virtual void BuildPropertyMap();

	virtual void AutoUpdateKeyFrame(CinemaKeyFrame* kf);

protected:

	GameLight* _light;

	virtual CinemaKeyFrame* CreateKeyFrameImpl(float timePos);
	virtual void OnChangeColour( const ColourValue& clr );
};
//-----------------------------------------------------------------------------
class _OgeEnginePluginExport CinemaTrack_LightColourFactory : public TCinemaTrackFactory<CinemaTrack_LightColour>
{
public:

	static const String FactoryName;

	virtual const String& GetTypeName() const
	{	
		return FactoryName;
	}
};