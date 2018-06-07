// FPSCounter.h
//
// FPS counter utility class
#ifndef COMMON_FPSCOUNTER_H
#define COMMON_FPSCOUNTER_H

class FPSCounter 
{
public:
    // --------------------------------------------------------------------------------------------
    FPSCounter()
    : _framesPerSecond(0)
    , _frameCounter(0)
    , _elapsedTime(0)
    , _lastFPS(0)
    {}
	
    // --------------------------------------------------------------------------------------------
    ~FPSCounter()
    {}

    // --------------------------------------------------------------------------------------------
    void update( float dTime )
    {
        _times[_frameCounter++] = dTime;
        _elapsedTime += dTime;

        if( _frameCounter == TIMES )
        {
            float avgTime = 0.f;
            for( int i=0; i<TIMES; ++i )
                avgTime += _times[i];

            _framesPerSecond = (((float)TIMES/avgTime) + _lastFPS) * 0.5f;
            _lastFPS         = _framesPerSecond;
            _elapsedTime     = 0;
            _frameCounter    = 0;
        }
    }

    // --------------------------------------------------------------------------------------------
    float	value() const  {return _framesPerSecond;}

private:
    enum {TIMES = 10};
    float	_times[TIMES];
    long	_frameCounter;
    float	_framesPerSecond;
    float   _lastFPS;
    float	_elapsedTime;
};

}}

#endif
