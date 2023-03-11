#include "stdafx.h"
#include "Resource.h"
#include "EditState_TerrainSurface.h"
#include "Property_TerrainBrush.h"
#include "Cmd_TerrainSurface.h"

int EditStateID_TerrainSurface = GetAutoEditStateID();
const CString TerrainSurfaceResGroup = "../Data/DataDemoGame/TerrainSurface";

template<> EditState_TerrainSurface* Singleton<EditState_TerrainSurface>::ms_Singleton = 0;
//--------------------------------------------------------------------------------
void EditState_TerrainSurface::BuildPropertyMap()
{
	AbstractTerrainEditState::BuildPropertyMap();

	AddProp(Property_String, TextureName, EditState_TerrainSurface, "Selected texture name", "TerrainBrush");
	AddRangeFloatProp(BrushWeight, 0.01f, 1.0f, EditState_TerrainSurface, "Brush blending weight", "TerrainBrush");	
}
//--------------------------------------------------------------------------------
EditState_TerrainSurface::EditState_TerrainSurface(IStateManager* pMgr)
: AbstractTerrainEditState(pMgr)
, _brushWeight(0.1f)
, _eraseLayer(false)
, _isPainting(false)
{
	_stateID = "Terrain Surface";
	_commandID = EditStateID_TerrainSurface;

	BuildPropertyMap();
	String name = "__Terrain_Surface_Brush__";
	CreateBrushModel(name);
}
//--------------------------------------------------------------------------------
EditState_TerrainSurface::~EditState_TerrainSurface()
{
}
//--------------------------------------------------------------------------------
void EditState_TerrainSurface::CreateToolbar( CMFCRibbonCategory* mainCategory )
{
	CMFCRibbonPanel* panel = MFCGetRibbonPanel(
		_UIDisplayString("ID_TERRAIN_SURFACE_EDIT_STATE").c_str(), mainCategory);

	if(!panel)	
	{
		panel = mainCategory->AddPanel(
			_UIDisplayString("ID_TERRAIN_SURFACE_EDIT_STATE").c_str());
	}

	HICON hicon = AfxGetApp()->LoadIcon(IDI_ICON_TERRAINSURFACE);
	CMFCRibbonButton* btn = new CMFCRibbonButton(GetCommandID(),
		_UIDisplayString("ID_TERRAIN_SURFACE_EDIT_STATE").c_str(), hicon,  TRUE, hicon, TRUE, TRUE);

	panel->Add(btn);
}
//--------------------------------------------------------------------------------
// const String& EditState_TerrainSurface::GetTypeName() const
// {
// 	return _name;
// }
//--------------------------------------------------------------------------------
BOOL EditState_TerrainSurface::OnLButtonDown(UINT nFlags, CPoint point)
{
	AbstractTerrainEditState::OnLButtonDown(nFlags, point);

	if(_selectedTexture.empty())
	{
		AfxMessageBox(_UIDisplayString("Select texture first!").c_str());
		return FALSE;
	}

	EditorApp::getSingleton().GetCommandHistory().BeginBatch();

	TerrainGroup::RayResult r =
		EditorApp::getSingleton().PickOnTerrain(point.x, point.y);
	if(r.hit)
	{
		_isPainting = true;

		EditorConfig& cfg = EditorConfig::getSingleton();
		if(cfg.GetSnapToGrid())
		{
			const Vector2& gridSz = cfg.GetSnapGridSize();
			
			float sx = r.position.x > 0 ? 0.5f : -0.5f;
			float sz = r.position.z > 0 ? 0.5f : -0.5f;

			r.position.x = ((int)(r.position.x) / (int)gridSz.x) * (int)gridSz.x + gridSz.x * sx;
			r.position.z = ((int)(r.position.z) / (int)gridSz.y) * (int)gridSz.y + gridSz.y * sz;
		}

		TerrainGroup::TerrainList terrainList = GetBrushAffectedTiles(r.position);
		for (TerrainGroup::TerrainList::iterator ti = terrainList.begin();
			ti != terrainList.end(); ++ti)
		{
			Terrain* terrain = *ti;
			if(!HasTexture(terrain, _selectedTexture))
			{
				StringVector tex;
				tex.push_back(_selectedTexture);
				String bumpTex = GetBumpTexture(_selectedTexture);
				if(ResourceGroupManager::getSingleton().resourceExists("TerrainSurface",
					bumpTex))
				{
					tex.push_back(bumpTex);
				}

				AddCommand(OGRE_NEW Cmd_AddSurfaceTexture(terrain, tex));
			}

			uint8 layerIdx = GetTextureLayerIndex(terrain, _selectedTexture);
			if(layerIdx != 0)
			{
				_eraseLayer = MFCInputManager::getSingleton().IsKeyDown(VK_CONTROL);
				PaintSurface(layerIdx, terrain, r.position, _eraseLayer);
				EraseAboveLayer(layerIdx, terrain, r.position);
			}
			else
			{
				EraseAboveLayer(layerIdx, terrain, r.position);
			}
		}
	}
	
	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL EditState_TerrainSurface::OnLButtonUp(UINT nFlags, CPoint point)
{
	_isPainting = false;

	//EditorApp::getSingleton().GetCommandHistory().BeginBatch();
	CommitLayerBlendChange();
	EditorApp::getSingleton().GetCommandHistory().EndBatch();

	return AbstractTerrainEditState::OnLButtonUp(nFlags, point);
}
//--------------------------------------------------------------------------------
BOOL EditState_TerrainSurface::OnRButtonUp(UINT nFlags, CPoint point)
{
	return AbstractTerrainEditState::OnRButtonUp(nFlags, point);
}
//--------------------------------------------------------------------------------
BOOL EditState_TerrainSurface::OnRButtonDown(UINT nFlags, CPoint point)
{
	AbstractTerrainEditState::OnRButtonDown(nFlags, point);

	return FALSE;
}
//--------------------------------------------------------------------------------
BOOL EditState_TerrainSurface::OnMouseMove(UINT nFlags, CPoint point)
{		
	AbstractTerrainEditState::OnMouseMove(nFlags, point);
	
	if(_isPainting)
	{
		TerrainGroup::RayResult r =
			EditorApp::getSingleton().PickOnTerrain(point.x, point.y);
		if(r.hit)
		{
			EditorConfig& cfg = EditorConfig::getSingleton();
			if(cfg.GetSnapToGrid())
			{
				const Vector2& gridSz = cfg.GetSnapGridSize();

				float sx = r.position.x > 0 ? 0.5f : -0.5f;
				float sz = r.position.z > 0 ? 0.5f : -0.5f;

				r.position.x = ((int)(r.position.x) / (int)gridSz.x) * (int)gridSz.x + gridSz.x * sx;
				r.position.z = ((int)(r.position.z) / (int)gridSz.y) * (int)gridSz.y + gridSz.y * sz;
			}

			TerrainGroup::TerrainList terrainList = GetBrushAffectedTiles(r.position);
			for (TerrainGroup::TerrainList::iterator ti = terrainList.begin();
				ti != terrainList.end(); ++ti)
			{
				Terrain* terrain = *ti;

				uint8 layerIdx = GetTextureLayerIndex(terrain, _selectedTexture);
				if(layerIdx != 0)
				{
					PaintSurface(layerIdx, terrain, r.position, _eraseLayer);
					EraseAboveLayer(layerIdx, terrain, r.position);
				}
				else
				{
					EraseAboveLayer(layerIdx, terrain, r.position);
				}
			}
		}
	}
	else
	{
		UpdateBrush(point);
	}

	return FALSE;
}
//--------------------------------------------------------------------------------
void EditState_TerrainSurface::OnActive()
{
	AbstractTerrainEditState::OnActive();

	ResPanel_Texture::getSingleton().Show();
	ResPanel_Texture::getSingleton().OnClickTreeItem(0, TerrainSurfaceResGroup);
}
//--------------------------------------------------------------------------------
void EditState_TerrainSurface::OnInActive()
{
	AbstractTerrainEditState::OnInActive();	
}
//--------------------------------------------------------------------------------
BOOL EditState_TerrainSurface::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	AbstractTerrainEditState::OnKeyDown(nChar, nRepCnt, nFlags);
	return FALSE;
}
//--------------------------------------------------------------------------------
void EditState_TerrainSurface::OnEvents( const String& eventName, const Any& eventContex, bool& stopBroadCast )
{
	AbstractTerrainEditState::OnEvents(eventName, eventContex, stopBroadCast);
	if(eventName == "SelectTexture")
	{
		_selectedTexture = any_cast<String>(eventContex);		

		EventsManager::getSingleton().CreateEvent(PropertyEvents::PropertyChanged, Any(String("TextureName")));
	}
}
//--------------------------------------------------------------------------------
bool EditState_TerrainSurface::HasTexture( Terrain* terrain, const String& texName )
{
	const TerrainLayerDeclaration& tld = terrain->getLayerDeclaration();
	uint8 cnt = terrain->getLayerCount();
	for(uint8 i = 0; i < cnt; ++i)
	{
		String difSpecTex = terrain->getLayerTextureName(i, 0);
		if(difSpecTex == texName)
		{
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------------------------
Ogre::uint8 EditState_TerrainSurface::GetTextureLayerIndex(Terrain* terrain, const String& texName)
{
	uint8 cnt = terrain->getLayerCount();
	for(uint8 i = 0; i < cnt; ++i)
	{
		String difSpecTex = terrain->getLayerTextureName(i, 0);
		if(difSpecTex == texName)
		{
			return i;
		}
	}

	return 0;
}
//--------------------------------------------------------------------------------
void EditState_TerrainSurface::PaintSurface(
	uint8 layerIdx, Terrain* terrain, const Vector3& pos, bool eraseLayer )
{
	Vector3 tsPosMin, tsPosMax;
	terrain->getTerrainPosition(pos + Vector3(-_brushRadius, 0, _brushRadius), &tsPosMin);
	terrain->getTerrainPosition(pos + Vector3(_brushRadius, 0, -_brushRadius), &tsPosMax);

	// Here returns the relative size of terrain coord

	Real imgSize = terrain->getLayerBlendMapSize();
	long startx = tsPosMin.x * imgSize;
	long starty = tsPosMin.y * imgSize;
	long endx = tsPosMax.x * imgSize;
	long endy = tsPosMax.y * imgSize;

	startx = std::max(startx, 0L);
	starty = std::max(starty, 0L);
	endx = std::min(endx, (long)imgSize);
	endy = std::min(endy, (long)imgSize);

	float dwx = endx - startx;
	float dwy = endy - starty;

	float brushWeight = _brushWeight;
	if(eraseLayer)
	{
		brushWeight = 1.0f;
	}

	TerrainLayerBlendMap* layer = terrain->getLayerBlendMap(layerIdx);
	for (long y = starty; y < endy; ++y)
	{
		Real dy = (float)(y - starty);

		for (long x = startx; x < endx; ++x)
		{
			Real dx = (float)(x - startx);

			float weight =
				_selectedBrush->GetWeight(dx, dy, dwx, dwy) * _brushWeight;

			size_t imgY = imgSize - y;
			float oldVal = layer->getBlendValue(x, imgY);
			
			AddOldBlendValue(layer, x, imgY, oldVal);

			float newVal = 0.0f;

			if (eraseLayer)
			{
				newVal = oldVal - weight;
			}
			else
			{
				newVal = oldVal + weight;
			}

			newVal = Math::Clamp(newVal, 0.0f, 1.0f);
			layer->setBlendValue(x, imgY, newVal);
		}
	}

	layer->update();
}
//--------------------------------------------------------------------------------
void EditState_TerrainSurface::EraseAboveLayer( uint8 layerIdx, Terrain* terrain, const Vector3& pos )
{
	uint8 cnt = terrain->getLayerCount();
	for(uint8 i = layerIdx + 1; i < cnt; ++i)
	{
		PaintSurface(i, terrain, pos, true);
	}
}
//--------------------------------------------------------------------------------
String EditState_TerrainSurface::GetBumpTexture( const String& diffTex )
{
	String file, ext;
	StringUtil::splitBaseFilename(diffTex, file, ext);
	file = file + "_NM." + ext;
	return file;
}
//--------------------------------------------------------------------------------
void EditState_TerrainSurface::AddOldBlendValue(
	TerrainLayerBlendMap* layer, size_t x, size_t y, float val )
{
	LayerBlendValueMap::iterator i = _oldData.find(layer);
	if (i == _oldData.end())
	{
		i = _oldData.insert(std::make_pair(layer, BlendValueMap())).first;
	}

	BlendValueMap& xMap = i->second;
	BlendValueMap::iterator ix = xMap.find(x);
	if(ix == xMap.end())
	{
		ix = xMap.insert(std::make_pair(x, BlendValue())).first;
	}

	BlendValue& yMap = ix->second;
	BlendValue::iterator iy = yMap.find(y);
	if(iy == yMap.end())
	{
		yMap.insert(std::make_pair(y, val));
	}
}
//--------------------------------------------------------------------------------
void EditState_TerrainSurface::CommitLayerBlendChange()
{
	LayerBlendValueMap::iterator i = _oldData.begin();
	for (; i != _oldData.end(); ++i)
	{
		EditorApp::getSingleton().GetCommandHistory()
			.AddCommand(OGRE_NEW Cmd_PaintLayer(
			i->first, i->second), false);
	}

	_oldData.clear();
}
//--------------------------------------------------------------------------------
