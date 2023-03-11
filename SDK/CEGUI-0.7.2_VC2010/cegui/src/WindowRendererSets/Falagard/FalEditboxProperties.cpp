/***********************************************************************
    filename:   FalEditboxProperties.cpp
    created:    Wed Jul 29 2009
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "FalEditboxProperties.h"
#include "FalEditbox.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{

namespace FalagardEditboxProperties
{
//----------------------------------------------------------------------------//
String BlinkCaret::get(const PropertyReceiver* receiver) const
{
    FalagardEditbox* wr = static_cast<FalagardEditbox*>(
        static_cast<const Window*>(receiver)->getWindowRenderer());

    return PropertyHelper::boolToString(wr->isCaretBlinkEnabled());
}

//----------------------------------------------------------------------------//
void BlinkCaret::set(PropertyReceiver* receiver, const String& value)
{
    FalagardEditbox* wr = static_cast<FalagardEditbox*>(
        static_cast<const Window*>(receiver)->getWindowRenderer());

    wr->setCaretBlinkEnabled(PropertyHelper::stringToBool(value));
}

//----------------------------------------------------------------------------//
String BlinkCaretTimeout::get(const PropertyReceiver* receiver) const
{
    FalagardEditbox* wr = static_cast<FalagardEditbox*>(
        static_cast<const Window*>(receiver)->getWindowRenderer());

    return PropertyHelper::floatToString(wr->getCaretBlinkTimeout());
}

//----------------------------------------------------------------------------//
void BlinkCaretTimeout::set(PropertyReceiver* receiver, const String& value)
{
    FalagardEditbox* wr = static_cast<FalagardEditbox*>(
        static_cast<const Window*>(receiver)->getWindowRenderer());

    wr->setCaretBlinkTimeout(PropertyHelper::stringToFloat(value));
}

} // End of FalagardEditboxProperties namespace section

} // End of  CEGUI namespace section
