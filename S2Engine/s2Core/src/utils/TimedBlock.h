// TimedBlock.h
//
#ifndef CORE_TIMEDBLOCK_ONCE
#define CORE_TIMEDBLOCK_ONCE

#include <string>
#include <iostream>
#include <iomanip>
#include <chrono>


class TimedBlock
{
public:
    // ------------------------------------------------------------------------------------------------
    TimedBlock( const std::string &s )
    : _blockName(s)
    {
        _startTime = std::chrono::high_resolution_clock::now();
    }

    // ------------------------------------------------------------------------------------------------
    ~TimedBlock()
    {
        using namespace std::chrono;
        auto endTime = high_resolution_clock::now();

        std::cout << _blockName
                  << " [" << std::fixed << std::setprecision(0)
                  << (duration_cast<microseconds>( endTime - _startTime ).count()) << " us]" << std::endl;
    }

private:
    std::string _blockName;
    std::chrono::high_resolution_clock::time_point _startTime;
};


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class Timer
{
public:
    // ------------------------------------------------------------------------------------------------
    Timer() { _startTime = std::chrono::high_resolution_clock::now(); }

    inline long long elapsed_msec() const { using namespace std::chrono; auto now = high_resolution_clock::now(); return duration_cast<milliseconds>(now - _startTime).count(); }
    inline long long elapsed_usec() const { using namespace std::chrono; auto now = high_resolution_clock::now(); return duration_cast<microseconds>(now - _startTime).count(); }
    inline long long elapsed_nsec() const { using namespace std::chrono; auto now = high_resolution_clock::now(); return duration_cast<nanoseconds >(now - _startTime).count(); }

    // ------------------------------------------------------------------------------------------------
    ~Timer()
    {}

private:
    std::chrono::high_resolution_clock::time_point _startTime;
};
#endif
