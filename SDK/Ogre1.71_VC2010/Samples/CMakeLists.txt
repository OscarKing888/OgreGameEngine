# Configure Samples build

if (OGRE_BUILD_SAMPLES AND NOT OIS_FOUND)
  message(STATUS "Could not find dependency: OIS")
  message(STATUS "Skipping samples build")
  set(OGRE_BUILD_SAMPLES 0)
endif()
if (APPLE AND NOT OGRE_BUILD_PLATFORM_IPHONE AND OGRE_BUILD_SAMPLES AND NOT IOKit_FOUND)
  message(STATUS "Could not find dependency: IOKit")
  message(STATUS "Skipping samples build")
  set(OGRE_BUILD_SAMPLES 0)
endif()

if (OGRE_BUILD_SAMPLES)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common/include)

  # Make sure all plugins are built
  if (OGRE_BUILD_PLUGIN_OCTREE)
	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} Plugin_OctreeSceneManager)
  endif ()
  if (OGRE_BUILD_PLUGIN_BSP)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} Plugin_BSPSceneManager)
  endif ()
  if (OGRE_BUILD_PLUGIN_CG)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} Plugin_CgProgramManager)
  endif (OGRE_BUILD_PLUGIN_CG)
  if (OGRE_BUILD_PLUGIN_PFX)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} Plugin_ParticleFX)
  endif ()
  
  if (OGRE_BUILD_PLUGIN_PCZ)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} Plugin_PCZSceneManager)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} Plugin_OctreeZone)
  endif ()
  
  if (OGRE_BUILD_COMPONENT_PAGING)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} OgrePaging)
  endif ()
  
  if (OGRE_BUILD_COMPONENT_RTSHADERSYSTEM)
	add_definitions(-DUSE_RTSHADER_SYSTEM)
	ogre_add_component_include_dir(RTShaderSystem)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} OgreRTShaderSystem)
  endif ()
  
  if (OGRE_BUILD_COMPONENT_TERRAIN)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} OgreTerrain)
  endif ()

  if (OGRE_BUILD_COMPONENT_PAGING)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} OgrePaging)
  endif ()

  if (OGRE_BUILD_RENDERSYSTEM_D3D9)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} RenderSystem_Direct3D9)
  endif ()
  if (OGRE_BUILD_RENDERSYSTEM_D3D10)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} RenderSystem_Direct3D10)
  endif ()
  if (OGRE_BUILD_RENDERSYSTEM_D3D11)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} RenderSystem_Direct3D11)
  endif ()
  if (OGRE_BUILD_RENDERSYSTEM_GL)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} RenderSystem_GL)
  endif ()
  if (OGRE_BUILD_RENDERSYSTEM_GLES)
  	set(SAMPLE_DEPENDENCIES ${SAMPLE_DEPENDENCIES} RenderSystem_GLES)
  endif ()
  if (APPLE)
  	set(OGRE_LIBRARIES ${OGRE_LIBRARIES} IOKit)
  endif ()

  if (OGRE_STATIC)
  	# Static linking means we need to directly use plugins
  	include_directories(${OGRE_SOURCE_DIR}/PlugIns/BSPSceneManager/include)
  	include_directories(${OGRE_SOURCE_DIR}/PlugIns/CgProgramManager/include)
  	include_directories(${OGRE_SOURCE_DIR}/PlugIns/OctreeSceneManager/include)
  	include_directories(${OGRE_SOURCE_DIR}/PlugIns/OctreeZone/include)
  	include_directories(${OGRE_SOURCE_DIR}/PlugIns/ParticleFX/include)
  	include_directories(${OGRE_SOURCE_DIR}/PlugIns/PCZSceneManager/include)
  	include_directories(${OGRE_SOURCE_DIR}/RenderSystems/Direct3D9/include)
  	include_directories(${OGRE_SOURCE_DIR}/RenderSystems/Direct3D10/include)
  	include_directories(${OGRE_SOURCE_DIR}/RenderSystems/Direct3D11/include)
  	include_directories(${OGRE_SOURCE_DIR}/RenderSystems/GLES/include)
  	include_directories(
		${OGRE_SOURCE_DIR}/RenderSystems/GL/include
		${OGRE_SOURCE_DIR}/RenderSystems/GL/src/GLSL/include
		${OGRE_SOURCE_DIR}/RenderSystems/GL/src/atifs/include
		${OGRE_SOURCE_DIR}/RenderSystems/GL/src/nvparse
		)

	# Link to all enabled plugins
	set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${SAMPLE_DEPENDENCIES})

	# Need to include resource files so that icons are linked
	if (WIN32)
		set(RESOURCE_FILES
    		${OGRE_SOURCE_DIR}/OgreMain/src/WIN32/resource.h
	    	${OGRE_SOURCE_DIR}/OgreMain/src/WIN32/OgreWin32Resources.rc
    		${OGRE_SOURCE_DIR}/OgreMain/src/WIN32/winres.h
		)
		source_group(Resources FILES ${RESOURCE_FILES})
	endif ()
  else ()
	# Link to RT Shader System if enabled.
	if (OGRE_BUILD_COMPONENT_RTSHADERSYSTEM)	
  		set(OGRE_LIBRARIES ${OGRE_LIBRARIES} OgreRTShaderSystem)
	endif () 
  endif ()
  
  add_subdirectory(BezierPatch)
  add_subdirectory(CameraTrack)
  add_subdirectory(Character)
  add_subdirectory(Compositor)
  add_subdirectory(FacialAnimation)
  add_subdirectory(Grass)
  add_subdirectory(Instancing)
  add_subdirectory(Lighting)
  add_subdirectory(OceanDemo)
  add_subdirectory(ParticleFX)
  if (OGRE_BUILD_COMPONENT_RTSHADERSYSTEM)
    add_subdirectory(ShaderSystem)
  endif ()
  add_subdirectory(Shadows)
  add_subdirectory(SkeletalAnimation)
  add_subdirectory(SkyBox)
  add_subdirectory(SkyDome)
  add_subdirectory(SkyPlane)
  add_subdirectory(Smoke)
  add_subdirectory(SphereMapping)
