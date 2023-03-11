#pragma once
#include "CinemaTrack_LightColour.h"

class _OgeEnginePluginExport CinemaTrack_MainLightColour
	: public CinemaTrack_LightColour
{
public:

	CinemaTrack_MainLightColour(Cinema* parent, IGameObject* object);
	virtual ~CinemaTrack_MainLightColour(void);

	virtual const String& GetTypeName() const;

protected:

	virtual void OnChangeColour( const ColourValue& clr );
};
//-----------------------------------------------------------------------------
class _OgeEnginePluginExport CinemaTrack_MainLightColourFactory : public TCinemaTrackFactory<CinemaTrack_MainLightColour>
{
public:

	static const String FactoryName;

	virtual const String& GetTypeName() const
	{	
		return FactoryName;
	}
};