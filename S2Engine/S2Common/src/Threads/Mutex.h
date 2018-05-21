// Mutex.h
//
#ifndef MUTEX_ONCE
#define MUTEX_ONCE

#include "S2CommonAPI.h"

namespace s2 {

class S2COMMON_API Mutex
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