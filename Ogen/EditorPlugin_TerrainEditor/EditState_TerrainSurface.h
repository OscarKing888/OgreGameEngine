#pragma once
#include "AbstractTerrainEditState.h"
#include "CmdTypes.h"

// Editor State ID for terrain surface
extern int EditStateID_TerrainSurface;

class EditState_TerrainSurface
	: public AbstractTerrainEditState
	, public Singleton<EditState_TerrainSurface>
{
public:

	EditState_TerrainSurface(IStateManager* pMgr);
	virtual ~EditState_TerrainSurface();

	//virtual const String& GetTypeName() const;
	virtual void CreateToolbar(CMFCRibbonCategory* mainCategory);

	virtual void		OnActive();
	virtual void		OnInActive();

	const String& GetTextureName() const { return _selectedTexture; }
	void SetTextureName(const String& name) { _selectedTexture = name; }

	float GetBrushWeight() const { return _brushWeight; }
	void SetBrushWeight(float weight) { _brushWeight = weight; }
	String GetBumpTexture(const String& diffTex);

protected:

	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnRButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	virtual void BuildPropertyMap();
	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast);

	String	_selectedTexture;
	float	_brushWeight;
	bool	_eraseLayer;

	bool HasTexture(Terrain* terrain, const String& texName);
	uint8 GetTextureLayerIndex(Terrain* terrain, const String& texName);
	void PaintSurface(uint8 layerIdx,
		Terrain* terrain, const Vector3& pos, bool eraseLayer = false);

	void EraseAboveLayer(uint8 layerIdx, Terrain* terrain, const Vector3& pos);

	bool _isPainting;
	LayerBlendValueMap _oldData;
	void AddOldBlendValue(TerrainLayerBlendMap* layer, size_t x, size_t y, float val);
	void CommitLayerBlendChange();
};