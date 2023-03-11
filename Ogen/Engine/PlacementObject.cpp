#include "StdAfx.h"
#include "PlacementObject.h"

PlacementObject::PlacementObject(void)
	: _placementMesh(M_DEBUG_MESH_FILE)
	, _placementModel(0)
{
}
//--------------------------------------------------------------------------------
PlacementObject::~PlacementObject(void)
{
}
//--------------------------------------------------------------------------------
void PlacementObject::PostCreateNode()
{
	VisualObject::PostCreateNode();
	CreateEditModel();
}
//--------------------------------------------------------------------------------
void PlacementObject::PreDestroyNode()
{
	VisualObject::PreDestroyNode();
	DestroyEditModel();
}
//--------------------------------------------------------------------------------
void PlacementObject::CreateEditModel()
{
	if((GameApp::CurrentRunType == ERT_Editor) || _displayEditorObjectInGame)
	{
		uint32 flag = MovableObject::getDefaultQueryFlags();
		SceneManager* sm = _node->getCreator();
		_placementModel = sm->createEntity(GetGUID().toString() + "/EditorModel", _placementMesh);
		_placementModel->setUserAny(Any(AnyGameObject(this)));
		_placementModel->setQueryFlags(flag);
		_node->attachObject(_placementModel);
	}
}
//--------------------------------------------------------------------------------
void PlacementObject::DestroyEditModel()
{
	if((GameApp::CurrentRunType == ERT_Editor) || _displayEditorObjectInGame)
	{
		_node->getCreator()->destroyEntity(_placementModel);
		_placementModel = 0;
	}
}