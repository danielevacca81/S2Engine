// Timer_w32.cpp
//
// Simple timing class
// Thanks to NeHe

#include "Timer.h"
#include <windows.h>

using namespace s2Fwk;


Timer::Timer()
{
	__int64 frequency;	// Frequency

	QueryPerformanceFrequency((LARGE_INTEGER *) &frequency);	// Get frequency
	resolution = (float) ((1.0)/((double) frequency));			// Resolution
}

// Reset all values
// 
void Timer::reset()
{
	QueryPerformanceCounter((LARGE_INTEGER *) &startTime);		// reset start time

	totalTime	= 0;
	deltaTime	= 0;
	lastUpdate	= startTime;
	paused		= false;
}



// Time elapsed since the last reset
//
float Timer::getTotalTime()	{return static_cast<float> (totalTime * resolution);}


// Delta time between the last update
//
float Timer::getDeltaTime()	{return static_cast<float> (deltaTime * resolution);}


// Pause
//
void Timer::pause(bool _paused)	{paused = _paused;}

	
// Update all values
//
float Timer::update()
{
	__int64 now;

	QueryPerformanceCounter((LARGE_INTEGER *) &now);
			
	deltaTime = paused ? 0 : now - lastUpdate;	// delta time
	lastUpdate = now;						// set now as last update
	totalTime += deltaTime;			// advance total time by delta

	return static_cast<float> (deltaTime * resolution);
}