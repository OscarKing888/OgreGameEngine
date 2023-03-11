// EditorPlugin_CinemaEditor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CinemaEditor.h"
#include "Resource.h"

#include "TrackEditor_FadeScreen.h"
#include "TrackEditor_Camera.h"
#include "TrackEditor_Movement.h"
#include "TrackEditor_Property.h"
#include "TrackEditor_LightColour.h"

/** Plugin instance for game object edit in editor*/
class CinemaEditorPlugin : public EditorPlugin
{
public:
	CinemaEditorPlugin();
	virtual ~CinemaEditorPlugin();

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

	/// @copydoc EditorPlugin::onCreateToolsCategory
	virtual void onCreateToolsCategory(CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel);

	virtual void onPostCreateEditStateCategory( CMFCRibbonCategory* cat );

	virtual void OnAppShutdown();

	virtual void onMapLoad(const String& mapName);
};

CinemaEditorPlugin* plugin = 0;
const String sPluginName = "CinemaEditor";

//-----------------------------------------------------------------------
extern "C" void _CinemaEditorExport dllStartPlugin(void) throw()
{
	plugin = OGRE_NEW CinemaEditorPlugin();
	Root::getSingleton().installPlugin(plugin);
}

//-----------------------------------------------------------------------
extern "C" void _CinemaEditorExport dllStopPlugin(void)
{
	Root::getSingleton().uninstallPlugin(plugin);
	OGRE_DELETE plugin;
}
//---------------------------------------------------------------------
CinemaEditorPlugin::CinemaEditorPlugin()
: EditorPlugin(sPluginName)
{
	_description = "Cinema edit states plugin.";
}
//---------------------------------------------------------------------
CinemaEditorPlugin::~CinemaEditorPlugin()
{

}
//---------------------------------------------------------------------
void CinemaEditorPlugin::install()
{
	OGRE_NEW CinemaEditor();
	new CinemaEditorMsgHandler();
	
	M_REGEDIT_TRACKEDITOR(TrackEditor_Movement);
	M_REGEDIT_TRACKEDITOR(TrackEditor_Camera);
	M_REGEDIT_TRACKEDITOR(TrackEditor_FadeScreen);
	M_REGEDIT_TRACKEDITOR(TrackEditor_Property);
	M_REGEDIT_TRACKEDITOR(TrackEditor_LightColour);
	M_REGEDIT_TRACKEDITOR(TrackEditor_MainLightColour);
	M_REGEDIT_TRACKEDITOR(TrackEditor_AmbientLightColour);
}
//---------------------------------------------------------------------
void CinemaEditorPlugin::initialise()
{
}
//---------------------------------------------------------------------
void CinemaEditorPlugin::shutdown()
{
}
//---------------------------------------------------------------------
void CinemaEditorPlugin::uninstall()
{
}
//---------------------------------------------------------------------
void CinemaEditorPlugin::onCreateEditorMainWnd( CFrameWndEx* mainWnd )
{

}
//---------------------------------------------------------------------
void CinemaEditorPlugin::onCreateEditorMainToolbar( CMFCRibbonBar* mainToolbar )
{

}
//---------------------------------------------------------------------
void CinemaEditorPlugin::onCreateEditStates( IStateManager* sm )
{
}
//---------------------------------------------------------------------
void CinemaEditorPlugin::onWindowResized( RenderWindow* rw )
{

}
//---------------------------------------------------------------------
void CinemaEditorPlugin::onCreateToolsCategory(
	CMFCRibbonCategory* cat, CMFCRibbonPanel* mainPanel )
{
}
//---------------------------------------------------------------------
void CinemaEditorPlugin::onPostCreateEditStateCategory( CMFCRibbonCategory* cat )
{
	CMFCRibbonPanel* mainPanel = cat->AddPanel(
		_UIDisplayString("IDI_ICON_CINEMAEDITOR").c_str());

	HICON icon = AfxGetApp()->LoadIcon(ID_CINEMAEDITOR);
	
	CMFCRibbonButton* btn = new CMFCRibbonButton(ActiveCinemaEditor, 
		_UIDisplayString("IDI_ICON_CINEMAEDITOR").c_str(), icon);

	mainPanel->Add(btn);
}
//---------------------------------------------------------------------
void CinemaEditorPlugin::OnAppShutdown()
{
	delete CinemaEditorMsgHandler::getSingletonPtr();
	OGRE_DELETE CinemaEditor::getSingletonPtr();
}
//---------------------------------------------------------------------
void CinemaEditorPlugin::onMapLoad( const String& mapName )
{
	CinemaManager::getSingleton().LoadCinemasFromResourceSystem(
		CinemaManager::CinemaXMLFileName, AbstractGame::SceneResourceGroupName);
}
//---------------------------------------------------------------------
