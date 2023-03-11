#include "StdAfx.h"
#include "ParticleSystemEditor.h"
#include "ParticleUtil.h"
#include "ParticleEditorProxy.h"
#include "ParticleEditorEvents.h"
#include "ParticleEditActionCmd.h"
#include "Cmd_AddAffector.h"
#include "Cmd_AddEmitter.h"
#include "Cmd_RemoveAffector.h"
#include "Cmd_RemoveEmitter.h"
#include "Cmd_SetParticleRenderer.h"
#include "ParticleSystemProxy.h"

ParticleAttributeEditor	g_particleAttrEditorProxy;

template<> ParticleSystemEditor* Singleton<ParticleSystemEditor>::ms_Singleton = 0;

ParticleSystemEditor::ParticleSystemEditor(void)
: _currentParticles(0)
, _mesh(0)
, _renderEditor(0)
, _emitterEditor(0)
, _affectorEditor(0)
, _selectedEmitter(-1)
, _selectedAffector(-1)
, _meshFile("axes.mesh")
, _node(0)
, _showBounding(false)
, _particleEditor(&g_particleAttrEditorProxy)
{
	//_particleSysProxy = OGRE_NEW ParticleSystemProxy();
	
	GameApp::getSingleton().AddUpdateable(this);

	EventsManager::getSingleton().AddListener(this);


	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	_node = sm->getRootSceneNode()->createChildSceneNode("__Particle_Editor__");
	//_node->attachObject(_particleSysProxy);

#define REGCMD(e, x) RegisterCommand(ParticleEditorEvents::e, OGRE_NEW x());

	// Add action commands
	REGCMD(AddEmitter, Action_AddEmitter);
	REGCMD(RemoveEmitter, Action_RemoveEmitter);

	REGCMD(AddAffector, Action_AddAffector);
	REGCMD(RemoveAffector, Action_RemoveAffector);

	REGCMD(ChangeParticleSysRenderer, Action_ChangeParticleSysRenderer);

	REGCMD(SelectEmitter, Action_SelectEmitter);
	REGCMD(SelectAffector, Action_SelectAffector);
	
	REGCMD(SelectParticleSysProp, Action_SelectParticleSysProp);
	REGCMD(SelectParticleSysRendererProp, Action_SelectParticleSysRendererProp);	

	REGCMD(NewParticleSystem, Action_AddNewParticleSys);
	REGCMD(EditParticleSystem, Action_EditParticleSystem);

	REGCMD(Undo, Action_Undo);
	REGCMD(Redo, Action_Redo);
}
//--------------------------------------------------------------------------------
ParticleSystemEditor::~ParticleSystemEditor(void)
{
	GameApp::getSingleton().RemoveUpdateable(this);

	EventsManager::getSingleton().RemoveListener(this);

	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();

	if(_currentParticles)
	{
		_node->detachObject(_currentParticles);
		sm->destroyParticleSystem(_currentParticles);
		_currentParticles = 0;
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
void ParticleSystemEditor::EditParticle(const String& templateName)
{
	DestroyCurrentParticleSystemInstance();
	CreateParticleSystemInstance(templateName);

	g_particleAttrEditorProxy.SetParticleSystem(_currentParticles);
	UpdateRendererEditor();

	_cmdHistory.Clear();
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::SetMeshFile(const String& meshFile)
{
	_meshFile = meshFile;

	DestroyMesh();
	CreateMesh();
}
//--------------------------------------------------------------------------------
const String& ParticleSystemEditor::GetMeshFile() const
{
	return _meshFile;
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::CreateMesh()
{
	//if(ResourceGroupManager::getSingleton().resourceExists(ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, _meshFile))
	{
		SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
		_mesh = sm->createEntity("__Particle_Editor_Reference_Mesh__", _meshFile);
		
		_node->attachObject(_mesh);
		_mesh->setVisibilityFlags(C_ViewportVisibilityFlag);
	}
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::DestroyMesh()
{
	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();

	if(_mesh)
	{
		_node->detachObject(_mesh);
		sm->destroyEntity(_mesh);
	}
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::DeleteCurrentParticleTemplate()
{
	if(!_currentTemplate.empty())
	{
		ParticleUtil::RemoveParticleTemplate(_currentTemplate);

		DestroyCurrentParticleSystemInstance();
		//RefreshParticleList();
		/*EventsManager::getSingleton().CreateEvent(
			ParticleEditorEvents::u,
			Any(_currentParticles));*/
	}
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::SaveAllParticleTemplate()
{
	if(_currentParticles)
	{
		ParticleUtil::ReplaceParticleTemplate(_currentParticles, _currentTemplate);

		String saveFile = FileEx::GetResourceFilePath("All.particle");
		ParticleUtil::SaveAllParticleTemplate(saveFile); // "Data/Effect/All.particle"

	}
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::CreateParticleSystemInstance(const String& templateName)
{
	try
	{
		SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
		_currentTemplate = templateName;
		_currentParticles = sm->createParticleSystem("__Auto_Edit_Particle__", templateName);
		_currentParticles->setVisibilityFlags(C_ViewportVisibilityFlag);

		_node->attachObject(_currentParticles);
		//_particleSysProxy->setParticleSys(_currentParticles);

		EventsManager::getSingleton().CreateEvent(
			ParticleEditorEvents::EditParticleSystemInstance,
			Any(_currentParticles));
	}
	catch (const Ogre::Exception& ex)
	{
		AfxMessageBox(ex.getFullDescription().c_str());
	}
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::DestroyCurrentParticleSystemInstance()
{
	if(_currentParticles)
	{
		SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
		_node->detachObject(_currentParticles);
		//_particleSysProxy->setParticleSys(0);
		sm->destroyParticleSystem(_currentParticles);
		_currentParticles = 0;
		_currentTemplate.clear();
		
		EventsManager::getSingleton().CreateEvent(
			ParticleEditorEvents::ClearObjectProp, Any());

		EventsManager::getSingleton().CreateEvent(
			ParticleEditorEvents::DestroyParticleSysInstance, Any());
	}
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::EditAffector(int idx)
{
	if(_affectorEditor)
	{
		_affectorEditor->ShowEditorProxyHelper(false);
		ParticleEditorFactoryManager::getSingleton()
			.DestroyAffectorEditor(_affectorEditor);

		_affectorEditor = 0;
	}

	ParticleAffector* affector = _currentParticles->getAffector(idx);
	_affectorEditor = ParticleEditorFactoryManager::getSingleton()
		.CreateAffectorEditor(affector->getType(), affector);

	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::ShowParticleObjectProp,
		Any(dynamic_cast<IPropertyObject*>(_affectorEditor)));

	if(_affectorEditor)
	{
		_affectorEditor->ShowEditorProxyHelper(true);
		CWnd* timeLineEditor = _affectorEditor->GetEditorWnd();
		if(timeLineEditor)
		{
			EventsManager::getSingleton().CreateEvent(
				ParticleEditorEvents::UpdateTimeLineEditor, Any());
		}
	}
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::EditEmitter( ushort idx )
{
	if(_emitterEditor)
	{
		_emitterEditor->ShowEditorProxyHelper(false);
		ParticleEditorFactoryManager::getSingleton().DestroyEmitterEditor(_emitterEditor);
		_emitterEditor = 0;
	}

	ParticleEmitter* emitter = _currentParticles->getEmitter(idx);
	_emitterEditor = ParticleEditorFactoryManager::getSingleton()
		.CreateEmitterEditor(emitter->getType(), emitter);

	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::ShowParticleObjectProp,
		Any(dynamic_cast<IPropertyObject*>(_emitterEditor)));

	if(_emitterEditor)
	{
		_emitterEditor->ShowEditorProxyHelper(true);
	}
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	if(!Execute(eventName, eventContex))
	{
		//assert(false &&  "Events not found!");
	}
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::AddCommand( ICommandXPtr cmd, bool execute /*= true*/ )
{
	_cmdHistory.AddCommand(cmd, execute);
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::Undo()
{
	_cmdHistory.Undo();
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::Redo()
{
	_cmdHistory.Redo();
}
//--------------------------------------------------------------------------------
bool ParticleSystemEditor::IsUndoEnable() const
{
	return _cmdHistory.IsUndoEnable();
}
//--------------------------------------------------------------------------------
bool ParticleSystemEditor::IsRedoEnable() const
{
	return _cmdHistory.IsRedoEnable();
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::SetShowBounding( bool val )
{
	_showBounding = val;
	if(_node)
	{
		_node->showBoundingBox(val);
	}
}
//--------------------------------------------------------------------------------
void ParticleSystemEditor::UpdateRendererEditor()
{
	if(_currentParticles)
	{
		const String& renderName = _currentParticles->getRendererName();
		if(_renderEditor)
		{
			_renderEditor->ShowEditorProxyHelper(false);
			ParticleEditorFactoryManager::getSingleton().DestroyParticleRenderEditor(_renderEditor);
			_renderEditor = 0;
		}

		_renderEditor = ParticleEditorFactoryManager::getSingleton().CreateParticleRenderEditor(
			renderName, _currentParticles->getRenderer());
		if(_renderEditor)
		{
			_renderEditor->ShowEditorProxyHelper(true);
		}
	}
}
//--------------------------------------------------------------------------------
Ogre::NameValuePairList ParticleSystemEditor::GetDefaultEmitterParams( const String& typeName )
{
	IParticleComponentEditorFactory* f =
		ParticleEditorFactoryManager::getSingleton().GetEmitterFactory(typeName);

	return f->GetInitDefaultParameters();
}
//--------------------------------------------------------------------------------
Ogre::NameValuePairList ParticleSystemEditor::GetDefaultAffectorParams( const String& typeName )
{
	IParticleComponentEditorFactory* f =
		ParticleEditorFactoryManager::getSingleton().GetAffectorFactory(typeName);

	return f->GetInitDefaultParameters();
}
//--------------------------------------------------------------------------------
Ogre::NameValuePairList ParticleSystemEditor::GetDefaultRendererParams( const String& typeName )
{
	IParticleComponentEditorFactory* f =
		ParticleEditorFactoryManager::getSingleton().GetRendererFactory(typeName);

	return f->GetInitDefaultParameters();
}

void ParticleSystemEditor::Update( float deltaTime )
{
	//_particleSysProxy->update(deltaTime);
}
//--------------------------------------------------------------------------------
