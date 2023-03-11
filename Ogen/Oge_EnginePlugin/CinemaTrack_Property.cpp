#include "StdAfx.h"
#include "CinemaTrack_Property.h"
#include "Cinema.h"

const String CinemaTrack_PropertyFactory::FactoryName = "Property";

//-----------------------------------------------------------------------------
// CinemaKeyFrame_Property
//-----------------------------------------------------------------------------
CinemaKeyFrame_Property::CinemaKeyFrame_Property( ICinemaTrack* parent, float timePos )
: CinemaKeyFrame(parent, timePos)
, _actived(false)
{
}
//-----------------------------------------------------------------------------
CinemaKeyFrame_Property::~CinemaKeyFrame_Property()
{
}
//-----------------------------------------------------------------------------
String CinemaKeyFrame_Property::ToString() const
{
	String val;
	NameValuePairList::const_iterator i = _properties.begin();
	for (; i != _properties.end(); ++i)
	{
		val += i->first;
		val += "|";
		val += i->second;
	}
	return val;
}
//-----------------------------------------------------------------------------
void CinemaKeyFrame_Property::FromString( const String& val )
{
	StringVector sv = StringUtil::split(val, "|");
	if(sv.size() % 2 == 0)
	{
		_properties.clear();
		for (size_t i = 0; i < sv.size(); i += 2)
		{
			_properties.insert(std::make_pair(sv[i], sv[i + 1]));
		}
	}
}
//-----------------------------------------------------------------------------
// CinemaTrack_Property
//-----------------------------------------------------------------------------
CinemaTrack_Property::CinemaTrack_Property(Cinema* parent, IGameObject* object)
: AbstractCinemaTrack(parent, object)
{
	_needLoad = true;
}
//-----------------------------------------------------------------------------
CinemaTrack_Property::~CinemaTrack_Property(void)
{
}
//-----------------------------------------------------------------------------
const String& CinemaTrack_Property::GetTypeName() const
{
	return CinemaTrack_PropertyFactory::FactoryName;
}
//-----------------------------------------------------------------------------
CinemaKeyFrame* CinemaTrack_Property::CreateKeyFrameImpl( float timePos )
{
	return OGRE_NEW CinemaKeyFrame_Property(this, timePos);
}
//-----------------------------------------------------------------------------
void CinemaTrack_Property::SetTimePosition( float timePos )
{
	// Nothing to do if no keyframes or zero weight or no node
	if (_keyframes.empty())
		return;

	if(timePos == 0.0f)
	{
		ResetAllKF();
	}

// 	Real scl = 1.0f;
 	TimeIndex timeIndex(timePos);
 
 	CinemaKeyFrame_Property kf(0, timeIndex.getTimePos());

	// Keyframe pointers
	CinemaKeyFrame *kBase1, *kBase2;
	CinemaKeyFrame_Property *k1, *k2;
	unsigned short firstKeyIndex;

	Real t = GetKeyFramesAtTime(timeIndex, &kBase1, &kBase2, &firstKeyIndex);
	k1 = static_cast<CinemaKeyFrame_Property*>(kBase1);
	k2 = static_cast<CinemaKeyFrame_Property*>(kBase2);

	if (t >= 0.0)
	{
		if(!k1->_actived)
		{
			k1->_actived = true;
			_target->SetValues(k1->GetProperties());
		}
	}
}
//--------------------------------------------------------------------------------
void CinemaTrack_Property::BuildPropertyMap()
{
	AbstractCinemaTrack::BuildPropertyMap();
}
//--------------------------------------------------------------------------------
void CinemaTrack_Property::AutoUpdateKeyFrame( CinemaKeyFrame* kf )
{
}
//--------------------------------------------------------------------------------
void CinemaTrack_Property::ResetAllKF()
{
	for (size_t i = 0; i < _keyframes.size(); ++i)
	{
		CinemaKeyFrame_Property* kf = dynamic_cast<CinemaKeyFrame_Property*>(_keyframes[i]);
		kf->_actived = false;
	}
}
//--------------------------------------------------------------------------------
