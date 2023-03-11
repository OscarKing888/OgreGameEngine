#pragma once

///
/// Grid class for displaying on editor views
///
class _EditorCoreExport Grid : public OgeAlloc
{
public:

	Grid();
	virtual ~Grid();

	float CellSizeOnX() const { return _cellSizeOnX; }
	void CellSizeOnX(float val) { _cellSizeOnX = val; _UpdateGrid(); }

	float CellSizeOnZ() const { return _cellSizeOnZ; }
	void CellSizeOnZ(float val) { _cellSizeOnZ = val; _UpdateGrid(); }

	int BigCellCntOnX() const { return _bigCellCntOnX; }
	void BigCellCntOnX(int val) { _bigCellCntOnX = val; _UpdateGrid(); }

	int BigCellCntOnZ() const { return _bigCellCntOnZ; }
	void BigCellCntOnZ(int val) { _bigCellCntOnZ = val; _UpdateGrid(); }

	int XCellCnt() const { return _xCellCnt; }
	void XCellCnt(int val) { _xCellCnt = val; _UpdateGrid(); }

	int ZCellCnt() const { return _zCellCnt; }
	void ZCellCnt(int val) { _zCellCnt = val; _UpdateGrid(); }

	const Ogre::ColourValue& MainAxisClr() const { return _mainAxisClr; }
	void MainAxisClr(const Ogre::ColourValue& val) { _mainAxisClr = val; _UpdateGrid(); }

	const Ogre::ColourValue& CellAxisClr() const { return _cellAxisClr; }
	void CellAxisClr(const Ogre::ColourValue& val) { _cellAxisClr = val;  _UpdateGrid(); }

	const Ogre::ColourValue& BigCellAxisClr() const { return _bigCellAxisClr; }
	void BigCellAxisClr(const Ogre::ColourValue& val) { _bigCellAxisClr = val; _UpdateGrid(); }

	Ogre::uint32 VisibilityFlag() const { return _visibilityFlag; }
	void VisibilityFlag(Ogre::uint32 val);

	bool IsVisible() const { return _visible; }
	void SetVisible(bool val);

	SceneNode* GetNode() const { return _node; }

	void Offset(const Vector3& pos);
	const Ogre::Vector3& Offset() const;

protected:

	typedef vector<DynamicLines*>::type LinesList;
	LinesList _lineList;

	float _cellSizeOnX;
	float _cellSizeOnZ;
	int _bigCellCntOnX;
	int _bigCellCntOnZ;
	int _xCellCnt;
	int _zCellCnt;
	ColourValue _mainAxisClr;
	ColourValue _cellAxisClr;
	ColourValue _bigCellAxisClr;

	uint32 _visibilityFlag;

	SceneNode* _node;
	bool _visible;
	Vector3 _offset;

	void _UpdateGrid();
	void Destroy();
};