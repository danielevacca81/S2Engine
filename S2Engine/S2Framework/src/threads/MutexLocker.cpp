// MutexLocker.cpp
//
#include "MutexLocker.h"

#include "Mutex.h"

using namespace s2Fwk;


// ------------------------------------------------------------------------------------------------
//	Acquire the state of the semaphore
MutexLocker::MutexLocker( Mutex & p_mutex )
: _mutex(p_mutex)
{
	_mutex.lock();
}

// ------------------------------------------------------------------------------------------------
//	Release the state of the semaphore
MutexLocker::~MutexLocker()
{
	_mutex.unlock();
}