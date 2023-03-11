/***********************************************************************
    filename:   CEGUIWRFactoryRegisterer.cpp
    created:    Thu Mar 19 2009
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
#include "CEGUIWRFactoryRegisterer.h"
#include "CEGUIWindowRendererManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
WRFactoryRegisterer::WRFactoryRegisterer(const CEGUI::utf8* type) :
    d_type(type)
{}

//----------------------------------------------------------------------------//
WRFactoryRegisterer::~WRFactoryRegisterer()
{}

//----------------------------------------------------------------------------//
void WRFactoryRegisterer::registerFactory()
{
    if (CEGUI::WindowRendererManager::getSingleton().isFactoryPresent(d_type))
        CEGUI::Logger::getSingleton().logEvent(
            "Factory for '" + CEGUI::String(d_type) +
            "' appears to be  already registered, skipping.",
            CEGUI::Informative);
    else
        this->doFactoryAdd();
}

//----------------------------------------------------------------------------//
void WRFactoryRegisterer::unregisterFactory()
{
    WindowRendererManager::getSingleton().removeFactory(d_type);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
