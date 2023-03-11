// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-4-12
// *************************************************************************************

#pragma once
#include "UIControllerPrerequisites.h"

class _OgeCEGUIExports Ogre2CEImage : public RefObject
{
public:

	explicit Ogre2CEImage(
		const String& textureFileName,				// 贴图的文件名,一般就是配置在material中的图像文件名
		float x = 0.0f, float y = 0.0f,						// 生成的image在这张贴图上的起始点比例值
		float width = 1.0f, float height = 1.0f,			// 生成的贴图占整张图的大小比例,1为全部
		float xOffset = 0.0f, float yOffset = 0.0f);		// 在绘制时进行偏移,主要是用在字体图像上

	virtual ~Ogre2CEImage(void);

	String GetImageName() const { return _imageName; }

	CEGUI::Imageset* GetCEGUIImageSet();
	CEGUI::Image* GetCEGUIImage();

protected:

	String			_imageName;

	CEGUI::Imageset*		_ceguiImageSet;
	CEGUI::Image*			_ceguiImage;

	static int _autoID;
};

SmartPointer(Ogre2CEImage);
typedef vector<Ogre2CEImagePtr>::type Ogre2CEImageList;

