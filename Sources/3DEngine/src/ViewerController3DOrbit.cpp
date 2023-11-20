// ViewerController3DOrbit.cpp
// 
#include "ViewerController3DOrbit.h"

#include "Core/Math.h"

#include <cmath>

using namespace Scene;

// ------------------------------------------------------------------------------------------------
ViewerController3DOrbit::ViewerController3DOrbit()
	: _pivotPointUnderDragBegin( true )
	, _minDragThreshold( 4 )
	, _horizontalRotSpeed( 0.20 )
	, _verticalRotSpeed( 0.08 )
	, _minTiltAngle( -90.0 )
	, _minZoomFactor( 0.0 )
{
}

// ------------------------------------------------------------------------------------------------
void  ViewerController3DOrbit::setHorizontalRotInverted( const bool invert )
{
	_horizontalRotSpeed = std::abs( _horizontalRotSpeed );
	if( invert ) _horizontalRotSpeed *= -1.0;
}

// ------------------------------------------------------------------------------------------------
void  ViewerController3DOrbit::setVerticalRotInverted( const bool invert )
{
	_verticalRotSpeed = std::abs( _verticalRotSpeed );
	if( invert ) _verticalRotSpeed *= -1.0;
}

// ------------------------------------------------------------------------------------------------
void  ViewerController3DOrbit::setMinTiltAngle( const double minAngle )
{
	assert( minAngle <= 90. && minAngle >= -90. );
	_minTiltAngle  = minAngle;
}

// ------------------------------------------------------------------------------------------------
void  ViewerController3DOrbit::setMinZoomFactor( const double zoomFactor )
{
	assert( _minZoomFactor >= 0.0 );
	_minZoomFactor = zoomFactor;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController3DOrbit::start()
{
	if( !canStart() )  return false;

	_dragInProgress = false;

	doStart();  return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController3DOrbit::doDragBegin( const Math::ivec2 &screenPos )
{
	_screenPickPos = screenPos;

	const Math::dvec2 screenCenter = viewer()->viewportLocalCenter();
	const Math::dvec2 screenRotationOrigin = _pivotPointUnderDragBegin ? Math::dvec2(screenPos) + Math::dvec2(0.5) : screenCenter;
	
	_dragInProgress = viewer()->depth0PlaneAtScreenPos( screenRotationOrigin, _rotationOrigin );
	if( !_dragInProgress )
	{
		doReleaseViewer( false );
		return false;
	}

		
	const auto dist = Math::length( viewer()->camera().position() - _rotationOrigin );
		
	const Math::dvec3 cDir = -viewer()->camera().front();	
		
	if( cDir.y != 0.0 || cDir.x != 0.0 ) // todo da migliorare
		_startTheta = _lastTheta = Math::degrees( std::atan2( cDir.y, cDir.x ) );
	else
	{
		Math::dvec3 side = viewer()->camera().side();
		_startTheta = _lastTheta = Math::degrees( std::atan2( side.y, side.x ) ) - 90.0;
	}

	_startPhi = _lastPhi = Math::degrees( std::atan2( cDir.z, std::hypot(cDir.x,cDir.y) ) );

	const Math::dvec2 screenDeltaPos = screenCenter - screenRotationOrigin;
	_cameraOffset = Math::dvec3( screenDeltaPos, viewer()->focalLengthPixel() );
		
	const auto cameraOffsetLen = Math::length( _cameraOffset );
	if( cameraOffsetLen != 0.0 )
		_cameraOffset *= dist / cameraOffsetLen;
			
	_minPhiAngle = _minTiltAngle;
	if( _minZoomFactor != 0.0 && _cameraOffset.y < 0.0 && _minTiltAngle > 0.0 )
	{
		double zoomDist = viewer()->zoomDist( _minZoomFactor );
		double maxZoomPhi = Math::degrees( Math::atan( -_cameraOffset.y / (_cameraOffset.z-zoomDist) ) );
		_minPhiAngle = std::max( maxZoomPhi, _minPhiAngle );
	}
			
	return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController3DOrbit::doDragUpdate( const Math::ivec2 &screenPos )
{
	if( !_dragInProgress )
		return false;

	updatePos( screenPos );
	return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController3DOrbit::doDragEnd( const Math::ivec2 &screenPos )
{
	if( !_dragInProgress )
		return false;

	updatePos( screenPos );
	doReleaseViewer( true );
	return true;
}

// ------------------------------------------------------------------------------------------------
inline void ViewerController3DOrbit::updatePos( const Math::ivec2 &screenPos )
{
	assert( viewer() );
	assert( _dragInProgress );

	auto diff = screenPos - _screenPickPos;
	diff = Math::sign( diff ) * Math::max( Math::ivec2(0), Math::abs(diff) - Math::ivec2(_minDragThreshold) );

	if( diff == Math::ivec2() )
		return;

	_lastPhi = _startPhi - diff.y * _verticalRotSpeed;
	_lastPhi = Math::clamp( _lastPhi, Math::max(-90.0, _minPhiAngle), 90.0 );

	if( Math::abs(_lastPhi) < 89.0 )
		_lastTheta = _startTheta - diff.x * _horizontalRotSpeed;
	while( _lastTheta < -180.0 ) _lastTheta += 360.0;
	while( _lastTheta >  180.0 ) _lastTheta -= 360.0;

	// da rivedere
	const Math::dmat4 pivotTrans = Math::translate( Math::dmat4( 1.0 ), _rotationOrigin );
	const Math::dmat4 startOrient = Math::dmat4( Math::dvec4{0,1,0,0}, Math::dvec4{0,0,1,0}, 
												 Math::dvec4{1,0,0,0}, Math::dvec4{0,0,0,1} ); 
	const Math::dmat4 thetaRot   = Math::rotate( Math::dmat4( 1.0 ), Math::radians(_lastTheta), {0.,1.,0.} );
	const Math::dmat4 phiRot     = Math::rotate( Math::dmat4( 1.0 ), Math::radians(-_lastPhi),   {1.,0.,0.} );
	const Math::dmat4 rhoTrans   = Math::translate( Math::dmat4( 1.0 ), _cameraOffset );

	viewer()->setCameraDirectMatrix( pivotTrans * startOrient * thetaRot * phiRot * rhoTrans );
}













				     