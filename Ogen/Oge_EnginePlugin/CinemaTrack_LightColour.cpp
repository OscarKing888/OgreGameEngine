#include "StdAfx.h"
#include "CinemaTrack_LightColour.h"
#include "Cinema.h"
#include "CinemaKeyFrame_Colour.h"
#include "Engine/GameLight.h"

const String CinemaTrack_LightColourFactory::FactoryName = "LightColour";

//-----------------------------------------------------------------------------
// CinemaTrack_LightColour
//-----------------------------------------------------------------------------
CinemaTrack_LightColour::CinemaTrack_LightColour(Cinema* parent, IGameObject* object)
: AbstractCinemaTrack(parent, object)
, _light(0)
{
	_needLoad = true;
}
//-----------------------------------------------------------------------------
CinemaTrack_LightColour::~CinemaTrack_LightColour(void)
{
}
//-----------------------------------------------------------------------------
const String& CinemaTrack_LightColour::GetTypeName() const
{
	return CinemaTrack_LightColourFactory::FactoryName;
}
//-----------------------------------------------------------------------------
CinemaKeyFrame* CinemaTrack_LightColour::CreateKeyFrameImpl( float timePos )
{
	return OGRE_NEW CinemaKeyFrame_Colour(this, timePos);
}
//-----------------------------------------------------------------------------
void CinemaTrack_LightColour::SetTimePosition( float timePos )
{
	// Nothing to do if no keyframes or zero weight or no node
	if (_keyframes.empty())
		return;

// 	Real scl = 1.0f;
 	TimeIndex timeIndex(timePos);
 
 	CinemaKeyFrame_Colour kf(0, timeIndex.getTimePos());

	// Keyframe pointers
	CinemaKeyFrame *kBase1, *kBase2;
	CinemaKeyFrame_Colour *k1, *k2;
	unsigned short firstKeyIndex;

	Real t = GetKeyFramesAtTime(timeIndex, &kBase1, &kBase2, &firstKeyIndex);
	k1 = static_cast<CinemaKeyFrame_Colour*>(kBase1);
	k2 = static_cast<CinemaKeyFrame_Colour*>(kBase2);

	if (t == 0.0)
	{
		// Just use k1
		ColourValue clr = k1->GetColor();
		kf.SetColor(clr);
	}
	else
	{
		ColourValue base = k1->GetColor();
		kf.SetColor( base + ((k2->GetColor() - base) * t) );
	}

	OnChangeColour(kf.GetColor());
}
//--------------------------------------------------------------------------------
void CinemaTrack_LightColour::BuildPropertyMap()
{
	AbstractCinemaTrack::BuildPropertyMap();
}
//--------------------------------------------------------------------------------
void CinemaTrack_LightColour::AutoUpdateKeyFrame( CinemaKeyFrame* kf )
{
}
//--------------------------------------------------------------------------------
void CinemaTrack_LightColour::OnChangeColour( const ColourValue& clr )
{
	if(_target && !_light)
	{
		_light = MainLight::getSingletonPtr();
	}

	if(_light)
	{
		_light->SetDiffuseColour(clr);
	}
}
//--------------------------------------------------------------------------------