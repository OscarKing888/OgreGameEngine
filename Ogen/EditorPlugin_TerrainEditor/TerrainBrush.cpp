#include "stdafx.h"
#include "TerrainBrush.h"

AbstractTerrainBrush::AbstractTerrainBrush()
: _paintRadius(500.0f)
, _width(0)
, _height(0)
, _hbmp(0)
{
}
//--------------------------------------------------------------------------------
AbstractTerrainBrush::~AbstractTerrainBrush()
{

}
//--------------------------------------------------------------------------------
// TerrainBrush_Bmp
//--------------------------------------------------------------------------------
TerrainBrush_Bmp::TerrainBrush_Bmp(const String& name, size_t width, size_t height, uchar* brushBuffer)
: _brushBuffer(brushBuffer)
{
	_name = name;
	_width = width;
	_height = height;

	size_t start = name.find_first_of('_');
	if(start !=String::npos)
	{
		_name = name.substr(0, start);
	}
}
//--------------------------------------------------------------------------------
TerrainBrush_Bmp::~TerrainBrush_Bmp()
{
	OGE_FREE(_brushBuffer);
	_brushBuffer = 0;
}
//--------------------------------------------------------------------------------
float TerrainBrush_Bmp::GetWeight(float dX, float dY, float srcW, float srcH)
{
	size_t x = (size_t)MathUtil::ProjectValueWithRange(dX, 0, srcW, 0, (float)_width);
	size_t y = (size_t)MathUtil::ProjectValueWithRange(dY, 0, srcH, 0, (float)_height);
	uchar* pos = _brushBuffer + y * _width + x;
	uchar val = *pos;
	float r = float(val) / 255.0f;
	return r;
}
//--------------------------------------------------------------------------------
// TerrainBrushManager
//--------------------------------------------------------------------------------
void TerrainBrushManager::RefreshBrushList()
{
	ShutDown();

	// add sin apex brush
	TerrainBrush_Apex* sinApex = OGRE_NEW TerrainBrush_Apex();
	_brushList.push_back(sinApex);

	TerrainBrush_Obtuse* obtuseApex = OGRE_NEW TerrainBrush_Obtuse();
	_brushList.push_back(obtuseApex);	

	TerrainBrush_Cone* coneBrush = OGRE_NEW TerrainBrush_Cone();
	_brushList.push_back(coneBrush);
	

	// add bmp brushes from file
	StringVectorPtr brushList = ResourceGroupManager::getSingleton().findResourceNames("EditorBrush", "*.bmp");
	for (size_t i = 0; i < brushList->size(); ++i)
	{
		String fileName = (*brushList)[i];
		TexturePtr tex = TextureManager::getSingleton().load(fileName, "EditorBrush",
			TEX_TYPE_2D, MIP_DEFAULT, 1.0f, false, PF_A8R8G8B8);

		size_t w = tex->getWidth();
		size_t h = tex->getHeight();
		
		uchar* buffer = OGE_ALLOC_T(uchar, w * h);
		uint32* srcBuffer =  OGE_ALLOC_T(uint32, w * h);
		
		HardwarePixelBufferSharedPtr texBuffer = tex->getBuffer();
		PixelFormat fmt = texBuffer->getFormat();
		assert(fmt == PF_A8R8G8B8);
		PixelBox box(w, h, texBuffer->getDepth(), PF_A8R8G8B8, srcBuffer);
		texBuffer->blitToMemory(box);

		for (size_t y = 0; y < h; ++y)
		{
			for (size_t x = 0; x < w; ++x)
			{
				uchar* pDest = buffer + y * w + x;
				uint32* pSrc = srcBuffer + y * w + x;
				uint32 clr = *pSrc;
				*pDest = 0xff - (uchar)GetGValue(clr);
			}
		}

		OGE_FREE(srcBuffer);

		TerrainBrush_Bmp* brush = OGRE_NEW TerrainBrush_Bmp(fileName, w, h , buffer);
		_brushList.push_back(brush);

		String filePath = FileEx::GetResourceFilePath(fileName);
		HBITMAP bmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), filePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		brush->SetThumbImage(bmp);
	}
}
//--------------------------------------------------------------------------------
void TerrainBrushManager::ShutDown()
{
	for (size_t i = 0; i < _brushList.size(); ++i)
	{
		TerrainBrush_Bmp* brush = dynamic_cast<TerrainBrush_Bmp*>(_brushList[i]);
		if(brush)
		{
			OGRE_DELETE brush;
		}
	}

	_brushList.clear();
}
//--------------------------------------------------------------------------------
size_t TerrainBrushManager::GetBrushCount()
{
	return _brushList.size();
}
//--------------------------------------------------------------------------------
ITerrainBrush* TerrainBrushManager::GetBrush(int idx)
{
	assert(idx < (int)_brushList.size());
	return _brushList[idx];
}
//--------------------------------------------------------------------------------
ITerrainBrush* TerrainBrushManager::GetBrush(const String& brushName)
{
	for (size_t i = 0; i < _brushList.size(); ++i)
	{
		ITerrainBrush* br = _brushList[i];
		if(br->GetName() == brushName)
		{
			return br;
		}
	}
	return 0;
}
//--------------------------------------------------------------------------------
size_t TerrainBrushManager::GetBrushIndex(const String& brushName)
{
	for (size_t i = 0; i < _brushList.size(); ++i)
	{
		ITerrainBrush* br = _brushList[i];
		if(br->GetName() == brushName)
		{
			return i;
		}
	}

	assert(false);
	LogErrorIDf(EditorLogID, "[TerrainBrush::GetBrushIndex] can't found brush index by name:[%s]", brushName.c_str());
	return -1;
}
//--------------------------------------------------------------------------------
StringVector TerrainBrushManager::GetBrushNameList() const
{
	StringVector lst;
	for (size_t i = 0; i < _brushList.size(); ++i)
	{
		ITerrainBrush* br = _brushList[i];
		lst.push_back(br->GetName());
	}

	return lst;
}
//--------------------------------------------------------------------------------
void TerrainBrushManager::AddBrush(ITerrainBrush* br)
{
	_brushList.push_back(br);
}
//--------------------------------------------------------------------------------
// TerrainBrush_Apex
//--------------------------------------------------------------------------------
TerrainBrush_Apex::TerrainBrush_Apex()
{
	_name = "Apex";

	String apexThumb = FileEx::GetResourceFilePath("EditorBrushThumb/Apex.bmp");
	HBITMAP bmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), apexThumb.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SetThumbImage(bmp);
}
//--------------------------------------------------------------------------------
TerrainBrush_Apex::~TerrainBrush_Apex()
{

}
//--------------------------------------------------------------------------------
float TerrainBrush_Apex::GetWeight(float dX, float dY, float srcW, float srcH)
{
#if 0
	Real weight = std::min((Real)1.0, 
		Math::Sqrt(dY * dY + dX * dX) / Real(0.5 * 0.5 * srcW * srcW));
	weight = 1.0 - (weight * weight);
	return weight;
#else
	float x = dX / srcW - 0.5f;
	float y = dY / srcH - 0.5f;
	// [-0.5, 0.5]
	float r = sqrt(x * x + y * y);

	r = r * Math::PI;
	return Math::Clamp(cos(r), 0.0f, 1.0f);
#endif
}
//--------------------------------------------------------------------------------
// TerrainBrush_Obtuse
//--------------------------------------------------------------------------------
TerrainBrush_Obtuse::TerrainBrush_Obtuse()
{
	_name = "Obtuse";

	String obtuseThumb = FileEx::GetResourceFilePath("EditorBrushThumb/Obtuse.bmp");
	HBITMAP bmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), obtuseThumb.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SetThumbImage(bmp);
}
//--------------------------------------------------------------------------------
TerrainBrush_Obtuse::~TerrainBrush_Obtuse()
{

}
//--------------------------------------------------------------------------------
float TerrainBrush_Obtuse::GetWeight(float dX, float dY, float srcW, float srcH)
{
	float x = dX / srcW - 0.5f;
	float y = dY / srcH - 0.5f;
	// [-0.5, 0.5]
	float r = sqrt(x * x + y * y);

	if((r > -0.4f) && (r < 0.4f))
	{
		return 0.8f;
	}

	r = r * Math::PI;
	return Math::Clamp(cos(r), 0.0f, 1.0f);
}
//--------------------------------------------------------------------------------
// TerrainBrush_Cone
//--------------------------------------------------------------------------------
TerrainBrush_Cone::TerrainBrush_Cone()
{
	_name = "Cone";

	String thumbImg = FileEx::GetResourceFilePath("EditorBrushThumb/Cone.bmp");
	HBITMAP bmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), thumbImg.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SetThumbImage(bmp);

}
//--------------------------------------------------------------------------------
TerrainBrush_Cone::~TerrainBrush_Cone()
{

}
//--------------------------------------------------------------------------------
float TerrainBrush_Cone::GetWeight( float dX, float dY, float srcW, float srcH )
{
	float x = dX / srcW - 0.5f;
	float y = dY / srcH - 0.5f;
	float r = sqrt(x * x + y * y);
	return Ogre::Math::Clamp(1.0f - r, 0.0f, 1.0f);
}
//--------------------------------------------------------------------------------
