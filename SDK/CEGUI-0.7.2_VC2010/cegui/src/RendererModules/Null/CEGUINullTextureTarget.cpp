/***********************************************************************
    filename:   CEGUINullTextureTarget.cpp
    created:    Sat Jan 16 2010
    author:     Eugene Marcotte
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
#include "CEGUINullTextureTarget.h"
#include "CEGUINullTexture.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const float NullTextureTarget::DEFAULT_SIZE = 128.0f;

//----------------------------------------------------------------------------//
NullTextureTarget::NullTextureTarget(NullRenderer& owner) :
    NullRenderTarget(owner),
    d_CEGUITexture(0)
{
    d_CEGUITexture = static_cast<NullTexture*>(&d_owner.createTexture());

    // setup area and cause the initial texture to be generated.
    declareRenderSize(Size(DEFAULT_SIZE, DEFAULT_SIZE));
}

//----------------------------------------------------------------------------//
NullTextureTarget::~NullTextureTarget()
{
    d_owner.destroyTexture(*d_CEGUITexture);
}

//----------------------------------------------------------------------------//
bool NullTextureTarget::isImageryCache() const
{
    return true;
}

//----------------------------------------------------------------------------//
void NullTextureTarget::clear()
{
}

//----------------------------------------------------------------------------//
Texture& NullTextureTarget::getTexture() const
{
    return *d_CEGUITexture;
}

//----------------------------------------------------------------------------//
void NullTextureTarget::declareRenderSize(const Size& sz)
{
	Rect r;
	r.setSize(sz);
	r.setPosition(Point(0, 0));
    setArea(r);
}

//----------------------------------------------------------------------------//
bool NullTextureTarget::isRenderingInverted() const
{
    return false;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
