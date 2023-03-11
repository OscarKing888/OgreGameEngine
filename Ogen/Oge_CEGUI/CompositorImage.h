// ***************************************************************
//  CompositorImage   version:  1.0   ��  date: 03/07/2008
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
*	����ͼ��N��M�еĸ�ʽ��ϳ�һ��ͼ��
*/

/**	��ͼ��StaticImage���ھ�������*/ 
typedef std::vector<CEGUI::Window*> WindowList;
/**	��ͼ��Imageset����*/ 
typedef std::map<unsigned int, CEGUI::Imageset*> ImagesetMap;

class _OgeCEGUIExports CompositorImage 
	: public RefObject
{
public:
	explicit CompositorImage();
	virtual ~CompositorImage(void);

public:
	/** ֱ�ӴӴ����ļ����أ�LoadDirect("../Data/GUI/ImagesetConfig.xml") */
	void LoadDirect(const String& xmlFile);
	/** ͨ��Ogre���ļ�ϵͳ���أ�ʹ��Ĭ�ϵ�ResourceGroup(General) */
	void LoadFromResourceSystem(const String& xmlFile);
	/** ֱ�Ӵ�xml�ַ������� */
	void LoadFromXML(const String& xmlString);

	/**	��ָ��N * M��ʽ��ʼ������*/ 
	bool Init( CEGUI::String parentWindow, unsigned int rowNum = 1, unsigned int columnNum = 1 );
	/** ����Ӳõ��������ݺ���Դ*/ 
	bool Reset( void );
	/**	��ʾ�����ظ�����ͼ*/ 
	void Show(bool on);
	/**	��ȡ��ͼ���ڵ�ָ��*/
	WindowList GetImageWindow(void);

protected:
	WindowList _windowList;
	ImagesetMap _imagesetMap;

	/**	��ͼ�ĸ�������*/ 
	CEGUI::String _parentWindowName;

	/**	������ͼ���к�����*/ 
	unsigned int _rowNum;
	unsigned int _columnNum;

	/** �Ӹ���������������*/
	void _LoadConfig(XMLElement& rootNode);
private:
	//������Լ����ص�ImageSet,���Ǳ��˼��ص�imageset,����Ǳ��˼��ص�IMAGESET����ô�Լ�����Ҫ�ͷţ�������Լ����ص�IMAGESET����ô�Լ���Ҫ�ͷ���Դ�����ﲻ֧�ֲ��ּ��أ�����ʹ�ñ��˼��ص�
	bool _bLoaded;

};

SmartPointer(CompositorImage);
