// Camera.cpp
// 
#include "Camera.h"

using namespace s2;
using namespace s2::SceneGraph;

// ------------------------------------------------------------------------------------------------
Camera::Camera()
{}

// ------------------------------------------------------------------------------------------------
Camera::~Camera()
{}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Camera::position()      const { return _position; }
Math::dvec3 Camera::target()        const { return _target; }
Math::dvec3 Camera::upVector()      const { return _up;}


// ------------------------------------------------------------------------------------------------
//Math::dvec3 Camera::rightVector() const { return Math::mat3_cast(_orientation)[0]; }
//Math::dvec3 Camera::upVector()    const { return Math::mat3_cast(_orientation)[1]; }
//Math::dvec3 Camera::viewVector()  const { return _target-_position; }
Math::dmat4 Camera::matrix()      const { return  Math::lookAt(_position,_target,_up); }

// ------------------------------------------------------------------------------------------------
void Camera::set( const Math::dvec3 &pos, const Math::dvec3 &target, const Math::dvec3 &up )
{
	_position = pos;
	_target   = target;
	_up       = up;
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
void Camera::fitBoundingBox( const Math::box3 &box )
{}

// ------------------------------------------------------------------------------------------------
void Camera::fitScreenRegion( const Math::Rectangle &rectangle )
{}
