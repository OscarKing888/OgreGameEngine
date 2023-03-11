#pragma once
#include "EditorCore/ColourLerpWnd.h"

class ColourInterpolatorAffectorEditor;

class ColourInterpolatorAffector_TimeLineEditor
	: public ColourLerpWnd
{
public:

	ColourInterpolatorAffector_TimeLineEditor();
	virtual ~ColourInterpolatorAffector_TimeLineEditor();

	ColourInterpolatorAffectorEditor* GetEditor() const { return _editor; }
	void SetEditor(ColourInterpolatorAffectorEditor* val) { _editor = val; }

protected:

	ColourInterpolatorAffectorEditor* _editor;

	/// 通知函数，子类重载实现
	virtual void OnColorChange(int idx, COLORREF currentClr);

	/// 某个时间改变
	virtual void OnTimeChange(int idx, float timeChange, COLORREF currentClr);

};