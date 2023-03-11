#include "stdafx.h"
#include "TrackEditor_FadeScreen.h"
#include "FloatValueDlg.h"
#include "Oge_EnginePlugin/CinemaKeyFrame_Colour.h"

TrackEditor_FadeScreen::TrackEditor_FadeScreen()
{

}
//--------------------------------------------------------------------------------
TrackEditor_FadeScreen::~TrackEditor_FadeScreen()
{

}
//--------------------------------------------------------------------------------
const String& TrackEditor_FadeScreen::GetTypeName() const
{
	return CinemaTrack_FadeScreenFactory::FactoryName;
}
//--------------------------------------------------------------------------------
bool TrackEditor_FadeScreen::CanCreateKeyFrame( IGameObject* object )
{
	return true;
}
//--------------------------------------------------------------------------------
bool TrackEditor_FadeScreen::NeedShowDlgWhenCreateKeyFrame() const
{
	return true;
}
//--------------------------------------------------------------------------------
void TrackEditor_FadeScreen::EditKeyFrameWithDlg( ICinemaTrack* track, CinemaKeyFrame* kf )
{
	CinemaKeyFrame_Colour* k = dynamic_cast<CinemaKeyFrame_Colour*>(kf);
	
	FloatValueDlg dlg(k->GetAlpha());

	if(dlg.DoModal() == IDOK)
	{
		k->SetAlpha(dlg.GetValue());
	}
}
//--------------------------------------------------------------------------------
