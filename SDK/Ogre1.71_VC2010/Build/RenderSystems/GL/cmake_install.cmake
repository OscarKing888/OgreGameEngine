# Install script for directory: W:/SDK/Ogre1.71_VC2010/RenderSystems/GL

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
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Release/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/Release/RenderSystem_GL.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Release" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/Release/RenderSystem_GL.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/RelWithDebInfo/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/RelWithDebInfo/RenderSystem_GL.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/RelWithDebInfo" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/RelWithDebInfo/RenderSystem_GL.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/MinSizeRel/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/MinSizeRel/RenderSystem_GL.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/MinSizeRel" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/MinSizeRel/RenderSystem_GL.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Debug/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/Debug/RenderSystem_GL_d.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Debug" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/Debug/RenderSystem_GL_d.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/RenderSystems/GL" TYPE FILE FILES
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLATIFSInit.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLContext.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLDefaultHardwareBufferManager.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLFBOMultiRenderTarget.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLFBORenderTexture.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLFrameBufferObject.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLGpuNvparseProgram.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLGpuProgram.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLGpuProgramManager.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLHardwareBufferManager.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLHardwareIndexBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLHardwareOcclusionQuery.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLHardwarePixelBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLHardwareVertexBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLPBRenderTexture.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLPBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLPixelFormat.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLPlugin.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLPrerequisites.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLRenderSystem.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLRenderTexture.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLRenderToVertexBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLSupport.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLTexture.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreGLTextureManager.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/RenderSystems/GL" TYPE FILE FILES
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreWin32Context.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreWin32GLSupport.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreWin32Prerequisites.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreWin32RenderTexture.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/OgreWin32Window.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/src/win32/OgreGLUtil.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/RenderSystems/GL" TYPE DIRECTORY FILES "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/include/GL")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/RenderSystems/GL" TYPE DIRECTORY FILES "W:/SDK/Ogre1.71_VC2010/RenderSystems/GL/src/GLSL/include/")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

