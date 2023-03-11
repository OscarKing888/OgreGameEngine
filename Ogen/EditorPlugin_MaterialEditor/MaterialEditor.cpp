#include "StdAfx.h"
#include "MaterialEditor.h"

template<> MaterialEditor* Singleton<MaterialEditor>::ms_Singleton = 0;

MaterialEditor::MaterialEditor(void)
: _currentMaterial(0)
, _mesh(0)
, _meshFile("axes.mesh")
, _node(0)
, _showBounding(false)
{
	//_particleSysProxy = OGRE_NEW ParticleSystemProxy();
	
	GameApp::getSingleton().AddUpdateable(this);

	EventsManager::getSingleton().AddListener(this);


	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	_node = sm->getRootSceneNode()->createChildSceneNode("__Material_Editor__");
	//_node->attachObject(_particleSysProxy);

#define REGCMD(e, x) RegisterCommand(MaterialEditorEvents::e, OGRE_NEW x());

	// Add action commands
// 	REGCMD(AddEmitter, Action_AddEmitter);
// 	REGCMD(RemoveEmitter, Action_RemoveEmitter);
// 
// 	REGCMD(AddAffector, Action_AddAffector);
// 	REGCMD(RemoveAffector, Action_RemoveAffector);
// 
// 	REGCMD(ChangeParticleSysRenderer, Action_ChangeParticleSysRenderer);
// 
// 	REGCMD(SelectEmitter, Action_SelectEmitter);
// 	REGCMD(SelectAffector, Action_SelectAffector);
// 	
// 	REGCMD(SelectParticleSysProp, Action_SelectParticleSysProp);
// 	REGCMD(SelectParticleSysRendererProp, Action_SelectParticleSysRendererProp);	
// 
// 	REGCMD(NewParticleSystem, Action_AddNewParticleSys);
// 	REGCMD(EditParticleSystem, Action_EditParticleSystem);
// 
// 	REGCMD(Undo, Action_Undo);
// 	REGCMD(Redo, Action_Redo);
}
//--------------------------------------------------------------------------------
MaterialEditor::~MaterialEditor(void)
{
	GameApp::getSingleton().RemoveUpdateable(this);

	EventsManager::getSingleton().RemoveListener(this);

	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();

	if(_currentMaterial)
	{
		//_node->detachObject(_currentSFX);
		//sm->destroyParticleSystem(_currentSFX);
		_currentMaterial = 0;
	}

	DestroyMesh();

	if(_node)
	{
		sm->destroySceneNode(_node);
		_node = 0;
	}

	//OGRE_DELETE _particleSysProxy;
	//_particleSysProxy = 0;
}
//--------------------------------------------------------------------------------
void MaterialEditor::EditMaterial(const String& templateName)
{
	//g_particleAttrEditorProxy.SetParticleSystem(_currentSFX);
	_cmdHistory.Clear();
}
//--------------------------------------------------------------------------------
void MaterialEditor::SetMeshFile(const String& meshFile)
{
	_meshFile = meshFile;

	DestroyMesh();
	CreateMesh();
}
//--------------------------------------------------------------------------------
const String& MaterialEditor::GetMeshFile() const
{
	return _meshFile;
}
//--------------------------------------------------------------------------------
void MaterialEditor::CreateMesh()
{
	//if(ResourceGroupManager::getSingleton().resourceExists(ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, _meshFile))
	{
		SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
		_mesh = sm->createEntity("__Material_Editor_Reference_Mesh__", _meshFile);
		
		_node->attachObject(_mesh);
		_mesh->setVisibilityFlags(C_ViewportVisibilityFlag);
	}
}
//--------------------------------------------------------------------------------
void MaterialEditor::DestroyMesh()
{
	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();

	if(_mesh)
	{
		_node->detachObject(_mesh);
		sm->destroyEntity(_mesh);
	}
}
//--------------------------------------------------------------------------------
void MaterialEditor::SaveAllSFXTemplate()
{
	if(_currentMaterial)
	{
//		ParticleUtil::ReplaceParticleTemplate(_currentSFX, _currentTemplate);

		String saveFile = FileEx::GetResourceFilePath("All.sfx");
		//ParticleUtil::SaveAllParticleTemplate(saveFile); // "Data/Effect/All.particle"

	}
}
//--------------------------------------------------------------------------------
void MaterialEditor::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(!Execute(eventName, eventContex))
	{
		//assert(false &&  "Events not found!");
	}
}
//--------------------------------------------------------------------------------
void MaterialEditor::AddCommand( ICommandXPtr cmd, bool execute /*= true*/ )
{
	_cmdHistory.AddCommand(cmd, execute);
}
//--------------------------------------------------------------------------------
void MaterialEditor::Undo()
{
	_cmdHistory.Undo();
}
//--------------------------------------------------------------------------------
void MaterialEditor::Redo()
{
	_cmdHistory.Redo();
}
//--------------------------------------------------------------------------------
bool MaterialEditor::IsUndoEnable() const
{
	return _cmdHistory.IsUndoEnable();
}
//--------------------------------------------------------------------------------
bool MaterialEditor::IsRedoEnable() const
{
	return _cmdHistory.IsRedoEnable();
}
//--------------------------------------------------------------------------------
void MaterialEditor::SetShowBounding( bool val )
{
	_showBounding = val;
	if(_node)
	{
		_node->showBoundingBox(val);
	}
}
//--------------------------------------------------------------------------------
void MaterialEditor::Update( float deltaTime )
{
	//_particleSysProxy->update(deltaTime);
}
//--------------------------------------------------------------------------------
