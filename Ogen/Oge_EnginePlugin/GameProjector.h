#pragma once
#include "OgeEnginePluginPrerequisites.h"
#include "Engine/PlacementObject.h"
#include "Plugin_OgreEx/OgreProjector.h"

///
/// Project object to place in scene
///
class _OgeEnginePluginExport GameProjector : public PlacementObject
{
public:

	GameProjector(void);
	virtual ~GameProjector(void);

	virtual const String& GetTypeName() const;

	String GetMaterial() const;
	void SetMaterial(const String& material);

	bool GetShowProjectorBoundingBox() const;
	void SetShowProjectorBoundingBox(bool on);

	bool GetShowFrustum() const;
	void SetShowFrustum(bool on);

	float GetOrthoWindowWidth() const;
	void SetOrthoWindowWidth(float val);

	bool GetPerspective() const;
	void SetPerspective(bool on);

	float GetFovDegree() const;
	void SetFovDegree(float val);

protected:

	Ogre::Projector* _projector;

	virtual void BuildPropertyMap();
	virtual void PostCreateNode();
	virtual void PreDestroyNode();
	
	void CreateProjector();
	void DestroyProjector();
};