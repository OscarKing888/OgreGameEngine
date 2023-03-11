#include "stdafx.h"
#include "MeshEditorMsgHandler.h"
#include "MeshEditorFrm.h"
#include "Resource.h"

UINT ShowHideMeshEditor = GetAutoWMID();

template<> MeshEditorMsgHandler* Singleton<MeshEditorMsgHandler>::ms_Singleton = 0;

IMPLEMENT_DYNAMIC(MeshEditorMsgHandler, CCmdTarget)
BEGIN_MESSAGE_MAP(MeshEditorMsgHandler, CCmdTarget)

	ON_COMMAND(ShowHideMeshEditor, &MeshEditorMsgHandler::OnShowMeshEditor)
	ON_UPDATE_COMMAND_UI(ShowHideMeshEditor, &MeshEditorMsgHandler::OnUpdateShowMeshEditor)

END_MESSAGE_MAP()


//--------------------------------------------------------------------------------
// MeshEditorMsgHandler
//--------------------------------------------------------------------------------
MeshEditorMsgHandler::MeshEditorMsgHandler()
{
	EditorApp::getSingleton().RegisterWMHandler(this);

	_modelEditorDoc = new CSingleDocTemplate(
		IDR_MODELEDITOR,
		RUNTIME_CLASS(CDocument),
		RUNTIME_CLASS(MeshEditorFrm),
		RUNTIME_CLASS(RenderWnd));
	
	assert (_modelEditorDoc);

	AfxGetApp()->AddDocTemplate(_modelEditorDoc);
}
//--------------------------------------------------------------------------------
MeshEditorMsgHandler::~MeshEditorMsgHandler()
{
	EditorApp::getSingleton().UnRegisterWMHandler(this);

	if(MeshEditorFrm::getSingletonPtr())
	{
		MeshEditorFrm::getSingleton().SendMessage(WM_CLOSE);
	}
}
//--------------------------------------------------------------------------------
void MeshEditorMsgHandler::OnShowMeshEditor()
{
	if(!MeshEditorFrm::getSingletonPtr())
	{
		//delete MeshEditorFrm::getSingletonPtr();
		MFCCreateNewWindow(_modelEditorDoc, NULL);
		MeshEditorFrm::getSingleton().Init();
	}

	MeshEditorFrm::getSingleton().ShowWindow(SW_RESTORE);
	MeshEditorFrm::getSingleton().ShowWindow(SW_SHOWMAXIMIZED);
}
//--------------------------------------------------------------------------------
void MeshEditorMsgHandler::OnUpdateShowMeshEditor( CCmdUI* ui )
{
	if(!MeshEditorFrm::getSingletonPtr())
	{
		ui->SetCheck(0);
	}
	else
	{
		ui->SetCheck(MeshEditorFrm::getSingleton().IsWindowVisible());
	}
}
//--------------------------------------------------------------------------------
