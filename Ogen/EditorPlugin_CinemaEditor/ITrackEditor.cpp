#include "stdafx.h"
#include "ITrackEditor.h"
#include "CinemaEditor.h"

SRegTrackEditorHelper::SRegTrackEditorHelper( ITrackEditor* editor )
{
	CinemaEditor::getSingleton().AddTrackEditor(editor);
}