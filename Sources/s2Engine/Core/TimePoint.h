// TimePoint.h
//
#ifndef TIMEPOINT_ONCE
#define TIMEPOINT_ONCE

#include <chrono>

template<typename T>
class TimePoint
{
public:
	// diff methods
	inline long long elapsed_msec() const { auto now = T::now(); return std::chrono::duration_cast<std::chrono::milliseconds>( now - _startTime ).count(); }
	inline long long elapsed_usec() const { auto now = T::now(); return std::chrono::duration_cast<std::chrono::microseconds>( now - _startTime ).count(); }
	inline long long elapsed_nsec() const { auto now = T::now(); return std::chrono::duration_cast<std::chrono::nanoseconds>( now - _startTime ).count(); }

private:
	T::time_point _startTime = T::now();
};

using HighResTimePoint = TimePoint<std::chrono::high_resolution_clock>;
using SteadyTimePoint = TimePoint<std::chrono::steady_clock>;
using SystemTimePoint = TimePoint<std::chrono::system_clock>;

#endif