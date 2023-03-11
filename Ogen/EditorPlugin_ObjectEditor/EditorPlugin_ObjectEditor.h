#pragma once

#include "ObjectEditPrerequisites.h"

/** Plugin instance for game object edit in editor*/
class ObjectEditorPlugin : public EditorPlugin
{
public:
	ObjectEditorPlugin();
	virtual ~ObjectEditorPlugin();

	virtual void install();
	virtual void uninstall();
	virtual void initialise();
	virtual void shutdown();

	/// Notify when MainFrameWnd is created
	virtual void onCreateEditorMainWnd(CFrameWndEx* mainWnd);

	/// Notify whe main ribbon bar is created
	virtual void onCreateEditorMainToolbar(CMFCRibbonBar* mainToolbar);

	/// Notify when state manager is created
	virtual void onCreateEditStates(IStateManager* sm);

	/// Notify when the main render window is resized.
	virtual void onWindowResized(RenderWindow* rw);

protected:

};