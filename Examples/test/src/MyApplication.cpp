// MyApplication.cpp
//
#include "MyApplication.h"

#include "Math/Easing.h"

//#include "GameWindow.h"

// ------------------------------------------------------------------------------------------------
void MyApplication::updateState()
{
	static auto startTime = elapsedTime_ms();

	// use delta time and iterpolate animations
	const auto sizeInterpolator = Math::Easing::Ease( 0.0, // start value
													  1.0, // end value
													  3.0, // duration in seconds
													  Math::Easing::Curve::QuadraticIn, 
													  Math::Easing::Mode::PingPong );
	//scaleFactor = sizeInterpolator.valueAt( elapsedTime_ms() * 1e-3 );

	//static_cast<GameWindow*>(_windows[0].get())->update( (elapsedTime_ms() - startTime) * 1e-3f );

}