#include "stdafx.h"
#include "CinemaKeyFrame_Colour.h"

//-----------------------------------------------------------------------------
// CinemaKeyFrame_Colour
//-----------------------------------------------------------------------------
CinemaKeyFrame_Colour::CinemaKeyFrame_Colour( ICinemaTrack* parent, float timePos )
: CinemaKeyFrame(parent, timePos)
, _color(0, 0, 0)
{
}
//-----------------------------------------------------------------------------
CinemaKeyFrame_Colour::~CinemaKeyFrame_Colour()
{
}
//-----------------------------------------------------------------------------
void CinemaKeyFrame_Colour::SetColor( float r, float g, float b )
{
	_color.r = r;
	_color.g = g;
	_color.b = b;

	if (_parent)
	{
		_parent->_OnKeyFrameDataChanged();
	}
}
//-----------------------------------------------------------------------------
const ColourValue& CinemaKeyFrame_Colour::GetColor( void ) const
{
	return _color;
}
//-----------------------------------------------------------------------------
void CinemaKeyFrame_Colour::SetAlpha( float a )
{
	_color.a = a;
	if (_parent)
	{
		_parent->_OnKeyFrameDataChanged();
	}
}
//-----------------------------------------------------------------------------
float CinemaKeyFrame_Colour::GetAlpha( void ) const
{
	return _color.a;
}
//-----------------------------------------------------------------------------
String CinemaKeyFrame_Colour::ToString() const
{
	return StringConverter::toString(_color);
}

void CinemaKeyFrame_Colour::FromString( const String& val )
{
	_color = StringConverter::parseColourValue(val);
}