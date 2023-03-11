#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

#######################################################################
# This file takes care of configuring Ogre to build with the settings
# given in CMake. It creates the necessary config.h file and will 
# also prepare package files for pkg-config and CMake.
#######################################################################

string(TOLOWER "${CMAKE_BUILD_TYPE}" OGRE_BUILD_TYPE)
if (OGRE_BUILD_TYPE STREQUAL "debug")
	add_definitions(-DDEBUG)
endif()

if (OGRE_BUILD_PLATFORM_IPHONE)
  set(OGRE_SET_BUILD_PLATFORM_IPHONE 1)
  set(OGRE_STATIC TRUE)
  set(OGRE_STATIC_LIB TRUE)
  set(OGRE_CONFIG_ENABLE_VIEWPORT_ORIENTATIONMODE TRUE)
  set(OGRE_CONFIG_ENABLE_PVRTC TRUE)
endif()

# should we build static libs?
if (OGRE_STATIC)
  set(OGRE_LIB_TYPE STATIC)
else ()
  set(OGRE_LIB_TYPE SHARED)
endif ()

# configure threading options
set(OGRE_THREAD_PROVIDER 0)
if (OGRE_CONFIG_THREADS)
	if (UNIX)
		add_definitions(-pthread)
	endif ()

	if (OGRE_CONFIG_THREAD_PROVIDER STREQUAL "boost")
		set(OGRE_THREAD_PROVIDER 1)
		include_directories(${Boost_INCLUDE_DIRS})
		# On MSVC Boost usually tries to autolink boost libraries. However since
		# this behaviour is not available on all compilers, we need to find the libraries
		# ourselves, anyway. Disable auto-linking to avoid mess-ups.
		add_definitions(-DBOOST_ALL_NO_LIB)
		set(OGRE_THREAD_LIBRARIES ${Boost_LIBRARIES})
	endif ()

	if (OGRE_CONFIG_THREAD_PROVIDER STREQUAL "poco")
		set(OGRE_THREAD_PROVIDER 2)
		include_directories(${POCO_INCLUDE_DIRS})
		set(OGRE_THREAD_LIBRARIES ${POCO_LIBRARIES})
	endif ()

	if (OGRE_CONFIG_THREAD_PROVIDER STREQUAL "tbb")
		set(OGRE_THREAD_PROVIDER 3)
		include_directories(${TBB_INCLUDE_DIRS})
		set(OGRE_THREAD_LIBRARIES ${TBB_LIBRARIES})
	endif ()
endif()


# determine config values depending on build options 
set(OGRE_SET_DOUBLE 0)
set(OGRE_SET_ALLOCATOR ${OGRE_CONFIG_ALLOCATOR})
set(OGRE_SET_CONTAINERS_USE_ALLOCATOR 0)
set(OGRE_SET_STRING_USE_ALLOCATOR 0)
set(OGRE_SET_MEMTRACK_DEBUG 0)
set(OGRE_SET_MEMTRACK_RELEASE 0)
set(OGRE_SET_THREADS ${OGRE_CONFIG_THREADS})
set(OGRE_SET_THREAD_PROVIDER ${OGRE_THREAD_PROVIDER})
set(OGRE_SET_DISABLE_FREEIMAGE 0)
set(OGRE_SET_DISABLE_DDS 0)
set(OGRE_SET_DISABLE_PVRTC 0)
set(OGRE_SET_DISABLE_ZIP 0)
set(OGRE_SET_DISABLE_VIEWPORT_ORIENTATIONMODE 0)
set(OGRE_SET_NEW_COMPILERS 0)
set(OGRE_STATIC_LIB 0)
set(OGRE_SET_USE_BOOST 0)
set(OGRE_SET_PROFILING 0)
if (OGRE_CONFIG_DOUBLE)
  set(OGRE_SET_DOUBLE 1)
endif()
if (OGRE_CONFIG_CONTAINERS_USE_CUSTOM_ALLOCATOR)
  set(OGRE_SET_CONTAINERS_USE_ALLOCATOR 1)
endif ()
if (OGRE_CONFIG_STRING_USE_CUSTOM_ALLOCATOR)
  set(OGRE_SET_STRING_USE_ALLOCATOR 1)
endif ()
if (OGRE_CONFIG_MEMTRACK_DEBUG)
  set(OGRE_SET_MEMTRACK_DEBUG 1)
