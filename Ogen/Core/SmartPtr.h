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

#ifndef SmartPointer

#include <assert.h>

// Smart pointers. The class T must be derived from Object.
//--------------------------------------------------------------------------------------

template<class T> class TSmartPointer
{
public:

	TSmartPointer(T* p = 0)
	{
		this->_pointer = p;
		if (this->_pointer)
		{
			this->_pointer->AddRef();
		}
	}

	//--------------------------------------------------------------------------------------

	template<class Y>
		TSmartPointer(const TSmartPointer<Y>& r)
	{
		this->_pointer = r._pointer;
		if(this->_pointer)
		{
			this->_pointer->AddRef();
		}
	}

	//--------------------------------------------------------------------------------------

#if _MSC_VER <= 1300
	template<>
#endif
	TSmartPointer(const TSmartPointer<T>& r)
	{
		this->_pointer = r._pointer;
		if (this->_pointer)
		{
			this->_pointer->AddRef();
		}
	}

	//--------------------------------------------------------------------------------------

	TSmartPointer& operator=(T* p)
	{
		if(this->_pointer != p)
		{
			if(p)
			{
				p->AddRef();
			}
			T* ptr = this->_pointer;
			this->_pointer = p;
			if(ptr)
			{
				ptr->Release();
			}
		}
		return *this;
	}

	//--------------------------------------------------------------------------------------

	template<typename Y>
		TSmartPointer& operator=(const TSmartPointer<Y>& r)
	{
		if(this->_pointer != r._pointer)
		{
			if (r._pointer)
			{
				r._pointer->AddRef();
			}
			T* ptr = this->_pointer;
			this->_pointer = r._pointer;
			if(ptr)
			{
				ptr->Release();
			}
		}
		return *this;
	}

	//--------------------------------------------------------------------------------------

#if _MSC_VER <= 1300
	template<>
#endif
	TSmartPointer& operator=(const TSmartPointer<T>& r)
	{
		if(this->_pointer != r._pointer)
		{
			if(r._pointer)
			{
				r._pointer->AddRef();
			}
			T* ptr = this->_pointer;
			this->_pointer = r._pointer;
			if(ptr)
			{
				ptr->Release();
			}
		}
		return *this;
	}

	//--------------------------------------------------------------------------------------

	T* operator->() const
	{
		assert(_pointer);
		return _pointer;
	}

	//--------------------------------------------------------------------------------------

	operator bool() const
	{
		return _pointer ? true : false;
	}

	//--------------------------------------------------------------------------------------

	T& operator*() const
	{
		assert(_pointer);
		return *_pointer;
	}

	//--------------------------------------------------------------------------------------

	~TSmartPointer()
	{
		if (_pointer)
		{
			_pointer->Release();
		}
	}

	//--------------------------------------------------------------------------------------

	template<class Y>
		static TSmartPointer DynamicCast(const TSmartPointer<Y>& r)
	{
		return TSmartPointer(dynamic_cast<T*>(r._pointer));
	}

	//--------------------------------------------------------------------------------------

	template<class Y>
		static TSmartPointer DynamicCast(Y* p)
	{
		return TSmartPointer(dynamic_cast<T*>(p));
	}

	//--------------------------------------------------------------------------------------

	template<class Y>
		static TSmartPointer ConstCast(const TSmartPointer<Y>& r)
	{
		return TSmartPointer(const_cast<T*>(r._pointer));
	}

	//--------------------------------------------------------------------------------------

	template<class Y>
		static TSmartPointer ConstCast(Y* p)
	{
		return TSmartPointer(const_cast<T*>(p));
	}

	//--------------------------------------------------------------------------------------

	T* GetPointer() const
	{
		return _pointer;
	}

	//--------------------------------------------------------------------------------------

	T*	_pointer;
}; // TSmartPointer

//--------------------------------------------------------------------------------------

template<class T, class U>
inline bool operator==(const TSmartPointer<T>& lhs, const TSmartPointer<U>& rhs)
{
	T* l = lhs.GetPointer();
	U* r = rhs.GetPointer();
	if(l && r)
	{
		return l == r;
	}
	else
	{
		return !l && !r;
	}	
}

//--------------------------------------------------------------------------------------

template<class T, class U>
inline bool operator!=(const TSmartPointer<T>& lhs, const TSmartPointer<U>& rhs)
{
	T* l = lhs.GetPointer();
	U* r = rhs.GetPointer();
	if(l && r)
	{
		return l != r;
	}
	else
	{
		return l || r;
	}	
}


//---------------------------------------------------------------------------

#	define SmartPointerEx(classType, className) \
	classType className; \
	typedef TSmartPointer<className> className##Ptr;

#	define SmartPointer(className) SmartPointerEx(class, className)
#	define SmartPointerS(className) SmartPointerEx(struct, className)

#endif