#pragma once
#include "CinemaTrack_LightColour.h"

class GameLight;

class _OgeEnginePluginExport CinemaTrack_AmbientLightColour
	: public CinemaTrack_LightColour
{
public:

	CinemaTrack_AmbientLightColour(Cinema* parent, IGameObject* object);
	virtual ~CinemaTrack_AmbientLightColour(void);

	virtual const String& GetTypeName() const;

protected:

	virtual void OnChangeColour( const ColourValue& clr );
};
//-----------------------------------------------------------------------------
class _OgeEnginePluginExport CinemaTrack_AmbientLightColourFactory : public TCinemaTrackFactory<CinemaTrack_AmbientLightColour>
{
public:

	static const String FactoryName;

	virtual const String& GetTypeName() const
	{	
		return FactoryName;
	}
};