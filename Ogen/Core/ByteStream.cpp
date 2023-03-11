#include "stdafx.h"
#include "ByteStream.h"
#include <hash_map>

XGUID XGUID ::createGUID(uint16 typeFlag)
{
	// 前100个为系统物体准备，如SkyBox等
	static uint16 g_autoID = 100;

	++g_autoID;

	time_t tm;
	time(&tm);
	
	XGUID guid;
	guid._value = tm;
	guid._value <<= 32;
	uint32 low32 = (typeFlag << 16);
	low32 |= (0xFFFF0000 | g_autoID);
	guid._value |= (0xFFFFFFFF00000000 | low32);

	return  guid;
}

XGUID::XGUID()
: _value(0)
{		
}

XGUID::XGUID(const String& str)
{
	fromString(str);
}

XGUID::XGUID(const XGUID& src)
{
	_value = src._value;
}

String XGUID::toString() const
{		
	String str;
	str = formatString("%I64X", _value);
	return str;
}

void XGUID::fromString(const String& str)
{
	int readCnt = sscanf_s(str.c_str(), "%I64X", &_value);
	if(readCnt != 1)
	{
		*this = createGUID();
	}
}

const XGUID& XGUID::operator=(const XGUID& src)
{
	if(this == &src)
	{
		return *this;
	}

	_value = src._value;
	return *this;
}

bool XGUID::operator==(const XGUID& src) const
{
	return (_value == src._value);
}

bool XGUID::operator!=(const XGUID& src) const
{
	return _value != src._value;
}

bool XGUID::operator<(const XGUID& src) const
{
	return _value < src._value;
}

bool XGUID::operator>(const XGUID& src) const
{
	return _value > src._value;
}

//------------------------------------------------------------------------
OutStream& operator << (OutStream& is, const std::string& v)
{
	size_t s = v.size();
	if (s > OGE_MAX_TMP_STR_BUFFER_SIZE)
	{
		throw InvalidStringException();
	}

	is.__write(&s, sizeof(size_t));

	if (s == 0)
	{
		return is;
	}

	is.__write((void*)v.c_str(), s);
	return is;
}
//----------------------------------------------------------------------------------------------
OutStream& operator << (OutStream& is, const std::wstring& v)
{
	size_t s = v.size();
	if (s > OGE_MAX_TMP_STR_BUFFER_SIZE)
	{
		throw InvalidStringException();
	}

	is.__write(&s, sizeof(size_t));

	if (s == 0)
	{
		return is;
	}

	is.__write((void*)v.c_str(), s * sizeof(wchar_t));
	return is;
}
//----------------------------------------------------------------------------------------------
InStream& operator >> (InStream& os, std::string& v)
{
	size_t l = 0;
	os.__read(&l, sizeof(size_t));

	if (l == 0)
	{
		v = std::string("");
		return os;
	}

	if (l > OGE_MAX_TMP_STR_BUFFER_SIZE)
	{
		throw InvalidStringException();
	}

	char buf[OGE_MAX_TMP_STR_BUFFER_SIZE] = {'\0'};
	os.__read((void*)buf, l);
	v = std::string(buf);
	return os;
}
//----------------------------------------------------------------------------------------------
InStream& operator >> (InStream& os, std::wstring& v)
{
	size_t l = 0;
	os.__read(&l, sizeof(size_t));

	if (l == 0)
	{
		v = std::wstring(L"");
		return os;
	}

	if (l > OGE_MAX_TMP_STR_BUFFER_SIZE)
	{
		throw InvalidStringException();
	}

	wchar_t buf[OGE_MAX_TMP_STR_BUFFER_SIZE] = {'\0'};
	os.__read((void*)buf, l);
	v = std::wstring(buf);

	return os;
}

