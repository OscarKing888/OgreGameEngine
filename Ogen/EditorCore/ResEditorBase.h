#pragma once

class RenderWnd;
class Grid;

class _EditorCoreExport ResEditorBase : public PropertyObject
{
public:

	ResEditorBase();
	virtual ~ResEditorBase();

	void Init(uint visibilityFlag, const String& name);
	void Shutdown();

	void SetShowGrid(bool on);
	bool GetShowGrid() const;

	void SetViewportColour(const ColourValue& clr);
	const ColourValue& GetViewportColour() const;

	void SetShowWireframe(bool on);
	bool GetShowWireframe() const { return _showWire; }

	void ShowAxes(bool on);
	bool IsAxesVisible() const;

	void CaptureThumbImage(const String& thumbFile);

protected:

	Entity*		_axes;
	SceneNode*	_axesNode;
	RenderWnd*	_renderWindow;
	bool		_showGrid;
	ColourValue	_bgColour;
	bool		_showWire;
	Grid*		_grid;

	virtual void BuildPropertyMap();
	virtual CWnd* _GetActiveView() = 0;
};