#pragma once

class ITerrainBrush;
class Grid;

class AbstractTerrainEditState
	: public AbstractEditState
	, public IEventLisitener
	, public PropertyObject
{
public:

	AbstractTerrainEditState(IStateManager* pMgr);

	void CreateBrushModel( String name );
	virtual ~AbstractTerrainEditState();

	virtual void		OnActive();
	virtual void		OnInActive();

	// 根据当前地形刷大小及点取坐标返回影响到的Tile列表
	TerrainGroup::TerrainList GetBrushAffectedTiles(const Vector3& pos);

	void UpdateWholeTerrain();

	// properties
	float GetBrushRadius() const { return _brushRadius; }
	void SetBrushRadius(float radius);

	ITerrainBrush* GetSelectedBrush() const { return _selectedBrush; }
	void SetSelectedBrush(ITerrainBrush* br) { _selectedBrush = br; }	

	StringVector GetBrushList() const;
	void SetBrushList(const StringVector& lst);

	void SetBrushDownPosition(const Vector3& pos) { _brushDownPoint = pos; }
	const Vector3& GetBrushDownPosition() const { return _brushDownPoint; }

	/// 是否显示每个地形块的边界框
	bool GetShowTileBoundingBox() const { return _showTileBoundingBox; }
	void SetShowTileBoundingBox(bool val);

	void UpdateBrush(const Vector3& brushPosition);
	void UpdateBrush(const CPoint& mousePoint);

	static void CreateTerrainBoundingBox();
	static void ShowTerrainBoundingBox(bool on);
	static void DestroyTerrainBoundingBox();

protected:

	DynamicLines*		_brushRange;
	SceneNode*			_brushNode;
	Entity*				_brushAnchor;
	Vector3				_brushDownPoint;
	FogMode				_fogMode;
	float				_brushRadius;
	ITerrainBrush*		_selectedBrush;
	bool				_showTileBoundingBox;

	static Grid*			_grid;
	

	virtual void BuildPropertyMap();

	SceneManager* GetSceneManager();
	virtual void OnEvents(const String& eventName, const Any& eventContex, bool& stopBroadCast);

	virtual BOOL OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);


};