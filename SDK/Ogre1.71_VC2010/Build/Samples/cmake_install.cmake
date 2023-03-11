# Install script for directory: W:/SDK/Ogre1.71_VC2010/Samples

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "W:/SDK/Ogre1.71_VC2010/Build/sdk")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE" TYPE FILE FILES
    "W:/SDK/Ogre1.71_VC2010/Samples/Common/include/ExampleApplication.h"
    "W:/SDK/Ogre1.71_VC2010/Samples/Common/include/ExampleFrameListener.h"
    "W:/SDK/Ogre1.71_VC2010/Samples/Common/include/ExampleLoadingBar.h"
    "W:/SDK/Ogre1.71_VC2010/Samples/Common/include/FileSystemLayer.h"
    "W:/SDK/Ogre1.71_VC2010/Samples/Common/include/OgreStaticPluginLoader.h"
    "W:/SDK/Ogre1.71_VC2010/Samples/Common/include/Sample.h"
    "W:/SDK/Ogre1.71_VC2010/Samples/Common/include/SampleContext.h"
    "W:/SDK/Ogre1.71_VC2010/Samples/Common/include/SamplePlugin.h"
    "W:/SDK/Ogre1.71_VC2010/Samples/Common/include/SdkCameraMan.h"
    "W:/SDK/Ogre1.71_VC2010/Samples/Common/include/SdkSample.h"
    "W:/SDK/Ogre1.71_VC2010/Samples/Common/include/SdkTrays.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/BezierPatch/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/CameraTrack/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Character/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Compositor/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/FacialAnimation/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Grass/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Instancing/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Lighting/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/OceanDemo/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/ParticleFX/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/ShaderSystem/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Shadows/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/SkeletalAnimation/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/SkyBox/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/SkyDome/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/SkyPlane/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Smoke/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/SphereMapping/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Terrain/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/TextureFX/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Transparency/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/VolumeTex/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Water/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/BSP/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/CelShading/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/DeferredShading/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/CubeMapping/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Dot3Bump/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/DynTex/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Fresnel/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Isosurf/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/ParticleGS/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/Samples/Browser/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