endif()
if (OGRE_CONFIG_MEMTRACK_RELEASE)
  set(OGRE_SET_MEMTRACK_RELEASE 1)
endif()
if (NOT OGRE_CONFIG_ENABLE_FREEIMAGE)
  set(OGRE_SET_DISABLE_FREEIMAGE 1)
endif()
if (NOT OGRE_CONFIG_ENABLE_DDS)
  set(OGRE_SET_DISABLE_DDS 1)
endif()
if (NOT OGRE_CONFIG_ENABLE_PVRTC)
  set(OGRE_SET_DISABLE_PVRTC 1)
endif()
if (NOT OGRE_CONFIG_ENABLE_ZIP)
  set(OGRE_SET_DISABLE_ZIP 1)
endif()
if (NOT OGRE_CONFIG_ENABLE_VIEWPORT_ORIENTATIONMODE)
  set(OGRE_SET_DISABLE_VIEWPORT_ORIENTATIONMODE 1)
endif()
if(OGRE_CONFIG_NEW_COMPILERS)
  set(OGRE_SET_NEW_COMPILERS 1)
endif()
if (OGRE_STATIC)
  set(OGRE_STATIC_LIB 1)
endif()
if (OGRE_USE_BOOST)
  set(OGRE_SET_USE_BOOST 1)
endif()
if (OGRE_PROFILING)
  set(OGRE_SET_PROFILING 1)
endif()

if (OGRE_TEST_BIG_ENDIAN)
  set(OGRE_CONFIG_BIG_ENDIAN 1)
else ()
  set(OGRE_CONFIG_LITTLE_ENDIAN 1)
endif ()

if (OGRE_BUILD_RTSHADERSYSTEM_CORE_SHADERS)
	set(RTSHADER_SYSTEM_BUILD_CORE_SHADERS 1)
else ()
	set(RTSHADER_SYSTEM_BUILD_CORE_SHADERS 0)
endif ()

if (OGRE_BUILD_RTSHADERSYSTEM_EXT_SHADERS)	
	set(RTSHADER_SYSTEM_BUILD_EXT_SHADERS 1)
else ()
	set(RTSHADER_SYSTEM_BUILD_EXT_SHADERS 0)
endif ()

# generate OgreBuildSettings.h 
configure_file(${OGRE_TEMPLATES_DIR}/OgreBuildSettings.h.in ${OGRE_BINARY_DIR}/include/OgreBuildSettings.h @ONLY)
install(FILES ${OGRE_BINARY_DIR}/include/OgreBuildSettings.h DESTINATION include/OGRE)


