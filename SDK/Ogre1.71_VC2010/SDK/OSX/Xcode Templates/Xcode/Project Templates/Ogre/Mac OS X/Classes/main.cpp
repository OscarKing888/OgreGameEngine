//|||||||||||||||||||||||||||||||||||||||||||||||

#include "DemoApp.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

#ifdef OGRE_STATIC_LIB
#  define OGRE_STATIC_GL
#  if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    define OGRE_STATIC_Direct3D9
// dx10 will only work on vista, so be careful about statically linking
#    if OGRE_USE_D3D10
#      define OGRE_STATIC_Direct3D10
#    endif
#  endif
#  define OGRE_STATIC_BSPSceneManager
#  define OGRE_STATIC_ParticleFX
#  define OGRE_STATIC_CgProgramManager
#  ifdef OGRE_USE_PCZ
#    define OGRE_STATIC_PCZSceneManager
#    define OGRE_STATIC_OctreeZone
#  else
#    define OGRE_STATIC_OctreeSceneManager
#  endif
#  if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
#     undef OGRE_STATIC_CgProgramManager
#     undef OGRE_STATIC_GL
#     define OGRE_STATIC_GLES 1
#     ifdef __OBJC__
#       import <UIKit/UIKit.h>
#     endif
#  endif
#  include "OgreStaticPluginLoader.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
#include "macUtils.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
#include <UIKit/UIKit.h>
#endif

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, @"UIApplication", @"AppDelegate");
    [pool release];
    return retVal;
#else
	try
    {
		DemoApp demo;
		demo.startDemo();
    }
	catch(Ogre::Exception& e)
    {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
#endif
    }

    return 0;
#endif
}

//|||||||||||||||||||||||||||||||||||||||||||||||

#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
#   ifdef __OBJC__
@interface AppDelegate : NSObject <UIApplicationDelegate>
{
    DemoApp demo;
    NSTimer *mTimer;
	double timeSinceLastFrame;
	double startTime;
}

- (void)go;
- (void)renderOneFrame:(id)sender;

@property (retain) NSTimer *mTimer;

@end

@implementation AppDelegate

@synthesize mTimer;

- (void)go {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    try {
        timeSinceLastFrame = 0;
        startTime = 0;

        new OgreFramework();
        if(!OgreFramework::getSingletonPtr()->initOgre("DemoApp v1.0", &demo, 0))
            return;
        
        demo.setShutdown(false);

        OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");

        demo.setupDemoScene();
        OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();

        mTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1.0f / 60.0f)
                                          target:self
                                        selector:@selector(renderOneFrame:)
                                        userInfo:nil
                                         repeats:YES];
    } catch( Ogre::Exception& e ) {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }

    [pool release];
}

- (void)renderOneFrame:(id)sender
{
#pragma unused(sender)
    if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
    {
        startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();
                
        OgreFramework::getSingletonPtr()->m_pMouse->capture();

        OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
        OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
    
        timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
    }
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {
#pragma unused(application)
    // Hide the status bar
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
    
    // Create a window
    UIWindow *window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    
    // Create an image view
    UIImageView *imageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"Default.png"]];
    [window addSubview:imageView];
    
    // Create an indeterminate status indicator
    UIActivityIndicatorView *indicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
    [indicator setFrame:CGRectMake(150, 280, 20, 20)];
    [indicator startAnimating];
    [window addSubview:indicator];
    
    // Display our window
    [window makeKeyAndVisible];
    
    // Clean up
    [imageView release];
    [indicator release];
    
    [self go];
    
    [window release];
}

- (void)applicationWillTerminate:(UIApplication *)application {
#pragma unused(application)
    Ogre::Root::getSingleton().queueEndRendering();
}

//- (void)applicationWillResignActive:(UIApplication *)application
//{
//    // Pause FrameListeners and rendering
//}
//
//- (void)applicationDidBecomeActive:(UIApplication *)application
//{
//    // Resume FrameListeners and rendering
//}

- (void)dealloc {
    [super dealloc];
}

@end
#   endif
#endif
