// Camera.cpp
//
#include "Camera.h"

using namespace OpenGL;

// ------------------------------------------------------------------------------------------------
Camera::Camera()
: _eye(0,0,1)
, _center(0,0,0)
, _up(0,1,0)
{
}

// ------------------------------------------------------------------------------------------------
Camera::~Camera()
{}

// ------------------------------------------------------------------------------------------------
void Camera::lookAt( const Math::dvec3 &eyePos, const Math::dvec3 &centerPos, const Math::dvec3 &upVector)
{
	_eye      = eyePos;
	_center   = centerPos;
	_up       = upVector;
}

// ------------------------------------------------------------------------------------------------
void Camera::lookAt( const Math::dvec3 &pos )
{
	const Math::dvec3 viewDir = _center-_eye;
	const double viewDist     = Math::length(viewDir);


	_eye    = Math::dvec3(pos.x,pos.y,_eye.z);
	_center = _eye + viewDir; //Math::dvec3(pos.x,pos.y,_center.z);
}

// ------------------------------------------------------------------------------------------------
void Camera::set( const Math::dvec3 &position, const Math::dmat4 &orientation )
{
	const Math::dvec4 side  = orientation[0];
	const Math::dvec4 up    = orientation[1];
	const Math::dvec4 front = orientation[2];

	const double viewDist    = Math::length(_center-_eye);


	_eye    = position;
	_up     = Math::dvec3(up);
	_center = _eye - Math::dvec3(front)*viewDist;
}

// ------------------------------------------------------------------------------------------------
void Camera::rotate( double degrees, const Math::dvec3 &axis, const Math::dvec3 &pivot )
{
	const Math::dmat4 currR = Math::transpose(orientation());
	const Math::dmat4 r    = Math::rotate( Math::dmat4(1),degrees, axis );
	_up                    = Math::dvec3( r * Math::dvec4(0.0,1.0,0.0,1.0) );

	const Math::dvec4 c    = Math::dvec4(_center-pivot,1.0);
	const Math::dvec4 q    = r*currR * c;
	
	lookAt( Math::dvec3(q) + pivot );
}

// ------------------------------------------------------------------------------------------------
void Camera::rotate( double degrees, const Math::dvec3 &axis )
{
	const Math::dmat4 r    = Math::rotate( Math::dmat4(1),degrees, axis );
	_up                    = Math::dvec3( r * Math::dvec4(0.0,1.0,0.0,1.0) );
}

// ------------------------------------------------------------------------------------------------
Math::dmat4 Camera::orientation()    const
{
	Math::dmat4 m = matrix();

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;

	return Math::transpose(m); // inverse
}

// ------------------------------------------------------------------------------------------------
Math::dmat4 Camera::matrix()         const
{
	return Math::lookAt( _eye,_center,_up);
}