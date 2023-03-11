/***********************************************************************
    filename:   CEGUIRenderedStringWidgetComponent.cpp
    created:    27/05/2009
    author:     Paul Turner
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIRenderedStringWidgetComponent.h"
#include "CEGUIWindowManager.h"
#include "CEGUIWindow.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
RenderedStringWidgetComponent::RenderedStringWidgetComponent() :
    d_window(0)
{
}

//----------------------------------------------------------------------------//
RenderedStringWidgetComponent::RenderedStringWidgetComponent(
        const String& widget_name) :
    d_window(WindowManager::getSingleton().getWindow(widget_name))
{
}

//----------------------------------------------------------------------------//
RenderedStringWidgetComponent::RenderedStringWidgetComponent(Window* widget) :
    d_window(widget)
{
}

//----------------------------------------------------------------------------//
void RenderedStringWidgetComponent::setWindow(const String& widget_name)
{
    d_window = WindowManager::getSingleton().getWindow(widget_name);
}

//----------------------------------------------------------------------------//
void RenderedStringWidgetComponent::setWindow(Window* widget)
{
    d_window = widget;
}

//----------------------------------------------------------------------------//
const Window* RenderedStringWidgetComponent::getWindow() const
{
    return d_window;
}

//----------------------------------------------------------------------------//
void RenderedStringWidgetComponent::draw(GeometryBuffer& /*buffer*/,
                                         const Vector2& position,
                                         const CEGUI::ColourRect* /*mod_colours*/,
                                         const Rect* /*clip_rect*/,
                                         const float vertical_space,
                                         const float /*space_extra*/) const
{
    if (!d_window)
        return;

    // HACK: re-adjust for inner-rect of parent
    float x_adj = 0, y_adj = 0;
    Window* parent = d_window->getParent();
    
    if (parent)
    {
        const CEGUI::Rect outer(parent->getUnclippedOuterRect());
        const CEGUI::Rect inner(parent->getUnclippedInnerRect());
        x_adj = inner.d_left - outer.d_left;
        y_adj = inner.d_top - outer.d_top;
    }
    // HACK: re-adjust for inner-rect of parent (Ends)

    Vector2 final_pos(position);
    // handle formatting options
    switch (d_verticalFormatting)
    {
    case VF_BOTTOM_ALIGNED:
        final_pos.d_y += vertical_space - getPixelSize().d_height;
        break;

    case VF_STRETCHED:
        Logger::getSingleton().logEvent("RenderedStringWidgetComponent::draw: "
            "VF_STRETCHED specified but is unsupported for Widget types; "
            "defaulting to VF_CENTRE_ALIGNED instead.");
        
        // intentional fall-through.
        
    case VF_CENTRE_ALIGNED:
        final_pos.d_y += (vertical_space - getPixelSize().d_height) / 2 ;
        break;


    case VF_TOP_ALIGNED:
        // nothing additional to do for this formatting option.
        break;

    default:
        CEGUI_THROW(InvalidRequestException("RenderedStringTextComponent::draw: "
                "unknown VerticalFormatting option specified."));
    }

    // we do not actually draw the widget, we just move it into position.
    const UVector2 wpos(UDim(0, final_pos.d_x + d_padding.d_left - x_adj),
                        UDim(0, final_pos.d_y + d_padding.d_top - y_adj));

    d_window->setPosition(wpos);
}

//----------------------------------------------------------------------------//
Size RenderedStringWidgetComponent::getPixelSize() const
{
    Size sz(0, 0);

    if (d_window)
    {
        sz = d_window->getPixelSize();
        sz.d_width += (d_padding.d_left + d_padding.d_right);
        sz.d_height += (d_padding.d_top + d_padding.d_bottom);
    }

    return sz;
}

//----------------------------------------------------------------------------//
bool RenderedStringWidgetComponent::canSplit() const
{
    return false;
}

//----------------------------------------------------------------------------//
RenderedStringWidgetComponent* RenderedStringWidgetComponent::split(
    float /*split_point*/, bool /*first_component*/)
{
    CEGUI_THROW(InvalidRequestException(
        "RenderedStringWidgetComponent::split: this "
        "component does not support being split."));
}

//----------------------------------------------------------------------------//
RenderedStringWidgetComponent* RenderedStringWidgetComponent::clone() const
{
    return new RenderedStringWidgetComponent(*this);
}

//----------------------------------------------------------------------------//
size_t RenderedStringWidgetComponent::getSpaceCount() const
{
    // widgets do not have spaces
    return 0;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
