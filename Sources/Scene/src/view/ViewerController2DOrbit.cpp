// ViewerController2DOrbit.cpp
// 
#include "ViewerController2DOrbit.h"

#include "Math/Math.h"

#include <cmath>

using namespace RenderPipeline;

// ------------------------------------------------------------------------------------------------
ViewerController2DOrbit::ViewerController2DOrbit()
	//: _pivotPointUnderDragBegin( false )
	: _minDragThreshold( 4 )
	, _rotationSpeed( 0.10 )
	, _minZoomFactor( 0.0 )
{}

// ------------------------------------------------------------------------------------------------
void  ViewerController2DOrbit::setMinZoomFactor( const double zoomFactor )
{
	assert( _minZoomFactor >= 0.0 );
	_minZoomFactor = zoomFactor;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DOrbit::start()
{
	if( !canStart() )  return false;

	_dragInProgress = false;

	doStart();  return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DOrbit::doDragBegin( const Math::ivec2 &screenPos )
{
	_dragInProgress = true;
	_screenPickPos  = screenPos; // for pixel threshold

	const auto screenCenter = viewer()->viewportLocalCenter();

	// assumes origin can't change during rotation
	const auto theView = viewer()->view(); 
	_rotationOrigin = Math::unProject( Math::dvec3( screenCenter, 0.0 ), 
										  theView.viewMatrix(), 
										  theView.projectionMatrix(), 
										  Math::dvec4( theView.viewport().left(), theView.viewport().bottom(), theView.viewport().width(), theView.viewport().height() ) ); 
	_rotationOrigin.z = 0.0;
	
	const auto P = Math::dvec2( screenPos ) - screenCenter;				 
	_currQ       = Math::angleAxis( std::atan2( P.y, P.x ), Math::dvec3{ 0.,0.,1. } );
	_currQ       = Math::normalize( _currQ );

	_startQ      = Math::inverse( _currQ );
	
	_startOrientation = viewer()->camera().orientation();
	
	return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DOrbit::doDragUpdate( const Math::ivec2 &screenPos )
{
	if( !_dragInProgress )
		return false;

	updatePos( screenPos );
	return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DOrbit::doDragEnd( const Math::ivec2 &screenPos )
{
	if( !_dragInProgress )
		return false;

	updatePos( screenPos );
	doReleaseViewer( true );
	return true;
}

// ------------------------------------------------------------------------------------------------
inline void ViewerController2DOrbit::updatePos( const Math::ivec2 &screenPos )
{
	assert( viewer() );
	assert( _dragInProgress );

	auto diff = screenPos - _screenPickPos;
	{
		diff = Math::sign( diff ) * Math::max( Math::ivec2( 0 ), Math::abs( diff ) - Math::ivec2( _minDragThreshold ) );
		if( diff == Math::ivec2() )
				return;
	}

	const Math::dvec2 screenCenter = viewer()->viewportLocalCenter();
	const auto P                   = Math::dvec2( screenPos ) - screenCenter;
	_currQ                         = Math::angleAxis( std::atan2( P.y, P.x ), Math::dvec3 { 0.,0.,1. } );
	_currQ                         = _currQ * _startQ;

	const Math::dvec3 pivot( _rotationOrigin.x, _rotationOrigin.y, viewer()->camera().position().z );
	const Math::dmat4 pivotTrans  = Math::translate( Math::dmat4( 1.0 ), pivot ); 
	viewer()->setCameraDirectMatrix( pivotTrans * _startOrientation * Math::transpose( Math::mat4_cast( _currQ ) ) );
}