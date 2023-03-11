#include "stdafx.h"
#include "CEGUIUDim.h"
#include "CEGUIString.h"
#include "CEGUIImageset.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIWindow.h"
#include "CEGUIWindowManager.h"

#include "CompositorImage.h"

// using namespace CEGUI;
// using namespace UIController;



CompositorImage::CompositorImage()
:_parentWindowName(_T("")),
_rowNum(1),
_columnNum(1)
{
	_bLoaded = false;
}

CompositorImage::~CompositorImage(void)
{
}


void CompositorImage::LoadDirect( const String& xmlFile )
{
	XMLDoc doc;
	if(doc.loadFromFile(xmlFile))
	{
		XMLElement e = doc.getRoot();
		_LoadConfig(e);
	}
	else
	{
		LogErrorIDf("UI", ("[CompositorImage::LoadDirect] Error xml file format:\"%s\""), xmlFile.c_str());
		assert(false);
	}
}

void CompositorImage::LoadFromResourceSystem( const String& xmlFile )
{
	DataStreamPtr dataStreamPtr =
		ResourceGroupManager::getSingleton().openResource(xmlFile);

	LoadFromXML(dataStreamPtr->getAsString());
}

void CompositorImage::LoadFromXML( const String& xmlString )
{
	XMLDoc doc;
	if(doc.loadFromXML(xmlString.c_str()))
	{
		XMLElement e = doc.getRoot();
		_LoadConfig(e);
	}
	else
	{
		LogErrorIDf("UI", ("[CompositorImage::LoadFromXML] Error xml string format:\"%s\""), xmlString.c_str());
		assert(false);
	}
}

void CompositorImage::_LoadConfig(XMLElement& rootNode)
{
	XMLElement e = rootNode.firstChild();
	while(!e.isNull())
	{
		if(e.name() == (_T("SubImage")))
		{
			unsigned int imageIndex = getIntAttribuate("Index", e);
			String imagesetName = e.getAttribute("Name");
			String imageFile = e.getAttribute("Imagefile");
			CEGUI::Imageset* imageset = NULL;
			try
			{
				imageset = &CEGUI::ImagesetManager::getSingleton().get(imagesetName);				
			}
			catch (...)
			{				
			}
			if (NULL == imageset)
			{
				imageset = &CEGUI::ImagesetManager::getSingleton().createFromImageFile( imagesetName, imageFile);
				_bLoaded = true;
			}
			_imagesetMap.insert(std::make_pair( imageIndex, imageset));

		}
		e = e.nextSibling();
	}	
}

bool CompositorImage::Init( CEGUI::String parentWindow, unsigned int rowNum /*= 1*/, unsigned int columnNum /*= 1 */ )
{
	assert( (rowNum * columnNum) <= _imagesetMap.size() );

	_parentWindowName = parentWindow;
	CEGUI::Window* window = CEGUI::WindowManager::getSingleton().getWindow( _parentWindowName );

	ImagesetMap::const_iterator imagetIterator = _imagesetMap.begin();
	for (unsigned int row = 0; row < rowNum; row++)
	{
		for ( unsigned int column = 0; column < columnNum; column++ )
		{
			if ( imagetIterator != _imagesetMap.end() )
			{
				CEGUI::String strTemp = "set:" + (imagetIterator->second)->getName() + " image:full_image";

				CEGUI::UVector2 sizeTemp     = CEGUI::UVector2(cegui_reldim( (float)1.000000 / (float)columnNum )
					, cegui_reldim( (float)1.000000 / (float)rowNum ));
				CEGUI::UVector2 positionTemp = CEGUI::UVector2(cegui_reldim( (float)column / (float)columnNum)
					, cegui_reldim( (float)row / (float)rowNum));

				CEGUI::String windowName = window->getName() + "/ImageWindow/SubImage"  + StringConverter::toString(row * columnNum + column);
				CEGUI::Window* windowTemp = CEGUI::WindowManager::getSingleton().createWindow( "WindowsLook/StaticImage", windowName);
				window->addChildWindow(windowTemp);

				windowTemp->setPosition(positionTemp);
				windowTemp->setSize(sizeTemp);

				windowTemp->setProperty("FrameEnabled", "false");
				windowTemp->setProperty("BackgroundEnabled", "false");
				windowTemp->setProperty("Image", strTemp );

				//LogConsolef("Cell [%s] position(%f, %f)", windowName.c_str(), windowTemp->getPosition().d_x.d_scale, windowTemp->getPosition().d_y.d_scale);
				//LogConsolef("Cell [%s] size(%f, %f)", windowName.c_str(), windowTemp->getSize().d_x.d_scale, windowTemp->getSize().d_y.d_scale);

				_windowList.push_back( windowTemp );
				imagetIterator++;
			}
		}
	}

	return true;
}

void CompositorImage::Show( bool on )
{
	for ( WindowList::iterator windowIterator = _windowList.begin(); windowIterator != _windowList.end(); windowIterator++ )
	{
		if ( (*windowIterator) != NULL )
		{
			(*windowIterator)->setVisible( on );
		}
	}
}

bool CompositorImage::Reset( void )
{
	for ( WindowList::iterator windowIterator = _windowList.begin(); windowIterator != _windowList.end(); windowIterator++ )
	{
		if ( (*windowIterator) != NULL )
		{
			CEGUI::WindowManager::getSingleton().destroyWindow(*windowIterator);
		}
	}
	_windowList.clear();

	if (_bLoaded)
	{
		for ( ImagesetMap::iterator imageIterator = _imagesetMap.begin(); imageIterator != _imagesetMap.end(); imageIterator++ )
		{
			if ( (imageIterator->second) != NULL )
			{
				CEGUI::ImagesetManager::getSingleton().destroy( *imageIterator->second );
			}
		}
	}
	
	_imagesetMap.clear();
	_bLoaded = false;

	return true;
}

WindowList CompositorImage::GetImageWindow( void )
{
	return _windowList;
}