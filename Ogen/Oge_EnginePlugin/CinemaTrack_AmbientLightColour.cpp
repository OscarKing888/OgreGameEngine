#include "StdAfx.h"
#include "CinemaTrack_AmbientLightColour.h"
#include "Engine/SceneEnvironment.h"

const String CinemaTrack_AmbientLightColourFactory::FactoryName = "AmbientLightColour";

//-----------------------------------------------------------------------------
// CinemaTrack_AmbientLightColour
//-----------------------------------------------------------------------------
CinemaTrack_AmbientLightColour::CinemaTrack_AmbientLightColour(Cinema* parent, IGameObject* object)
: CinemaTrack_LightColour(parent, object)
{
	_needTarget =  false;
}
//-----------------------------------------------------------------------------
CinemaTrack_AmbientLightColour::~CinemaTrack_AmbientLightColour(void)
{
}
//-----------------------------------------------------------------------------
const String& CinemaTrack_AmbientLightColour::GetTypeName() const
{
	return CinemaTrack_AmbientLightColourFactory::FactoryName;
}
//--------------------------------------------------------------------------------
void CinemaTrack_AmbientLightColour::OnChangeColour( const ColourValue& clr )
{
	SceneEnvironment::getSingleton().SetAmbientLight(clr);
}