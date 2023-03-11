/***********************************************************************
    filename:   CEGUISamplesConfig.h
    created:    Mon Sep 26 2005
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
//////////////////////////////////////////////////////////////////////////
/*************************************************************************

This file can be used to set certain configuration options which are used
when compiling the samples for Crazy Eddie's GUI System.

Each item in here has a comment to describe what it's for.

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

#ifndef _CEGUISamplesConfig_h_
#define _CEGUISamplesConfig_h_

// Bring in main config file
#include "CEGUIConfig.h"

//////////////////////////////////////////////////////////////////////////
// The following are intended for X-Code users.
// 
// The Linux build can both auto-configure itself, and also take
// configure parameters which control the settings presented below.
//
// The Premake solution will generate these for you according to the
// config.lua script.
//////////////////////////////////////////////////////////////////////////
#if defined(__APPLE__)

//////////////////////////////////////////////////////////////////////////
// Comment/Uncomment these lines to control renderer use in samples.
//////////////////////////////////////////////////////////////////////////
#define CEGUI_SAMPLES_USE_OPENGL

// If you want to enable the Irrlicht renderer in the samples:
// - In main project settings, set CEGUI_IRR_SDK_PATH to where your SDK is.
// - In main project settings, set CEGUI_IRR_SDK_VERSION to 12, 13 or 14
// - Build the CEGUIrrlichtRenderer framework(!) + add it to CEGUISampleHelper
//   'Linked libraries' in the General build settings. 
// - Add $(CEGUI_IRR_LINK_FLAGS) and $(CEGUI_IRR_SAMPLE_LINK_FLAGS) to the
//   'Other Linker Flags' in the CEGUISampleHelper target build settings.
// - Uncomment the following line and build.
//#define CEGUI_SAMPLES_USE_IRRLICHT

//#define CEGUI_SAMPLES_USE_OGRE

#endif // defined(__APPLE__)

#endif  // end of guard _CEGUISamplesConfig_h_
