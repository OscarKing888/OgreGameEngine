========================================================================
    DYNAMIC LINK LIBRARY : Core Project Overview
========================================================================

AppWizard has created this Core DLL for you.

This file contains a summary of what you will find in each of the files that
make up your Core application.


Core.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

Core.cpp
    This is the main DLL source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named Core.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

========================================================================
2009-08-26
========================================================================
1.XML相关功能整理完成，加入XMLConfigReader类。
2.加入UTF8字符串转换功能类，从OGRE的UTF8String复制过来，并加入到MBCS转换的功能。
3.整理原引擎获取类名字符串函数。
4.文件属性相关工具函数整理。
5.调试输出到VS IDE Output面板函数加入debugTrace，使用DEBUG_TRACE宏调用。
6.编码风格统一与OGRE一致。
7.StringUtil与StringConverterBase整理大部分平台无关函数到Core中。