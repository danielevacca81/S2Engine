// Camera.cpp
// 
#include "Camera.h"

using namespace Scene;

// ------------------------------------------------------------------------------------------------
Camera::Camera()
: Camera( Math::dvec3( 0.0, 0.0, 1.0 ),
		  Math::dvec3( 0.0, 0.0, 0.0 ),
		  Math::dvec3( 0.0, 1.0, 0.0 ) )
{}

// ------------------------------------------------------------------------------------------------
Camera::Camera( const Math::dvec3 &pos, const Math::dvec3 &target, const Math::dvec3 &up )
{
	_position = pos;
	_target   = target;
	_up       = up;
}

// ------------------------------------------------------------------------------------------------
void Camera::set( const Math::dvec3 &position, const Math::dmat4 &orientation )
{
	const Math::dvec4 side  = orientation[0];
	const Math::dvec4 up    = orientation[1];
	const Math::dvec4 front = orientation[2];

	const double viewDist   = Math::length( _target - _position );


	_position = position;
	_up       = Math::dvec3( up );
	_target   = _position - Math::dvec3( front )*viewDist;

	applyConstraints();
}

// ------------------------------------------------------------------------------------------------
void Camera::lookAt( const Math::dvec3 &eyePos, const Math::dvec3 &centerPos, const Math::dvec3 &upVector )
{
	_position = eyePos;
	_target   = centerPos;
	_up       = upVector;

	applyConstraints();
}

// ------------------------------------------------------------------------------------------------
void Camera::lookAt( const Math::dvec3 &pos )
{
	const Math::dvec3 viewDir = _target - _position;

	_position = Math::dvec3( pos.x, pos.y, _position.z );
	_target   = _position + viewDir; //Math::dvec3(pos.x,pos.y,_center.z);

	applyConstraints();
}

// ------------------------------------------------------------------------------------------------
void Camera::moveToLookAt( const Math::dvec3 &pos )
{
	const Math::dvec3 viewDir = _target - _position;

	_target   = pos; 
	_position = pos - viewDir;

	applyConstraints();
}

// ------------------------------------------------------------------------------------------------
void Camera::rotate( double degrees, const Math::dvec3 &axis, const Math::dvec3 &pivot )
{
	const Math::dmat4 currR = Math::transpose( orientation() );
	const Math::dmat4 r     = Math::rotate( Math::dmat4( 1.0 ), degrees, axis );
	_up                     = Math::dvec3( r * Math::dvec4( 0.0, 1.0, 0.0, 1.0 ) );

	const Math::dvec4 c     = Math::dvec4( _target - pivot, 1.0 );
	const Math::dvec4 q     = r * currR * c;

	lookAt( Math::dvec3( q ) + pivot );

	applyConstraints();
}

// ------------------------------------------------------------------------------------------------
void Camera::rotate( double degrees, const Math::dvec3 &axis )
{
	const Math::dmat4 r    = Math::rotate( Math::dmat4( 1.0 ), degrees, axis );
	_up                    = Math::dvec3( r * Math::dvec4( 0.0, 1.0, 0.0, 1.0 ) );

	applyConstraints();
}

// ------------------------------------------------------------------------------------------------
void Camera::move( const Math::dvec3 &delta )
{
	_position += delta;
	
	_up = Math::cross( side(), front() ); // normalize?

	applyConstraints();
}

// ------------------------------------------------------------------------------------------------
void Camera::moveTarget( const Math::dvec3 &delta )
{
	_target += delta;

	_up = Math::cross( side(), front() ); // normalize?
	
	applyConstraints();
}

// ------------------------------------------------------------------------------------------------
void Camera::addConstraint( const std::function<Camera( Camera * )> &constraint )
{
	_constraints.push_back( constraint );
}

// ------------------------------------------------------------------------------------------------
void Camera::applyConstraints()
{
	for( auto &c : _constraints )
	{
		auto constrainedCamera = c( this );

		_position = constrainedCamera._position;
		_target   = constrainedCamera._target;
		_up       = constrainedCamera._up;
	}
}


// ------------------------------------------------------------------------------------------------
Math::dvec3 Camera::position() const { return _position; }
Math::dvec3 Camera::target()   const { return _target; }
Math::dvec3 Camera::up()       const { return _up; }
Math::dmat4 Camera::matrix()   const { return  Math::lookAt( _position, _target, _up ); }

// ------------------------------------------------------------------------------------------------
Math::dmat4 Camera::orientation()    const
{
	Math::dmat4 m = matrix();

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;

	return Math::transpose( m ); // inverse
}

// ------------------------------------------------------------------------------------------------
//Math::dquat Camera::orientation()   const { return _orientation; }

// ------------------------------------------------------------------------------------------------
//void Camera::setPosition( const Math::dvec3& pos )  { _position    = pos; }
//void Camera::setOrientation( const Math::dquat& q ) { _orientation = q; }
//void Camera::setTarget( const Math::dvec3& target ) { _target      = target;}

// ------------------------------------------------------------------------------------------------
//void Camera::rotatePosition( double degrees, const Math::dvec3 &axis, const Math::dvec3 &pivot )
//{
//	const Math::dmat4 currR = Math::transpose( Math::mat4_cast(orientation()) );
//	const Math::dmat4 r     = Math::rotate( Math::dmat4(1),degrees, axis );
//	//_up                     = Math::dvec3( r * Math::dvec4(0.0,1.0,0.0,1.0) );
//
//	const Math::dvec4 c     = Math::dvec4(_target-pivot,1.0);
//	const Math::dvec4 q     = r*currR * c;
//
//	setTarget( Math::dvec3(q) + pivot );
//}

// ------------------------------------------------------------------------------------------------
void Camera::fitSphere( const Math::dvec3 &center, double radius )
{}

// ------------------------------------------------------------------------------------------------
void Camera::fitBoundingBox( const Math::dbox3 &box )
{}

// ------------------------------------------------------------------------------------------------
void Camera::fitScreenRegion( const Math::irect &rectangle )
{}
