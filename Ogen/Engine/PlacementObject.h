#pragma once
#include "VisualObject.h"

///
/// Abstract class for placement object
///
class _EngineExport PlacementObject : public VisualObject
{
public:

	PlacementObject(void);
	virtual ~PlacementObject(void);


protected:

	Entity*	_placementModel;
	String _placementMesh;
	
	virtual void PostCreateNode();
	virtual void PreDestroyNode();

	virtual void CreateEditModel();
	virtual void DestroyEditModel();
};

