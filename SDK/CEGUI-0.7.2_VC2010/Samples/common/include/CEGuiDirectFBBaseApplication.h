/***********************************************************************
    filename:   CEGuiDirectFBBaseApplication.h
    created:    Sun Oct 4 2008
    author:     Keith Mok
*************************************************************************/
/***************************************************************************
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
#ifndef _CEGuiDirectFBBaseApplication_h_
#define _CEGuiDirectFBBaseApplication_h_

#include "CEGuiBaseApplication.h"
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>
#include <directfb.h>

// forward declare struct that will hold DirectFB specifics
struct CEGuiBaseApplicationImpl;

//! CEGuiBaseApplication implementation for DirectFB
class CEGuiDirectFBBaseApplication : public CEGuiBaseApplication
{
public:
    //!Constructor.
    CEGuiDirectFBBaseApplication();

    //! Destructor.
    ~CEGuiDirectFBBaseApplication();

    // Implementation of base class abstract methods.
    bool execute(CEGuiSample* sampleApp);
    void cleanup();

protected:
    /*************************************************************************
        Implementation Methods
    *************************************************************************/
    //! Initialises DirectFB system.
    bool initialiseDirectFB(unsigned int width, unsigned int height,
                            bool windowed);

    void handleKeyDownEvent(DFBWindowEvent *evt);
    void handleKeyUpEvent(DFBWindowEvent *evt);
    void handleWindowEvent(DFBWindowEvent *evt);

    //! Perform update on the FPS counter fields.
    void updateFPS(void);

    //! cleanup DirectFB objects / interfaces
    void cleanupDirectFB();

    /*************************************************************************
        Data fields
    *************************************************************************/
    //! Pointer to the struct holding DirectFB specific fields.
    CEGuiBaseApplicationImpl* pimpl;;

    // FPS stuff
    int d_frames;
    int d_FPS;
    // counter used to track elapsed time (for time pulse injection)
    struct timeval d_lastTime;
};

#endif  // end of guard _CEGuiDirectFBBaseApplication_h_
