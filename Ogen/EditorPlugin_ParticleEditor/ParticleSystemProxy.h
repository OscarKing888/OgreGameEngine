#pragma once
#if 0
class ParticleSystemProxy :
	public MovableObject
{
public:

	ParticleSystemProxy(void);
	virtual ~ParticleSystemProxy(void);

	Ogre::ParticleSystem* getParticleSys() const { return _particleSys; }
	void setParticleSys(Ogre::ParticleSystem* val);
	
	virtual const Ogre::String& getMovableType(void) const;
	virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;
	virtual Ogre::Real getBoundingRadius(void) const;
	virtual void _updateRenderQueue(Ogre::RenderQueue* queue);
	virtual void visitRenderables(Ogre::Renderable::Visitor* visitor, 
		bool debugRenderables = false);

	virtual void _notifyCurrentCamera(Ogre::Camera* cam);

	void update(Ogre::Real deltaTime);
	bool getEnableUpdate() const { return _enableUpdate; }
	void setEnableUpdate(bool val) { _enableUpdate = val; }

	void setElapsedTime(Ogre::Real time);

protected:

	Ogre::ParticleSystem*	_particleSys;
	Ogre::SceneNode*		_node;
	Ogre::WireBoundingBox*	_boundingBox;
	bool _enableUpdate;
	virtual void _notifyAttached(Ogre::Node* parent, bool isTagPoint = false);
};

#endif