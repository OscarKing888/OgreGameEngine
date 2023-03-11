// UIController.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "UIControllerPrerequisites.h"
#include <CEGUIWindowManager.h>
#include <CEGUIExceptions.h>


CEGUI::Window* GetCEGUIWindow(const String& wndName)
{
	try
	{
		CEGUI::Window* wndPointer = CEGUI::WindowManager::getSingleton().getWindow(wndName.c_str());
		assert(wndPointer != NULL);
		return wndPointer;
	}
	catch (const CEGUI::UnknownObjectException e)
	{
		LogErrorIDf("UI", ("Window %s not found! %s:%d"), wndName.c_str(), __FILE__, __LINE__);
	}
	return 0;
}

CEGUI::String MultipleByteToUTF8( String str )
{
	if ( str == "" )
	{
		return CEGUI::String("");
	}
	else
	{
		//UTFString utfStr(str, false); 
		UTFString utfStr(str.c_str(), str.size()); 
		return CEGUI::String((CEGUI::utf8*)utfStr.asUTF8_c_str());
	}
}
/**
Convert a C style string encoded in UTF8 into another C style string encoded in UCS2.
*/
size_t	UTF8_to_UCS2(const char* src, wchar_t* dest, size_t dest_len, size_t src_len = 0)
{
	if (src_len == 0)
		src_len = strlen(src);

	size_t destCapacity = dest_len;

	// while there is data in the source buffer, and space in the dest buffer
	for (size_t idx = 0; ((idx < src_len) && (destCapacity > 0));)
	{
		wchar_t			cp;
		unsigned char	cu = src[idx++];

		if (cu < 0x80)
		{
			cp = (wchar_t)(cu);
		}
		else if (cu < 0xE0)
		{
			cp = ((cu & 0x1F) << 6);
			cp |= (src[idx++] & 0x3F);
		}
		else if (cu < 0xF0)
		{
			cp = ((cu & 0x0F) << 12);
			cp |= ((src[idx++] & 0x3F) << 6);
			cp |= (src[idx++] & 0x3F);
		}
		else
		{
			cp = ((cu & 0x07) << 18);
			cp |= ((src[idx++] & 0x3F) << 12);
			cp |= ((src[idx++] & 0x3F) << 6);
			cp |= (src[idx++] & 0x3F);
		}

		*dest++ = cp;
		--destCapacity;
	}

	return dest_len - destCapacity;
}

std::string CeguiStringToStdString(CEGUI::String str)
{
	std::string stdString;
	size_t s = strlen(str.c_str());
	wchar_t* wBuffer = OGE_ALLOC_T(wchar_t, s + 1);

	memset(wBuffer, 0, (s + 1) * sizeof(wchar_t));
	UTF8_to_UCS2(str.c_str(), wBuffer, s, s);
	std::wstring wstrBuff = wBuffer;

	//
	s = wstrBuff.size() * 2;
	char* _strBuffer = NULL;
	_strBuffer = OGE_ALLOC_T(char, s + 1);
	memset(_strBuffer, 0, s + 1);
	wcstombs(_strBuffer, wstrBuff.c_str(), s);
	stdString = _strBuffer; 
	OGE_FREE(wBuffer);
	OGE_FREE(_strBuffer);
	return stdString;
}