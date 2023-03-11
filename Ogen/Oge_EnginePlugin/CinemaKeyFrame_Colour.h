#pragma once
#include "ICinemaTrack.h"

class _OgeEnginePluginExport CinemaKeyFrame_Colour : public CinemaKeyFrame
{
public:

	CinemaKeyFrame_Colour(ICinemaTrack* parent, float timePos);
	virtual ~CinemaKeyFrame_Colour();

	void SetColor(const ColourValue& clr) { _color = clr; }
	void SetColor(float r, float g, float b);
	const ColourValue& GetColor(void) const;

	void SetAlpha(float a);
	float GetAlpha(void) const;

	virtual String ToString() const;
	virtual void FromString(const String& val);

protected:

	ColourValue _color;
};