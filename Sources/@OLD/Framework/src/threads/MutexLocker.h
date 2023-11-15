// MutexLocker.h
//
#ifndef MUTEXLOCKER_ONCE
#define MUTEXLOCKER_ONCE

#include "S2FrameworkAPI.h"

namespace s2Fwk {
class Mutex;

/**
*	A lock is a clever object that you construct on the stack
*	and for the duration of its lifetime your object is protected
*	from any other threads.
*	You have to put Locks inside all the methods of your object that
*	access data shared with the captive thread.
*/
class S2FRAMEWORK_API MutexLocker
{
public:
	///	Acquire the state of the semaphore
	MutexLocker( Mutex & p_mutex );

	///	Release the state of the semaphore
	~MutexLocker();
private:
	Mutex &_mutex;

	// Prevent copying
	MutexLocker(const MutexLocker&);
	MutexLocker& operator=(const MutexLocker&);
};

}
#endif