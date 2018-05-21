// Thread.h
//
#ifndef THREAD_ONCE
#define THREAD_ONCE

#include <Poco/Runnable.h>
#include <Poco/Thread.h>

namespace s2Core {

class Thread : protected Poco::Runnable
{
protected:
	Poco::Thread *_thread;

public:
	explicit Thread( const std::string &threadName );
	virtual ~Thread();

	virtual void start();
	virtual void stop();

	virtual void run(){};
};

}


#endif