# Install script for directory: W:/SDK/Ogre1.71_VC2010/PlugIns

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

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/PlugIns/OctreeSceneManager/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/PlugIns/BSPSceneManager/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/PlugIns/CgProgramManager/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/PlugIns/ParticleFX/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/PlugIns/PCZSceneManager/cmake_install.cmake")
  INCLUDE("W:/SDK/Ogre1.71_VC2010/Build/PlugIns/OctreeZone/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

