// TrackBall.cpp
// 
#include "TrackBall.h"

using namespace Graphics;

// ------------------------------------------------------------------------------------------------
void TrackBall::resize( int pixelWidth, int pixelHeight )
{
	if( pixelWidth <= 1 ) pixelWidth  = 2;
	if( pixelHeight <= 1 ) pixelHeight = 2;

	_size.x = 1.0f / ( ( pixelWidth ) * 0.5f );
	_size.y = 1.0f / ( ( pixelHeight ) * 0.5f );
}

// ------------------------------------------------------------------------------------------------
void TrackBall::setOrientation( const Math::dmat4 &orientation )   { _currQ = Math::quat_cast( orientation );	_prevQ = _currQ; }
void TrackBall::setRadius( double radius )                         { _radius = radius; }
void TrackBall::setCenter( const Math::ivec2 &screenPos)           { _center = screenPos; }

// ------------------------------------------------------------------------------------------------
Math::dvec2 TrackBall::normalized( const Math::ivec2 &screenPos ) const
{
	// Adjust point coords and scale down to range of [-1 ... 1]
	return Math::dvec2( screenPos.x * _size.x - 1.0, screenPos.y * _size.y - 1.0 );	// no flip y

	//return Math::dvec2( screenPos - _center ) / 64.0;// _radius;

}

// ------------------------------------------------------------------------------------------------
Math::dvec3 TrackBall::mapToSphere( const Math::ivec2 &screenPos ) const
{
	const Math::dvec2 p = normalized( screenPos );

	const double lenSqr = Math::dot( p, p );
	const double r2     = _radius * _radius;

	Math::dvec3 P;
	// sphere and hyperbolic sheet intersect on the circle
	//             r^2
	// x^2 + y^2 = ---
	//              2 
	if( lenSqr >= r2 * 0.5 )
	{
		// outside the sphere use hyperbolic sheet
		P = Math::dvec3( p.x, p.y, r2 * 0.5 / Math::sqrt( lenSqr ) );
	}
	else
	{
		// inside the sphere
		P = Math::dvec3( p.x, p.y, Math::sqrt( r2 - lenSqr ) );
	}

	return P;
}

// ------------------------------------------------------------------------------------------------
void TrackBall::update( const DragEvent& e, const Math::ivec2 &screenPos )
{
	switch( e )
	{
	case DragEvent::Begin:  enterDrag( screenPos ); break;
	case DragEvent::End:    exitDrag();             break;
	case DragEvent::Update: update( screenPos );    break;
	}
}

// ------------------------------------------------------------------------------------------------
void TrackBall::enterDrag( const Math::ivec2& screenPos )
{
	// already dragging, do nothing
	if( _dragging )
		return;

	// set the dragging flag
	_dragging = true;

	// start dragging, compute the start point on the sphere
	_startPoint = mapToSphere( screenPos );
}

// ------------------------------------------------------------------------------------------------
void TrackBall::exitDrag()
{
	// not dragging? do nothing
	if( !_dragging )
		return;
	
	_prevQ = _currQ;
	_dragging = false;
}

// ------------------------------------------------------------------------------------------------
void TrackBall::update( const Math::ivec2 &screenPos )
{
	// not dragging? do nothing
	if( !_dragging )
		return;

	// generate two vector on the sphere surface
	const Math::dvec3 currPoint   = mapToSphere( screenPos );

	// compute the rotation axis
	const Math::dvec3 cross = Math::cross( _startPoint, currPoint );

	if( Math::length( cross ) < 1e-5 )
		_currQ = Math::dquat();
	else
	{
		_currQ = Math::dquat( Math::dot( _startPoint, currPoint ), cross );
		_currQ = Math::normalize( _currQ );
	}

	if( Math::length( _prevQ ) > 0.0 )
		_currQ *= _prevQ;
}

// ------------------------------------------------------------------------------------------------
Math::dmat4 TrackBall::matrix()     const { return Math::mat4_cast( _currQ ); }
Math::dquat TrackBall::quaternion() const { return _currQ; }
double      TrackBall::radius()     const { return _radius; }
