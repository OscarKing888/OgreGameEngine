// ***************************************************************
//  CompositorImage   version:  1.0   ·  date: 03/07/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once
#include "UIControllerPrerequisites.h"

/**
*	将子图按N行M列的格式组合成一张图像
*/

/**	子图的StaticImage窗口句句柄容器*/ 
typedef std::vector<CEGUI::Window*> WindowList;
/**	子图的Imageset容器*/ 
typedef std::map<unsigned int, CEGUI::Imageset*> ImagesetMap;

class _OgeCEGUIExports CompositorImage 
	: public RefObject
{
public:
	explicit CompositorImage();
	virtual ~CompositorImage(void);

public:
	/** 直接从磁盘文件加载，LoadDirect("../Data/GUI/ImagesetConfig.xml") */
	void LoadDirect(const String& xmlFile);
	/** 通过Ogre的文件系统加载，使用默认的ResourceGroup(General) */
	void LoadFromResourceSystem(const String& xmlFile);
	/** 直接从xml字符串加载 */
	void LoadFromXML(const String& xmlString);

	/**	按指定N * M格式初始化界面*/ 
	bool Init( CEGUI::String parentWindow, unsigned int rowNum = 1, unsigned int columnNum = 1 );
	/** 清理加裁的配置数据和资源*/ 
	bool Reset( void );
	/**	显示或隐藏各个子图*/ 
	void Show(bool on);
	/**	获取子图窗口的指针*/
	WindowList GetImageWindow(void);

protected:
	WindowList _windowList;
	ImagesetMap _imagesetMap;

	/**	子图的父窗口名*/ 
	CEGUI::String _parentWindowName;

	/**	窗口子图的行和列数*/ 
	unsigned int _rowNum;
	unsigned int _columnNum;

	/** 从根结点解析配置数据*/
	void _LoadConfig(XMLElement& rootNode);
private:
	//标记是自己加载的ImageSet,还是别人加载的imageset,如果是别人加载的IMAGESET，那么自己不需要释放，如果是自己加载的IMAGESET，那么自己需要释放资源，这里不支持部分加载，部分使用别人加载的
	bool _bLoaded;

};

SmartPointer(CompositorImage);
