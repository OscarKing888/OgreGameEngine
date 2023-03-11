#include "stdafx.h"
#include "TrackEditor_LightColour.h"
#include "FloatValueDlg.h"
#include "Oge_EnginePlugin/CinemaKeyFrame_Colour.h"

TrackEditor_LightColour::TrackEditor_LightColour()
{

}
//--------------------------------------------------------------------------------
TrackEditor_LightColour::~TrackEditor_LightColour()
{

}
//--------------------------------------------------------------------------------
const String& TrackEditor_LightColour::GetTypeName() const
{
	return CinemaTrack_LightColourFactory::FactoryName;
}
//--------------------------------------------------------------------------------
bool TrackEditor_LightColour::CanCreateKeyFrame( IGameObject* object )
{
	GameLight* light = dynamic_cast<GameLight*>(object);
	return light != 0;
}
//--------------------------------------------------------------------------------
bool TrackEditor_LightColour::NeedShowDlgWhenCreateKeyFrame() const
{
	return true;
}
//--------------------------------------------------------------------------------
void TrackEditor_LightColour::EditKeyFrameWithDlg( ICinemaTrack* track, CinemaKeyFrame* kf )
{
	CinemaKeyFrame_Colour* k = dynamic_cast<CinemaKeyFrame_Colour*>(kf);
	
	//FloatValueDlg dlg(k->GetAlpha());
	CColorDialog dlg;

	if(dlg.DoModal() == IDOK)
	{		
		ColourValue c;
		c.setAsABGR(dlg.GetColor());
		k->SetColor(c);
	}
}
//--------------------------------------------------------------------------------
// TrackEditor_MainLightColour
//--------------------------------------------------------------------------------
TrackEditor_MainLightColour::TrackEditor_MainLightColour()
{

}
//--------------------------------------------------------------------------------
TrackEditor_MainLightColour::~TrackEditor_MainLightColour()
{

}
//--------------------------------------------------------------------------------
const String& TrackEditor_MainLightColour::GetTypeName() const
{
	return CinemaTrack_MainLightColourFactory::FactoryName;
}
//--------------------------------------------------------------------------------
bool TrackEditor_MainLightColour::CanCreateKeyFrame( IGameObject* object )
{
	return true;
}
//--------------------------------------------------------------------------------
// TrackEditor_AmbientLightColour
//--------------------------------------------------------------------------------
TrackEditor_AmbientLightColour::TrackEditor_AmbientLightColour()
{

}
//--------------------------------------------------------------------------------
TrackEditor_AmbientLightColour::~TrackEditor_AmbientLightColour()
{

}const String& TrackEditor_AmbientLightColour::GetTypeName() const
{
	return CinemaTrack_AmbientLightColourFactory::FactoryName;
}
//--------------------------------------------------------------------------------
bool TrackEditor_AmbientLightColour::CanCreateKeyFrame( IGameObject* object )
{
	return true;
}
//--------------------------------------------------------------------------------
