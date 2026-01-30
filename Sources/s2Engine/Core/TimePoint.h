// TimePoint.h
//
#ifndef TIMEPOINT_ONCE
#define TIMEPOINT_ONCE

#include <chrono>

namespace s2 {

template<typename T>
class TimePoint
{
public:
	// diff methods
	inline long long elapsed_msec() const { auto now = T::now(); return std::chrono::duration_cast<std::chrono::milliseconds>( now - _startTime ).count(); }
	inline long long elapsed_usec() const { auto now = T::now(); return std::chrono::duration_cast<std::chrono::microseconds>( now - _startTime ).count(); }
	inline long long elapsed_nsec() const { auto now = T::now(); return std::chrono::duration_cast<std::chrono::nanoseconds>( now - _startTime ).count(); }

private:
	typename T::time_point _startTime = T::now();
};

// ================================================================================================
// TYPE ALIASES
// ================================================================================================

/// High-resolution timer (WARNING: May alias to system_clock on some platforms)
/// Use SteadyTimePoint for performance measurements
using HighResTimePoint = TimePoint<std::chrono::high_resolution_clock>;

/// Monotonic timer (RECOMMENDED for performance measurements)
/// Not affected by system clock adjustments (NTP, DST, etc.)
using SteadyTimePoint = TimePoint<std::chrono::steady_clock>;

/// System wall-clock timer (use only for timestamps, not for timing measurements)
/// Can go backwards due to NTP synchronization
using SystemTimePoint = TimePoint<std::chrono::system_clock>;
}

#endif