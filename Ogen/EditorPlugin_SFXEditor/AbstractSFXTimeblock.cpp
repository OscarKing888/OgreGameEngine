#include "stdafx.h"
#include "AbstractSFXTimeblock.h"

AbstractSFXTimeblock::AbstractSFXTimeblock()
{

}

AbstractSFXTimeblock::~AbstractSFXTimeblock()
{

}

void AbstractSFXTimeblock::SetElement( IEffectsElement* val )
{
	_element = val;

	UpdateUI();

}

void AbstractSFXTimeblock::UpdateUI()
{
	_startTime = _element->GetStartTime();
	_length = _element->GetLifeTime();
	_caption = _element->GetTypeName().c_str();
}