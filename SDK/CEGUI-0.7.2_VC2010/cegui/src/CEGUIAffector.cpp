/***********************************************************************
    filename:   CEGUIAffector.cpp
    created:    7/8/2010
    author:     Martin Preisler

    purpose:    Implements the Affector class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUIAffector.h"
#include "CEGUIKeyFrame.h"
#include "CEGUIAnimationManager.h"
#include "CEGUIInterpolator.h"
#include "CEGUIPropertySet.h"
#include "CEGUIAnimationInstance.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
Affector::Affector(Animation* parent):
        d_parent(parent),
        d_applicationMethod(AM_Absolute),
        d_targetProperty(""),
        d_interpolator(0)
{}

//----------------------------------------------------------------------------//
Affector::~Affector(void)
{
    while (d_keyFrames.size() > 0)
    {
        destroyKeyFrame(getKeyFrameAtIdx(0));
    }
}

//----------------------------------------------------------------------------//
void Affector::setApplicationMethod(ApplicationMethod method)
{
    d_applicationMethod = method;
}

//----------------------------------------------------------------------------//
Affector::ApplicationMethod Affector::getApplicationMethod() const
{
    return d_applicationMethod;
}


//----------------------------------------------------------------------------//
void Affector::setTargetProperty(const String& target)
{
    d_targetProperty = target;
}

//----------------------------------------------------------------------------//
const String& Affector::getTargetProperty() const
{
    return d_targetProperty;
}

//----------------------------------------------------------------------------//
void Affector::setInterpolator(Interpolator* interpolator)
{
    d_interpolator = interpolator;
}

//----------------------------------------------------------------------------//
void Affector::setInterpolator(const String& name)
{
    d_interpolator = AnimationManager::getSingleton().getInterpolator(name);
}

//----------------------------------------------------------------------------//
Interpolator* Affector::getInterpolator() const
{
    return d_interpolator;
}

//----------------------------------------------------------------------------//
KeyFrame* Affector::createKeyFrame(float position)
{
    if (d_keyFrames.find(position) != d_keyFrames.end())
    {
        CEGUI_THROW(InvalidRequestException(
                        "Affector::createKeyFrame: Unable to create KeyFrame "
                        "at given position, there already is a KeyFrame "
                        "on that position."));
    }

    KeyFrame* ret = new KeyFrame(this, position);
    // todo: checking
    d_keyFrames.insert(std::make_pair(position, ret));

    return ret;
}

//----------------------------------------------------------------------------//
KeyFrame* Affector::createKeyFrame(float position, const String& value,
                                   KeyFrame::Progression progression, const String& sourceProperty)
{
    KeyFrame* ret = createKeyFrame(position);
    ret->setValue(value);
    ret->setProgression(progression);
    ret->setSourceProperty(sourceProperty);

    return ret;
}

//----------------------------------------------------------------------------//
void Affector::destroyKeyFrame(KeyFrame* keyframe)
{
    KeyFrameMap::iterator it = d_keyFrames.find(keyframe->getPosition());

    if (it == d_keyFrames.end())
    {
        CEGUI_THROW(InvalidRequestException(
                        "Affector::destroyKeyFrame: Unable to destroy given KeyFrame! "
                        "No such KeyFrame was found."));
    }

    d_keyFrames.erase(it);
}

//----------------------------------------------------------------------------//
KeyFrame* Affector::getKeyFrameAtPosition(float position) const
{
    KeyFrameMap::const_iterator it = d_keyFrames.find(position);

    if (it == d_keyFrames.end())
    {
        CEGUI_THROW(InvalidRequestException(
                        "Affector::getKeyFrameAtPosition: Can't find a KeyFrame with given "
                        "position."));
    }

    return it->second;
}

//----------------------------------------------------------------------------//
KeyFrame* Affector::getKeyFrameAtIdx(size_t index) const
{
    if (index >= d_keyFrames.size())
    {
        CEGUI_THROW(InvalidRequestException(
                        "Affector::getKeyFrameAtIdx: Out of bounds!"));
    }

    KeyFrameMap::const_iterator it = d_keyFrames.begin();
    std::advance(it, index);

    return it->second;
}

//----------------------------------------------------------------------------//
size_t Affector::getNumKeyFrames() const
{
    return d_keyFrames.size();
}

//----------------------------------------------------------------------------//
void Affector::moveKeyFrameToPosition(KeyFrame* keyframe, float newPosition)
{
    moveKeyFrameToPosition(keyframe->getPosition(), newPosition);
}

//----------------------------------------------------------------------------//
void Affector::moveKeyFrameToPosition(float oldPosition, float newPosition)
{
    KeyFrame* kf = getKeyFrameAtPosition(oldPosition);

    // no need to check for existance, getKeyFrameAtPosition already
    // does that for us (and throws an exception when kf is not found)
    d_keyFrames.erase(d_keyFrames.find(oldPosition));
    d_keyFrames.insert(std::make_pair(newPosition, kf));

    kf->notifyPositionChanged(newPosition);
}

//----------------------------------------------------------------------------//
void Affector::savePropertyValues(AnimationInstance* instance)
{
    switch (d_applicationMethod)
    {
    case AM_Relative:
    case AM_RelativeMultiply:
        instance->savePropertyValue(d_targetProperty);
        break;

    default:
        break;
    }

    // now let all keyframes save their desired property values too
    for (KeyFrameMap::const_iterator it = d_keyFrames.begin();
            it != d_keyFrames.end(); ++it)
    {
        it->second->savePropertyValue(instance);
    }
}

//----------------------------------------------------------------------------//
void Affector::apply(AnimationInstance* instance)
{
    PropertySet* target = instance->getTarget();
    const float position = instance->getPosition();

    // special case
    if (d_keyFrames.empty())
    {
        return;
    }

    if (d_targetProperty.empty())
    {
        Logger::getSingleton().logEvent(
            "Affector can't be applied when target property is set!", Warnings);
        return;
    }

    if (!d_interpolator)
    {
        Logger::getSingleton().logEvent(
            "Affector can't be applied when no interpolator is set!", Warnings);
        return;
    }

    KeyFrame* left = 0;
    KeyFrame* right = 0;

    // find 2 neighbouring keyframes
    for (KeyFrameMap::const_iterator it = d_keyFrames.begin();
            it != d_keyFrames.end(); ++it)
    {
        KeyFrame* current = it->second;

        if (current->getPosition() <= position)
        {
            left = current;
        }

        if (current->getPosition() >= position && !right)
        {
            right = current;
        }
    }

    float leftDistance, rightDistance;

    if (left)
    {
        leftDistance = position - left->getPosition();
    }
    else
        // if no keyframe is suitable for left neighbour, pick the first one
    {
        left = d_keyFrames.begin()->second;
        leftDistance = 0;
    }

    if (right)
    {
        rightDistance = right->getPosition() - position;
    }
    else
        // if no keyframe is suitable for the right neighbour, pick the last one
    {
        right = d_keyFrames.rbegin()->second;
        rightDistance = 0;
    }

    // if there is just one keyframe and we are right on it
    if (leftDistance + rightDistance == 0)
    {
        leftDistance = rightDistance = 0.5;
    }

    // alter interpolation position using the right neighbours progression
    // method
    const float interpolationPosition =
        right->alterInterpolationPosition(
            leftDistance / (leftDistance + rightDistance));

    // absolute application method
    if (d_applicationMethod == AM_Absolute)
    {
        const String result = d_interpolator->interpolateAbsolute(
                                  left->getValueForAnimation(instance),
                                  right->getValueForAnimation(instance),
                                  interpolationPosition);

        target->setProperty(d_targetProperty, result);
    }
    // relative application method
    else if (d_applicationMethod == AM_Relative)
    {
        const String& base = instance->getSavedPropertyValue(getTargetProperty());

        const String result = d_interpolator->interpolateRelative(
                                  base,
                                  left->getValueForAnimation(instance),
                                  right->getValueForAnimation(instance),
                                  interpolationPosition);

        target->setProperty(d_targetProperty, result);
    }
    // relative multiply application method
    else if (d_applicationMethod == AM_RelativeMultiply)
    {
        const String& base = instance->getSavedPropertyValue(getTargetProperty());

        const String result = d_interpolator->interpolateRelativeMultiply(
                                  base,
                                  left->getValueForAnimation(instance),
                                  right->getValueForAnimation(instance),
                                  interpolationPosition);

        target->setProperty(d_targetProperty, result);
    }
    // todo: more application methods?
    else
    {
        assert(0);
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

