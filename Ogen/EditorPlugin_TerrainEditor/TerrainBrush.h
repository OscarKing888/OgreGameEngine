#pragma once

///
/// Terrain brush interface
///
class ITerrainBrush : public OgeAlloc
{
public:

	virtual const String& GetName() const = 0;
	virtual float GetWeight(float dX, float dY, float srcW, float srcH) = 0;
	virtual size_t GetWidth() const = 0;
	virtual size_t GetHeight() const = 0;
	virtual void SetPaintRadius(float r) = 0;
	virtual float GetPaintRadius() const = 0;
	virtual HBITMAP GetThumbImage() const = 0;
};

typedef vector<ITerrainBrush*>::type BrushList;
//--------------------------------------------------------------------------------
///
///  Abstract terrain brush class
///
class AbstractTerrainBrush : public ITerrainBrush
{
public:

	AbstractTerrainBrush();
	virtual ~AbstractTerrainBrush();

	virtual const String& GetName() const { return _name; }

	virtual size_t GetWidth() const { return _width; }
	virtual size_t GetHeight() const { return _height; }

	virtual void SetPaintRadius(float r) { _paintRadius = r; }
	virtual float GetPaintRadius() const { return _paintRadius; }

	virtual HBITMAP GetThumbImage() const{ return _hbmp; }
	void SetThumbImage(HBITMAP hbmp) { _hbmp = hbmp; }

protected:

	HBITMAP _hbmp;
	String _name;
	size_t _width;
	size_t _height;
	float _paintRadius;
};
//------------------------------------------------------------------------
///
/// Bitmap file terrain brush
///
class TerrainBrush_Bmp : public AbstractTerrainBrush
{
public:

	TerrainBrush_Bmp(const String& name, size_t width, size_t height, uchar* brushBuffer);
	virtual ~TerrainBrush_Bmp();

	//const String& GetFileName() const { return _fileName; }
	virtual float GetWeight(float dX, float dY, float srcW, float srcH);

	virtual HBITMAP GetThumbImage() const{ return _hbmp; }

protected:

	uchar* _brushBuffer;
	//String _fileName;
};
//------------------------------------------------------------------------
///
/// Terrain brush manager
///
class TerrainBrushManager : public SingletonMyers<TerrainBrushManager>
{
public:

	void RefreshBrushList();
	void ShutDown();
	size_t GetBrushCount();

	void AddBrush(ITerrainBrush* br);
	ITerrainBrush* GetBrush(int idx);
	ITerrainBrush* GetBrush(const String& brushName);
	size_t GetBrushIndex(const String& brushName);
	StringVector GetBrushNameList() const;

	const BrushList& GetBrushList() const { return _brushList; }

protected:

	BrushList	_brushList;
};
//------------------------------------------------------------------------
///
/// TerrainBrush_Apex
///
class TerrainBrush_Apex : public AbstractTerrainBrush
{
public:

	TerrainBrush_Apex();
	virtual ~TerrainBrush_Apex();

	virtual float GetWeight(float dX, float dY, float srcW, float srcH);
};
//------------------------------------------------------------------------
///
///  TerrainBrush_Obtuse
///
class TerrainBrush_Obtuse : public AbstractTerrainBrush
{
public:

	TerrainBrush_Obtuse();
	virtual ~TerrainBrush_Obtuse();

	virtual float GetWeight(float dX, float dY, float srcW, float srcH);
};
//------------------------------------------------------------------------
///
/// TerrainBrush_Cone
///
class TerrainBrush_Cone : public AbstractTerrainBrush
{
public:

	TerrainBrush_Cone();
	virtual ~TerrainBrush_Cone();

	virtual float GetWeight(float dX, float dY, float srcW, float srcH);
};
//------------------------------------------------------------------------
