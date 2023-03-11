#include "StdAfx.h"
#include "CinemaTrack_MainLightColour.h"
#include "Engine/GameLight.h"

const String CinemaTrack_MainLightColourFactory::FactoryName = "MainLightColour";

//-----------------------------------------------------------------------------
// CinemaTrack_MainLightColour
//-----------------------------------------------------------------------------
CinemaTrack_MainLightColour::CinemaTrack_MainLightColour(Cinema* parent, IGameObject* object)
: CinemaTrack_LightColour(parent, object)
{
	_needTarget =  false;
}
//-----------------------------------------------------------------------------
CinemaTrack_MainLightColour::~CinemaTrack_MainLightColour(void)
{
}
//-----------------------------------------------------------------------------
const String& CinemaTrack_MainLightColour::GetTypeName() const
{
	return CinemaTrack_MainLightColourFactory::FactoryName;
}
//--------------------------------------------------------------------------------
void CinemaTrack_MainLightColour::OnChangeColour( const ColourValue& clr )
{
	if(MainLight::getSingletonPtr())
	{
		MainLight::getSingleton().SetDiffuseColour(clr);
	}
}