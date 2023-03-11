#include "stdafx.h"
#include "SFXEditorMsgHandler.h"
#include "SFXEditorFrm.h"
#include "Resource.h"

UINT ShowHideSFXEditor = GetAutoWMID();

template<> SFXEditorMsgHandler* Singleton<SFXEditorMsgHandler>::ms_Singleton = 0;

IMPLEMENT_DYNAMIC(SFXEditorMsgHandler, CCmdTarget)
BEGIN_MESSAGE_MAP(SFXEditorMsgHandler, CCmdTarget)

	ON_COMMAND(ShowHideSFXEditor, &SFXEditorMsgHandler::OnShowMeshEditor)
	ON_UPDATE_COMMAND_UI(ShowHideSFXEditor, &SFXEditorMsgHandler::OnUpdateShowMeshEditor)

END_MESSAGE_MAP()


//--------------------------------------------------------------------------------
// SFXEditorMsgHandler
//--------------------------------------------------------------------------------
SFXEditorMsgHandler::SFXEditorMsgHandler()
{
	EditorApp::getSingleton().RegisterWMHandler(this);

	_sfxEditorDoc = new CSingleDocTemplate(
		IDR_SFXEDITOR,
		RUNTIME_CLASS(CDocument),
		RUNTIME_CLASS(SFXEditorFrm),
		RUNTIME_CLASS(RenderWnd));
	
	assert (_sfxEditorDoc);

	AfxGetApp()->AddDocTemplate(_sfxEditorDoc);
}
//--------------------------------------------------------------------------------
SFXEditorMsgHandler::~SFXEditorMsgHandler()
{
	EditorApp::getSingleton().UnRegisterWMHandler(this);

	if(SFXEditorFrm::getSingletonPtr())
	{
		SFXEditorFrm::getSingleton().SendMessage(WM_CLOSE);
	}
}
//--------------------------------------------------------------------------------
void SFXEditorMsgHandler::OnShowMeshEditor()
{
	if(!SFXEditorFrm::getSingletonPtr())
	{
		//delete SFXEditorFrm::getSingletonPtr();
		MFCCreateNewWindow(_sfxEditorDoc, NULL);
		SFXEditorFrm::getSingleton().Init();
	}

	SFXEditorFrm::getSingleton().ShowWindow(SW_RESTORE);
	SFXEditorFrm::getSingleton().ShowWindow(SW_SHOWMAXIMIZED);
}
//--------------------------------------------------------------------------------
void SFXEditorMsgHandler::OnUpdateShowMeshEditor( CCmdUI* ui )
{
	if(!SFXEditorFrm::getSingletonPtr())
	{
		ui->SetCheck(0);
	}
	else
	{
		ui->SetCheck(SFXEditorFrm::getSingleton().IsWindowVisible());
	}
}
//--------------------------------------------------------------------------------
