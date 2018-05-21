// Thread.cpp
//
#include "Thread.h"

#include "MutexLocker.h"
#include "Mutex.h"


#ifdef _WIN32

#include <windows.h>

#include <iostream>

namespace s2 {
//-----------------------------------------------------------------------------------------------
class ThreadEntry
{
public:
	static DWORD WINAPI entry( void* pArg)
	{
		Thread* thread = (Thread*) pArg;
		try
		{
			thread->run();
		}
		catch(...)
		{
			std::cerr<< "Unhandled Exception in thread entry";
		}
		return 0;
	}
};
}

using namespace s2;


//-----------------------------------------------------------------------------------------------
//	Constructs a new thread.
Thread::Thread ()
: _handle(0),
  _threadID(0)
{}

//-----------------------------------------------------------------------------------------------
//	Destroys the thread
Thread::~Thread() 
{
	::CloseHandle((HANDLE)_handle);
}

//-----------------------------------------------------------------------------------------------
Thread::Thread (const Thread& p_thread)
: _handle(0),
 _threadID(p_thread._threadID)
{
	MutexLocker lock(p_thread._mutex);

	if (p_thread._handle)
	{
		::DuplicateHandle(
			::GetCurrentProcess(),
			p_thread._handle,
			::GetCurrentProcess(),
			&_handle,
			0,
			FALSE,
			DUPLICATE_SAME_ACCESS);
	}
}

//-----------------------------------------------------------------------------------------------
Thread& Thread::operator=(const Thread& p_thread)
{
	MutexLocker lock1(_mutex);
	::CloseHandle(_handle);
	_handle = 0;

	MutexLocker lock2(p_thread._mutex);
	if( p_thread._handle )
	{
		::DuplicateHandle(
			::GetCurrentProcess(),
			p_thread._handle,
			::GetCurrentProcess(),
			&_handle,
			0,
			FALSE,
			DUPLICATE_SAME_ACCESS);
	}
	_threadID = p_thread._threadID;

	return *this;
}

//-----------------------------------------------------------------------------------------------
void Thread::suspend()
{
	::SuspendThread((HANDLE)_handle);
}

//-----------------------------------------------------------------------------------------------
void Thread::resume()
{
	::ResumeThread((HANDLE)_handle);
}

//----------------------------------------------------------------------
//
// Usage: SetThreadName (handle, "MainThread");
//
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType;     // Must be 0x1000.
	LPCSTR szName;    // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags;    // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void Thread::setThreadName( const std::string &threadName )
{
	const DWORD MS_VC_EXCEPTION=0x406D1388;
	THREADNAME_INFO info;
	info.dwType     = 0x1000;
	info.szName     = threadName.c_str();
	info.dwThreadID = _threadID;
	info.dwFlags    = 0;

	__try
	{
		RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

//-----------------------------------------------------------------------------------------------
//	Returns true if the thread is finished; otherwise returns false.
bool Thread::isFinished () const
{
	if( _handle == 0 )
		return true;

	DWORD res;
	BOOL ret = ::GetExitCodeThread((HANDLE)_handle,&res);
	if(ret == 0)
	{
		std::cerr << "GetExitCodeThread Failed";
		return true;
	}

	if( res == STILL_ACTIVE )
		return false;
	return true;
}

//-----------------------------------------------------------------------------------------------
//	Returns true if the thread is running; otherwise returns false.
bool Thread::isRunning () const
{
	return !isFinished();
}

//-----------------------------------------------------------------------------------------------
//	Wait the end of the thread.
bool Thread::wait ( unsigned long time )
{
	if( _handle == 0 )
		return true;

	DWORD res = ::WaitForSingleObject((HANDLE)_handle , time);
	if( res == WAIT_TIMEOUT )
		return false;

	::CloseHandle(_handle);
	_handle   = 0;
	_threadID = 0;

	if( res == WAIT_FAILED )
	{
		std::cerr << "WaitForSingleObject Failed";
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------------------------
//	Begins execution of the thread.
bool Thread::start ()
{
	if( _handle )
		return false;

	_handle = ::CreateThread( 0, 0, ThreadEntry::entry, this, 0, &_threadID);

	if( _handle == 0 )
	{
		std::cerr << "CreateThread Failed";
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
void Thread::sleep ( unsigned int secs )
{
	::Sleep( secs*1000);
}

//-----------------------------------------------------------------------------------------------
void Thread::msleep( unsigned int msecs )
{
	::Sleep( msecs );
}


//-----------------------------------------------------------------------------------------------
void Thread::usleep( unsigned int usecs )
{
	::Sleep( usecs/1000);
}

//-----------------------------------------------------------------------------------------------
void Thread::yield()
{
	::SwitchToThread();
}

#endif // WIN32



