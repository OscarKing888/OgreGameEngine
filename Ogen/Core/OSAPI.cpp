#include "stdafx.h"
#include "OSAPI.h"

void XInterLockedIncrement(volatile long* val)
{
#ifdef _WIN32
	::InterlockedIncrement(val);
#endif
}

void XInterLockedDecrement(volatile long* val)
{
#ifdef _WIN32
	::InterlockedDecrement(val);
#endif
}