if (OGRE_BUILD_COMPONENT_TERRAIN)
  add_subdirectory(Terrain)
endif ()
  add_subdirectory(TextureFX)
  add_subdirectory(Transparency)
  add_subdirectory(VolumeTex)
  add_subdirectory(Water)

if (NOT OGRE_BUILD_PLATFORM_IPHONE)
  add_subdirectory(BSP)
  add_subdirectory(CelShading)
  add_subdirectory(DeferredShading)
  add_subdirectory(CubeMapping)
  add_subdirectory(Dot3Bump)
  add_subdirectory(DynTex)
  add_subdirectory(Fresnel)
  add_subdirectory(Isosurf)
  add_subdirectory(ParticleGS)
endif ()

  # Add browser last
  add_subdirectory(Browser)
endif ()


  
# Install sample sources
if (OGRE_INSTALL_SAMPLES_SOURCE)
  if (WIN32 OR APPLE)
    set(OGRE_SAMPLES_DIR Samples)
  elseif (UNIX)
    set(OGRE_SAMPLES_DIR share/OGRE/Samples)
  endif ()
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${OGRE_SAMPLES_DIR}
    REGEX "^CMakeLists.txt$" EXCLUDE
    PATTERN "Makefile.am" EXCLUDE
    PATTERN "Media" EXCLUDE
    PATTERN "bin" EXCLUDE
    PATTERN "setup" EXCLUDE
    PATTERN ".svn" EXCLUDE
	PATTERN "obj" EXCLUDE
	PATTERN "scripts" EXCLUDE
  )
  # install a new CMakeLists.txt file to allow building of samples
  configure_file(${OGRE_TEMPLATES_DIR}/SDK_CMakeLists.txt.in ${CMAKE_CURRENT_BINARY_DIR}/../CMakeLists.txt @ONLY)
  configure_file(${OGRE_TEMPLATES_DIR}/SDK_Samples_CMakeLists.txt.in ${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/../CMakeLists.txt DESTINATION ${OGRE_SAMPLES_DIR}/../)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt DESTINATION ${OGRE_SAMPLES_DIR})
endif ()

# Install sample headers, some people rely on these
file(GLOB SAMPLE_COMMON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Common/include/*.h")
install(FILES ${SAMPLE_COMMON_HEADERS}
		DESTINATION include/OGRE)
		
if (MSVC)
  find_package(Wix)
  if (Wix_FOUND)
    # Create WiX setup for demo build
    configure_file(${OGRE_TEMPLATES_DIR}/demos.wxs.in ${CMAKE_CURRENT_BINARY_DIR}/demos.wxs @ONLY)
    configure_file(${OGRE_TEMPLATES_DIR}/demomedia.wxi.in ${CMAKE_CURRENT_BINARY_DIR}/demomedia.wxi @ONLY)
    configure_file(${OGRE_TEMPLATES_DIR}/DemoLicense.rtf ${CMAKE_CURRENT_BINARY_DIR}/DemoLicense.rtf COPYONLY)
    if (MSVC_VERSION EQUAL 1400)
      configure_file(${OGRE_TEMPLATES_DIR}/vcrt_vc8.wxi.in ${CMAKE_CURRENT_BINARY_DIR}/vcrt.wxi @ONLY)
    elseif(MSVC_VERSION EQUAL 1500)
      configure_file(${OGRE_TEMPLATES_DIR}/vcrt_vc9.wxi.in ${CMAKE_CURRENT_BINARY_DIR}/vcrt.wxi @ONLY)
    endif()
	# Configure files, set media dir temporarily
	set(OGRE_MEDIA_DIR_TMP ${OGRE_MEDIA_DIR_REL})
	set(OGRE_MEDIA_DIR_REL "Media")
    configure_file(${OGRE_TEMPLATES_DIR}/resources.cfg.in ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg @ONLY)
    configure_file(${OGRE_TEMPLATES_DIR}/quakemap.cfg.in ${CMAKE_CURRENT_BINARY_DIR}/quakemap.cfg @ONLY)
	# restore
	set(OGRE_MEDIA_DIR_REL ${OGRE_MEDIA_DIR_TMP})
    add_custom_target(demo_installer 
	  COMMAND ${Wix_BINARY_DIR}/candle demos.wxs 
	  COMMAND ${Wix_BINARY_DIR}/light -ext WixUIExtension -cultures:en-us -out OgreDemos_v${OGRE_VERSION_DASH_SEPARATED}.msi demos.wixobj
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Building demo installer" VERBATIM
    )
	# Make sure we build samples first
	add_dependencies(demo_installer SampleBrowser)
  endif()
endif()
