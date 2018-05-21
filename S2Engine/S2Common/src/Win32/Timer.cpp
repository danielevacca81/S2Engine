// Timer_w32.cpp
//
// Simple timing class
// Thanks to NeHe

#include "Timer.h"
#include <windows.h>

using namespace s2;


// ------------------------------------------------------------------------------------------------
Timer::Timer()
{
	__int64 frequency;	// Frequency

	QueryPerformanceFrequency((LARGE_INTEGER *) &frequency);	// Get frequency
	_resolution = (float) ((1.0)/((double) frequency));			// Resolution
}

// ------------------------------------------------------------------------------------------------
// Reset all values
void Timer::reset()
{
	QueryPerformanceCounter((LARGE_INTEGER *) &_startTime);		// reset start time

	_totalTime	= 0;
	_deltaTime	= 0;
	_lastUpdate	= _startTime;
	_paused		= false;
}



// ------------------------------------------------------------------------------------------------
// Time elapsed since the last reset
float Timer::totalTime() const {return static_cast<float> (_totalTime * _resolution);}


// ------------------------------------------------------------------------------------------------
// Delta time between the last update
float Timer::deltaTime() const {return static_cast<float> (_deltaTime * _resolution);}


// ------------------------------------------------------------------------------------------------
// Pause
void Timer::pause(bool paused)	{ _paused = paused;}
	
// ------------------------------------------------------------------------------------------------
// Update all values
float Timer::update()
{
	__int64 now;

	QueryPerformanceCounter((LARGE_INTEGER *) &now);
			
	_deltaTime = _paused ? 0 : now - _lastUpdate;	// delta time
	_lastUpdate = now;								// set now as last update
	_totalTime += _deltaTime;						// advance total time by delta

	return static_cast<float> (_deltaTime * _resolution);
}