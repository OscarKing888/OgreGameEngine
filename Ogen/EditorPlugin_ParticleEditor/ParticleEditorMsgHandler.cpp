#include "stdafx.h"
#include "ParticleEditorMsgHandler.h"
#include "ParticleEditorFrm.h"
#include "Resource.h"

UINT ShowHideParticleEditor = GetAutoWMID();

template<> ParticleEditorMsgHandler* Singleton<ParticleEditorMsgHandler>::ms_Singleton = 0;

IMPLEMENT_DYNAMIC(ParticleEditorMsgHandler, CCmdTarget)
BEGIN_MESSAGE_MAP(ParticleEditorMsgHandler, CCmdTarget)

	ON_COMMAND(ShowHideParticleEditor, &ParticleEditorMsgHandler::OnShowMeshEditor)
	ON_UPDATE_COMMAND_UI(ShowHideParticleEditor, &ParticleEditorMsgHandler::OnUpdateShowMeshEditor)

END_MESSAGE_MAP()


//--------------------------------------------------------------------------------
// ParticleEditorMsgHandler
//--------------------------------------------------------------------------------
ParticleEditorMsgHandler::ParticleEditorMsgHandler()
{
	EditorApp::getSingleton().RegisterWMHandler(this);

	_particleEditorDoc = new CSingleDocTemplate(
		IDR_PARTICLE_WND,
		RUNTIME_CLASS(CDocument),
		RUNTIME_CLASS(ParticleEditorFrm),
		RUNTIME_CLASS(RenderWnd));
	
	assert (_particleEditorDoc);

	AfxGetApp()->AddDocTemplate(_particleEditorDoc);
}
//--------------------------------------------------------------------------------
ParticleEditorMsgHandler::~ParticleEditorMsgHandler()
{
	EditorApp::getSingleton().UnRegisterWMHandler(this);

	if(ParticleEditorFrm::getSingletonPtr())
	{
		ParticleEditorFrm::getSingleton().SendMessage(WM_CLOSE);
	}
}
//--------------------------------------------------------------------------------
void ParticleEditorMsgHandler::OnShowMeshEditor()
{
	if(!ParticleEditorFrm::getSingletonPtr())
	{
		//delete ParticleEditorFrm::getSingletonPtr();
		MFCCreateNewWindow(_particleEditorDoc, NULL);
		ParticleEditorFrm::getSingleton().Init();
	}

	ParticleEditorFrm::getSingleton().ShowWindow(SW_RESTORE);
	ParticleEditorFrm::getSingleton().ShowWindow(SW_SHOWMAXIMIZED);
}
//--------------------------------------------------------------------------------
void ParticleEditorMsgHandler::OnUpdateShowMeshEditor( CCmdUI* ui )
{
	if(!ParticleEditorFrm::getSingletonPtr())
	{
		ui->SetCheck(0);
	}
	else
	{
		ui->SetCheck(ParticleEditorFrm::getSingleton().IsWindowVisible());
	}
}
//--------------------------------------------------------------------------------
