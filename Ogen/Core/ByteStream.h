// *************************************************************************************
// 
// 	Copyright (c) 2009 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2009-6-14
// *************************************************************************************

#pragma once
#include "CorePrerequisites.h"

struct _CoreExport XGUID
{
	uint64 _value;

	String toString() const;
	void fromString(const String& str);
	static XGUID createGUID(uint16 typeFlag = std::numeric_limits<uint16>::max());

	XGUID();
	XGUID(const String& str);
	XGUID(const XGUID& src);

	const XGUID& operator=(const XGUID& src);
	bool operator==(const XGUID& src) const;
	bool operator!=(const XGUID& src) const;
	bool operator<(const XGUID& src) const;
	bool operator>(const XGUID& src) const;
};

class _CoreExport InvalidStringException : public std::exception
{
public:
	virtual const char *what() const _THROW0()
	{	
		// return pointer to message string
		return "invalid string length";
	}
};

class _CoreExport OutOfBoundException : public std::exception
{
public:
	virtual const char *what() const throw()
	{	
		// return pointer to message string
		return "read exceeds the size of the buf";
	}
};

class _CoreExport Stream
{
public:
	const ByteBuffer& getByteStream()
	{
		return _stream;
	}

	virtual size_t getSize()
	{
		return _pointer;
	}

protected:
	ByteBuffer	_stream;
	size_t		_pointer;
};

class _CoreExport OutStream : public Stream
{
public:
	OutStream(int initsize = 32)
	{
		_stream.reserve(initsize);
		_pointer = 0;
	}

	inline void __write(void* buf, size_t size)
	{
		byte* b = (byte*)buf;
		while(size>0)
		{
			_stream.push_back(b[0]);
			_pointer++;
			size--;
			b++;
		}
	}

};

inline OutStream& operator << (OutStream& is, int v)
{
	is.__write((void*)&v, sizeof(v));
	return is;
}

inline OutStream& operator << (OutStream& is, unsigned int v)
{
	is.__write((void*)&v, sizeof(v));
	return is;
}

inline OutStream& operator << (OutStream& is, short v)
{
	is.__write((void*)&v, sizeof(v));
	return is;
}

inline OutStream& operator << (OutStream& is, unsigned short v)
{
	is.__write((void*)&v, sizeof(v));
	return is;
}

inline OutStream& operator << (OutStream& is, bool v)
{
	is.__write((void*)&v, sizeof(v));
	return is;
}

inline OutStream& operator << (OutStream& is, float v)
{
	is.__write((void*)&v, sizeof(v));
	return is;
}

inline OutStream& operator << (OutStream& is, double v)
{
	is.__write((void*)&v, sizeof(v));
	return is;
}

inline OutStream& operator << (OutStream& is, __int64 v)
{
	is.__write((void*)&v, sizeof(v));
	return is;
}

inline OutStream& operator << (OutStream& is, unsigned __int64 v)
{
	is.__write((void*)&v, sizeof(v));
	return is;
}

inline OutStream& operator << (OutStream& is, char v)
{
	is.__write((void*)&v, sizeof(v));
	return is;
}

inline OutStream& operator << (OutStream& is, byte v)
{
	is.__write((void*)&v, sizeof(v));
	return is;
}

inline OutStream& operator << (OutStream& is, const XGUID& v)
{
	is.__write((void*)&v, sizeof(v._value));
	return is;
}	

_CoreExport OutStream& operator << (OutStream& is, const std::string& v);
_CoreExport OutStream& operator << (OutStream& is, const std::wstring& v);

inline OutStream& operator << (OutStream& is, const ByteBuffer& v)
{
	size_t sz = v.size();
	is.__write((void*)&sz, sizeof(size_t));

	if(sz > 0)
	{
		is.__write((void*)&v[0], sz);
	}

	return is;
}

// 	template<class T>
// 	inline OutStream& operator << (OutStream& is, const std::list<T>& v)
// 	{
// 		size_t sz = v.size();
// 		is.__write((void*)&sz, sizeof(size_t));
// 		for (size_t i = 0; i < sz; ++i)
// 		{
// 			is.__write((void*)&v[i], sizeof(T));
// 		}
// 		return is;
// 	}

