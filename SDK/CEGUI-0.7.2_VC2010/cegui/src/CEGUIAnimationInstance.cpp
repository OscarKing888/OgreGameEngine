/***********************************************************************
    filename:   CEGUIAnimationInstance.cpp
    created:    7/8/2010
    author:     Martin Preisler

    purpose:    Implements the AnimationInstance class
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
#include "CEGUIAnimationInstance.h"
#include "CEGUIAnimation.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindow.h"
#include "CEGUIAffector.h"

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
const String AnimationInstance::EventNamespace("AnimationInstance");

const String AnimationInstance::EventAnimationStarted("AnimationStarted");
const String AnimationInstance::EventAnimationStopped("AnimationStopped");
const String AnimationInstance::EventAnimationPaused("AnimationPaused");
const String AnimationInstance::EventAnimationUnpaused("AnimationUnpaused");
const String AnimationInstance::EventAnimationEnded("AnimationEnded");
const String AnimationInstance::EventAnimationLooped("AnimationLooped");

//----------------------------------------------------------------------------//
AnimationInstance::AnimationInstance(Animation* definition):
        d_definition(definition),

        d_target(0),
        d_eventReceiver(0),
        d_eventSender(0),

        d_position(0.0),
        d_speed(1.0),
        d_bounceBackwards(false),
        d_running(false)
{}

//----------------------------------------------------------------------------//
AnimationInstance::~AnimationInstance(void)
{
    if (d_eventSender)
    {
        d_definition->autoUnsubscribe(this);
    }
}

//----------------------------------------------------------------------------//
Animation* AnimationInstance::getDefinition() const
{
    return d_definition;
}

//----------------------------------------------------------------------------//
void AnimationInstance::setTarget(PropertySet* target)
{
    d_target = target;

    purgeSavedPropertyValues();

    if (d_definition->getAutoStart() && !isRunning())
    {
        start();
    }
}

//----------------------------------------------------------------------------//
PropertySet* AnimationInstance::getTarget() const
{
    return d_target;
}

//----------------------------------------------------------------------------//
void AnimationInstance::setEventReceiver(EventSet* receiver)
{
    d_eventReceiver = receiver;
}

//----------------------------------------------------------------------------//
EventSet* AnimationInstance::getEventReceiver() const
{
    return d_eventReceiver;
}

//----------------------------------------------------------------------------//
void AnimationInstance::setEventSender(EventSet* sender)
{
    if (d_eventSender)
    {
        d_definition->autoUnsubscribe(this);
    }

    d_eventSender = sender;

    if (d_eventSender)
    {
        d_definition->autoSubscribe(this);
    }
}

//----------------------------------------------------------------------------//
EventSet* AnimationInstance::getEventSender() const
{
    return d_eventSender;
}

//----------------------------------------------------------------------------//
void AnimationInstance::setTargetWindow(Window* target)
{
    setTarget(target);
    setEventReceiver(target);
    setEventSender(target);
}

//----------------------------------------------------------------------------//
void AnimationInstance::setPosition(float position)
{
    if (position < 0.0 || position > d_definition->getDuration())
    {
        CEGUI_THROW(InvalidRequestException(
                        "AnimationInstance::setPosition: Unable to set position "
                        "of this animation instace because given position isn't "
                        "in interval [0.0, duration of animation]."));
    }

    d_position = position;
}

//----------------------------------------------------------------------------//
float AnimationInstance::getPosition() const
{
    return d_position;
}

//----------------------------------------------------------------------------//
void AnimationInstance::setSpeed(float speed)
{
    // first sort out the adventurous users
    if (speed < 0.0f)
    {
        CEGUI_THROW(InvalidRequestException(
                        "AnimationInstance::setSpeed: You can't set playback speed "
                        "to a value that's lower than 0.0"));
    }

    if (speed == 0.0f)
    {
        CEGUI_THROW(InvalidRequestException(
                        "AnimationInstance::setSpeed: You can't set playback speed "
                        "to zero, please use AnimationInstance::pause instead"));
    }

    d_speed = speed;
}

//----------------------------------------------------------------------------//
float AnimationInstance::getSpeed() const
{
    return d_speed;
}

//----------------------------------------------------------------------------//
void AnimationInstance::start()
{
    setPosition(0.0);
    d_running = true;
    onAnimationStarted();
}

//----------------------------------------------------------------------------//
void AnimationInstance::stop()
{
    setPosition(0.0);
    d_running = false;
    onAnimationStopped();
}

//----------------------------------------------------------------------------//
void AnimationInstance::pause()
{
    d_running = false;
    onAnimationPaused();
}

//----------------------------------------------------------------------------//
void AnimationInstance::unpause()
{
    d_running = true;
    onAnimationUnpaused();
}

//----------------------------------------------------------------------------//
void AnimationInstance::togglePause()
{
    if (isRunning())
    {
        pause();
    }
    else
    {
        unpause();
    }
}

//----------------------------------------------------------------------------//
bool AnimationInstance::isRunning() const
{
    return d_running;
}

//----------------------------------------------------------------------------//
void AnimationInstance::step(float delta)
{
    if (!d_running)
    {
        // nothing to do if this animation instance isn't running
        return;
    }

    if (delta < 0.0f)
    {
        CEGUI_THROW(InvalidRequestException(
                        "AnimationInstance::step: You can't step the Animation Instance "
                        "with negative delta! You can't reverse the flow of time, stop "
                        "trying!"));
    }

    const float duration = d_definition->getDuration();

    // we modifty the delta according to playback speed
    delta *= d_speed;

    // the position could have gotten out of the desired range, we have to
    // alter it depending on replay method of our animation definition

    // first a simple clamp with RM_Once
    if (d_definition->getReplayMode() == Animation::RM_Once)
    {
        float newPosition = d_position + delta;

        newPosition = std::max(0.0f, newPosition);

        if (newPosition >= duration)
        {
            newPosition = duration;

            stop();
            onAnimationEnded();
        }

        setPosition(newPosition);
    }
    // a both sided wrap with RM_Loop
    else if (d_definition->getReplayMode() == Animation::RM_Loop)
    {
        float newPosition = d_position + delta;

        while (newPosition > duration)
        {
            newPosition -= duration;
            onAnimationLooped();
        }

        setPosition(newPosition);
    }
    // bounce back and forth with RM_Bounce
    else if (d_definition->getReplayMode() == Animation::RM_Bounce)
    {
        if (d_bounceBackwards)
        {
            delta = -delta;
        }

        float newPosition = d_position + delta;

        while (newPosition <= 0 || newPosition > duration)
        {
            if (newPosition <= 0)
            {
                d_bounceBackwards = false;

                newPosition = -newPosition;
                onAnimationLooped();
            }

            if (newPosition > duration)
            {
                d_bounceBackwards = true;

                newPosition = 2.0f * duration - newPosition;
                onAnimationLooped();
            }
        }

        setPosition(newPosition);
    }

    apply();
}

//----------------------------------------------------------------------------//
bool AnimationInstance::handleStart(const CEGUI::EventArgs& e)
{
    start();

    return true;
}

//----------------------------------------------------------------------------//
bool AnimationInstance::handleStop(const CEGUI::EventArgs& e)
{
    stop();

    return true;
}

//----------------------------------------------------------------------------//
bool AnimationInstance::handlePause(const CEGUI::EventArgs& e)
{
    pause();

    return true;
}

//----------------------------------------------------------------------------//
bool AnimationInstance::handleUnpause(const CEGUI::EventArgs& e)
{
    unpause();

    return true;
}

//----------------------------------------------------------------------------//
bool AnimationInstance::handleTogglePause(const CEGUI::EventArgs& e)
{
    togglePause();

    return true;
}

//----------------------------------------------------------------------------//
void AnimationInstance::savePropertyValue(const String& propertyName)
{
    assert(d_target);

    d_savedPropertyValues[propertyName] = d_target->getProperty(propertyName);
}

//----------------------------------------------------------------------------//
void AnimationInstance::purgeSavedPropertyValues(void)
{
    d_savedPropertyValues.clear();
}

//----------------------------------------------------------------------------//
const String& AnimationInstance::getSavedPropertyValue(const String& propertyName)
{
    PropertyValueMap::iterator it = d_savedPropertyValues.find(propertyName);

    if (it == d_savedPropertyValues.end())
    {
        // even though we explicitly save all used property values when
        // starting the animation, this can happen when user changes
        // animation definition whilst the animation is running
        // (Yes, it's nasty, but people do nasty things)
        savePropertyValue(propertyName);
        return getSavedPropertyValue(propertyName);
    }

    return it->second;
}

//----------------------------------------------------------------------------//
void AnimationInstance::addAutoConnection(Event::Connection conn)
{
    d_autoConnections.push_back(conn);
}

//----------------------------------------------------------------------------//
void AnimationInstance::unsubscribeAutoConnections()
{
    for (ConnectionTracker::iterator it = d_autoConnections.begin();
            it != d_autoConnections.end(); ++it)
    {
        (*it)->disconnect();
    }

    d_autoConnections.clear();
}

//----------------------------------------------------------------------------//
void AnimationInstance::apply()
{
    if (d_target)
    {
        d_definition->apply(this);
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationStarted()
{
    purgeSavedPropertyValues();
    d_definition->savePropertyValues(this);

    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationStarted, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationStopped()
{
    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationStopped, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationPaused()
{
    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationPaused, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationUnpaused()
{
    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationUnpaused, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationEnded()
{
    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationEnded, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationLooped()
{
    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationLooped, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

