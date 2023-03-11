// EditorPlugin_Terrain.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EditorPlugin_Terrain.h"
#include "TerrainBrush.h"
#include "PropertyEditor_TerrainBrush.h"
#include "EditState_TerrainSurface.h"
#include "EditState_TerrainHeight.h"
//---------------------------------------------------------------------
Plugin_TerrainEditor::Plugin_TerrainEditor()
: EditorPlugin("TerrainEditor")
{
	_description = "Terrain edit states plugin.";
}
//--------------------------------------------------------------------------------
void Plugin_TerrainEditor::install()
{
	M_REG_TYPEUIEDITOR_FACTORY(TerrainBrush, PropertyEditor_TerrainBrushFactory);
}
//---------------------------------------------------------------------
void Plugin_TerrainEditor::initialise()
{
	// nothing to do
}
//---------------------------------------------------------------------
void Plugin_TerrainEditor::shutdown()
{
	// nothing to do
	TerrainBrushManager::getSingleton().ShutDown();
}
//---------------------------------------------------------------------
void Plugin_TerrainEditor::uninstall()
{
	
}
//---------------------------------------------------------------------
void Plugin_TerrainEditor::onCreateEditStates( IStateManager* sm )
{
	TerrainBrushManager::getSingleton().RefreshBrushList();
	EditorApp::getSingleton().AddState(OGRE_NEW EditState_TerrainHeight(sm));
	EditorApp::getSingleton().AddState(OGRE_NEW EditState_TerrainSurface(sm));
}
//---------------------------------------------------------------------
void Plugin_TerrainEditor::onMapCreate( const String& mapName )
{
	AbstractTerrainEditState::DestroyTerrainBoundingBox();
	AbstractTerrainEditState::CreateTerrainBoundingBox();
}
//---------------------------------------------------------------------
void Plugin_TerrainEditor::onMapLoad( const String& mapName )
{
	AbstractTerrainEditState::DestroyTerrainBoundingBox();
	AbstractTerrainEditState::CreateTerrainBoundingBox();
}
//---------------------------------------------------------------------
void Plugin_TerrainEditor::onMapUnload( const String& mapName )
{
	AbstractTerrainEditState::DestroyTerrainBoundingBox();
}