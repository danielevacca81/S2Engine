// Event.h
//
#ifndef EVENT_ONCE
#define EVENT_ONCE

#include "S2FrameworkAPI.h"

namespace s2Fwk {
/**
*	An Event is a signaling device that threads use to communicate with each other.
*	You embed an Event in your MyThread. Then you make the captive thread wait on it until
*	some other thread releases it.
*	Remember however that if your captive thread waits on a event it can't be terminated.
*/
class S2FRAMEWORK_API Event
{
private:
	void  *_handle;

	// Prevent copying
	Event(const Event&);
	Event& operator=(const Event&);

public:
	//	Start in non-signaled state (red light). Auto reset after every wait.
	Event();

	~Event();

	//	Put into signaled state.
	void release();

	//	Wait until event is in signaled (green) state.
	void wait();
			
	//	Set in non-signaled (red light).
	void reset();
};

}
#endif