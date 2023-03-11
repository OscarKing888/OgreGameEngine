#include "stdafx.h"
#include "ColourInterpolatorAffector_TimeLineEditor.h"
#include "ParticleAffectorEditor.h"

ColourInterpolatorAffector_TimeLineEditor::ColourInterpolatorAffector_TimeLineEditor()
: _editor(0)
{

}

ColourInterpolatorAffector_TimeLineEditor::~ColourInterpolatorAffector_TimeLineEditor()
{

}

void ColourInterpolatorAffector_TimeLineEditor::OnColorChange( int idx, COLORREF currentClr )
{	
	ColourValue clr;
	clr.setAsABGR(currentClr);
	_editor->SetColourAdjust(idx, clr);

	EventsManager::getSingleton().CreateEvent(PropertyEvents::PropertyChanged,
		Any(formatString("ColourAdjust%d", idx)));
}

void ColourInterpolatorAffector_TimeLineEditor::OnTimeChange( int idx, float timeChange, COLORREF currentClr )
{
	ColourValue clr;
	clr.setAsABGR(currentClr);
	_editor->SetColourAdjust(idx, clr);
	_editor->SetTimeAdjust(idx, timeChange);

	EventsManager::getSingleton().CreateEvent(PropertyEvents::PropertyChanged,
		Any(formatString("TimeAdjust%d", idx)));

	EventsManager::getSingleton().CreateEvent(PropertyEvents::PropertyChanged,
		Any(formatString("ColourAdjust%d", idx)));
}
