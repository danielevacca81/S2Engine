// MyApplication.cpp
//
#include "MyApplication.h"

#include "Math/Interpolator.h"

// ------------------------------------------------------------------------------------------------
void MyApplication::updateState()
{
	// use delta time and iterpolate animations
	const auto sizeInterpolator = Math::Interpolator<double>( 1.0, 5.0, 2, Math::EasingCurve::QuadraticIn, Math::Mode::PingPong );
	scaleFactor = sizeInterpolator.valueAt( elapsedTime_ms() * 1e-3 );
}