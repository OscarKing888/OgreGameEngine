#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

######################################################################
# OGRE BUILD SYSTEM
# Welcome to the CMake build system for OGRE.
# This is the main file where we prepare the general build environment
# and provide build configuration options.
######################################################################

cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

# Use relative paths
# This is mostly to reduce path size for command-line limits on windows
if(WIN32)
  # This seems to break Xcode projects so definitely don't enable on Apple builds
  set(CMAKE_USE_RELATIVE_PATHS true)
  set(CMAKE_SUPPRESS_REGENERATION true)
endif()

# Assign compiler for Mac OS X-based systems
# NOTE: This logic is order-dependent and must occur here because the
# configuration phase of CMake projects will fall into an infinite loop
# if compiler assignments are modified after issuing a project(...) command.
#
if (OGRE_BUILD_PLATFORM_IPHONE)
  # Force gcc <= 4.2 on iPhone
  include(CMakeForceCompiler)
  CMAKE_FORCE_C_COMPILER(gcc-4.2 GNU)
  CMAKE_FORCE_CXX_COMPILER(gcc-4.2 GNU)
elseif (APPLE AND NOT OGRE_BUILD_PLATFORM_IPHONE)
  # Force gcc <= 4.0 on Mac OS X because 4.2 is not supported prior to Mac OS X 10.5
  include(CMakeForceCompiler)
  CMAKE_FORCE_C_COMPILER(gcc-4.0 GNU)
  CMAKE_FORCE_CXX_COMPILER(gcc-4.0 GNU)
endif ()

project(OGRE)

# Include necessary submodules
set(CMAKE_MODULE_PATH 
  "${OGRE_SOURCE_DIR}/CMake"
  "${OGRE_SOURCE_DIR}/CMake/Utils" 
  "${OGRE_SOURCE_DIR}/CMake/Packages"
)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)
include(MacroLogFeature)
include(OgreConfigTargets)
include(PreprocessorUtils)
set(OGRE_TEMPLATES_DIR "${OGRE_SOURCE_DIR}/CMake/Templates")
set(OGRE_WORK_DIR ${OGRE_BINARY_DIR})


#####################################################################
# Set up the basic build environment
#####################################################################

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

# determine Ogre version numbers
include(OgreGetVersion)
ogre_get_version(${OGRE_SOURCE_DIR}/OgreMain/include/OgrePrerequisites.h)
message(STATUS "Configuring OGRE ${OGRE_VERSION}")
# Configure version file for use by scripts
configure_file("${OGRE_TEMPLATES_DIR}/version.txt.in" "${OGRE_BINARY_DIR}/version.txt" @ONLY)

if (NOT APPLE)
  # Create debug libraries with _d postfix
  set(CMAKE_DEBUG_POSTFIX "_d")
endif ()

# Set compiler specific build flags
if (CMAKE_COMPILER_IS_GNUCXX)
  check_cxx_compiler_flag(-msse OGRE_GCC_HAS_SSE)
  if (OGRE_GCC_HAS_SSE)
    add_definitions(-msse)
  endif ()
endif ()
if (MSVC)
  if (CMAKE_BUILD_TOOL STREQUAL "nmake")
    # set variable to state that we are using nmake makefiles
	set(NMAKE TRUE)
  endif ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
  # Enable intrinsics on MSVC in debug mode
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Oi")
  if (CMAKE_CL_64)
    # Visual Studio bails out on debug builds in 64bit mode unless
	# this flag is set...
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /bigobj")
  endif ()
endif ()
if (MINGW)
  add_definitions(-D_WIN32_WINNT=0x0500)
endif ()

if (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
  # Test for GCC visibility
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-fvisibility=hidden OGRE_GCC_VISIBILITY)
  if (OGRE_GCC_VISIBILITY)
    # determine gcc version
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion 
      OUTPUT_VARIABLE OGRE_GCC_VERSION)
    message(STATUS "Detected g++ ${OGRE_GCC_VERSION}")
    message(STATUS "Enabling GCC visibility flags")
    set(OGRE_GCC_VISIBILITY_FLAGS "-DOGRE_GCC_VISIBILITY -fvisibility=hidden")
  
    # check if we can safely add -fvisibility-inlines-hidden
    string(TOLOWER "${CMAKE_BUILD_TYPE}" OGRE_BUILD_TYPE)
    if (OGRE_BUILD_TYPE STREQUAL "debug" AND OGRE_GCC_VERSION VERSION_LESS "4.2")
      message(STATUS "Skipping -fvisibility-inlines-hidden due to possible bug in g++ < 4.2")
    else ()
      set(OGRE_GCC_VISIBILITY_FLAGS "${OGRE_GCC_VISIBILITY_FLAGS} -fvisibility-inlines-hidden")
    endif ()
  endif (OGRE_GCC_VISIBILITY)
  
  # Fix x64 issues on Linux
  if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" AND NOT APPLE)
    add_definitions(-fPIC)
  endif()  
