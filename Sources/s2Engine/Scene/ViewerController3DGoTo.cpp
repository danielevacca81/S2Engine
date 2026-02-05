// ViewerController3DGoTo.cpp
// 
#include "ViewerController3DGoTo.h"

#include "Math/Math.h"


using namespace RenderPipeline;

// ------------------------------------------------------------------------------------------------
bool ViewerController3DGoTo::start( const Math::ivec2 &screenPos, 
									const double zoomMultiplier, const double animationDurationSec )
{
	if( !canStart() )  return false;

	_startPosOk = viewer()->depth0PlaneAtScreenPos( viewer()->viewportLocalCenter(), _startTargetPos );
	_endPosOk = viewer()->depth0PlaneAtScreenPos( screenPos, _endTargetPos );
	if( !_startPosOk || !_endPosOk ) // todo _startPosOk==false case: improve management changing camera orientation
	{
		doReleaseViewer( false );
		return false;
	}	

	_startTargetDist = -( viewer()->camera().matrix() * Math::dvec4(_startTargetPos, 1.0) ).z;
	_startPos = viewer()->camera().position();

	_endTargetDist = _startTargetDist / zoomMultiplier;
	_endPos = _endTargetPos - viewer()->camera().front() * _endTargetDist;

	_cameraOrientation = viewer()->camera().orientation();

	//double relativePathLen = Math::distance( _startPos, _endPos ) / (Math::abs( _startTargetDist )*0.66);
	//relativePathLen = Math::min( relativePathLen, 2.0 );
	//_durationSec = relativePathLen / speedFactor;
	_durationSec = animationDurationSec;

	_startTime   = std::chrono::high_resolution_clock::now();

	doStart();  return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController3DGoTo::doAnimate()  
{
	const auto time = std::chrono::high_resolution_clock::now();
	const double dt = std::chrono::duration_cast< std::chrono::milliseconds >(time - _startTime).count() / 1000.0;

	bool completed = dt >= _durationSec;
	double normalizedT = completed ? 1.0 : dt/_durationSec;
	normalizedT = Math::smoothstep( 0.0, 1.0, normalizedT );

	Camera camera;
	camera.set( Math::mix( _startPos, _endPos, normalizedT ), _cameraOrientation );
	viewer()->setCamera( camera );

	if( completed )
		doReleaseViewer( true );

	return true;
}