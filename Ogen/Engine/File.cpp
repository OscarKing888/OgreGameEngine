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

#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>

String FileEx::GetResourceFilePath(const String& fileName, bool withFileName)
{
	String pth;

	ResourceGroupManager& rgm = ResourceGroupManager::getSingleton();
	StringVector resGrps = rgm.getResourceGroups();
	for (size_t i = 0; i < resGrps.size(); ++i)
	{
		String resGrp = resGrps[i];
		FileInfoListPtr fil = rgm.findResourceFileInfo(resGrp, fileName);
		if(!fil->empty())
		{
			FileInfo& info = fil->front();
			pth = info.archive->getName();
			pth = StringUtil::standardisePath(pth);

			if(withFileName)
			{
				pth += info.filename;
			}

			break;
		}
	}


	return pth;
}
