// Mutex.h
//
#ifndef MUTEX_ONCE
#define MUTEX_ONCE

#include "S2FrameworkAPI.h"

namespace s2Fwk {

class S2FRAMEWORK_API Mutex
{
public:
	Mutex();
	~Mutex();

	void lock();
	void unlock();

private:
	void *_data;

	// Prevent copying
	Mutex(const Mutex&);
	Mutex& operator=(const Mutex&);
};

}
#endif