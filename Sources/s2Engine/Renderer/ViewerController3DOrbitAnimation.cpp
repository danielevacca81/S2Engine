// ViewerController3DOrbitAnimation.cpp
// 
#include "ViewerController3DOrbitAnimation.h"

#include "Math/Math.h"


using namespace RenderPipeline;

// ------------------------------------------------------------------------------------------------
/*
	      z
	      |
	      |    /|
	      |   / |
	      |  /  |
	      | /p  |
	      |/_)__|______y
	     / \ )  |
	    /___\)  |
	   /  t  \  |
	  /       \ |
	 /         \|
	x
*/
bool ViewerController3DOrbitAnimation::start( const Math::dvec3 &origin,
											  const double startTheta, const double startPhi, const double startRho,
											  const double endTheta, const double endPhi, const double endRho,
											  const double animationDurationSec )
{
	assert( startPhi >= -90.0 && startPhi <= 90.0 );
	assert( endPhi   >= -90.0 && endPhi   <= 90.0 );
	assert( startRho >= 0.0 );
	assert( endRho   >= 0.0 );

	if( !canStart() )
		return false;

	_validOrigin      = true;
	_origin           = origin;

	_startTheta       = Math::normalizeAngleDeg( startTheta );
	_startPhi         = startPhi  ;
	_startRho         = startRho  ;

	_endTheta         = _startTheta + Math::normalizeAngleDeg( endTheta - _startTheta );
	_endPhi           = endPhi    ;
	_endRho           = endRho    ;

	_durationSec = animationDurationSec;
	_startTime   = std::chrono::high_resolution_clock::now();

	doStart();
	return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController3DOrbitAnimation::start( const double endTheta, const double endPhi, const double endRho,
											  const double animationDurationSec )
{
	assert( endPhi   >= -90.0 && endPhi   <= 90.0 );
	assert( endRho   >= 0.0 || endRho == -1.0 );

	if( !canStart() )
		return false;

	_validOrigin = viewer()->depth0LocalSphericalCoordinate( _origin, _startTheta, _startPhi, _startRho );
	if( !_validOrigin )
	{
		doReleaseViewer( false );
		return false;
	}
		
	_endTheta         = _startTheta + Math::normalizeAngleDeg( endTheta - _startTheta );
	_endPhi           = endPhi    ;
	_endRho           = (endRho==-1.0) ? _startRho : endRho;

	_durationSec = animationDurationSec;		
	_startTime   = std::chrono::high_resolution_clock::now();

	doStart();
	return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController3DOrbitAnimation::doAnimate()  
{
	const auto time = std::chrono::high_resolution_clock::now();
	const double dt = std::chrono::duration_cast< std::chrono::milliseconds >(time - _startTime).count() / 1000.0;

	bool completed = dt >= _durationSec;
	double normalizedT = completed ? 1.0 : dt/_durationSec;
	normalizedT = Math::smoothstep( 0.0, 1.0, normalizedT );

	viewer()-> setLocalSphericalCoordinate( _origin, 
											Math::mix( _startTheta, _endTheta, normalizedT ),
											Math::mix( _startPhi,   _endPhi,   normalizedT ),
											Math::mix( _startRho,   _endRho,   normalizedT ) );

	if( completed )
		doReleaseViewer( true );

	return true;
}