#include "StdAfx.h"
#include "SFXEditor.h"
#include "SFXEditActionCmd.h"
#include "Cmd_AddElement.h"

template<> SFXEditor* Singleton<SFXEditor>::ms_Singleton = 0;

SFXEditor::SFXEditor(void)
: _currentSFX(0)
, _mesh(0)
, _meshFile("axes.mesh")
, _node(0)
, _showBounding(false)
{
	//_particleSysProxy = OGRE_NEW ParticleSystemProxy();
	
	GameApp::getSingleton().AddUpdateable(this);

	EventsManager::getSingleton().AddListener(this);


	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	_node = sm->getRootSceneNode()->createChildSceneNode("__SFX_Editor__");
	//_node->attachObject(_particleSysProxy);

#define REGCMD(e, x) RegisterCommand(SFXEditorEvents::e, OGRE_NEW x());

	// Add action commands
	REGCMD(NewSFX, Action_NewSFX);
	REGCMD(AddElement, Action_AddElement);
	REGCMD(ShowSFXObjectProp, Action_ShowSFXObjectProp);
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
SFXEditor::~SFXEditor(void)
{
	GameApp::getSingleton().RemoveUpdateable(this);

	EventsManager::getSingleton().RemoveListener(this);

	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();

	if(_currentSFX)
	{
		//_node->detachObject(_currentSFX);
		//sm->destroyParticleSystem(_currentSFX);
		_currentSFX = 0;
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
void SFXEditor::EditSFX(const String& templateName)
{
	DestroyCurrentSFXInstance();
	CreateSFXInstance(templateName);

	//g_particleAttrEditorProxy.SetParticleSystem(_currentSFX);
	_cmdHistory.Clear();
}
//--------------------------------------------------------------------------------
void SFXEditor::SetMeshFile(const String& meshFile)
{
	_meshFile = meshFile;

	DestroyMesh();
	CreateMesh();
}
//--------------------------------------------------------------------------------
const String& SFXEditor::GetMeshFile() const
{
	return _meshFile;
}
//--------------------------------------------------------------------------------
void SFXEditor::CreateMesh()
{
	//if(ResourceGroupManager::getSingleton().resourceExists(ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, _meshFile))
	{
		SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
		_mesh = sm->createEntity("__SFX_Editor_Reference_Mesh__", _meshFile);
		
		_node->attachObject(_mesh);
		_mesh->setVisibilityFlags(C_ViewportVisibilityFlag);
	}
}
//--------------------------------------------------------------------------------
void SFXEditor::DestroyMesh()
{
	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();

	if(_mesh)
	{
		_node->detachObject(_mesh);
		sm->destroyEntity(_mesh);
	}
}
//--------------------------------------------------------------------------------
void SFXEditor::DeleteCurrentSFXTemplate()
{
	if(!_currentTemplate.empty())
	{
		//ParticleUtil::RemoveParticleTemplate(_currentTemplate);

		DestroyCurrentSFXInstance();
		//RefreshParticleList();
		/*EventsManager::getSingleton().CreateEvent(
			ParticleEditorEvents::u,
			Any(_currentParticles));*/
	}
}
//--------------------------------------------------------------------------------
void SFXEditor::SaveAllSFXTemplate()
{
	if(_currentSFX)
	{
//		ParticleUtil::ReplaceParticleTemplate(_currentSFX, _currentTemplate);

		String saveFile = FileEx::GetResourceFilePath("All.sfx");
		SpecialEffectsManager::getSingleton().SaveTemplateToFile(saveFile);
		//ParticleUtil::SaveAllParticleTemplate(saveFile); // "Data/Effect/All.particle"

	}
}
//--------------------------------------------------------------------------------
void SFXEditor::CreateSFXInstance(const String& templateName)
{
	try
	{
		SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
		_currentTemplate = templateName;
//		_currentSFX = sm->createParticleSystem("__Auto_Edit_SFX__", templateName);
// 		_currentSFX->setVisibilityFlags(C_ViewportVisibilityFlag);

		//_node->attachObject(_currentSFX);
		//_particleSysProxy->setParticleSys(_currentParticles);

// 		EventsManager::getSingleton().CreateEvent(
// 			ParticleEditorEvents::EditParticleSystemInstance,
// 			Any(_currentSFX));
	}
	catch (const Ogre::Exception& ex)
	{
		AfxMessageBox(ex.getFullDescription().c_str());
	}
}
//--------------------------------------------------------------------------------
void SFXEditor::DestroyCurrentSFXInstance()
{
	if(_currentSFX)
	{
		SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
//		_node->detachObject(_currentSFX);
		//_particleSysProxy->setParticleSys(0);
//		sm->destroyParticleSystem(_currentSFX);
		_currentSFX = 0;
		_currentTemplate.clear();
// 		
// 		EventsManager::getSingleton().CreateEvent(
// 			ParticleEditorEvents::ClearObjectProp, Any());
// 
// 		EventsManager::getSingleton().CreateEvent(
// 			ParticleEditorEvents::DestroyParticleSysInstance, Any());
	}
}
//--------------------------------------------------------------------------------
void SFXEditor::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(!Execute(eventName, eventContex))
	{
		//assert(false &&  "Events not found!");
	}
}
//--------------------------------------------------------------------------------
void SFXEditor::AddCommand( ICommandXPtr cmd, bool execute /*= true*/ )
{
	_cmdHistory.AddCommand(cmd, execute);
}
//--------------------------------------------------------------------------------
void SFXEditor::Undo()
{
	_cmdHistory.Undo();
}
//--------------------------------------------------------------------------------
void SFXEditor::Redo()
{
	_cmdHistory.Redo();
}
//--------------------------------------------------------------------------------
bool SFXEditor::IsUndoEnable() const
{
	return _cmdHistory.IsUndoEnable();
}
//--------------------------------------------------------------------------------
bool SFXEditor::IsRedoEnable() const
{
	return _cmdHistory.IsRedoEnable();
}
//--------------------------------------------------------------------------------
void SFXEditor::SetShowBounding( bool val )
{
	_showBounding = val;
	if(_node)
	{
		_node->showBoundingBox(val);
	}
}
//--------------------------------------------------------------------------------
void SFXEditor::Update( float deltaTime )
{
	//_particleSysProxy->update(deltaTime);
}
//--------------------------------------------------------------------------------
void SFXEditor::CreateNewSFX( const String& templateName )
{
	_currentSFX = OGRE_NEW SpecialEffects(templateName);
	_currentTemplate = templateName;
	_currentSFX->SetName(templateName);
	_currentSFX->SetPosition(Vector3::ZERO);
}
//--------------------------------------------------------------------------------
void SFXEditor::AddElement( const String& elementType )
{
	static uint autoID = 0;
	String autoName = formatString("%s/%s[%d]",
		_currentTemplate.c_str(), elementType.c_str(), autoID);
	++autoID;

	AddCommand(OGRE_NEW Cmd_AddElement(_currentSFX, elementType, autoName, NameValuePairList()));
}
//--------------------------------------------------------------------------------
void SFXEditor::StepUpdate( float t, float step /*= 0.01f*/ )
{
	if(_currentSFX)
	{
		_currentSFX->Reset();
		for (float f = 0; f < t; f += step)
		{
			_currentSFX->Update(step);
		}
	}
}
//--------------------------------------------------------------------------------
