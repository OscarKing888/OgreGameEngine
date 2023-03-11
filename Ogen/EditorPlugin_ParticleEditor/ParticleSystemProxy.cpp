#include "StdAfx.h"
#include "ParticleSystemProxy.h"
#if 0
#include <OgreWireBoundingBox.h>

ParticleSystemProxy::ParticleSystemProxy(void)
: _particleSys(0)
, _node(0)
, _boundingBox(0)
, _enableUpdate(true)
{
}

ParticleSystemProxy::~ParticleSystemProxy(void)
{
	if(_node)
	{
		OGRE_DELETE _node;
		_node = 0;
	}

	if(_boundingBox)
	{
		OGRE_DELETE _boundingBox;
		_boundingBox = 0;
	}
}

const String& ParticleSystemProxy::getMovableType( void ) const
{
	static String typeName = "ParticleSystemProxy";
	return typeName;
}

const AxisAlignedBox& ParticleSystemProxy::getBoundingBox( void ) const
{
	if(_particleSys)
	{
		return _particleSys->getBoundingBox();
	}

	return AxisAlignedBox::BOX_NULL;
}

Ogre::Real ParticleSystemProxy::getBoundingRadius( void ) const
{
	if(_particleSys)
	{
		return _particleSys->getBoundingRadius();
	}

	return 0.5;
}

void ParticleSystemProxy::_updateRenderQueue( RenderQueue* queue )
{
	if(_particleSys)
	{
		_particleSys->_updateRenderQueue(queue);
	}

	if(!_boundingBox)
	{
		_boundingBox = OGRE_NEW Ogre::WireBoundingBox();
	}

	_boundingBox->setupBoundingBox(_node->_getWorldAABB());
	queue->addRenderable(_boundingBox);
}

void ParticleSystemProxy::visitRenderables( Renderable::Visitor* visitor, bool debugRenderables /*= false*/ )
{
	if(_particleSys)
	{
		_particleSys->visitRenderables(visitor, debugRenderables);
	}
}

void ParticleSystemProxy::update( Real deltaTime )
{
	if(_particleSys && _enableUpdate)
	{
		_particleSys->_update(deltaTime);
	}
}

void ParticleSystemProxy::_notifyAttached( Node* parent, bool isTagPoint /*= false*/ )
{
	MovableObject::_notifyAttached(parent, isTagPoint);
	if(!_node && parent)
	{
		SceneNode* n = dynamic_cast<SceneNode*>(parent);
		_node = OGRE_NEW SceneNode(n->getCreator());
	}

	if(parent)
	{
		_node->setPosition(parent->_getDerivedPosition());
		_node->setScale(parent->_getDerivedScale());
		_node->setOrientation(parent->_getDerivedOrientation());
	}

	if(_particleSys && _node)
	{
		_node->attachObject(_particleSys);
	}
}

void ParticleSystemProxy::setParticleSys( Ogre::ParticleSystem* val )
{
	if(_node)
	{
		_node->detachAllObjects();
	}

	_particleSys = val;

	if(_node && _particleSys)
	{
		_node->attachObject(_particleSys);
	}
}

void ParticleSystemProxy::_notifyCurrentCamera( Camera* cam )
{
	MovableObject::_notifyCurrentCamera(cam);
	if(_particleSys)
	{
		_particleSys->_notifyCurrentCamera(cam);
	}
}

void ParticleSystemProxy::setElapsedTime( Ogre::Real time )
{
	_particleSys->clear();
	_particleSys->fastForward(time);
}

#endif