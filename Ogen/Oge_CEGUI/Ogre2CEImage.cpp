// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-4-12
// *************************************************************************************

#include "stdafx.h"
#include "Ogre2CEImage.h"
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUIRenderer.h>
#include <CEGUISystem.h>
#include <CEGUIImagesetManager.h>
#include <CEGUIImageset.h>



int Ogre2CEImage::_autoID = 0;

Ogre2CEImage::Ogre2CEImage(const String& textureFileName,
						   float x, float y, float width, float height, float xOffset, float yOffset)
{
	// Retrieve CEGUI texture for the RTT
	CEGUI::Renderer* render = CEGUI::System::getSingleton().getRenderer();
	Ogre::TexturePtr ogreTex = Ogre::TextureManager::getSingleton().getByName(textureFileName);
	CEGUI::OgreRenderer* ogreRender = dynamic_cast<CEGUI::OgreRenderer*>(render);
	CEGUI::Texture* tex =  &ogreRender->createTexture(ogreTex);

	String imgSet = formatString(("Ogre2CEImage_%d"), _autoID);

	_ceguiImageSet = &CEGUI::ImagesetManager::getSingleton().create(imgSet, *tex);
	_ceguiImageSet->setAutoScalingEnabled(false);

	CEGUI::Size sz = tex->getSize();
	_ceguiImageSet->defineImage(("Ogre2CEImage"), 
		CEGUI::Point(sz.d_width * x, sz.d_height * y),
		CEGUI::Size(sz.d_width * width, sz.d_width * height),
		CEGUI::Point(xOffset, yOffset));

	_imageName = ("set:") + imgSet + (" image:Ogre2CEImage");

	_ceguiImage = (CEGUI::Image*)&CEGUI::ImagesetManager::getSingleton().get(imgSet).getImage(("Ogre2CEImage"));
	assert(_ceguiImage != 0);

	++_autoID;
}

Ogre2CEImage::~Ogre2CEImage(void)
{
}