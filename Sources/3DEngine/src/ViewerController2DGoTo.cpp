// ViewerController2DGoTo.cpp
// 
#include "ViewerController2DGoTo.h"

#include "Core/Math.h"

using namespace Scene;

// ------------------------------------------------------------------------------------------------
bool ViewerController2DGoTo::start( const Math::dvec3 &destPos, const double zoomFactor, 
									const double animationDurationSec )
{
	if( !canStart() )  return false;


	_endPos             = destPos;
	_endZoomFactor      = zoomFactor;
	_endZoomFactorInv   = 1.0 / _endZoomFactor;

	_durationSec        = animationDurationSec;

	_startZoomFactor    = viewer()->zoomFactor();
	_startZoomFactorInv = 1.0 / _startZoomFactor;
	_startPos           = viewer()->camera().target();
	_startPos.z         = 0.0;
	_startTime          = std::chrono::high_resolution_clock::now();


	doStart();  return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DGoTo::start( const Math::ivec2 &screenPos, 
									const double zoomMultiplier, const double animationDurationSec )
{
	auto endPos = viewer()->view().worldPoint( screenPos );
	endPos.z = 0;

	return start( endPos, 
				  viewer()->zoomFactor() * zoomMultiplier,
				  animationDurationSec );
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DGoTo::doAnimate()  
{
	const auto time = std::chrono::high_resolution_clock::now();
	const double dt = std::chrono::duration_cast< std::chrono::milliseconds >(time - _startTime).count() / 1000.0;

	bool completed = dt >= _durationSec;
	double normalizedT = completed ? 1.0 : dt/_durationSec;
	normalizedT = Math::smoothstep( 0.0, 1.0, normalizedT );

	viewer()->moveToLookAt( Math::mix( _startPos, _endPos, normalizedT ) );
	viewer()->setZoomFactor( 1.0 / Math::mix( _startZoomFactorInv, _endZoomFactorInv, normalizedT ) );

	if( completed )
		doReleaseViewer( true );

	return true;
}









				     