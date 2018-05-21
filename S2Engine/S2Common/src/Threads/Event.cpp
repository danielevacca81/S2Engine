// Event.cpp
//
#include "Event.h"

using namespace s2;

#ifdef _WIN32

#include <Windows.h>

//-----------------------------------------------------------------------------------------------
Event::Event() { _handle = ::CreateEvent(0, FALSE, FALSE, 0); }

//-----------------------------------------------------------------------------------------------
Event::~Event() { ::CloseHandle(_handle); }

//-----------------------------------------------------------------------------------------------
void Event::release() { ::SetEvent(_handle); }
void Event::wait()    { ::WaitForSingleObject(_handle, INFINITE); }
void Event::reset()   { ::ResetEvent(_handle); }

#endif