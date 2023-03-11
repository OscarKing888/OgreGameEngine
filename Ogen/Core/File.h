// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-24
// *************************************************************************************

#pragma once
#include "CorePrerequisites.h"

class _CoreExport File
{
public:

	static bool isFileExist(const String& filePath);
	static bool	isDirExist(const String& dirPath);
	static bool makeDir(const String& dirName);
	static bool deleteFileFromDisk(const String& fiePath);

	static String getDirFromFullPath(const String& fullPath);		

	static void removeReadonly(const String& fileName);
};

