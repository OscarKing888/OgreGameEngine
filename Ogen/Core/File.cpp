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
#include "stdafx.h"
#include "File.h"
#include "OSAPI.h"
#include "tstring.h"

#include <direct.h>
#include <io.h>



	bool File::isFileExist(const String& filePath)
	{
		return _taccess(filePath.c_str(), 0) != -1;
	}

	//--------------------------------------------------------------------------------------

	bool File::isDirExist(const String& dirPath)
	{
		return _taccess(dirPath.c_str(), 0) != -1;
	}

	//--------------------------------------------------------------------------------------

	bool File::makeDir(const String& dirNameSrc)
	{
		String dirName = StringUtil::standardisePath(dirNameSrc);
		TCHAR strDir[MAX_PATH];
		TCHAR strPath[MAX_PATH];
		TCHAR strName[MAX_PATH];
		TCHAR strExt[MAX_PATH];
		_tsplitpath(dirName.c_str(), strDir, strPath, strName, strExt);

		StringVector dirList = StringUtil::split(strPath, ("/"));

		StringVector::iterator it = dirList.begin();
		while (it != dirList.end())
		{
			String str = *it;
			if(str.empty())
			{
				it = dirList.erase(it);
			}
			else
			{
				++it;
			}
		}

		size_t cnt = dirList.size();

		for (size_t i = 0; i < cnt; ++i)
		{
			String tmpDir;
			String dirPath(strDir);
			if(!dirPath.empty())
			{
				tmpDir = dirPath + ("/");
			}

			for(size_t j = 0; j <= i; ++j)
			{
				tmpDir += dirList[j] + ("/");
			}

			if(!isDirExist(tmpDir.c_str()))
			{
				//tmpDir = ("\"") + tmpDir + ("\"");
				_tmkdir(tmpDir.c_str());
			}
		}

		return isDirExist(dirName);
	}

	//--------------------------------------------------------------------------------------

	bool File::deleteFileFromDisk(const String& fiePath)
	{
		return _tremove(fiePath.c_str()) == 0;
	}

	//--------------------------------------------------------------------------------------

	String File::getDirFromFullPath(const String& fullPath)
	{
		TCHAR strDir[MAX_PATH];
		TCHAR strPath[MAX_PATH];
		TCHAR strName[MAX_PATH];
		TCHAR strExt[MAX_PATH];
		_tsplitpath(fullPath.c_str(), strDir, strPath, strName, strExt);

		String strDirPath = strDir;
		strDirPath += strPath;
		//strDirPath = StringUtil::standardisePath(strDirPath);
		return strDirPath;
	}

	void File::removeReadonly(const String& fileName)
	{
		DWORD attr = ::GetFileAttributes(fileName.c_str());
		attr &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(fileName.c_str(), attr);
	}
