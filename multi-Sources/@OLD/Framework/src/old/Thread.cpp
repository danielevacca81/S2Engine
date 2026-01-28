// Thread.cpp
//
#include "Thread.h"

using namespace s2Core;

// ------------------------------------------------------------------------------------------------
Thread::Thread( const std::string &threadName )
: Poco::Runnable()
{
	_thread = new Poco::Thread(threadName);
}

// ------------------------------------------------------------------------------------------------
Thread::~Thread()
{
	delete _thread;
}

// ------------------------------------------------------------------------------------------------
void Thread::start()
{
	_thread->start(*this);
}

// ------------------------------------------------------------------------------------------------
void Thread::stop()
{
	_thread->join();
}