template<class K, class V>
inline OutStream& operator << (OutStream& is, const std::map<K, V>& v)
{
	size_t sz = v.size();
	is.__write((void*)&sz, sizeof(size_t));
	std::map<K, V>::const_iterator i = v.begin();
	for (; i != v.end(); ++i)
	{
		is.__write((void*)&i->first, sizeof(K));
		is.__write((void*)&i->second, sizeof(V));
	}		
	return is;
}

class _CoreExport InStream : public Stream
{
public:
	InStream(const ByteBuffer& stream)
	{
		_stream = stream;
		_pointer = 0;
		_size = _stream.size();
	}

	inline void __read(void* buf, size_t size)
	{
		if (_pointer + size > _size)
		{
			throw OutOfBoundException();
		}

		void* srcbuf = (void*)(&_stream[_pointer]);
		memcpy(buf, srcbuf, size);
		_pointer += size;
	}
	size_t		_size;
};

inline InStream& operator >> (InStream& os, int& v)
{
	os.__read((void*)&v, sizeof(v));
	return os;
}

inline InStream& operator >> (InStream& os, unsigned int& v)
{
	os.__read((void*)&v, sizeof(v));
	return os;
}

inline InStream& operator >> (InStream& os, short& v)
{
	os.__read((void*)&v, sizeof(v));
	return os;
}

inline InStream& operator >> (InStream& os, unsigned short& v)
{
	os.__read((void*)&v, sizeof(v));
	return os;
}

inline InStream& operator >> (InStream& os, bool& v)
{
	os.__read((void*)&v, sizeof(v));
	return os;
}

inline InStream& operator >> (InStream& os, byte& v)
{
	os.__read((void*)&v, sizeof(v));
	return os;
}

inline InStream& operator >> (InStream& os, char& v)
{
	os.__read((void*)&v, sizeof(v));
	return os;
}

inline InStream& operator >> (InStream& os, float& v)
{
	os.__read((void*)&v, sizeof(v));
	return os;
}

inline InStream& operator >> (InStream& os, double& v)
{
	os.__read((void*)&v, sizeof(v));
	return os;
}

inline InStream& operator >> (InStream& os, __int64& v)
{
	os.__read((void*)&v, sizeof(v));
	return os;
}

inline InStream& operator >> (InStream& os, unsigned __int64& v)
{
	os.__read((void*)&v, sizeof(v));
	return os;
}

inline InStream& operator >> (InStream& os, XGUID& v)
{
	os.__read((void*)&v._value, sizeof(v._value));
	return os;
}

_CoreExport InStream& operator >> (InStream& os, std::string& v);
_CoreExport InStream& operator >> (InStream& os, std::wstring& v);

inline InStream& operator >> (InStream& os, ByteBuffer& v)
{
	size_t sz = 0;
	os.__read(&sz, sizeof(size_t));

	if(sz > 0)
	{
		v.resize(sz);
		os.__read(&v[0], sz);
	}

	return os;
}

// 	template<class T>
// 	inline InStream& operator >> (InStream& os, std::list<T>& v)
// 	{
// 		size_t sz = 0;
// 		os.__read(&sz, sizeof(size_t));
// 
// 		for (size_t i = 0; i < sz; ++i)
// 		{
// 			T val;
// 			os.__read(&val, sizeof(T));
// 			v.push_back(val);
// 		}
// 
// 		return os;
// 	}

template<class K, class V>
inline InStream& operator >> (InStream& os, std::map<K, V>& v)
{
	size_t sz = 0;
	os.__read(&sz, sizeof(size_t));

	for (size_t i = 0; i < sz; ++i)
	{
		K key;
		V val;
		os.__read(&key, sizeof(K));
		os.__read(&val, sizeof(V));
		v.insert(std::make_pair(key, val));
	}

	assert(v.size() == sz);

	return os;
}