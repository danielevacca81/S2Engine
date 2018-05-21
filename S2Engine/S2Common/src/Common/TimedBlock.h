// TimedBlock.h
//
#ifndef TIMEDBLOCK_ONCE
#define TIMEDBLOCK_ONCE

#include <time.h>
#include <string>
#include <iostream>

namespace s2 {

class TimedBlock
{
protected:
	std::string str;
	clock_t     startTime;
	bool        deferred;
	bool        hmsFormat;

public:
	// ------------------------------------------------------------------------------------------------	
	TimedBlock( const char *s )
	{
		hmsFormat = false;
		deferred  = true;
		str       = s;

		startTime = clock();
	}

	// ------------------------------------------------------------------------------------------------	
	TimedBlock( const char * s, bool printAtEnd, bool showHMS )
	{
		hmsFormat = showHMS;
		deferred  = printAtEnd;
		str       = s;

		if (!deferred)
			std::cout << str << " ... " << std::endl;

		startTime = clock();
	}

	// ------------------------------------------------------------------------------------------------	
	~TimedBlock()
	{
		const clock_t endTime     = clock();
		const clock_t elapsedTime = endTime - startTime;

		if( hmsFormat )
		{
			int msec = elapsedTime/1000;  //sec
			int hours = msec/3600;
			//int my_msec -= hours*3600*1000;

			msec = msec - (hours*3600);
			int min = msec / 60;
			//my_msec -= min*60*1000;

			int sec = msec - (min*60);
			//my_msec -= sec*1000;
			//if(my_msec > 500) sec++;
			if( deferred )	std::cout << str << " " << "[" << hours << "h " << min << "min " << sec << "sec]" << std::endl;
			else			std::cout << "done. "   << "[" << hours << "h " << min << "min " << sec << "sec]" << std::endl;
		}
		else
		{
			if (deferred)	std::cout << str << " " << "[" << (int)elapsedTime << " ms]" << std::endl;
			else			std::cout << "done. "   << "[" << (int)elapsedTime << " ms]" << std::endl;
		}
	}
};

}

#endif