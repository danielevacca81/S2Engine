// Thread.h
//
#ifndef THREAD_IMPL_ONCE
#define THREAD_IMPL_ONCE

#include <Poco/Thread.h>
#include <Poco/Runnable.h>


namespace s2Core {

class Thread_Impl : public Poco::Runnable
{
private:
	bool _running;
	Poco::Thread _thread;

public:
	Thread_Impl();

	void start();
	void stop();
	void run();
};

}


#endif