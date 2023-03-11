#include "stdafx.h"
#include "TrackEditor_Property.h"
#include "PropertyEditDlg.h"

TrackEditor_Property::TrackEditor_Property()
{

}
//--------------------------------------------------------------------------------
TrackEditor_Property::~TrackEditor_Property()
{

}
//--------------------------------------------------------------------------------
const String& TrackEditor_Property::GetTypeName() const
{
	return CinemaTrack_PropertyFactory::FactoryName;
}
//--------------------------------------------------------------------------------
bool TrackEditor_Property::CanCreateKeyFrame( IGameObject* object )
{
	return object != 0;
}
//--------------------------------------------------------------------------------
bool TrackEditor_Property::NeedShowDlgWhenCreateKeyFrame() const
{
	return true;
}
//--------------------------------------------------------------------------------
void TrackEditor_Property::EditKeyFrameWithDlg( ICinemaTrack* track, CinemaKeyFrame* kf )
{
	CinemaKeyFrame_Property* k = dynamic_cast<CinemaKeyFrame_Property*>(kf);
	IGameObject* obj = track->GetTargetObjct();
	PropertyEditDlg dlg(obj);
	NameValuePairList oldProps = obj->GetValues();
	if(dlg.DoModal() == IDOK)
	{
		NameValuePairList newProps = obj->GetValues();
		NameValuePairList changedProps;
		NameValuePairList::const_iterator it = oldProps.begin();
		for (; it != oldProps.end(); ++it)
		{
			NameValuePairList::const_iterator it2 = newProps.find(it->first);
			if(it2 != newProps.end())
			{
				if(it->second != it2->second)
				{
					changedProps.insert(std::make_pair(it2->first, it2->second));
				}
			}
		}

		k->SetProperties(changedProps);
	}
}
//--------------------------------------------------------------------------------