# Create the pkg-config package files on Unix systems
if (UNIX)
  set(OGRE_LIB_SUFFIX "")
  set(OGRE_PLUGIN_PREFIX "")
  set(OGRE_PLUGIN_EXT ".so")
  set(OGRE_PAGING_ADDITIONAL_PACKAGES "")
  if (OGRE_STATIC)
    set(OGRE_LIB_SUFFIX "${OGRE_LIB_SUFFIX}Static")
    set(OGRE_PLUGIN_PREFIX "lib")
    set(OGRE_PLUGIN_EXT ".a")
  endif ()
  if (OGRE_BUILD_TYPE STREQUAL "debug")
    set(OGRE_LIB_SUFFIX "${OGRE_LIB_SUFFIX}_d")
  endif ()

  set(OGRE_ADDITIONAL_LIBS "")
  set(OGRE_CFLAGS "")
  set(OGRE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})
  if (OGRE_CONFIG_THREADS GREATER 0)
    set(OGRE_CFLAGS "-pthread")
    set(OGRE_ADDITIONAL_LIBS "${OGRE_ADDITIONAL_LIBS} -lpthread")
  endif ()
  if (OGRE_STATIC)
    if (OGRE_CONFIG_THREADS)
      set(OGRE_ADDITIONAL_LIBS "${OGRE_ADDITIONAL_LIBS} -lboost-thread-mt")
    endif ()
    # there is no pkgconfig file for freeimage, so we need to add that lib manually
    set(OGRE_ADDITIONAL_LIBS "${OGRE_ADDITIONAL_LIBS} -lfreeimage")
    configure_file(${OGRE_TEMPLATES_DIR}/OGREStatic.pc.in ${OGRE_BINARY_DIR}/pkgconfig/OGRE.pc @ONLY)
  else ()
    configure_file(${OGRE_TEMPLATES_DIR}/OGRE.pc.in ${OGRE_BINARY_DIR}/pkgconfig/OGRE.pc @ONLY)
  endif ()
  install(FILES ${OGRE_BINARY_DIR}/pkgconfig/OGRE.pc DESTINATION ${OGRE_LIB_DIRECTORY}/pkgconfig)

  # configure additional packages
  
  if (OGRE_BUILD_PLUGIN_PCZ)
    configure_file(${OGRE_TEMPLATES_DIR}/OGRE-PCZ.pc.in ${OGRE_BINARY_DIR}/pkgconfig/OGRE-PCZ.pc @ONLY)
    install(FILES ${OGRE_BINARY_DIR}/pkgconfig/OGRE-PCZ.pc DESTINATION ${OGRE_LIB_DIRECTORY}/pkgconfig)
  endif ()
  
  if (OGRE_BUILD_COMPONENT_PAGING)
    configure_file(${OGRE_TEMPLATES_DIR}/OGRE-Paging.pc.in ${OGRE_BINARY_DIR}/pkgconfig/OGRE-Paging.pc @ONLY)
    install(FILES ${OGRE_BINARY_DIR}/pkgconfig/OGRE-Paging.pc DESTINATION ${OGRE_LIB_DIRECTORY}/pkgconfig)
  endif ()

  if (OGRE_BUILD_COMPONENT_TERRAIN)
    if (OGRE_BUILD_COMPONENT_PAGING)
      set(OGRE_PAGING_ADDITIONAL_PACKAGES ", OGRE-Paging = ${OGRE_VERSION}")
    endif ()
    configure_file(${OGRE_TEMPLATES_DIR}/OGRE-Terrain.pc.in ${OGRE_BINARY_DIR}/pkgconfig/OGRE-Terrain.pc @ONLY)
    install(FILES ${OGRE_BINARY_DIR}/pkgconfig/OGRE-Terrain.pc DESTINATION ${OGRE_LIB_DIRECTORY}/pkgconfig)
  endif ()

  if (OGRE_BUILD_COMPONENT_RTSHADERSYSTEM)
    configure_file(${OGRE_TEMPLATES_DIR}/OGRE-RTShaderSystem.pc.in ${OGRE_BINARY_DIR}/pkgconfig/OGRE-RTShaderSystem.pc @ONLY)
    install(FILES ${OGRE_BINARY_DIR}/pkgconfig/OGRE-RTShaderSystem.pc DESTINATION ${OGRE_LIB_DIRECTORY}/pkgconfig)
  endif ()

  if (OGRE_BUILD_COMPONENT_PROPERTY)
    configure_file(${OGRE_TEMPLATES_DIR}/OGRE-Property.pc.in ${OGRE_BINARY_DIR}/pkgconfig/OGRE-Property.pc @ONLY)
    install(FILES ${OGRE_BINARY_DIR}/pkgconfig/OGRE-Property.pc DESTINATION ${OGRE_LIB_DIRECTORY}/pkgconfig)
  endif ()

endif ()

if(OGRE_CONFIG_STATIC_LINK_CRT)
#We statically link to reduce dependancies
foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
    if(${flag_var} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MDd")
endforeach(flag_var)
endif(OGRE_CONFIG_STATIC_LINK_CRT)

### Commented because the FindOGRE script can currently fill this role better ###
# # Create the CMake package files
# if (WIN32)
#   set(OGRE_CMAKE_DIR CMake)
# elseif (UNIX)
#   set(OGRE_CMAKE_DIR lib/cmake)
# elseif (APPLE)
# endif ()
# configure_file(${OGRE_TEMPLATES_DIR}/OGREConfig.cmake.in ${OGRE_BINARY_DIR}/cmake/OGREConfig.cmake @ONLY)
# configure_file(${OGRE_TEMPLATES_DIR}/OGREConfigVersion.cmake.in ${OGRE_BINARY_DIR}/cmake/OGREConfigVersion.cmake @ONLY)
# install(FILES
#   ${OGRE_BINARY_DIR}/cmake/OGREConfig.cmake
#   ${OGRE_BINARY_DIR}/cmake/OGREConfigVersion.cmake
#   DESTINATION ${OGRE_CMAKE_DIR}
# )
# 
