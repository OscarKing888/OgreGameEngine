#include "stdafx.h"
#include "MaterialEditorMsgHandler.h"
#include "MaterialEditorFrm.h"
#include "Resource.h"

UINT ShowHideMaterialEditor = GetAutoWMID();

template<> MaterialEditorMsgHandler* Singleton<MaterialEditorMsgHandler>::ms_Singleton = 0;

IMPLEMENT_DYNAMIC(MaterialEditorMsgHandler, CCmdTarget)
BEGIN_MESSAGE_MAP(MaterialEditorMsgHandler, CCmdTarget)

	ON_COMMAND(ShowHideMaterialEditor, &MaterialEditorMsgHandler::OnShowMeshEditor)
	ON_UPDATE_COMMAND_UI(ShowHideMaterialEditor, &MaterialEditorMsgHandler::OnUpdateShowMeshEditor)

END_MESSAGE_MAP()


//--------------------------------------------------------------------------------
// MaterialEditorMsgHandler
//--------------------------------------------------------------------------------
MaterialEditorMsgHandler::MaterialEditorMsgHandler()
{
	EditorApp::getSingleton().RegisterWMHandler(this);

	_materialEditorDoc = new CSingleDocTemplate(
		IDR_MATERIAL_EDITOR,
		RUNTIME_CLASS(CDocument),
		RUNTIME_CLASS(MaterialEditorFrm),
		RUNTIME_CLASS(RenderWnd));
	
	assert (_materialEditorDoc);

	AfxGetApp()->AddDocTemplate(_materialEditorDoc);
}
//--------------------------------------------------------------------------------
MaterialEditorMsgHandler::~MaterialEditorMsgHandler()
{
	EditorApp::getSingleton().UnRegisterWMHandler(this);

	if(MaterialEditorFrm::getSingletonPtr())
	{
		MaterialEditorFrm::getSingleton().SendMessage(WM_CLOSE);
	}
}
//--------------------------------------------------------------------------------
void MaterialEditorMsgHandler::OnShowMeshEditor()
{
	if(!MaterialEditorFrm::getSingletonPtr())
	{
		//delete MaterialEditorFrm::getSingletonPtr();
		MFCCreateNewWindow(_materialEditorDoc, NULL);
		MaterialEditorFrm::getSingleton().Init();
	}

	MaterialEditorFrm::getSingleton().ShowWindow(SW_RESTORE);
	MaterialEditorFrm::getSingleton().ShowWindow(SW_SHOWMAXIMIZED);
}
//--------------------------------------------------------------------------------
void MaterialEditorMsgHandler::OnUpdateShowMeshEditor( CCmdUI* ui )
{
	if(!MaterialEditorFrm::getSingletonPtr())
	{
		ui->SetCheck(0);
	}
	else
	{
		ui->SetCheck(MaterialEditorFrm::getSingleton().IsWindowVisible());
	}
}
//--------------------------------------------------------------------------------
