#include "stdafx.h"
#include "ParticleEditActionCmd.h"
#include "ParticleSystemEditor.h"
#include "Cmd_AddAffector.h"
#include "Cmd_AddEmitter.h"
#include "Cmd_RemoveAffector.h"
#include "Cmd_RemoveEmitter.h"
#include "ParticleEditorProxy.h"
#include "Cmd_SetParticleRenderer.h"

void Action_SelectEmitter::Execute( const Any& contex )
{
	ParticleSystemEditor::getSingleton().EditEmitter(
		any_cast<int>(contex));
}

void Action_SelectAffector::Execute( const Any& contex )
{
	ParticleSystemEditor::getSingleton().EditAffector(
		any_cast<int>(contex));
}

void Action_EditParticleSystem::Execute( const Any& contex )
{
	ParticleSystemEditor::getSingleton().EditParticle(
		any_cast<String>(contex));
}

void Action_Undo::Execute( const Any& contex )
{
	ParticleSystemEditor::getSingleton().Undo();
}

void Action_Redo::Execute( const Any& contex )
{
	ParticleSystemEditor::getSingleton().Redo();
}

void Action_AddEmitter::Execute( const Any& contex )
{
	ParticleSystemEditor& editor = ParticleSystemEditor::getSingleton();
	String typeName = any_cast< String >(contex);
	NameValuePairList parms = editor.GetDefaultEmitterParams(typeName);

	editor.AddCommand(OGRE_NEW Cmd_AddEmitter(editor.GetCurrentParticles(),
		typeName, parms));
}

void Action_AddAffector::Execute( const Any& contex )
{
	ParticleSystemEditor& editor = ParticleSystemEditor::getSingleton();
	String typeName = any_cast< String >(contex);
	NameValuePairList parms = editor.GetDefaultAffectorParams(typeName);

	editor.AddCommand(OGRE_NEW Cmd_AddAffector(editor.GetCurrentParticles(),
		typeName, parms));
}

void Action_AddNewParticleSys::Execute( const Any& contex )
{
	SceneManager* sm = GameApp::getSingleton().GetGame()->GetVisualScene();
	ParticleSystem* ps = sm->createParticleSystem(any_cast<String>(contex));
	ps->setMaterialName("BaseVertexColor");
	ParticleSystemManager::getSingleton().addTemplate(any_cast<String>(contex), ps);

	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::EditParticleSystem, contex);
}

void Action_ChangeParticleSysRenderer::Execute( const Any& contex )
{
	ParticleSystemEditor& editor = ParticleSystemEditor::getSingleton();

	String typeName = any_cast< String >(contex);
	ParticleSystem* ps = editor.GetCurrentParticles();
	if(ps->getRendererName() != typeName)
	{
		NameValuePairList parms = editor.GetDefaultRendererParams(typeName);

		editor.AddCommand(OGRE_NEW
			Cmd_SetParticleRenderer(ps, typeName, parms));
	}

	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::ShowParticleObjectProp,
		Any(dynamic_cast<IPropertyObject*>(
		editor.RenderEditor())));
}

void Action_SelectParticleSysProp::Execute( const Any& contex )
{
	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::ShowParticleObjectProp,
		Any(ParticleSystemEditor::getSingleton().ParticleEditor()));
}

void Action_SelectParticleSysRendererProp::Execute( const Any& contex )
{
	EventsManager::getSingleton().CreateEvent(
		ParticleEditorEvents::ShowParticleObjectProp,
		Any(dynamic_cast<IPropertyObject*>(
		ParticleSystemEditor::getSingleton().RenderEditor())));
}

void Action_RemoveEmitter::Execute( const Any& contex )
{
	ParticleSystemEditor& editor = ParticleSystemEditor::getSingleton();
	editor.AddCommand(OGRE_NEW Cmd_RemoveEmitter(
		editor.GetCurrentParticles(), any_cast<int>(contex)));
}

void Action_RemoveAffector::Execute( const Any& contex )
{
	ParticleSystemEditor& editor = ParticleSystemEditor::getSingleton();
	editor.AddCommand(OGRE_NEW Cmd_RemoveAffector(
		editor.GetCurrentParticles(), any_cast<int>(contex)));
}