// Plugin_OgreEx.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Plugin_OgreEx.h"

#include "OgreMovementAffector.h"
#include "OgreScaleInterpolatorAffector.h"
#include "OgreRevolutionAffector.h"
#include "OgreColourFadingAffector.h"
#include "OgreMeshRotationAffector.h"
#include "OgreMeshAnimationAffector.h"

#include "OgrePolarEmitter.h"

#include "OgreTexCoordBillboardParticleRenderer.h"
#include "OgreMeshParticleRenderer.h"
#include "OgreRibbonRenderer.h"

#include "OgreProjector.h"

namespace Ogre 
{
	OgreEXPlugin* plugin = 0;
	const String sPluginName = "OgreEx";

//-----------------------------------------------------------------------
extern "C" void _OgreExExport dllStartPlugin(void) throw()
{
	plugin = OGRE_NEW OgreEXPlugin();
	Root::getSingleton().installPlugin(plugin);
}

//-----------------------------------------------------------------------
extern "C" void _OgreExExport dllStopPlugin(void)
{
	Root::getSingleton().uninstallPlugin(plugin);
	OGRE_DELETE plugin;
}

//---------------------------------------------------------------------
OgreEXPlugin::OgreEXPlugin()
{

}
//---------------------------------------------------------------------
const String& OgreEXPlugin::getName() const
{
	return sPluginName;
}
//---------------------------------------------------------------------
void OgreEXPlugin::install()
{
	// -------------------------------------------
	// Eimitter
	// -------------------------------------------
	ParticleEmitterFactory* pEmitFact = 0;

	//
	// PolarEmitter
	//
	pEmitFact = OGRE_NEW PolarEmitterFactory();
	ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
	mEmitterFactories.push_back(pEmitFact);



	// -------------------------------------------
	// Affector
	// -------------------------------------------
	ParticleAffectorFactory* pAffFact = 0;

	//
	// ScaleInterpolatorAffector
	//
	pAffFact = OGRE_NEW ScaleInterpolatorAffectorFactory();
	ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
	mAffectorFactories.push_back(pAffFact);

	//
	// RevolutionAffectorFactory
	//
	pAffFact = OGRE_NEW RevolutionAffectorFactory();
	ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
	mAffectorFactories.push_back(pAffFact);

	//
	// ColourFadingAffectorFactory
	//
	pAffFact = OGRE_NEW ColourFadingAffectorFactory();
	ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
	mAffectorFactories.push_back(pAffFact);

	//
	// MovementAffectorFactory
	//
	pAffFact = OGRE_NEW MovementAffectorFactory();
	ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
	mAffectorFactories.push_back(pAffFact);

	//
	// MeshRotationAffectorFactory
	//
	pAffFact = OGRE_NEW MeshRotationAffectorFactory();
	ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
	mAffectorFactories.push_back(pAffFact);

	//
	// MeshAnimationAffectorFactory
	//
	pAffFact = OGRE_NEW MeshAnimationAffectorFactory();
	ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
	mAffectorFactories.push_back(pAffFact);



	// -------------------------------------------
	// Renderer
	// -------------------------------------------
	ParticleSystemRendererFactory* pRenderFact = 0;

	//
	// TexCoordBillboardParticleRendererFactory
	//
	pRenderFact = OGRE_NEW TexCoordBillboardParticleRendererFactory();
	ParticleSystemManager::getSingleton().addRendererFactory(pRenderFact);
	mRendererFactories.push_back(pRenderFact);

	//
	// MeshParticleRendererFactory
	//
	pRenderFact = OGRE_NEW MeshParticleRendererFactory();
	ParticleSystemManager::getSingleton().addRendererFactory(pRenderFact);
	mRendererFactories.push_back(pRenderFact);

	//
	// RibbonRendererFactory
	//
	pRenderFact = OGRE_NEW RibbonRendererFactory();
	ParticleSystemManager::getSingleton().addRendererFactory(pRenderFact);
	mRendererFactories.push_back(pRenderFact);

	// -------------------------------------------
	// MovableObject
	// -------------------------------------------
	MovableObjectFactory* pMoFact = 0;

	//
	// ProjectorFactory
	//
	pMoFact = OGRE_NEW ProjectorFactory();
	Root::getSingleton().addMovableObjectFactory(pMoFact, true);
	mMovableObjectFactoryList.push_back(pMoFact);
}
//---------------------------------------------------------------------
void OgreEXPlugin::initialise()
{
	// nothing to do
}
//---------------------------------------------------------------------
void OgreEXPlugin::shutdown()
{
	// nothing to do
}
//---------------------------------------------------------------------
void OgreEXPlugin::uninstall()
{
	// destroy 
	vector<ParticleEmitterFactory*>::type::iterator ei;
	for (ei = mEmitterFactories.begin(); ei != mEmitterFactories.end(); ++ei)
	{
		OGRE_DELETE (*ei);
	}

	vector<ParticleAffectorFactory*>::type::iterator ai;
	for (ai = mAffectorFactories.begin(); ai != mAffectorFactories.end(); ++ai)
	{
		OGRE_DELETE (*ai);
	}

	vector<ParticleSystemRendererFactory*>::type::iterator ri;
	for (ri = mRendererFactories.begin(); ri != mRendererFactories.end(); ++ri)
	{		
		OGRE_DELETE (*ri);
	}

	vector<Ogre::MovableObjectFactory*>::type::iterator mi;
	for (mi = mMovableObjectFactoryList.begin(); mi != mMovableObjectFactoryList.end(); ++mi)
	{
		Root::getSingleton().removeMovableObjectFactory(*mi);
		OGRE_DELETE (*mi);
	}
}

}