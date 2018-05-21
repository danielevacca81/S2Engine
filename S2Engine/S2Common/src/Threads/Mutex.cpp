// Mutex.cpp
//
#include "Mutex.h"

using namespace s2;

#ifdef _WIN32

#include <Windows.h>


// ------------------------------------------------------------------------------------------------
Mutex::Mutex()
: _data(0)
{
	_data= new CRITICAL_SECTION;
	::InitializeCriticalSection((CRITICAL_SECTION*)_data);
}


// ------------------------------------------------------------------------------------------------
Mutex::~Mutex()	{ ::DeleteCriticalSection((CRITICAL_SECTION*)_data); }

// ------------------------------------------------------------------------------------------------
void Mutex::lock()	 { ::EnterCriticalSection((CRITICAL_SECTION*)_data); }
void Mutex::unlock() { ::LeaveCriticalSection((CRITICAL_SECTION*)_data); }
#endif