endif (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)

# determine system endianess
include(TestBigEndian)
test_big_endian(OGRE_TEST_BIG_ENDIAN)

# Add OgreMain include path
include_directories("${OGRE_SOURCE_DIR}/OgreMain/include")
include_directories("${OGRE_BINARY_DIR}/include")
if (APPLE)
  if (OGRE_BUILD_PLATFORM_IPHONE)
    include_directories("${OGRE_SOURCE_DIR}/OgreMain/include/iPhone")

    # Set static early for proper dependency detection
    set(OGRE_STATIC TRUE)
  else ()
    include_directories("${OGRE_SOURCE_DIR}/OgreMain/include/OSX")
  endif ()
endif (APPLE)

# Find dependencies
include(Dependencies)
# definitions for samples
set(OGRE_LIBRARIES OgreMain) 
set(OGRE_Paging_LIBRARIES OgrePaging)
set(OGRE_Terrain_LIBRARIES OgreTerrain)
set(OGRE_Plugin_PCZSceneManager_LIBRARIES Plugin_PCZSceneManager)
set(OGRE_Plugin_OctreeZone_LIBRARIES Plugin_OctreeZone)

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OGRE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OGRE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OGRE_BINARY_DIR}/bin")
if (WIN32 OR APPLE)
  if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    # We don't want to install in default system location, install is really for the SDK, so call it that
    set(CMAKE_INSTALL_PREFIX
	  "${OGRE_BINARY_DIR}/sdk" CACHE PATH "OGRE install prefix" FORCE
    )
  endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
endif(WIN32 OR APPLE)

