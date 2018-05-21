// Timer_w32.h
//
// Simple timing class
#pragma once

#include "S2CommonAPI.h"

namespace s2{

class S2COMMON_API Timer
{
private:
	bool	_paused;
	float	_resolution;	

	__int64	_lastUpdate;	
	__int64	_startTime;	
	__int64	_deltaTime;	
	__int64	_totalTime;	

		
public:
	Timer();
	~Timer(){}

	void	reset();
	void	pause(bool paused);
	float	update();
			
	float	totalTime() const;
	float	deltaTime() const;
};

}