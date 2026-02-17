// MyApplication.cpp
//
#include "MyApplication.h"

#include "Math/Easing.h"

// ------------------------------------------------------------------------------------------------
void MyApplication::updateState()
{
	// use delta time and iterpolate animations
	const auto sizeInterpolator = Math::Easing::Ease( 1.0, // start value
													  2.0, // end value
													  3.0, // duration in seconds
													  Math::Easing::Curve::QuadraticIn, 
													  Math::Easing::Mode::PingPong );
	//scaleFactor = sizeInterpolator.valueAt( elapsedTime_ms() * 1e-3 );
}