# Install script for directory: W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11

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
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Release/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/Release/RenderSystem_Direct3D11.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Release" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/Release/RenderSystem_Direct3D11.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/RelWithDebInfo/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/RelWithDebInfo/RenderSystem_Direct3D11.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/RelWithDebInfo" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/RelWithDebInfo/RenderSystem_Direct3D11.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/MinSizeRel/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/MinSizeRel/RenderSystem_Direct3D11.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/MinSizeRel" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/MinSizeRel/RenderSystem_Direct3D11.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Debug/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/Debug/RenderSystem_Direct3D11_d.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Debug" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/Debug/RenderSystem_Direct3D11_d.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/RenderSystems/Direct3D11" TYPE FILE FILES
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11Device.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11Driver.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11DriverList.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11GpuProgram.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11GpuProgramManager.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11HardwareBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11HardwareBufferManager.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11HardwareIndexBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11HardwareOcclusionQuery.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11HardwarePixelBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11HardwareVertexBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11HLSLProgram.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11HLSLProgramFactory.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11Mappings.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11MultiRenderTarget.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11Plugin.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11Prerequisites.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11RenderSystem.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11RenderToVertexBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11RenderWindow.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11Texture.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11TextureManager.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11VertexDeclaration.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11VideoMode.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreD3D11VideoModeList.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreFixedFuncEmuShaderGenerator.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreFixedFuncEmuShaderManager.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreFixedFuncState.h"
    "W:/SDK/Ogre1.71_VC2010/RenderSystems/Direct3D11/include/OgreHlslFixedFuncEmuShaderGenerator.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

