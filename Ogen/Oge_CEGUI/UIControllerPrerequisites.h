// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2008-02-18
// *************************************************************************************

#pragma once
#pragma warning(push)

//#pragma warning(disable : 4251)

// warning C4512: 'OIS::MouseEvent' : assignment operator could not be generated
#pragma warning(disable : 4512)

// warning C4245: 'initializing' : conversion from 'int' to 'const UTFString::size_type', signed/unsigned mismatch
#pragma warning(disable : 4245)

// warning C4100: 'xxx' : unreferenced formal parameter
#pragma warning(disable : 4100)

#include "OgeCEGUIExport.h"

#pragma warning(pop)


_OgeCEGUIExports CEGUI::Window* GetCEGUIWindow(const String& wndName);
_OgeCEGUIExports CEGUI::String  MultipleByteToUTF8( String str );
_OgeCEGUIExports std::string CeguiStringToStdString(CEGUI::String str);

//
// Convert MultipleByte to CEGUI's string
//
#define M2UTF8(str)	MultipleByteToUTF8(str)
