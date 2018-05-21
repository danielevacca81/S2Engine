// FPSCounter.h
//
// FPS counter utility class

#pragma once

namespace IDS {
namespace OpenGL {
	
class FPSCounter 
{
private:
	enum {TIMES = 10};
	long	frameCounter;
	float	framesPerSecond;
	float   lastFPS;
	float	times[TIMES];
	float	elapsedTime;

public:
	FPSCounter()
	{
		framesPerSecond = 0;
		frameCounter	= 0;
		elapsedTime		= 0;
		lastFPS         = 0;
	}
	
	~FPSCounter(){};

	void	update(float dTime);
	float	getFps()	{return framesPerSecond;}
};


inline void FPSCounter::update(float dTime)
{
	times[frameCounter++] = dTime;
	elapsedTime += dTime;

	if(frameCounter == TIMES)
	{
		float avgTime = 0.f;
		for(int i=0; i<TIMES; ++i)
			avgTime += times[i];

		framesPerSecond = (((float)TIMES/avgTime) + lastFPS) * 0.5f;
		lastFPS         = framesPerSecond;
		elapsedTime		= 0;
		frameCounter	= 0;
	}	


	//// compute FPS every 2 seconds
	//if(elapsedTime > 2.f)
	//{
	//	elapsedTime -= 2.f;

	//	framesPerSecond = frameCounter / 2.f;
	//	frameCounter   = 0;
	//}


	//// compute FPS every 10 frames
	//++frameCounter;
	//elapsedTime += dTime;
 //  
	//if(frameCounter == 10)
	//{
	//	framesPerSecond = 10.f/elapsedTime;
	//	elapsedTime		= 0;
	//	frameCounter	= 0;
	//}	
}

}
}