# Set up iPhone overrides.
if (OGRE_BUILD_PLATFORM_IPHONE)
  include_directories("${OGRE_SOURCE_DIR}/OgreMain/include/iPhone")

  # Set build variables
  set(CMAKE_OSX_SYSROOT iphoneos3.0)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "")
  set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_UNIVERSAL_IPHONE_OS)")
  set(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework CoreGraphics -framework QuartzCore -framework UIKit")
  set(XCODE_ATTRIBUTE_SDKROOT iphoneos3.0)
  set(OGRE_BUILD_RENDERSYSTEM_GLES TRUE CACHE BOOL "Forcing OpenGL ES RenderSystem for iPhone" FORCE)
  set(OGRE_STATIC TRUE CACHE BOOL "Forcing static build for iPhone" FORCE)
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.\${PRODUCT_NAME:rfc1034identifier}")
  set(OGRE_CONFIG_ENABLE_VIEWPORT_ORIENTATIONMODE TRUE CACHE BOOL "Forcing viewport orientation support for iPhone" FORCE)

  add_definitions(-fno-regmove)
  remove_definitions(-msse)
elseif (APPLE AND NOT OGRE_BUILD_PLATFORM_IPHONE)

  # Set 10.4 as the base SDK by default
  set(XCODE_ATTRIBUTE_SDKROOT macosx10.4)

  if (NOT CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES "i386")
  endif()
  
  # 10.6 sets x86_64 as the default architecture.
  # Because Carbon isn't supported on 64-bit and we still need it, force the architectures to ppc and i386
  if(CMAKE_OSX_ARCHITECTURES MATCHES "x86_64" OR CMAKE_OSX_ARCHITECTURES MATCHES "ppc64")
    string(REPLACE "x86_64" "" CMAKE_OSX_ARCHITECTURES ${CMAKE_OSX_ARCHITECTURES})
    string(REPLACE "ppc64" "" CMAKE_OSX_ARCHITECTURES ${CMAKE_OSX_ARCHITECTURES})
  endif()

  # Make sure that the OpenGL render system is selected for non-iPhone Apple builds
  set(OGRE_BUILD_RENDERSYSTEM_GL TRUE)
  set(OGRE_BUILD_RENDERSYSTEM_GLES FALSE)
endif ()

# Enable the PVRTC codec if OpenGL ES is being built
if(OGRE_BUILD_RENDERSYSTEM_GLES)
  set(OGRE_CONFIG_ENABLE_PVRTC TRUE CACHE BOOL "Forcing PVRTC codec for OpenGL ES" FORCE)
endif()

######################################################################
# Provide user options to customise the build process
######################################################################

# Customise what to build
option(OGRE_STATIC "Static build" FALSE)
cmake_dependent_option(OGRE_BUILD_RENDERSYSTEM_D3D9 "Build Direct3D9 RenderSystem" TRUE "WIN32;DirectX_FOUND" FALSE)
cmake_dependent_option(OGRE_BUILD_RENDERSYSTEM_D3D10 "Build Direct3D10 RenderSystem [EXPERIMENTAL]" FALSE "WIN32;DirectX_D3D10_FOUND" FALSE)
cmake_dependent_option(OGRE_BUILD_RENDERSYSTEM_D3D11 "Build Direct3D11 RenderSystem [EXPERIMENTAL]" FALSE "WIN32;DirectX_D3D11_FOUND" FALSE)
cmake_dependent_option(OGRE_BUILD_RENDERSYSTEM_GL "Build OpenGL RenderSystem" TRUE "OPENGL_FOUND;NOT OGRE_BUILD_PLATFORM_IPHONE" FALSE)
cmake_dependent_option(OGRE_BUILD_RENDERSYSTEM_GLES "Build OpenGL ES RenderSystem" FALSE "OPENGLES_FOUND" FALSE)
cmake_dependent_option(OGRE_BUILD_PLATFORM_IPHONE "Build Ogre for iPhone OS" FALSE "iPhoneSDK_FOUND;OPENGLES_FOUND" FALSE)
option(OGRE_BUILD_PLUGIN_BSP "Build BSP SceneManager plugin" TRUE)
option(OGRE_BUILD_PLUGIN_OCTREE "Build Octree SceneManager plugin" TRUE)
option(OGRE_BUILD_PLUGIN_PCZ "Build PCZ SceneManager plugin" TRUE)
option(OGRE_BUILD_PLUGIN_PFX "Build ParticleFX plugin" TRUE)
option(OGRE_BUILD_COMPONENT_PAGING "Build Paging component" TRUE)
option(OGRE_BUILD_COMPONENT_TERRAIN "Build Terrain component" TRUE)
cmake_dependent_option(OGRE_BUILD_COMPONENT_PROPERTY "Build Property component" TRUE "Boost_FOUND" FALSE)
cmake_dependent_option(OGRE_BUILD_PLUGIN_CG "Build Cg plugin" TRUE "Cg_FOUND;NOT OGRE_BUILD_PLATFORM_IPHONE" FALSE)
option(OGRE_BUILD_COMPONENT_RTSHADERSYSTEM "Build RTShader System component" TRUE)
cmake_dependent_option(OGRE_BUILD_RTSHADERSYSTEM_CORE_SHADERS "Build RTShader System FFP core shaders" TRUE "OGRE_BUILD_COMPONENT_RTSHADERSYSTEM" FALSE)
cmake_dependent_option(OGRE_BUILD_RTSHADERSYSTEM_EXT_SHADERS "Build RTShader System extensions shaders" TRUE "OGRE_BUILD_COMPONENT_RTSHADERSYSTEM" FALSE)

option(OGRE_BUILD_SAMPLES "Build Ogre demos" TRUE)
cmake_dependent_option(OGRE_BUILD_TOOLS "Build the command-line tools" TRUE "NOT OGRE_BUILD_PLATFORM_IPHONE" FALSE)
option(OGRE_BUILD_TESTS "Build the unit tests & PlayPen" FALSE)
option(OGRE_CONFIG_DOUBLE "Use doubles instead of floats in Ogre" FALSE)
set(OGRE_CONFIG_ALLOCATOR 4 CACHE STRING
"Specify the memory allocator to use. Possible values:
  1 - Standard allocator
  2 - nedmalloc
  3 - User-provided allocator
  4 - nedmalloc with pooling"
)
option(OGRE_CONFIG_CONTAINERS_USE_CUSTOM_ALLOCATOR "STL containers in Ogre use the custom allocator" TRUE)
option(OGRE_CONFIG_STRING_USE_CUSTOM_ALLOCATOR "Ogre String uses the custom allocator" FALSE)
option(OGRE_CONFIG_MEMTRACK_DEBUG "Enable Ogre's memory tracker in debug mode" FALSE)
option(OGRE_CONFIG_MEMTRACK_RELEASE "Enable Ogre's memory tracker in release mode" FALSE)
# determine threading options
include(PrepareThreadingOptions)
cmake_dependent_option(OGRE_CONFIG_ENABLE_FREEIMAGE "Build FreeImage codec. If you disable this option, you need to provide your own image handling codecs." TRUE "FreeImage_FOUND" FALSE)
option(OGRE_CONFIG_ENABLE_DDS "Build DDS codec." TRUE)
option(OGRE_CONFIG_ENABLE_PVRTC "Build PVRTC codec." FALSE)
cmake_dependent_option(OGRE_CONFIG_ENABLE_ZIP "Build ZIP archive support. If you disable this option, you cannot use ZIP archives resource locations. The samples won't work." TRUE "ZZip_FOUND" FALSE)
option(OGRE_CONFIG_ENABLE_VIEWPORT_ORIENTATIONMODE "Include Viewport orientation mode support." FALSE)
option(OGRE_CONFIG_NEW_COMPILERS "Use the new script compilers." TRUE)
cmake_dependent_option(OGRE_USE_BOOST "Use Boost extensions" TRUE "Boost_FOUND" FALSE)
# Customise what to install
option(OGRE_INSTALL_SAMPLES "Install Ogre demos." FALSE)
option(OGRE_INSTALL_TOOLS "Install Ogre tools." TRUE)
option(OGRE_INSTALL_DOCS "Install documentation." FALSE)
option(OGRE_INSTALL_SAMPLES_SOURCE "Install samples source files." FALSE)
cmake_dependent_option(OGRE_INSTALL_PDB "Install debug pdb files" FALSE "MSVC" FALSE)
cmake_dependent_option(OGRE_FULL_RPATH "Build executables with the full required RPATH to run from their install location." FALSE "NOT WIN32" FALSE)
option(OGRE_PROFILING "Enable internal profiling support." FALSE)
cmake_dependent_option(OGRE_CONFIG_STATIC_LINK_CRT "Statically link the MS CRT dlls (msvcrt)" FALSE "MSVC" FALSE)
set(OGRE_LIB_DIRECTORY "lib" CACHE STRING "Install path for libraries, e.g. 'lib64' on some 64-bit Linux distros.")

# hide advanced options
mark_as_advanced(
  OGRE_BUILD_RTSHADERSYSTEM_CORE_SHADERS 
  OGRE_BUILD_RTSHADERSYSTEM_EXT_SHADERS
  OGRE_CONFIG_DOUBLE
  OGRE_CONFIG_ALLOCATOR
  OGRE_CONFIG_CONTAINERS_USE_CUSTOM_ALLOCATOR
  OGRE_CONFIG_STRING_USE_CUSTOM_ALLOCATOR
  OGRE_CONFIG_MEMTRACK_DEBUG
  OGRE_CONFIG_MEMTRACK_RELEASE
  OGRE_CONFIG_NEW_COMPILERS
  OGRE_CONFIG_ENABLE_DDS
  OGRE_CONFIG_ENABLE_FREEIMAGE
  OGRE_CONFIG_ENABLE_PVRTC
  OGRE_CONFIG_ENABLE_VIEWPORT_ORIENTATIONMODE
  OGRE_CONFIG_ENABLE_ZIP
  OGRE_USE_BOOST
  OGRE_INSTALL_SAMPLES_SOURCE
  OGRE_FULL_RPATH
  OGRE_PROFILING
  OGRE_CONFIG_STATIC_LINK_CRT
  OGRE_LIB_DIRECTORY
)

###################################################################
# configure global build settings based on selected build options
###################################################################
include(ConfigureBuild)


##################################################################
# Now setup targets
##################################################################

# install resource files
include(InstallResources)

# Setup OgreMain project
add_subdirectory(OgreMain)

# Setup RenderSystems
add_subdirectory(RenderSystems)

# Setup Plugins
add_subdirectory(PlugIns)

# Setup Components
add_subdirectory(Components)

# Setup tests (before samples so that PlayPen is included in browser)
if (OGRE_BUILD_TESTS)
  add_subdirectory(Tests)
endif ()

# Setup samples
add_subdirectory(Samples) 

# Setup command-line tools
if (OGRE_BUILD_TOOLS)
  add_subdirectory(Tools)
endif ()

# Install documentation
add_subdirectory(Docs)

# Install media files
if (OGRE_INSTALL_SAMPLES OR OGRE_INSTALL_SAMPLES_SOURCE)
  add_subdirectory(Samples/Media)
endif ()

# Install CMake modules
add_subdirectory(CMake)

# Provide CPack packaging target
include(Packaging)


# Show feature summary
include(FeatureSummary)

