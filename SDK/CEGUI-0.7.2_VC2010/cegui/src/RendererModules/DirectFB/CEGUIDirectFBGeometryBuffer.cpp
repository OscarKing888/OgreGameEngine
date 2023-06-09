/***********************************************************************
    filename:   CEGUIDirectFBGeometryBuffer.cpp
    created:    Tue Mar 10 2009
    author:     Paul D Turner (parts based on code by Keith Mok)
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
#include "CEGUIDirectFBGeometryBuffer.h"
#include "CEGUIDirectFBTexture.h"
#include "CEGUIRenderEffect.h"
#include "CEGUIVertex.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
DirectFBGeometryBuffer::DirectFBGeometryBuffer(DirectFBRenderer& owner) :
    d_owner(owner),
    d_activeTexture(0),
    d_translation(0, 0, 0),
    d_rotation(0, 0, 0),
    d_pivot(0, 0, 0),
    d_effect(0),
    d_matrixValid(false)
{
}

//----------------------------------------------------------------------------//
DirectFBGeometryBuffer::~DirectFBGeometryBuffer()
{
}

//----------------------------------------------------------------------------//
void DirectFBGeometryBuffer::draw() const
{
    IDirectFBSurface* target_surface = &d_owner.getTargetSurface();

    // setup clip region
    const DFBRegion clip_region = {
        static_cast<int>(d_clipRect.d_left),
        static_cast<int>(d_clipRect.d_top),
        static_cast<int>(d_clipRect.d_right),
        static_cast<int>(d_clipRect.d_bottom) };
    target_surface->SetClip(target_surface, &clip_region);

    // apply the transformations we need to use.
    if (!d_matrixValid)
        updateMatrix();

    target_surface->SetMatrix(target_surface, d_matrix);

    const int pass_count = d_effect ? d_effect->getPassCount() : 1;
    for (int pass = 0; pass < pass_count; ++pass)
    {
        // set up RenderEffect
        if (d_effect)
            d_effect->performPreRenderFunctions(pass);

        // draw the batches
        size_t pos = 0;
        BatchList::const_iterator i = d_batches.begin();
        for ( ; i != d_batches.end(); ++i)
        {
            target_surface->TextureTriangles(target_surface, (*i).first,
                                            &d_vertices[pos], 0, (*i).second,
                                            DTTF_LIST);
            pos += (*i).second;
        }
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();
}

//----------------------------------------------------------------------------//
void DirectFBGeometryBuffer::setTranslation(const Vector3& v)
{
    d_translation = v;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void DirectFBGeometryBuffer::setRotation(const Vector3& r)
{
    d_rotation = r;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void DirectFBGeometryBuffer::setPivot(const Vector3& p)
{
    d_pivot = p;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void DirectFBGeometryBuffer::setClippingRegion(const Rect& region)
{
    d_clipRect.d_top    = PixelAligned(region.d_top);
    d_clipRect.d_bottom = PixelAligned(region.d_bottom);
    d_clipRect.d_left   = PixelAligned(region.d_left);
    d_clipRect.d_right  = PixelAligned(region.d_right);
}

//----------------------------------------------------------------------------//
void DirectFBGeometryBuffer::appendVertex(const Vertex& vertex)
{
    appendGeometry(&vertex, 1);
}

//----------------------------------------------------------------------------//
void DirectFBGeometryBuffer::appendGeometry(const Vertex* const vbuff,
                                            uint vertex_count)
{
    IDirectFBSurface* t =
        d_activeTexture ? d_activeTexture->getDirectFBSurface() : 0;

    // create a new batch if there are no batches yet, or if the active texture
    // differs from that used by the current batch.
    if (d_batches.empty() || (t != d_batches.back().first))
        d_batches.push_back(BatchInfo(t, 0));

    // update size of current batch
    d_batches.back().second += vertex_count;

    // buffer these vertices
    DFBVertex vd;
    const Vertex* vs = vbuff;
    for (uint i = 0; i < vertex_count; ++i, ++vs)
    {
        // copy vertex info the buffer, converting from CEGUI::Vertex to
        // something directly usable by DirectFB as needed.
        vd.x       = vs->position.d_x;
        vd.y       = vs->position.d_y;
        vd.z       = vs->position.d_z;
        vd.w       = 1.0f;
        //vd.diffuse = vs->colour_val.getARGB();
        vd.s       = vs->tex_coords.d_x;
        vd.t       = vs->tex_coords.d_y;
        d_vertices.push_back(vd);
    }
}

//----------------------------------------------------------------------------//
void DirectFBGeometryBuffer::setActiveTexture(Texture* texture)
{
    d_activeTexture = static_cast<DirectFBTexture*>(texture);
}

//----------------------------------------------------------------------------//
void DirectFBGeometryBuffer::reset()
{
    d_batches.clear();
    d_vertices.clear();
    d_activeTexture = 0;
}

//----------------------------------------------------------------------------//
Texture* DirectFBGeometryBuffer::getActiveTexture() const
{
    return d_activeTexture;
}

//----------------------------------------------------------------------------//
uint DirectFBGeometryBuffer::getVertexCount() const
{
    return d_vertices.size();
}

//----------------------------------------------------------------------------//
uint DirectFBGeometryBuffer::getBatchCount() const
{
    return d_batches.size();
}

//----------------------------------------------------------------------------//
void DirectFBGeometryBuffer::setRenderEffect(RenderEffect* effect)
{
    d_effect = effect;
}

//----------------------------------------------------------------------------//
RenderEffect* DirectFBGeometryBuffer::getRenderEffect()
{
    return d_effect;
}

//----------------------------------------------------------------------------//
void DirectFBGeometryBuffer::updateMatrix() const
{
    // TODO:

    d_matrix[0] = 0x00010000;
    d_matrix[1] = 0x00000000;
    d_matrix[2] = 0x00000000;
    d_matrix[3] = 0x00000000;
    d_matrix[4] = 0x00010000;
    d_matrix[5] = 0x00000000;
    d_matrix[6] = 0x00000000;
    d_matrix[7] = 0x00000000;
    d_matrix[8] = 0x00010000;

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
