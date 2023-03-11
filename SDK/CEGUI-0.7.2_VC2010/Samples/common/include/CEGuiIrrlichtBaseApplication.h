/***********************************************************************
    filename:   CEGuiIrrlichtBaseApplication.h
    created:    24/9/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGuiIrrlichtBaseApplication_h_
#define _CEGuiIrrlichtBaseApplication_h_

#if defined(_WIN32)
#  pragma comment(lib, "irrlicht.lib")
#if !defined(CEGUI_STATIC)
#  if defined(_DEBUG)
#      pragma comment(lib, "CEGUIIrrlichtRenderer_d.lib")
#  else
#      pragma comment(lib, "CEGUIIrrlichtRenderer.lib")
#  endif
#else
# define _IRR_STATIC_LIB_ //Define this regardless if we are using a dll or not
# if defined(_DEBUG)
#  pragma comment(lib, "CEGUIIrrlichtRenderer_Static_d.lib")
# else
#  pragma comment(lib, "CEGUIIrrlichtRenderer_Static.lib")
# endif
#endif
#endif

#include "CEGuiBaseApplication.h"
#include "RendererModules/Irrlicht/CEGUIIrrlichtRenderer.h"
#include "CEGUI.h"

#include <irrlicht.h>

class CEGuiIrrlichtBaseApplication : public CEGuiBaseApplication,
                                     public irr::IEventReceiver
{
public:
    CEGuiIrrlichtBaseApplication();
    ~CEGuiIrrlichtBaseApplication();

    // implementation of base class abstract methods.
    bool    execute(CEGuiSample* sampleApp);
    void    cleanup();

    // irrlicht event listener
#if CEGUI_IRR_SDK_VERSION >= 14
    bool OnEvent(const irr::SEvent& event);
#else
    bool OnEvent(irr::SEvent event);
#endif

    //! handler for rendering the CEGUI fps & logo geometry
    bool overlayHandler(const CEGUI::EventArgs& args);

protected:
    /// member to check and handle resizing of the display window.
    void checkWindowResize();

    irr::IrrlichtDevice*       d_device;
    irr::video::IVideoDriver*  d_driver;
    irr::scene::ISceneManager* d_smgr;
    irr::u32                   d_lastTime;
    CEGUI::IrrlichtRenderer*   d_renderer;

    int d_fps_value;
    char d_fps_textbuff[16];
    CEGUI::GeometryBuffer* d_fps_geometry;
    CEGUI::GeometryBuffer* d_logo_geometry;

    //! size of display last time a change was detected.
    CEGUI::Size d_lastDisplaySize;
};


#endif  // end of guard _CEGuiIrrlichtBaseApplication_h_
