# Install script for directory: W:/SDK/Ogre1.71_VC2010/OgreMain

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
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Release" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/Release/OgreMain.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Release" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/Release/OgreMain.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/RelWithDebInfo" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/RelWithDebInfo/OgreMain.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/RelWithDebInfo" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/RelWithDebInfo/OgreMain.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/MinSizeRel" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/MinSizeRel/OgreMain.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/MinSizeRel" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/MinSizeRel/OgreMain.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Debug" TYPE STATIC_LIBRARY OPTIONAL FILES "W:/SDK/Ogre1.71_VC2010/Build/lib/Debug/OgreMain_d.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Debug" TYPE SHARED_LIBRARY FILES "W:/SDK/Ogre1.71_VC2010/Build/bin/Debug/OgreMain_d.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE" TYPE FILE FILES
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/asm_math.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/Ogre.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreAlignedAllocator.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreAnimable.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreAnimation.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreAnimationState.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreAnimationTrack.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreAny.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreArchive.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreArchiveFactory.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreArchiveManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreAtomicWrappers.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreAutoParamDataSource.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreAxisAlignedBox.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreBillboard.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreBillboardChain.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreBillboardParticleRenderer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreBillboardSet.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreBitwise.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreBlendMode.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreBone.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreBorderPanelOverlayElement.h"
    "W:/SDK/Ogre1.71_VC2010/Build/include/OgreBuildSettings.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreCamera.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreCodec.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreColourValue.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreCommon.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreCompositionPass.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreCompositionTargetPass.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreCompositionTechnique.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreCompositor.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreCompositorChain.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreCompositorLogic.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreCompositorInstance.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreCompositorManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreConfig.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreConfigDialog.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreConfigFile.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreConfigOptionMap.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreController.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreControllerManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreConvexBody.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreCustomCompositionPass.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreDataStream.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreDefaultHardwareBufferManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreDistanceLodStrategy.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreDynLib.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreDynLibManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreEdgeListBuilder.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreEntity.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreErrorDialog.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreException.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreExternalTextureSource.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreExternalTextureSourceManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreFactoryObj.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreFileSystem.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreFont.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreFontManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreFrameListener.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreFrustum.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreGpuProgram.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreGpuProgramManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreGpuProgramParams.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreGpuProgramUsage.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreHardwareBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreHardwareBufferManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreHardwareIndexBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreHardwareOcclusionQuery.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreHardwarePixelBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreHardwareVertexBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreHeaderPrefix.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreHeaderSuffix.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreHighLevelGpuProgram.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreHighLevelGpuProgramManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreImage.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreImageCodec.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreInstancedGeometry.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreIteratorRange.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreIteratorWrapper.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreIteratorWrappers.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreKeyFrame.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreLight.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreLodListener.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreLodStrategy.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreLodStrategyManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreLog.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreLogManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreManualObject.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMaterial.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMaterialManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMaterialSerializer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMath.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMatrix3.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMatrix4.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMemoryAllocatedObject.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMemoryAllocatorConfig.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMemoryNedAlloc.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMemoryNedPooling.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMemoryStdAlloc.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMemorySTLAllocator.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMemoryTracker.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMesh.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMeshFileFormat.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMeshManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMeshSerializer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMeshSerializerImpl.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMovableObject.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreMovablePlane.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreNode.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreNumerics.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreOptimisedUtil.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreOverlay.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreOverlayContainer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreOverlayElement.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreOverlayElementCommands.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreOverlayElementFactory.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreOverlayManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePanelOverlayElement.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreParticle.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreParticleAffector.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreParticleAffectorFactory.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreParticleEmitter.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreParticleEmitterCommands.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreParticleEmitterFactory.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreParticleIterator.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreParticleScriptCompiler.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreParticleSystem.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreParticleSystemManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreParticleSystemRenderer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePass.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePatchMesh.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePatchSurface.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePixelCountLodStrategy.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePixelFormat.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePlane.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePlaneBoundedVolume.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePlatform.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePlatformInformation.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePlugin.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePolygon.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePose.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePredefinedControllers.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePrefabFactory.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePrerequisites.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreProfiler.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreProgressiveMesh.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreQuaternion.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRadixSort.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRay.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRectangle.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRectangle2D.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderable.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderObjectListener.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderOperation.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderQueue.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderQueueInvocation.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderQueueListener.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderQueueSortingGrouping.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderSystem.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderSystemCapabilities.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderSystemCapabilitiesManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderSystemCapabilitiesSerializer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderTarget.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderTargetListener.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderTexture.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderToVertexBuffer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRenderWindow.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreResource.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreResourceBackgroundQueue.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreResourceGroupManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreResourceManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRibbonTrail.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRoot.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreRotationalSpline.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSceneManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSceneManagerEnumerator.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSceneNode.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSceneQuery.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreScriptCompiler.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreScriptLexer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreScriptLoader.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreScriptParser.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreScriptTranslator.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSearchOps.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSerializer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreShadowCameraSetup.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreShadowCameraSetupFocused.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreShadowCameraSetupLiSPSM.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreShadowCameraSetupPlaneOptimal.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreShadowCameraSetupPSSM.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreShadowCaster.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreShadowTextureManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreShadowVolumeExtrudeProgram.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSharedPtr.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSimpleRenderable.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSimpleSpline.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSingleton.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSkeleton.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSkeletonFileFormat.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSkeletonInstance.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSkeletonManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSkeletonSerializer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSphere.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSpotShadowFadePng.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreStableHeaders.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreStaticFaceGroup.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreStaticGeometry.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreStdHeaders.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreStreamSerialiser.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreString.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreStringConverter.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreStringInterface.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreStringVector.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSubEntity.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreSubMesh.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreTagPoint.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreTangentSpaceCalc.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreTechnique.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreTextAreaOverlayElement.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreTexture.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreTextureManager.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreTextureUnitState.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreTimer.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreUnifiedHighLevelGpuProgram.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreUserObjectBindings.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreUTFString.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreVector2.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreVector3.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreVector4.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreVertexBoneAssignment.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreVertexIndexData.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreViewport.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreWindowEventUtilities.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreWireBoundingBox.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreWorkQueue.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreFreeImageCodec.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreDDSCodec.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgrePVRTCCodec.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/OgreZip.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/WIN32" TYPE FILE FILES
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/WIN32/OgreConfigDialogImp.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/WIN32/OgreErrorDialogImp.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/WIN32/OgreTimerImp.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/WIN32/OgreMinGWSupport.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/Threading" TYPE FILE FILES
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/Threading/OgreThreadDefines.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/Threading/OgreThreadHeaders.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/Threading/OgreDefaultWorkQueue.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/Threading/OgreThreadDefinesBoost.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/Threading/OgreThreadHeadersBoost.h"
    "W:/SDK/Ogre1.71_VC2010/OgreMain/include/Threading/OgreDefaultWorkQueueStandard.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

