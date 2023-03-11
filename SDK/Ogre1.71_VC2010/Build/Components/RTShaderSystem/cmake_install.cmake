# Install script for directory: W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem

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
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Release" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/Release/OgreRTShaderSystem.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Release" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/Release/OgreRTShaderSystem.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/RelWithDebInfo" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/RelWithDebInfo/OgreRTShaderSystem.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/RelWithDebInfo" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/RelWithDebInfo/OgreRTShaderSystem.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/MinSizeRel" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/MinSizeRel/OgreRTShaderSystem.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/MinSizeRel" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/MinSizeRel/OgreRTShaderSystem.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Debug" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/Debug/OgreRTShaderSystem_d.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Debug" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/Debug/OgreRTShaderSystem_d.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/RTShaderSystem" TYPE FILE FILES
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreRTShaderSystem.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderPrerequisites.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderFFPColour.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderFFPFog.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderFFPLighting.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderFFPRenderState.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderFFPRenderStateBuilder.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderFFPTexturing.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderFFPTransform.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderFunction.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderFunctionAtom.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderGenerator.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderParameter.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderProgram.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderProgramManager.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderProgramSet.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderProgramWriter.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderRenderState.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderSubRenderState.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderExNormalMapLighting.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderExPerPixelLighting.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderExIntegratedPSSM3.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderScriptTranslator.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderMaterialSerializerListener.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderProgramProcessor.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderCGProgramProcessor.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderCGProgramWriter.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderGLSLProgramProcessor.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderGLSLProgramWriter.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderProgramWriterManager.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderHLSLProgramProcessor.h"
    "W:/SDK/Ogre1.71_VC2010/Components/RTShaderSystem/include/OgreShaderHLSLProgramWriter.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

