#pragma once
#include "Core/EnumDescriptorManager.h"

class AnimationRotationInterpolationModeConverter : public AbstractEnumDescriptor
{
public:

	AnimationRotationInterpolationModeConverter();
	virtual ~AnimationRotationInterpolationModeConverter();
};

//--------------------------------------------------------------------------------

class AnimationInterpolationModeConverter : public AbstractEnumDescriptor
{
public:

	AnimationInterpolationModeConverter();
	virtual ~AnimationInterpolationModeConverter();
};
