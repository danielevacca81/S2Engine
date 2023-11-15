// Timer_w32.h
//
// Simple timing class
#pragma once

#include "S2FrameworkAPI.h"

namespace s2Fwk{

class S2FRAMEWORK_API Timer
{
private:
	bool	paused;
	float	resolution;	

	__int64	lastUpdate;	
	__int64	startTime;	
	__int64	deltaTime;	
	__int64	totalTime;	

		
public:
	Timer();
	~Timer(){}

	void	reset();
	void	pause(bool _paused);
	float	update();
			
	float	getTotalTime();
	float	getDeltaTime();
};

}