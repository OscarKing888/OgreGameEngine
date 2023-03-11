# Install script for directory: W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX

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
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Release/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/Release/Plugin_ParticleFX.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Release" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/Release/Plugin_ParticleFX.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/RelWithDebInfo/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/RelWithDebInfo/Plugin_ParticleFX.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/RelWithDebInfo" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/RelWithDebInfo/Plugin_ParticleFX.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/MinSizeRel/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/MinSizeRel/Plugin_ParticleFX.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/MinSizeRel" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/MinSizeRel/Plugin_ParticleFX.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Debug/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/Debug/Plugin_ParticleFX_d.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Debug" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/Debug/Plugin_ParticleFX_d.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/Plugins/ParticleFX" TYPE FILE FILES
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreAreaEmitter.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreBoxEmitterFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreBoxEmitter.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreColourFaderAffector2.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreColourFaderAffectorFactory2.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreColourFaderAffectorFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreColourFaderAffector.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreColourImageAffectorFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreColourImageAffector.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreColourInterpolatorAffectorFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreColourInterpolatorAffector.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreCylinderEmitterFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreCylinderEmitter.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreDeflectorPlaneAffectorFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreDeflectorPlaneAffector.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreDirectionRandomiserAffectorFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreDirectionRandomiserAffector.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreEllipsoidEmitterFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreEllipsoidEmitter.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreHollowEllipsoidEmitterFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreHollowEllipsoidEmitter.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreLinearForceAffectorFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreLinearForceAffector.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreParticleFXPlugin.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreParticleFXPrerequisites.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgrePointEmitterFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgrePointEmitter.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreRingEmitterFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreRingEmitter.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreRotationAffectorFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreRotationAffector.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreScaleAffectorFactory.h"
    "W:/SDK/Ogre1.71_VC2010/PlugIns/ParticleFX/include/OgreScaleAffector.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

