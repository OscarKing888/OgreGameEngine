#include "stdafx.h"
#include "SFXEditActionCmd.h"
#include "SFXEditor.h"
#include "SFXEditorFrm.h"

void Action_NewSFX::Execute( const Any& contex )
{
	SFXEditor& editor = SFXEditor::getSingleton();
	if(editor.IsUndoEnable())
	{
		int r = AfxMessageBox(
			formatString(_UIDisplayString("IDSF_SAVE_SFX").c_str(), 
			editor.GetCurrentTemplate().c_str()).c_str(), MB_YESNOCANCEL);

		if(r == IDCANCEL)
		{
			return;
		}

		if(r == IDYES)
		{
			editor.SaveAllSFXTemplate();
		}
	}

	InputDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		String str = dlg.GetInputText();
		if(SpecialEffectsManager::getSingleton().IsTemplateExists(str))
		{
			AfxMessageBox(formatString(
				_UIDisplayString("IDSF_SFX_ALREADY_EXIST").c_str(),
				str.c_str()).c_str());

			return;
		}

		editor.CreateNewSFX(str);

		//EventsManager::getSingleton().CreateEvent(
		//	ParticleEditorEvents::NewParticleSystem,
		//	Any(str));

		//RefreshParticleList();
	}
}
//--------------------------------------------------------------------------------
void Action_AddElement::Execute( const Any& contex )
{
	SFXEditor::getSingleton().AddElement(any_cast<String>(contex));
}
//--------------------------------------------------------------------------------
void Action_ShowSFXObjectProp::Execute( const Any& contex )
{
	SFXEditorFrm::getSingleton().GetPropGrid().SetObject(
		any_cast<IPropertyObject*>(contex));
}