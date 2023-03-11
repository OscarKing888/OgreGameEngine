#include "stdafx.h"
#include "AnimationEnum.h"

static AnimationRotationInterpolationModeConverter _AnimationRotationInterpolationModeConverter;
static AnimationInterpolationModeConverter _AnimationInterpolationModeConverter;

AnimationRotationInterpolationModeConverter::AnimationRotationInterpolationModeConverter()
{		
	M_AddEnumToDescriptorEx(Animation::RIM_LINEAR, Linear);
	M_AddEnumToDescriptorEx(Animation::RIM_SPHERICAL, Spherical);

	AbstractEnumDescriptor::Init();
}

AnimationRotationInterpolationModeConverter::~AnimationRotationInterpolationModeConverter()
{
}

//--------------------------------------------------------------------------------

AnimationInterpolationModeConverter::AnimationInterpolationModeConverter()
{		
	M_AddEnumToDescriptorEx(Animation::IM_LINEAR, Linear);
	M_AddEnumToDescriptorEx(Animation::IM_SPLINE, Spline);

	AbstractEnumDescriptor::Init();
}

AnimationInterpolationModeConverter::~AnimationInterpolationModeConverter()
{
}
