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
		const String& textureFileName,				// ��ͼ���ļ���,һ�����������material�е�ͼ���ļ���
		float x = 0.0f, float y = 0.0f,						// ���ɵ�image��������ͼ�ϵ���ʼ�����ֵ
		float width = 1.0f, float height = 1.0f,			// ���ɵ���ͼռ����ͼ�Ĵ�С����,1Ϊȫ��
		float xOffset = 0.0f, float yOffset = 0.0f);		// �ڻ���ʱ����ƫ��,��Ҫ����������ͼ����

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

