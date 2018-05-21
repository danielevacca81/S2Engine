// Thread.h
//
#ifndef THREAD_ONCE
#define THREAD_ONCE

#include "S2FrameworkAPI.h"

#include "Mutex.h"

#include <string>

namespace s2Fwk {

class ThreadEntry;

class S2FRAMEWORK_API Thread
{
private:
	void          *_handle;
	unsigned long _threadID;
	mutable Mutex _mutex;

	friend class ThreadEntry;

protected:
	//static void exit ();

	/**
	*	\brief	The starting point for the thread. 
	*
	*	After calling start(), the newly created thread calls this function.
	*	You have to reimplemented this function to do the useful work.
	*	Returning from this method will end the execution of the thread.
	*/
	virtual void  run () = 0;



public:
	//	Constructs a new thread. The thread does not begin executing until start() is called.
	Thread ();

	/** 
	*	\brief Destroys the thread.
	*
	*	Note that deleting a Thread object will not stop the execution of the thread it represents. 
	*	Deleting a running Thread (i.e. isFinished() returns false) will probably result in a program crash.
	*	You can wait() on a thread to make sure that it has finished.
	*/
	virtual ~Thread (); 
				
	//	Returns true if the thread is finished; otherwise returns false.
	bool  isFinished () const;

	//	Returns true if the thread is running; otherwise returns false.
	bool  isRunning () const;

	/**
	*	\brief Wait the end of the thread.
	*
	*	Blocks the thread until either of these conditions is met:
	*
	*	- The thread associated with this Thread has finished execution (i.e. when it returns from run()). 
	*	This function will return true if the thread has finished. 
	*	It also returns true if the thread has not been started yet.
	*	- Time milliseconds has elapsed. If time is ULONG_MAX (the default), 
	*	then the wait will never timeout (the thread must return from run()).
	*	This function will return false if the wait timed out.
	*
	*	This provides similar functionality to the POSIX pthread_join() function.
	*/
	bool  wait ( unsigned long time = ULONG_MAX );
			
	/**
	*	\brief Begins execution of the thread by calling run(), 
	*	which should be reimplemented in a Thread subclass to contain your code.
	*
	*	If the thread is already running, this function does nothing.
	*
	*/
	bool  start ();

	// Suspend the Thread.
	void  suspend();
			
	// Resume the Thread.
	void  resume();

	void sleep ( unsigned int secs );
	void msleep( unsigned int msecs ); 
	void usleep( unsigned int uecs );

	void yield();

	// Copy constructor.
	Thread (const Thread&);

	// Assign operator.
	Thread& operator=(const Thread&);

	//	Assign a name to the thread.
	void setThreadName( const std::string &threadName );
};

}

#endif